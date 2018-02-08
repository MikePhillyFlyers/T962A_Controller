/*
 * io.c - I/O handling for T-962 reflow controller
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
/* local includes */
#include "io.h"
#include "schedule.h"
#include "vic.h"






// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //







/**
**  @brief Set_Heater the function for turning on/off the heater
**	@param[in,out] 
**	@return Description of returned value.
*/
void Set_Heater(uint8_t enable)
{
   uint16_t pwm_value = 0; 
    
    // HEATER PWM==> PD2 via OC3
    pwm_value = (uint16_t)enable;
    if (enable == 0xff)
        pwm_value = 0x100;
     
	/* NOTE: pic32 PWM outputs are LOW until match, 
       LPC213x PWM outputs are HI until match...
       i.e. - no need to reverse for pic32...
       pic32 'fully on' == OCRx > PRx
    */
    OC9RS = (pwm_value * OC9_HEATER_MULTIPLIER);
    
    return;
}
/**/
/************************************************************************8*/




/**
**  @brief Set_Fan the function for turning on/off the main cooling fan
**	@param[in,out] 
**	@return Description of returned value.
*/
void Set_Fan(uint8_t enable)
{   
    // set the cooling fan speed
    SetCoolFanSpeed(enable);
    
    return;
}
/**/
/*******************************************************************8*/



/**
**  @brief Set_Fan the function for turning on/off the main cooling fan
**	@param[in,out] 
**	@return Description of returned value.
*/
static int32_t Sleep_Work(void)
{
    // flip the power led state (on/off)
    PORTD_INVBITS(POWER_LED_PORTBIT);

	// If interrupts are used they must be disabled around the following two instructions!
    #ifdef KEYBOARD_INTERRUPTS
	VIC_DisableIRQ();
    #endif

    // kick the watchdog...
	WDTCONbits.WDTCLRKEY = PIC32_WATCHDOG_KICK;
    
    #ifdef KEYBOARD_INTERRUPTS
	VIC_RestoreIRQ(save);
    #endif

    // return next tick count..
	return TICKS_SECS(1);
}
/**/
/*****************************************************************/



/*
 * 
 */
void IO_InitWatchdog(void) 
{
	// enable watchdog
	WDTCONbits.ON = 1;
    return;
}
/**/
/**********************************************************************/



/*
 *
 */
void IO_PrintResetReason(void)
{
	//uint8_t resetreason = RSIR;
    uint8_t resetreason = 0xaa;
	//RSIR = 0x0f; // Clear it out
	printf(
	       "\nReset reason(s): %s%s%s%s",
	       (resetreason & (1 << 0)) ? "[POR]" : "",
	       (resetreason & (1 << 1)) ? "[EXTR]" : "",
	       (resetreason & (1 << 2)) ? "[WDTR]" : "",
	       (resetreason & (1 << 3)) ? "[BODR]" : "");
}
/**/
/************************************************************8*/



/*
 *
 */
int IO_Partinfo(char* buf, int n, char* format)
{
	uint32_t partrev;
    int i = 0;

    /*
	// Request part number
	command[0] = IAP_READ_PART;
	iap_entry((void *)command, (void *)result);
	const char* partstrptr = NULL;
	for (i = 0; i < NUM_PARTS; i++) {
		if (result[1] == partmap[i].id) {
			partstrptr = partmap[i].name;
			break;
		}
	}

	// Read part revision
	partrev = *(uint8_t*)PART_REV_ADDR;
	if (partrev == 0 || partrev > 0x1a) {
		partrev = '-';
	} else {
		partrev += 'A' - 1;
	}
	return snprintf(buf, n, format, partstrptr, (int)partrev);
    */
    
    // return pic32mz info
   return snprintf(buf, n, format, "PIC32MZ2048EFH", "A1");
}
/**/
/****************************************************************/

/*
 *
 */
void IO_JumpBootloader(void)
{
	/* Hold F1-Key at boot to force ISP mode */
    
    // nothing yet for doing this type of ISP mode...
    
    
    return;
}
/**/
/************************************************************/


/*
 *
 */
void IO_Init(void)
{   
    // pic32 params:
    // --------------
    // Tpb = 1/100Mhz = 10ns
    // Pre-Scale = 1:256
    // Period Value: 0xFF (255) (50% duty)
    
    
    /* initialize SPI2 */
    InitSPI2();
    
    /* initialize UART1 */
    InitUART1(SERIAL_BAUDRATE);
    
    /* initialize the cool fan dac for pwm ctrl */
    SPI_DAC_ENABLE(FALSE);
    
    // init the OCx timers...
    InitTimer4_OCx();
    InitTimer67_OCx();
    
     // --------------------- OC1 - CPU FAN ------------ //
    // setup OC1 params
    OC1CONbits.OCM = 0b110;		// mode: PWM, no fault
    OC1CONbits.OCTSEL = 0;		// timer4 select
    OC1CONbits.OC32 = 0;		// 16-bit
    // setup compare values
    OC1R = 0x000;               // 50% duty
    OC1RS = 0x000;              // 50% duty    
    // ------------------------------------------------ //    
    
    // --------------------- OC9 - HEATER ------------- //
    // setup OC2 params
    OC9CONbits.OCM = 0b110;		// mode: PWM, no fault
    OC9CONbits.OCTSEL = 0;		// timer3 select
    OC9CONbits.OC32 = 1;		// 32-bit
    // setup compare values
    OC9R = 0x000;               // 50% duty
    OC9RS = 0x000;              // 50% duty    
    // ------------------------------------------------ //
    
    // turn on Timer4,6/7 and OC1,OC9 modules
    T4CONbits.TON = 1;          /* timer4 ON */
    T6CONbits.TON = 1;          /* timer6 ON */
    OC1CONbits.ON = 1;          // turn on OC1 module
    OC9CONbits.ON = 1;          // turn on OC9 module
        
    // schedule the sleep work function
	Sched_SetWorkfunc(SLEEP_WORK, Sleep_Work);
	Sched_SetState(SLEEP_WORK, 2, 0);
}
/**/
/*******************************************************************/