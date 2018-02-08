/*
 * max31855.c - SPI TC interface handling for T-962 reflow controller
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
#include "common_defs.h"
/* local headers */
#include "max31855.h"
#include "schedule.h"
#include "spi.h"




// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //



// local function definitions
static uint32_t SPI_COLLECT_DATA(uint8_t NumDevicesToScan);





/**
**  @brief SPI_COLLECT_DATA the function for collecting TC info
**	@param[in,out] 
**	@return Description of returned value.
*/
static uint32_t SPI_COLLECT_DATA(uint8_t NumDevicesToScan)
{
    uint8_t i = 0;
    uint32_t spidata = 0;
    uint32_t numdevices = 0;
    
    
    // iterate over the total possible 
    // max31855s and grab their data
	for (i = 0; i < NumDevicesToScan; i++)
    {        
        // get the TC data from the 'SPI' bus,
        // and make sure NO faults...
        spidata = (uint32_t)TsSensorReceive(i);
        if ( !(spidata & TC_ALL_FAULTS) && (spidata != 0) )
        {
            // setup the sensor data and collect readings
            g_Config.TempSensors.TCSensor[i].ispresent = 1;
            g_Config.TempSensors.TCSensor[i].tempvalid = 1;
            g_Config.TempSensors.TCSensor[i].devreadout = (int16_t)((spidata >> 16));
            g_Config.TempSensors.TCSensor[i].extrareadout = (int16_t)(spidata & (uint16_t)-1);
            numdevices++;
        }
        else
        {
            // TC invalid or faulted...
            g_Config.TempSensors.TCSensor[i].ispresent = 0;
            g_Config.TempSensors.TCSensor[i].tempvalid = 0;
            g_Config.TempSensors.TCSensor[i].devreadout = (int16_t)-1;
            g_Config.TempSensors.TCSensor[i].extrareadout = (int16_t)-1;
        }
    }
    
    // return number of valid devices
    return numdevices;
}
/**/
/******************************************************************8*/



/**
**  @brief SPI_TC_Work the function for collecting TC info
**	@param[in,out] 
**	@return Description of returned value.
*/
static int32_t SPI_TC_Work( void )
{
    
    // collect temp data for existing TC sensors..
    SPI_COLLECT_DATA(g_Config.numSensorsFound);
    
    // return next time to run...
	return TICKS_MS(100);
}
/**/
/******************************************************************************/




/**
**  @brief SPI_TC_Init the function for SPI init..
**	@param[in,out] 
**	@return Description of returned value.
*/
uint32_t SPI_TC_Init(void)
{
    int i = 0;
    
    
	printf("\n%s called", __FUNCTION__);
	Sched_SetWorkfunc(SPI_TC_WORK, SPI_TC_Work);

    // init the spi array to invalid
	for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
        g_Config.TempSensors.TCSensor[i].ispresent = 0;
        g_Config.TempSensors.TCSensor[i].tempvalid = 0;
		g_Config.TempSensors.TCSensor[i].devreadout = -1; // Assume we don't find any thermocouple interfaces
		g_Config.TempSensors.TCSensor[i].extrareadout = -1;
	}

    printf("\nProbing for MAX31855 devices...");

    // Run one iteration to update all data
    g_Config.numSensorsFound = SPI_COLLECT_DATA(MAX_SPI_DEVICES);

    // if we have at least 1, then schedule worker
    if (g_Config.numSensorsFound)
    {
        // Enable SPI TC task if there's at least one device
        Sched_SetState(SPI_TC_WORK, 2, 0);
    } 
    else
    {
        printf(" No MAX31855 devices found!");
    }
  
    // return num devices found
	return g_Config.numSensorsFound;
}
/**/
/*******************************************************************************/


/**
**  @brief SPI_IsTCPresent the function for verifying present/not present TC
**	@param[in,out] 
**	@return Description of returned value.
*/
int SPI_IsTCPresent(uint8_t tcid)
{
	if (tcid < g_Config.numSensorsFound)
    {
        // if bit16 is '1', then an 'SCV', 'SCG', or 'OC' fault exists
		if ( g_Config.TempSensors.TCSensor[tcid].ispresent ) {
			 // A faulty/not connected TC will not be flagged as present
			 return 1;
		}
	}
	return 0;
}
/**/
/*********************************************************************************/