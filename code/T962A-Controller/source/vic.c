/*
 * vic.c - Vectored Interrupt Controller interface for T-962 reflow controller
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
#include <cp0defs.h>
#include "common_defs.h"
/* local headers */
#include "vic.h"






/*
static void __attribute__ ((interrupt ("IRQ"))) VIC_Default_Handler( void )
{
	static uint32_t spuriousirq = 0;
	spuriousirq++;
	VICVectAddr = 0; // ACK irq
}
*/



void VIC_Init( void )
{
	//VICIntEnable = 0; // Make sure all interrupts are disabled
	//VICIntSelect = 0; // All interrupts are routed to IRQ (not FIQ)
	//VICDefVectAddr = (uint32_t)VIC_Default_Handler;
    
    return;
}



uint32_t VIC_IsIRQDisabled( void )
{
	uint32_t state;
	//asm("MRS %0,cpsr" : "=r" (state));
	//return !!(state&0x80);
}


/*
 *
 */
void VIC_DisableIRQ( void )
{   
    /* This will get the current setting, and then disable interrupts */
     /* Execution Hazard Barrier instruction, to make sure previous instruction take effect before proceeding. */
    __builtin_disable_interrupts();
    
	return;
}


/*
 *
 */
void VIC_RestoreIRQ( void )
{
    unsigned int saved_state = 0;
   
    saved_state = __builtin_get_isr_state();
    __builtin_set_isr_state(saved_state);    /* Set back to what was before. */ 
    
    return;
}
/**/
/***************************************************************/
