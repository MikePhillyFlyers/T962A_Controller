/*
 * sched.c - Simple scheduling for T-962 reflow controller
 *
 * Copyright (C) 2011,2012,2014 Werner Johansson, wj@unifiedengineering.se
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



/**
**  @brief Sched_Init the function 
**	@param[in,out] 
**	@return Description of returned value.
*/
void Sched_Init(void)
{
    // init and run 32-bit timer2/3
    InitTimer23();
}
/**/
/***********************************************************************************/




/**
**  @brief Sched_GetTick the function
**	@param[in,out] 
**	@return Description of returned value.
*/
uint32_t Sched_GetTick(void)
{
	// get tick timer count
    return RUNTIME_TICK;
}
/**/
/***********************************************************************************/





// The enable is atomic but the dueTicks are not, so only call this with 0 or 2 as enable parameter
// (2 will force scheduling as soon as possible without relying on dueTicks to be correct)
void Sched_SetState(Task_t tasknum, uint8_t enable, int32_t future)
{
	if (enable == 1)
    {
		g_Config.Schedule.tasks[tasknum].dueTicks = future;
	}
	g_Config.Schedule.tasks[tasknum].enabled = enable;
}
/**/
/***********************************************************************************/





/**
**  @brief Sched_IsOverride the function
**	@param[in,out] 
**	@return Description of returned value.
*/
uint8_t Sched_IsOverride(void)
{
	uint8_t retval = 0; // No override by default
    uint8_t lp = 0;
    
    // iterate over tasks
	for (lp = 0; lp < SCHED_NUM_ITEMS; lp++) 
    {
		if (g_Config.Schedule.tasks[lp].enabled == 2) {
			retval = 1;
			//if(SelectiveDebugIsEnabled(SD_SCHED_OVERRIDE)) wjprintf_P(PSTR("\nTask 0x%x overrides sleep!"), lp);
		}
	}
	return retval;
}
/**/
/***********************************************************************************/




/**
**  @brief Sched_SetWorkfunc the function
**	@param[in,out] 
**	@return Description of returned value.
*/
void Sched_SetWorkfunc(Task_t tasknum, SchedCall_t func)
{
	g_Config.Schedule.tasks[tasknum].workFunc = func;
}
/**/
/***********************************************************************************/




/**
**  @brief Sched_Do the function
**	@param[in,out] 
**	@return Description of returned value.
*/
int32_t Sched_Do(uint32_t fastforward) 
{
	static uint32_t oldTick = 0;
	int32_t shortestwait = 0x7fffffff;
	uint32_t curTick = Sched_GetTick();
    uint8_t lp = 0;
    
    

	// How many ticks will we should roll forward (including sleep time)
	uint32_t numRollFwd = (curTick - oldTick) + fastforward;
	oldTick = curTick;

    // iterate over tasks
	for (lp = 0; lp < SCHED_NUM_ITEMS; lp++)
    {
		if (g_Config.Schedule.tasks[lp].enabled >= 1)
        { 
            // Only deal with enabled tasks
			uint32_t tmp = g_Config.Schedule.tasks[lp].dueTicks;
			if ( (g_Config.Schedule.tasks[lp].enabled == 2) || (tmp <= numRollFwd) )
            {   
                // Time to execute this task
				int32_t nextdelta = g_Config.Schedule.tasks[lp].workFunc(); // Call the scheduled work
				if (nextdelta >= 0)
                {   
                    // Re-arm
					tmp = nextdelta;
					g_Config.Schedule.tasks[lp].enabled = 1;
				}
                else
                {   
                    // Putting task to sleep until awakened by Sched_SetState
					g_Config.Schedule.tasks[lp].enabled = 0;
				}
			} 
            else
            {
				tmp -= numRollFwd;
			}
			g_Config.Schedule.tasks[lp].dueTicks = tmp;
			if (tmp < shortestwait) {
				shortestwait = tmp;
			}
		}
	}
	// Unless a (wake-up) interrupt calls Sched_SetState, this is how
	// long it's OK to sleep until next task is due
	return shortestwait;
}
/**/
/***********************************************************************************/


/**
**  @brief BusyWait the function
**	@param[in,out] 
**	@return Description of returned value.
*/
void BusyWait( uint32_t numticks )
{
    IFS0CLR =_IFS0_T3IF_MASK;
    //IFS0bits.T3IF = 0;	
    PR2 = (1 + TMR2 + numticks);        // It's perfectly fine if this wraps
	while (IFS0bits.T3IF == 0 )         // wait for match    
    return;
}
/**/
/***********************************************************************************/
