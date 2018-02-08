/*
 * onewire.c - Bitbang 1-wire DS18B20 temp-sensor handling for T-962 reflow controller
 *
 * Copyright (C) 2014 Werner Johansson, wj@unifiedengineering.se
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
#include "onewire.h"
#include "timers.h"
#include "schedule.h"





// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //




static const unsigned char dscrc_table[] = 
{
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};


// function declarations
inline uint32_t resetbus(void);
inline uint32_t xferbyte(uint32_t thebyte);
static inline uint32_t xferbit(uint32_t thebit);
static uint8_t docrc8(uint8_t value);





static inline void setpin0() {
	// set pin '0'
    TEMP_SENSOR_DQ(LO);         // '0' out
    TEMP_SENSOR_OUTPUT;         // output
}

static inline void setpin1() {
	//set pin '1';
    TEMP_SENSOR_DQ(HI);         // '1' out
    TEMP_SENSOR_OUTPUT;         // output
}

static inline void setpinhiz() {
	// set pin 'Z'
    TEMP_SENSOR_DQ(HI);
    TEMP_SENSOR_INPUT;         // input
}

static inline uint32_t getpin() {
	// return DQ pin status
    return !!(TEMP_SENSOR_DQ_STATE);
}

inline uint32_t xferbyte(uint32_t thebyte)
{
    uint32_t bits = 0;
    
    // iterate through 8 bits
	for (bits = 0; bits < 8; bits++)
    {
		setpin0();		
        delay_us(1);                // 1.5us
		if (thebyte&0x01) setpinhiz();
        thebyte >>= 1;		
        delay_us(13);               // 13us
		if (getpin()) thebyte |= 0x80;        
        delay_us(45);               // 45us
		setpinhiz();		
        delay_us(10);               // 10us
	}
	return thebyte;
}
/**/
/***********************************************************************************/

/*
 *
 */
static inline uint32_t xferbit(uint32_t thebit) 
{
	setpin0();	
    delay_us(1.5);                  // 1.5us
	if (thebit) setpinhiz();
    thebit = 0;	
    delay_us(13);                   // 13us
	if (getpin()) thebit = 0x01;    
    delay_us(45);                   // 45us
	setpinhiz();	
    delay_us(10);                   // 10us
	return thebit;
}
/**/
/***********************************************************************************/

/*
 *
 */
inline uint32_t resetbus(void)
{
	uint32_t devicepresent = 0;

	setpin0();	
    delay_us(480);                  // 480us
	setpinhiz();	
    delay_us(70);                   // 70us
	if (getpin() == 0) devicepresent = 1;	
    delay_us(410);                  // 410us

	return devicepresent;
}
/**/
/***********************************************************************************/


/*
 * Calculate the CRC8 of the byte value provided with the current
 * global 'crc8' value.
 * Returns current global crc8 value
 */
static uint8_t docrc8(uint8_t value)
{
    uint8_t crc8 = 0;
    // See Application Note 27

	// TEST BUILD
	crc8 = dscrc_table[crc8 ^ value];
	return crc8;
}
/**/
/***********************************************************************************/


/*
 * OneWire_Work - work function for the DS18B20 device
 * 
 * Returns next tick time
 */
int32_t OneWire_Work(void) 
{
	static uint8_t mystate = 0;
	uint8_t scratch[9];
	int32_t retval = 0;
    int i = 0;
    uint32_t iter = 0;
    int16_t tmp = 0;
    
    
    
     // make sure ints are disabled..
    #ifdef KEYPAD_INTERRUPTS
    VIC_DisableIRQ();
    #endif


    // switch on the current 'ONE-WIRE' state...
	if (mystate == 0)
    {       
		if (resetbus())
        {
            // start the convert process...
			xferbyte(OW_SKIP_ROM); // All devices on the bus are addressed here
			xferbyte(OW_CONVERT_T);			
            setpinhiz();
			retval = TICKS_MS(800); // TC interface needs max 800ms to be ready
			mystate++;
		}
	} 
    else if (mystate == 1)
    {
        // reset the DS18B20, and read temp...
		if (resetbus())
        {
            //uint32_t save = VIC_DisableIRQ();
            xferbyte(OW_SKIP_ROM);             // only one device
            xferbyte(OW_READ_SCRATCHPAD);
            setpinhiz();
            for (iter = 0; iter < 4; iter++) { // Read four bytes
                scratch[iter] = xferbyte(0xff);
            }
            //VIC_RestoreIRQ(save);
            tmp = (scratch[1]<<8 | scratch[0]);
            g_Config.TempSensors.CJSensor.devreadout = tmp;
            tmp = (scratch[3]<<8 | scratch[2]);
            g_Config.TempSensors.CJSensor.extrareadout = tmp;
            
            // convert current value
            g_Config.TempSensors.CJSensor.tempvalid = 1;
            g_Config.TempSensors.CJSensor.temperature = OneWire_GetTempSensorReading();            
        }		
		mystate = 0;
	} else {
		retval = -1;
	}
    
    #ifdef KEYPAD_INTERRUPTS
    // reenable ints, **IF** they were enabled prior....
    VIC_RestoreIRQ();
    #endif

	return retval;
}
/**/
/***********************************************************************************/


/*
 *
 */
uint32_t OneWire_Init(void)
{
    int i = 0;
    int iter = 0;
    int idloop = 0;
    
    
	printf("\n%s called", __FUNCTION__);
	Sched_SetWorkfunc(ONEWIRE_WORK, OneWire_Work);
	printf("\nScanning 1-wire bus...");

   	// make sure ints are disabled..
    #ifdef KEYPAD_INTERRUPTS
    VIC_DisableIRQ();
    #endif
    
    // reset the one-wire bus, and check for the DS18B20
    if (resetbus() == TRUE)   
    {
        // set the presence flag
        g_Config.TempSensors.CJSensor.ispresent = 1;
		Sched_SetState(ONEWIRE_WORK, 2, 0); // Enable OneWire task if there's at least one device
	}
    // restore ints...
    #ifdef KEYPAD_INTERRUPTS
    VIC_RestoreIRQ();	
    #endif
    
	return g_Config.TempSensors.CJSensor.ispresent;
}
/**/
/***********************************************************************************/



/*
 *
 */
float OneWire_GetTempSensorReading(void) 
{
    int16_t temp = 0;
	float retval = 999.0f; // Report invalid temp if not found
    
    // if the DS18B20 was found, convert it's temp reading
	if (g_Config.TempSensors.CJSensor.ispresent == 1)
    {
        if (g_Config.TempSensors.CJSensor.tempvalid == 1)
        {
            retval = (g_Config.TempSensors.CJSensor.devreadout & 0x7FF);
            if (g_Config.TempSensors.CJSensor.devreadout & BIT_11)
            {
                temp = (0xF800 | (g_Config.TempSensors.CJSensor.devreadout & 0x7FF));
                retval = temp;          
            }
            retval *= 0.0625; // LSB = 0.0625 degrees
            printf(" (%.1f C)",retval);
        }
        else {
            // no temp valid yet, so return default for now...
            retval = 25.0f;     
        }
	} 
    else 
    {
		printf(" (%.1f C assumed)", retval);
	}
	return retval;
}
/**/
/***********************************************************************************/


/*
 *
 */
int OneWire_IsTCPresent(uint8_t tcid)
{
    return g_Config.TempSensors.CJSensor.ispresent;
}
/**/
/***********************************************************************************/
