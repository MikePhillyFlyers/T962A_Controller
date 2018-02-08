/*
 * lcd.c - Display handling (KS0108 compatible, with two chip selects, active high)
 * for T-962 reflow controller
 *
 * Copyright (C) 2010,2012,2013,2014 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"
#include "timers.h" 
#include "lcd.h"
#include "smallfont.h"





// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //



typedef struct __attribute__ ((packed))
{
	uint8_t bfType[2]; // 'BM' only in this case
	uint32_t bfSize; // Total size
	uint16_t bfReserved[2];
	uint32_t bfOffBits; // Pixel start byte
	uint32_t biSize; // 40 bytes for BITMAPINFOHEADER
	int32_t biWidth; // Image width in pixels
	int32_t biHeight; // Image height in pixels (if negative image is right-side-up)
	uint16_t biPlanes; // Must be 1
	uint16_t biBitCount; // Must be 1
	uint32_t biCompression; // Only 0 (uncompressed) supported at the moment
	uint32_t biSizeImage; // Pixel data size in bytes
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
	uint32_t aColors[2]; // Palette data, first color is used if pixel bit is 0, second if pixel bit is 1
} BMhdr_t;



// local function declarations
#define LCD_CS_1    1
#define LCD_CS_2    2
void LCD_BusyCheck(uint8_t CS_Mask);




/**
**  @brief 
**	@return 
*/
void charoutsmall(uint8_t theChar, uint8_t X, uint8_t Y)
{
    uint8_t x = 0;
    uint16_t temp = 0;
    uint16_t old = 0;
            
	// First of all, make lowercase into uppercase
	// (as there are no lowercase letters in the font)
	if ((theChar & 0x7f) >= 0x61 && (theChar & 0x7f) <= 0x7a) {
		theChar -= 0x20;
	}
	uint16_t fontoffset = ((theChar & 0x7f) - 0x20) * 6;
	uint8_t yoffset = Y & 0x7;
	Y >>= 3;

    #ifndef MINIMALISTIC
	uint8_t width = (theChar & 0x80) ? 7 : 6;
    #else
	uint8_t width=6;
    #endif

	for (x = 0; x < width; x++)
    {
		temp = smallfont[fontoffset++];
        #ifndef MINIMALISTIC
		if (theChar & 0x80) { temp ^= 0x7f; }
        #endif
		temp = temp << yoffset; // Shift pixel data to the correct lines
		old = (g_Config.LcdData.FB[Y][X] | (g_Config.LcdData.FB[Y + 1][X] << 8));
        #ifndef MINIMALISTIC
		old &= ~(0x7f << yoffset); //Clean out old data
        #endif
		temp |= old; // Merge old data in FB with new char
		if (X >= (FB_WIDTH)) return; // make sure we don't overshoot
		if (Y < ((FB_HEIGHT / 8) - 0)) {
			g_Config.LcdData.FB[Y][X] = temp & 0xff;
		}
		if (Y < ((FB_HEIGHT / 8) - 1)) {
			g_Config.LcdData.FB[Y + 1][X] = temp >> 8;
		}
		X++;
	}
    
    return;
}
/**/
/********************************************************/



/**
**  @brief 
**	@return 
*/
void LCD_disp_str(uint8_t* theStr, uint8_t theLen, uint8_t startx, uint8_t y, uint8_t theFormat)
{
    uint8_t q = 0;
    uint8_t invmask = 0;
    
    #ifdef MINIMALISTIC
	for (q = 0; q < theLen; q++) {
		charoutsmall(theStr[q], startx, y);
		startx += 6;
	}
    #else
	invmask = theFormat & 0x80;
	for (q = 0; q < theLen; q++) {
		charoutsmall(theStr[q] | invmask, startx, y);
		startx += 6;
	}
    #endif
    
    return;
}
/**/
/********************************************************/


