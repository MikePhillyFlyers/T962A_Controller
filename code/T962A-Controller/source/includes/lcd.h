#ifndef LCD_H_
#define LCD_H_


/*
 * lcd.h
 *
 *  Created on: 14 nov 2014
 *      Author: wjo
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <sys/attribs.h>





//#define MINIMALISTIC

#define FB_HEIGHT (64)
#define FB_WIDTH (128)

#define FONT6X6 (0)
#define INVERT (0x80)

#define LCD_CENTER (64)
#define LCD_ALIGN_CENTER(x) (LCD_CENTER - (x * 3))
#define LCD_ALIGN_RIGHT(x) (127 - (x * 6))


#define LCD_ON (0x3f)
#define LCD_RESET_X (0xb8)
#define LCD_RESET_Y (0x40)
#define LCD_RESET_STARTLINE (0xc0)

#define UNTIL_BUSY_IS_CLEAR while (FIO1PIN & 0x800000); // Wait for busy to clear



/* lcd data structure
 * 
 */
typedef struct
{
    // Frame buffer storage (each "page" is 8 pixels high)
    uint8_t FB[FB_HEIGHT / 8][FB_WIDTH];

} _LCD_DATA_;



 /* function declarations */
void Enable_Pulse();
void charoutsmall(uint8_t theChar, uint8_t X, uint8_t Y);
void LCD_disp_str(uint8_t* theStr, uint8_t theLen, uint8_t startx, uint8_t y, uint8_t theFormat);
void LCD_MultiLineH(uint8_t startx, uint8_t endx, uint64_t ymask);
uint8_t LCD_BMPDisplay(uint8_t* thebmp, uint8_t xoffset, uint8_t yoffset);
void LCD_SetPixel(uint8_t x, uint8_t y);
void LCD_SetBacklight(uint8_t backlight);
void LCD_Init(void);
void LCD_FB_Clear(void);
void LCD_FB_Update(void);

void LCD_WriteCmd(uint32_t cmdbyte);
void LCD_WriteData(uint32_t databyte, uint8_t chipnum);
#endif /* LCD_H_ */
