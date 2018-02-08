/*
 * eeprom.c - I2C EEPROM interface for T-962 reflow controller
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
#include "eeprom.h"
#include "i2c.h"







//#define DUMP_EEPROM



/**
**  @brief EEPROM_Init
**	@param[in,out]
**	@return 
*/
void EEPROM_Init(void) 
{
	#ifdef DUMP_EEPROM
	EEPROM_Dump();
	#endif
	
    // initialize
    EEPROM_Initialize();
    
    return;
}
/**/
/***********************************************************************************/




/**
**  @brief EEPROM_Dump
**	@param[in,out]
**	@return 
*/
void EEPROM_Dump(void) 
{
	uint8_t dumpbuf[EEPROM_SIZE];
    int i = 0;
    
    
    // dump the eeprom contents...
	EEPROM_Read(dumpbuf, 0, sizeof(dumpbuf));
        
	printf("\nEEPROM contents:");
	for (i = 0; i < sizeof(dumpbuf); i++)
    {
		if ((i & 0x0f) == 0){
			printf("\n0x%04x:", i);
		}
		printf(" %02x", dumpbuf[i]);
	}
    
    return;
}
/**/
/***********************************************************************************/



/**
**  @brief EEPROM_Read
**	@param[in,out]
**	@return 
*/
int32_t EEPROM_Read(uint8_t* dest, uint32_t startpos, uint32_t len)
{
	int32_t retval = 0;
    uint8_t offset = 0;
    
    
	if ( (startpos < EEPROM_SIZE) && dest && len && (len <= EEPROM_SIZE) )
    {
        offset = (uint8_t)startpos;	
        // perform the eeprom read operation...
        retval = I2C_BusOperation(startpos, dest, len, I2C_READ);
	}
    
    // return read status
	return retval;
}
/**/
/***********************************************************************************/



/**
**  @brief EEPROM_Write
**	@param[in,out]
**	@return 
*/
int32_t EEPROM_Write(uint32_t startdestpos, uint8_t* src, uint32_t len) 
{
	int32_t retval = 0;
    
    // if params are valid, do the write
	if ( (startdestpos < EEPROM_SIZE) && len && (len <= EEPROM_SIZE) ) 
    {		
		uint16_t writeaddr = (uint16_t)startdestpos;        
        
		while (len)
        {
			uint32_t loopcnt = 0;
			uint8_t startoffset = (writeaddr & (EEPROM_PAGE_SIZE-1));
			uint8_t maxwritesize = (EEPROM_PAGE_SIZE - startoffset);
			// up to 32 bytes at a time depending on alignment
			uint8_t bytestowrite = (len > maxwritesize) ? maxwritesize : len;
            
            
			// Set address pointer and provide up to 32 bytes of data for page write
			retval = I2C_BusOperation(writeaddr, src, bytestowrite, I2C_WRITE);
			if (!retval) 
            {
				do
                {
					// Dummy write to poll timed write cycle completion
					retval = I2C_BusOperation(writeaddr, src, 1, I2C_WRITE);
					loopcnt++;					
				} while (retval && loopcnt < 400);
                
                
				if (retval)
                {
					printf("\nTimeout getting ACK from EEPROM during write!");
					break;
				}
				len -= bytestowrite;
				writeaddr += bytestowrite;
				src += bytestowrite;
			}
            else 
            {
				printf("\nFailed to write to EEPROM!");
				retval = -2;
				break;
			}
		} // end while(...)
	} 
    else 
    {
		printf("\nInvalid EEPROM addressing");
		retval = -3;
	}
    
    // return final status..
	return retval;
}
/**/
/***********************************************************************************/



/**
**  @brief EEPROM_Initialize
**	@param[in,out]
**	@return 
*/
void EEPROM_Initialize(void)
{
    NV_Config_Page* pNvConfig = NULL;
    uint8_t eeprom_page[NV_PAGE_SIZE] = {0};
    uint16_t nvmagic = NVMAGIC;
    uint16_t i = 0;

        
    
    // read in the config page, and see if eeprom is initialized
    // with the 'magic' sig bytes (start and end markers)
    EEPROM_Read(eeprom_page, NV_START_ADDR, NV_PAGE_SIZE);
     
    pNvConfig = (NV_Config_Page*)eeprom_page;
    if ( (pNvConfig->MyNV.magic != NVMAGIC) && (pNvConfig->magic != NVMAGIC) ) 
    {  
        // zero out the write buffer
        memset(eeprom_page, 0, NV_PAGE_SIZE);
        for (i = 0; i < TOTAL_NV_PAGES; i++)
        {
            // erase the current eeprom block 
            EEPROM_Write(i*NV_PAGE_SIZE, eeprom_page, NV_PAGE_SIZE);             
        }
         
        // write the start marker byte
        EEPROM_Write(NV_START_ADDR, (uint8_t*)&nvmagic, NVMAGIC_SIZE);
        // write the end marker byte
        EEPROM_Write((NV_PAGE_SIZE-NVMAGIC_SIZE), (uint8_t*)&nvmagic, NVMAGIC_SIZE);    
    }
    
    // return
	return;
}
/**/
/***********************************************************************************/