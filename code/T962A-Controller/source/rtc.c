/*
 * rtc.c - RTC interface for T-962 reflow controller
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
#include "rtc.h"
#include "initialize.h"
#include "timers.h"



// local function declars
void SystemUnLockRtc();
void SystemLockRtc();





/**
**  @brief RTC_Init the function
**	@param[in,out]
**	@return 
*/
void RTC_Init(void)
{
	RTC_Zero();
}
/**/
/***********************************************************************************/



/**
**  @brief RTC_Read the function
**	@param[in,out]
**	@return 
*/
uint32_t RTC_Read(void) 
{   
    uint32_t time = 0;
	uint32_t retval = 0;
    uint8_t tmp = 0;
    
    
    // read the pic32 raw time data 
    time = RTCTIME;
    
    // *** CONVERT HOURS ***
    tmp = bcd2bin(RTC_HOURS_TENS(time));
    retval = (tmp * 10 * 3600);         // convert to secs
    tmp = bcd2bin(RTC_HOURS_ONES(time));
    retval+= (tmp * 3600);              // convert to secs
    // *** CONVERT MINS ***
    tmp = bcd2bin(RTC_MINS_TENS(time));
    retval+= (tmp * 10 * 60);           // convert to secs
    tmp = bcd2bin(RTC_MINS_ONES(time));
    retval+= (tmp * 60);                // convert to secs
    // *** CONVERT SECS ***
    tmp = bcd2bin(RTC_SECS_TENS(time));
    retval+= (tmp * 10);                // covert to secs
    tmp = bcd2bin(RTC_SECS_ONES(time));
    retval+= tmp; 	
    
	return retval;
}
/**/
/***********************************************************************************/


/**
**  @brief RTC_Zero the function
**	@param[in,out]
**	@return 
*/
void RTC_Zero(void) 
{
    uint8_t i = 0;
    
    // unlock the periphs
    SystemUnLockRtc();
    // turn off the RTC
    //RTCCONbits.ON = 0;              /* RTC OFF */
    Nop();
    Nop();
    //while(RTCCON&0x40);
    // reset the time/date
    RTCTIME = 0;
    RTCDATE = 0;
    // turn the RTC back on
    //RTCCONbits.ON = 1;              /* RTC ON */
    Nop();
    Nop();
    while (i < 100)
    {
        // wait for RTC to start up..
        if ( (RTCCON&0x40) )
            break;
        i++;
    }
    Nop();
    Nop();
    // lock the periphs back up
    SystemLockRtc();    
    delay_ms(1000);
    
    return;
}
/**/
/***********************************************************************************/



void SystemUnLockRtc()
{
    SYSKEY = 0;				// ensure OSCCON is locked
    SYSKEY = 0xAA996655;	// Write Key1 to SYSKEY
    SYSKEY = 0x556699AA;	// Write Key2 to SYSKEY
    RTCCONbits.RTCWREN = 1;         /* rtc write enable */ 
    Nop();
    Nop();
    SYSKEY = 0;				// ensure OSCCON is locked
}
/**/
/***********************************************************************************/



void SystemLockRtc()
{
    SYSKEY = 0;				// ensure OSCCON is locked
    SYSKEY = 0xAA996655;	// Write Key1 to SYSKEY
    SYSKEY = 0x556699AA;	// Write Key2 to SYSKEY
    RTCCONbits.RTCWREN = 0;         /* rtc write enable */ 
    Nop();
    Nop();
    SYSKEY = 0;				// ensure OSCCON is locked
}
/**/
/***********************************************************************************/



/* ---------------------------------------------------------------------
   bin2bcd()
   --------------------------------------------------------------------- */

uint8_t bin2bcd(uint8_t dec)
{
	return ((dec/10) << 4) + (dec % 10);
}
/**/
/***********************************************************************************/



/* ---------------------------------------------------------------------
   bcd2bin()
   --------------------------------------------------------------------- */

uint8_t bcd2bin(uint8_t bcd)
{
	return ((bcd >> 4) * 10) + bcd % 16;
}
/**/
/***********************************************************************************/