/**
**  @brief 
**	@return 
*/
void LCD_MultiLineH(uint8_t startx, uint8_t endx, uint64_t ymask) 
{
    uint8_t x = 0;    
    
	for (x = startx; x <= endx; x++) {
		g_Config.LcdData.FB[0][x] |= ymask & 0xff;
		g_Config.LcdData.FB[1][x] |= ymask >> 8;
		g_Config.LcdData.FB[2][x] |= ymask >> 16;
		g_Config.LcdData.FB[3][x] |= ymask >> 24;
        #if FB_HEIGHT == 64
		g_Config.LcdData.FB[4][x] |= ymask >> 32;
		g_Config.LcdData.FB[5][x] |= ymask >> 40;
		g_Config.LcdData.FB[6][x] |= ymask >> 48;
		g_Config.LcdData.FB[7][x] |= ymask >> 56;
        #endif
	}
    
    return;
}
/**/
/********************************************************/



/*
 * At the moment this is a very basic BMP file reader with the following limitations:
 * The bitmap must be 1-bit, uncompressed with a BITMAPINFOHEADER.
 */
uint8_t LCD_BMPDisplay(uint8_t* thebmp, uint8_t xoffset, uint8_t yoffset)
{
	BMhdr_t* bmhdr;
	uint8_t upsidedown = 1;
	uint8_t inverted = 0;
	uint16_t pixeloffset;
	uint8_t numpadbytes = 0;
    uint16_t xx = 0;
    int8_t y = 0;
    uint8_t x = 0;
    uint8_t b = 0;
    uint8_t realY = 0;
    uint8_t pagenum = 0;
    uint8_t pixelval = 0;
    uint8_t pixel = 0;
    uint8_t max_b = 0;
    
    
    
	// The following code grabs the header portion of the bitmap and caches it locally on the stack
	BMhdr_t temp;
	uint8_t* xxx = (uint8_t*) &temp;
	for (xx = 0; xx < sizeof(BMhdr_t); xx++) {
		xxx[xx] = *(thebmp + xx);
	}
	bmhdr = &temp;

//	printf("\n%s: bfSize=%x biSize=%x", __FUNCTION__, (uint16_t)bmhdr->bfSize, (uint16_t)bmhdr->biSize);
//	printf("\n%s: Image size is %d x %d", __FUNCTION__, (int16_t)bmhdr->biWidth, (int16_t)bmhdr->biHeight);
	if (bmhdr->biPlanes != 1 || bmhdr->biBitCount != 1 || bmhdr->biCompression != 0) {
		printf("\n%s: Incompatible bitmap format!", __FUNCTION__);
		return 1;
	}
	pixeloffset = bmhdr->bfOffBits;
	if (bmhdr->aColors[0] == 0) {
		inverted = 1;
	}
	if (bmhdr->biHeight<0) {
		bmhdr->biHeight = -bmhdr->biHeight;
		upsidedown = 0;
	}
	if ((bmhdr->biWidth+xoffset > FB_WIDTH) || (bmhdr->biHeight+yoffset > FB_HEIGHT)) {
		printf("\n%s: Image won't fit on display!", __FUNCTION__);
		return 1;
	}

	// Figure out how many dummy bytes that is present at the end of each line
	// If the image is 132 pixels wide then the pixel lines will be 20 bytes (160 pixels)
	// 132&31 is 4 which means that there are 3 bytes of padding
	numpadbytes = (4 - ((((bmhdr->biWidth) & 0x1f) + 7) >> 3)) & 0x03;
//	printf("\n%s: Skipping %d padding bytes after each line", __FUNCTION__, numpadbytes);

	for (y = bmhdr->biHeight - 1; y >= 0; y--)
    {
		realY = upsidedown ? (uint8_t)y : (uint8_t)(bmhdr->biHeight) - y;
		realY += yoffset;
		pagenum = realY >> 3;
		pixelval = 1 << (realY & 0x07);
        
		for(x = 0; x < bmhdr->biWidth; x += 8)
        {
			pixel = *(thebmp + (pixeloffset++));
			if (inverted) { pixel^=0xff; }
			max_b = bmhdr->biWidth - x;
			if (max_b>8) { max_b = 8; }
			for (b = 0; b < max_b; b++)
            {
				if (pixel & 0x80) {
					g_Config.LcdData.FB[pagenum][x + b + xoffset] |= pixelval;
				}
				pixel = pixel << 1;
			}
		}
		pixeloffset += numpadbytes;
	}
    
	return 0;
}
/**/
/********************************************************/


