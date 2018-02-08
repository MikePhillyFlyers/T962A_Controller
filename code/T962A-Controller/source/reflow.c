/*
 * reflow.c - Actual reflow profile logic for T-962 reflow controller
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
#include "t962.h"
#include "reflow_profiles.h"
#include "io.h"
#include "lcd.h"
#include "rtc.h"
#include "PID_v1.h"
#include "schedule.h"
#include "nvstorage.h"
#include "sensor.h"
#include "reflow.h"





// Standby temperature in degrees Celsius
#define STANDBYTEMP (50)

// 250ms between each run
#define PID_TIMEBASE (250)

#define TICKS_PER_SECOND (1000 / PID_TIMEBASE)



// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //








/*
 *
 *
 */
static int32_t Reflow_Work(void)
{
	static ReflowMode_t oldmode = REFLOW_INITIAL;
	static uint32_t lasttick = 0;
    _REFLOW_* pReflow = &g_Config.Reflow;
	uint8_t fan = 0;
    uint8_t heat = 0;
    uint32_t thistick = 0;
    int32_t nexttick = 0;
	uint32_t ticks = RTC_Read();
    


    
	Sensor_DoConversion();
	pReflow->avgtemp = Sensor_GetTemp(TC_AVERAGE);

	const char* modestr = "UNKNOWN";

	// Depending on mode we should run this with different parameters
	if (pReflow->mymode == REFLOW_STANDBY || pReflow->mymode == REFLOW_STANDBYFAN) 
    {
		pReflow->intsetpoint = STANDBYTEMP;
		// Cool to standby temp but don't heat to get there
		Reflow_Run(0, pReflow->avgtemp, &heat, &fan, pReflow->intsetpoint);
		heat = 0;

		// Suppress slow-running fan in standby
		if ( (pReflow->mymode == REFLOW_STANDBY) && (pReflow->avgtemp < (float)STANDBYTEMP) ) {
			 fan = 0;
		}
		modestr = "STANDBY";

	} 
    else if (pReflow->mymode == REFLOW_BAKE)
    {
		pReflow->reflowdone = Reflow_Run(0, pReflow->avgtemp, &heat, &fan, pReflow->intsetpoint) ? 1 : 0;
		modestr = "BAKE";

	}
    else if (pReflow->mymode == REFLOW_REFLOW)
    {
		pReflow->reflowdone = Reflow_Run(ticks, pReflow->avgtemp, &heat, &fan, 0) ? 1 : 0;
		modestr = "REFLOW";

	}
    else 
    {
		heat = fan = 0;
	}
    // set heater & fan to values...
	Set_Heater(heat);
	Set_Fan(fan);

	if (pReflow->mymode != oldmode)
    {
		printf("\n# Time,  Temp0, Temp1, Temp2, Temp3,  Set,Actual, Heat, Fan,  ColdJ, Mode");
		oldmode = pReflow->mymode;
		pReflow->numticks = 0;
	} 
    else if (pReflow->mymode == REFLOW_BAKE)
    {
		if ( (pReflow->bake_timer > 0) && (pReflow->numticks >= pReflow->bake_timer) ) {
			printf("\n DONE baking, set bake timer to 0.");
			pReflow->bake_timer = 0;
			Reflow_SetMode(REFLOW_STANDBY);
		}

		// start increasing ticks after setpoint is reached...
		if ( (pReflow->avgtemp < pReflow->intsetpoint) && (pReflow->bake_timer > 0) ) {
			modestr = "BAKE-PREHEAT";
		} else {
			pReflow->numticks++;
		}
	} 
    else if (pReflow->mymode == REFLOW_REFLOW)
    {
		pReflow->numticks++;
	}

	if ( !( (pReflow->mymode == REFLOW_STANDBY) && (pReflow->standby_logging == 0)) ) 
    {
		printf("\n%6.1f,  %5.1f, %5.1f, %5.1f, %5.1f,  %3u, %5.1f,  %3u, %3u,  %5.1f, %s",
		       ((float)numticks / TICKS_PER_SECOND),
		       Sensor_GetTemp(TC_LEFT),
		       Sensor_GetTemp(TC_RIGHT),
		       Sensor_GetTemp(TC_PCB1),
		       Sensor_GetTemp(TC_PCB2),
		       intsetpoint, avgtemp,
		       heat, fan,
		       Sensor_GetTemp(TC_COLD_JUNCTION),
		       modestr);
	}

	if (pReflow->numticks & 1) 
    {
		// Force UI refresh every other cycle
		Sched_SetState(MAIN_WORK, 2, 0);
	}

    // get current sched tick
	thistick = Sched_GetTick();
	if (lasttick == 0)
    {
		lasttick = thistick - TICKS_MS(PID_TIMEBASE);
	}

	nexttick = (2 * TICKS_MS(PID_TIMEBASE)) - (thistick - lasttick);
	if ((thistick - lasttick) > (2 * TICKS_MS(PID_TIMEBASE)))
    {
		printf("\nReflow can't keep up with desired PID_TIMEBASE!");
		nexttick = 0;
	}
	lasttick += TICKS_MS(PID_TIMEBASE);
    
    // return nexttick
	return nexttick;
}
/**/
/********************************************************************/


