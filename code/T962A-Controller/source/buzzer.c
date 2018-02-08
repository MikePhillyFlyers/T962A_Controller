/*
 * buzzer.c - Simple buzzer interface for T-962 reflow controller
 *
 * Copyright (C) 2011,2014 Werner Johansson, wj@unifiedengineering.se
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
#include "buzzer.h"
#include "schedule.h"


// ********************************************** //
// ****  global variables  declarations here **** //

BuzzFreq_t requested_buzz_freq;
uint8_t requested_buzz_volume;
int32_t requested_buzz_length;

// ********************************************** //
// ********************************************** //


/*
 * The buzzer is hooked up to PWM5 output, but contains an oscillator of
 * its own so volume and freq are ignored for now. :(
 */
static int32_t Buzzer_Work(void)
{
	if (requested_buzz_freq != BUZZ_NONE) {
		//FIO0SET = (1 << 21);
        BUZZER_ENABLE(TRUE);
		requested_buzz_freq = BUZZ_NONE;
	} else {
		// Don't schedule until next beep is requested
		requested_buzz_length = -1;
		//FIO0CLR = (1 << 21);
        BUZZER_ENABLE(LO);
	}
	return requested_buzz_length;
}
/**/
/***********************************************************************************/


/**
**  @brief Buzzer_Init
**	@param[in,out]
**	@return 
*/
void Buzzer_Init(void)
{
	printf("\n%s ", __FUNCTION__);

	Sched_SetWorkfunc(BUZZER_WORK, Buzzer_Work);
}
/**/
/***********************************************************************************/


/**
**  @brief Buzzer_Beep
**	@param[in,out]
**	@return 
*/
void Buzzer_Beep(BuzzFreq_t freq, uint8_t volume, int32_t ticks) 
{
	if (ticks > 0 || freq == BUZZ_NONE)
    {
		requested_buzz_freq = freq;
		requested_buzz_volume = volume;
		requested_buzz_length = ticks;
		Sched_SetState(BUZZER_WORK, 2, 0);
	}
}
/**/
/***********************************************************************************/