/**
**  @brief 
**	@return 
*/
void LCD_SetPixel(uint8_t x, uint8_t y)
{
	if (x >= FB_WIDTH || y >= FB_HEIGHT) {
		// No random memory overwrites thank you
		return;
	}
    // set the pixel
	g_Config.LcdData.FB[y >> 3][x] |= 1 << (y & 0x07);
    
    return;
}
/**/
/********************************************************/


/**
**  @brief 
**	@return 
*/
void LCD_SetBacklight(uint8_t backlight)
{
	if (backlight) {		
        LCD_BACKLIGHT_PIN(TRUE);
	} else {		
        LCD_BACKLIGHT_PIN(FALSE);
	}
    
    return;
}
/**/
/********************************************************/


/**
**  @brief 
**	@return 
*/
// No performance gain by inlining the command code
void LCD_WriteCmd(uint32_t cmdbyte)
{
    uint32_t temp = 0;
    
    
    // check both ChipSels for BUSY
    #ifdef LCD_BUSY_POLLING
    LCD_BusyCheck(LCD_CS_1 | LCD_CS_2);
    #endif
    
    // both CS active
    LCD_CS1_PIN(TRUE); 
    LCD_CS2_PIN(TRUE); 
    LCD_RS_PIN(LO);
    LCD_RW_PIN(LO);
    
    // CLEAR DATA BITS
    LCD_CLEAR_DATA;
    
    // set data as outputs
    LCD_DATA_OUTPUT;
    
    // read port setup              
    temp = (PORTB & 0xFFFFFFFC);    
    temp |= (cmdbyte << 5);    
    LATB = temp;

    // delay 4us
    delay_us(1); 
    
    // pulse EN line
    Enable_Pulse();  
     
    return;
}
/**/
/********************************************************/



/**
**  @brief 
**	@return 
*/
// Because of the cycle time requirements for E inlining actually does not boost performance
void LCD_WriteData(uint32_t databyte, uint8_t chipnum)
{	
    uint32_t temp = 0;
    
    
    // set appropriate CS bits
    if (chipnum == 0)
    {
       // enable CS#1, and make
       // sure chip is not busy
       LCD_CS1_PIN(TRUE);
       LCD_CS2_PIN(FALSE);
       #ifdef LCD_BUSY_POLLING
       LCD_BusyCheck(LCD_CS_1);
       #endif
    }
    else
    {
       // enable CS#2, and make
       // sure chip is not busy
       LCD_CS1_PIN(FALSE);
       LCD_CS2_PIN(TRUE);
       #ifdef LCD_BUSY_POLLING
       LCD_BusyCheck(LCD_CS_2);
       #endif
    }        
    
    // set RS hi
    LCD_RS_PIN(HI);
    LCD_RW_PIN(LO);
    LCD_EN_PIN(LO);
    
    // CLEAR DATA BITS
    LCD_CLEAR_DATA;
    
    // set data as outputs
    LCD_DATA_OUTPUT;
    
    // read port setup              
    temp = (PORTB & 0xFFFFFFFC);    
    temp |= (databyte << 5);    
    LATB = temp;       
    
    // delay 4us
    delay_us(4); 
    
    // pulse the enable line
    Enable_Pulse();    
    
   return;    
}
/**/
/********************************************************/