/*
 *
 */
void Reflow_Init(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    
    // schedule the reflow worker function...
	Sched_SetWorkfunc(REFLOW_WORK, Reflow_Work);
	//PID_init(&PID, 10, 0.04, 5, PID_Direction_Direct); // This does not reach the setpoint fast enough
	//PID_init(&PID, 30, 0.2, 5, PID_Direction_Direct); // This reaches the setpoint but oscillates a bit especially during cooling
	//PID_init(&PID, 30, 0.2, 15, PID_Direction_Direct); // This overshoots the setpoint
	//PID_init(&PID, 25, 0.15, 15, PID_Direction_Direct); // This overshoots the setpoint slightly
	//PID_init(&PID, 20, 0.07, 25, PID_Direction_Direct);
	//PID_init(&PID, 20, 0.04, 25, PID_Direction_Direct); // Improvement as far as I can tell, still work in progress
	PID_init(&pReflow->PID, 0, 0, 0, PID_Direction_Direct); // Can't supply tuning to PID_Init when not using the default timebase
	PID_SetSampleTime(&pReflow->PID, PID_TIMEBASE);
	PID_SetTunings(&pReflow->PID, 20, 0.016, 62.5); // Adjusted values to compensate for the incorrect timebase earlier
	//PID_SetTunings(&PID, 80, 0, 0); // This results in oscillations with 14.5s cycle time
	//PID_SetTunings(&PID, 30, 0, 0); // This results in oscillations with 14.5s cycle time
	//PID_SetTunings(&PID, 15, 0, 0);
	//PID_SetTunings(&PID, 10, 0, 0); // no oscillations, but offset
	//PID_SetTunings(&PID, 10, 0.020, 0); // getting there
	//PID_SetTunings(&PID, 10, 0.013, 0);
	//PID_SetTunings(&PID, 10, 0.0066, 0);
	//PID_SetTunings(&PID, 10, 0.2, 0);
	//PID_SetTunings(&PID, 10, 0.020, 1.0); // Experimental

	Reflow_LoadCustomProfiles();

	Reflow_ValidateNV();

	Reflow_LoadSetpoint();

	pReflow->PID.mySetpoint = (float)SETPOINT_DEFAULT;
	PID_SetOutputLimits(&pReflow->PID, 0, 255 + 248);
	PID_SetMode(&pReflow->PID, PID_Mode_Manual);
	pReflow->PID.myOutput = 248; // Between fan and heat
	PID_SetMode(&pReflow->PID, PID_Mode_Automatic);
	RTC_Zero();

	// Start work
	Sched_SetState(REFLOW_WORK, 2, 0);
}
/**/
/*********************************************************************/


/*
 *
 */
void Reflow_SetMode(ReflowMode_t themode) 
{
    _REFLOW_* pReflow = &g_Config.Reflow;    
    
	pReflow->mymode = themode;
	// reset reflowdone if mode is set to standby.
	if (themode == REFLOW_STANDBY)  {
		pReflow->reflowdone = 0;
	}
}
/**/
/**********************************************************/



/*
 *
 */
void Reflow_SetSetpoint(uint16_t thesetpoint)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    // set reflow points...
	pReflow->intsetpoint = thesetpoint;
	NV_SetConfig(REFLOW_BAKE_SETPOINT_H, (uint8_t)(thesetpoint >> 8));
	NV_SetConfig(REFLOW_BAKE_SETPOINT_L, (uint8_t)thesetpoint);
}
/**/
/**********************************************************/




/*
 *
 */
void Reflow_LoadSetpoint(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    // load reflow setpoint...
	pReflow->intsetpoint = NV_GetConfig(REFLOW_BAKE_SETPOINT_H) << 8;
	pReflow->intsetpoint |= NV_GetConfig(REFLOW_BAKE_SETPOINT_L);

	printf("\n bake setpoint values: %x, %x, %d\n",
		NV_GetConfig(REFLOW_BAKE_SETPOINT_H),
		NV_GetConfig(REFLOW_BAKE_SETPOINT_L), intsetpoint);
}
/**/
/************************************************************/



