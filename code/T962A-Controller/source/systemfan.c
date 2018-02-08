/*
 * systemfan.c - Temperature controlled system fan handling for T-962 reflow controller
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
#include <sys/attribs.h>
#include "common_defs.h"
#include "systemfan.h"
#include "schedule.h"
#include "sensor.h"



// ********************************************** //
// ****  global variables  declarations here **** //

#define SYSFAN_PWM_PERIOD (TICKS_MS( 10 ))

uint32_t syspwmval = 0;


// ********************************************** //
// ********************************************** //



/**
**  @brief SystemFanPWM_Work the function
**	@param[in,out] 
**	@return Description of returned value.
*/
static int32_t SystemFanPWM_Work(void) 
{
	static uint8_t state = 0;
	int32_t retval;

	if (state) {
		//FIO0CLR = (syspwmval != SYSFAN_PWM_PERIOD) ? (1<<25) : 0; // SysFan off
        CPUFAN_ENABLE(!(syspwmval != SYSFAN_PWM_PERIOD));
		retval = syspwmval ? (SYSFAN_PWM_PERIOD - syspwmval) : -1;
	} else {
		//FIO0SET = syspwmval ? (1<<25) : 0; // SysFan on
        CPUFAN_ENABLE(syspwmval > 0);
		retval = (syspwmval != SYSFAN_PWM_PERIOD) ? syspwmval : -1;
	}
	state ^= 1;
	return retval;
}
/**/
/***********************************************************************************/




/**
**  @brief SystemFanSense_Work the function
**	@param[in,out] 
**	@return Description of returned value.
*/
static int32_t SystemFanSense_Work(void)
{
	uint8_t sysfanspeed = 0;
    uint16_t pwm_value = 0;  
    float systemp = 0;
    
    /* NOTE: pic32 PWM outputs are LOW until match, 
       LPC213x PWM outputs are HI until match...
       i.e. - no need to reverse for pic32...
       pic32 'fully on' == OCRx > PRx
    */

	if (Sensor_IsValid(TC_COLD_JUNCTION))
    {
		systemp = Sensor_GetTemp(TC_COLD_JUNCTION);
        
		// Sort this out with something better at some point
		if (systemp > 50.0f) {
			sysfanspeed = 0xff;
		} else if (systemp > 45.0f) {
			sysfanspeed = 0xc0;
		} else if (systemp > 42.0f) {
			sysfanspeed = 0x80;
		} else if (systemp > 40.0f) {
			sysfanspeed = 0x50;
		}
	} 
    else
    {
		// No sensor, run at full speed as a precaution
		sysfanspeed = 0xff;
	}         
	
    // CPU FAN PWM==> PD0 via OC1
    pwm_value = (uint16_t)sysfanspeed;
    if (sysfanspeed == 0xff)
        pwm_value = 0x100;    
    
    // write the CPU FAN pwm value...
    OC1RS = pwm_value;

    /*
	uint32_t temp = SYSFAN_PWM_PERIOD >> 8;
	temp *= sysfanspeed;
	if (sysfanspeed == 0xff) {
		// Make sure we reach 100% duty cycle
		temp = SYSFAN_PWM_PERIOD;
	}
    // re-schedule pwm work...
	syspwmval = temp;
	Sched_SetState(SYSFANPWM_WORK, 2, 0);
    */
    
	return TICKS_SECS( 5 );
}
/**/
/***********************************************************************************/





/**
**  @brief SystemFan_Init the function
**	@param[in,out] 
**	@return Description of returned value.
*/
void SystemFan_Init(void) 
{
	printf("\n%s", __FUNCTION__);
	//Sched_SetWorkfunc(SYSFANPWM_WORK, SystemFanPWM_Work);
	Sched_SetWorkfunc(SYSFANSENSE_WORK, SystemFanSense_Work);

	// Turn on fan briefly at boot to indicate that it actually works
	//syspwmval = SYSFAN_PWM_PERIOD;
	//Sched_SetState(SYSFANPWM_WORK, 2, 0); // Enable PWM task
	Sched_SetState(SYSFANSENSE_WORK, 1, TICKS_SECS( 2 ) ); // Enable Sense task
}
/**/
/***********************************************************************************/