/**
**  @brief function for checking the LCD busy bit,
**         and waiting until it's not busy
**	@return 
*/
__inline void LCD_BusyCheck(uint8_t CS_Mask)
{
    
    // -------------------------------------------- //    
    // ---------- CHECK LCD CHIP #1 --------------- //
    // -------------------------------------------- //
    
    if (CS_Mask & LCD_CS_1)
    {
        // Start by making sure none of the display controllers are busy   
        LCD_EN_PIN(LO);    
        LCD_RS_PIN(LO);  
        LCD_RW_PIN(HI);
        LCD_CS1_PIN(FALSE);
        LCD_CS2_PIN(FALSE);    

        // CLEAR DATA BITS
        LCD_CLEAR_DATA;

        // set data as inputs
        LCD_DATA_INPUT;

        // lcd enable, check CS#2
        LCD_CS1_PIN(TRUE);

        // delay 4us
        delay_us(1); 

        // pulse EN line
        Enable_Pulse(); 

        // check and wait on the 'BUSY' bit
        while (LCD_STATUS_BUSY);
    }
    
    // -------------------------------------------- //    
    // ---------- CHECK LCD CHIP #2 --------------- //
    // -------------------------------------------- //
    
    if (CS_Mask & LCD_CS_2)
    {
        // Start by making sure none of the display controllers are busy   
        LCD_EN_PIN(LO);    
        LCD_RS_PIN(LO);  
        LCD_RW_PIN(HI);
        LCD_CS1_PIN(FALSE);
        LCD_CS2_PIN(FALSE);    

        // CLEAR DATA BITS
        LCD_CLEAR_DATA;

        // set data as inputs
        LCD_DATA_INPUT;

        // lcd enable, check CS#2
        LCD_CS2_PIN(TRUE);

        // delay 4us
        delay_us(1); 

        // pulse EN line
        Enable_Pulse(); 

        // check and wait on the 'BUSY' bit
        while (LCD_STATUS_BUSY);
    }
    
    return;
}
/**/
/*************************************************************************************/



/**
**  @brief 
**	@return 
*/
void LCD_Init(void)
{
    // clear the lcd data reg
    LCD_CLEAR_DATA;
            
    // set data as outputs
    LCD_DATA_OUTPUT; 
    
    // initialize/clear the LCD
	LCD_WriteCmd(LCD_ON);
	LCD_WriteCmd(LCD_RESET_X);
	LCD_WriteCmd(LCD_RESET_Y);
	LCD_WriteCmd(LCD_RESET_STARTLINE);
	LCD_FB_Clear();
	LCD_FB_Update();
	LCD_SetBacklight(1);
    
    return;
}
/**/
/***********************************************/


/**
**  @brief 
**	@return 
*/
void LCD_FB_Clear(void) 
{
    uint8_t j = 0;

    // Init FB storage
    for (j = 0; j < (FB_HEIGHT / 8); j++) {
        memset(g_Config.LcdData.FB[j], 0, FB_WIDTH);
    }    

    return;
}
/**/
/***********************************************/


/**
**  @brief 
**	@return 
*/
void LCD_FB_Update() 
{
    uint32_t page = 0;
    uint32_t i = 0;
    
            
	for (page = 0; page < (FB_HEIGHT >> 3); page++) 
    {
		LCD_WriteCmd(LCD_RESET_X + page);
		LCD_WriteCmd(LCD_RESET_Y);

		for(i = 0; i < 64; i++)
        {
			LCD_WriteData(g_Config.LcdData.FB[page][i], 0);
			LCD_WriteData(g_Config.LcdData.FB[page][i + 64], 1);
		}
	}
}
/**/
/*************************************************************************************/


/**
**  @brief 
**	@return 
*/
void Enable_Pulse()
{ 
   LCD_EN_PIN(TRUE);
   delay_us(5);
   LCD_EN_PIN(FALSE);
   delay_us(5); 
   
   return;
}
/**/
/*************************************************************************************/