/*
 *
 */
int16_t Reflow_GetActualTemp(void)
{  
	int16_t retval = (int16_t)Sensor_GetTemp(TC_AVERAGE);
    
    return retval;
}
/**/
/*****************************************************************/


/*
 *
 */
uint8_t Reflow_IsDone(void) 
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    // return reflowdone..
	return pReflow->reflowdone;
}
/**/
/****************************************************************/



/*
 *
 */
uint16_t Reflow_GetSetpoint(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    // return intsetpoint
	return pReflow->intsetpoint;
}
/**/
/****************************************************************/



/*
 *
 */
void Reflow_SetBakeTimer(int seconds) 
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
	// reset ticks to 0 when adjusting timer.
	pReflow->numticks = 0;
	pReflow->bake_timer = (seconds * TICKS_PER_SECOND);
}
/**/
/******************************************************************/


/*
 *
 */
int Reflow_IsPreheating(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    // return if pre-heating...
	return ( (pReflow->bake_timer > 0) && (pReflow->avgtemp < pReflow->intsetpoint) );
}
/**/
/********************************************************************/



/*
 *
 */
int Reflow_GetTimeLeft(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    int retval = 0;
    
	if (pReflow->bake_timer == 0) {
		retval = -1;
	}
    else {
        retval = ( (pReflow->bake_timer - pReflow->numticks) / TICKS_PER_SECOND );
    }
    
    // return retval
    return retval;
}
/**/
/********************************************************************/


/*
 *
 */
// returns -1 if the reflow process is done.
int32_t Reflow_Run(uint32_t thetime, float meastemp, uint8_t* pheat, uint8_t* pfan, int32_t manualsetpoint)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
	int32_t retval = 0;
    uint8_t minfan = 0;
    
    
    // get minimum 'convection' fan speed
    minfan = NV_GetConfig(REFLOW_MIN_FAN_SPEED);
    

    // begin the reflow run entry...
	if (manualsetpoint)
    {
		pReflow->PID.mySetpoint = (float)manualsetpoint;

		if ( (pReflow->bake_timer > 0) && ( (Reflow_GetTimeLeft() == 0) || (Reflow_GetTimeLeft() == -1) ))
        {
			retval = -1;
		}
	} 
    else 
    {
		// Figure out what setpoint to use from the profile, brute-force way. Fix this.
		uint8_t idx = thetime / 10;
		uint16_t start = idx * 10;
		uint16_t offset = thetime - start;
		if (idx < (NUMPROFILETEMPS - 2))
        {
			uint16_t value = Reflow_GetSetpointAtIdx(idx);
			uint16_t value2 = Reflow_GetSetpointAtIdx(idx + 1);

			if (value > 0 && value2 > 0)
            {
				uint16_t avg = (value * (10 - offset) + value2 * offset) / 10;

				// Keep the setpoint for the UI...
				pReflow->intsetpoint = avg;
				if (value2 > avg) 
                {
					// Temperature is rising,
					// using the future value for PID regulation produces better result when heating
					pReflow->PID.mySetpoint = (float)value2;
				} 
                else
                {
					// Use the interpolated value when cooling
					pReflow->PID.mySetpoint = (float)avg;
				}
			} else {
				retval = -1;
			}
		} else {
			retval = -1;
		}
	}

	if (!manualsetpoint)
    {
		// Plot actual temperature on top of desired profile
		int realx = (thetime / 5) + XAXIS;
		int y = (uint16_t)(meastemp * 0.2f);
		y = YAXIS - y;
		LCD_SetPixel(realx, y);
	}

	pReflow->PID.myInput = meastemp;
	PID_Compute(&pReflow->PID);
	uint32_t out = pReflow->PID.myOutput;
	if (out < 248)
    { // Fan in reverse
		*pfan = 255 - out;
		*pheat = 0;
	} else {
		*pheat = out - 248;

		// When heating like crazy make sure we can reach our setpoint
		// if(*pheat>192) { *pfan=2; } else { *pfan=2; }

		// Run at a low fixed speed during heating for now
		*pfan = minfan;
	}
    
    // if still running, then set fan to 'minfan' speed
    if ( ( retval != -1) && (minfan > *pfan) )
        *pfan = minfan;
    
	return retval;
}
/**/
/********************************************************************/


/*
 *
 */
void Reflow_ToggleStandbyLogging(void) 
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    // toggle standby logging...
	pReflow->standby_logging = !pReflow->standby_logging;
}
/**/
/*********************************************************************/
