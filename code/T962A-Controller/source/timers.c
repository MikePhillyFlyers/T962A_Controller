
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
#include <cp0defs.h>
/* custom includes */
#include "common_defs.h"
#include "timers.h" 
 
 






/**
**  @brief InitTimer23 the function for setting Timer2/3 and running it.
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return
*/
void InitTimer23()
{
    /* turn off Timer4 & 5 */
    T2CONbits.TON = 0;          /* timer2 OFF */    
    T3CONbits.TON = 0;          /* timer3 OFF */
    IFS0bits.T2IF = 0;          /* clear timer2 interrupt flag */
    IFS0bits.T3IF = 0;          /* clear timer3 interrupt flag */
    T2CONbits.TCS = 0;          /* internal PBCLK */
    T2CONbits.T32 = 1;          /* 32-bit timer */
    T2CONbits.TCKPS = 0b011;    /* 1:8 pre-scale */    

    T2CONbits.TON = 1;          /* timer2/3 ON */

    // return
    return;        
};
 /**/
 /***********************************************************************************/



/**
**  @brief InitTimer4_OCx the function for setting Timer4 and running it,
**  setup specific to the OCx PWM function...
**	@return
*/
void InitTimer4_OCx()
{
    /* turn off Timer6/7 */
    T4CONbits.TON = 0;          /* timer4 OFF */        
    IFS0bits.T4IF = 0;          /* clear timer4 interrupt flag */
    T4CONbits.TCS = 0;          /* internal PBCLK */
    T4CONbits.T32 = 0;          /* 16-bit timer */
    T4CONbits.TCKPS = 0b110;    /* 1:64 pre-scale */ 
    // 16-bit resolution, 6.127khz
    PR4 = OC1_CPUFAN_PERIOD;    /* 16-bits */

    // return
    return;        
};
 /**/
 /***********************************************************************************/


/**
**  @brief InitTimer67_OCx the function for setting Timer6/7 and running it,
**  setup specific to the OCx PWM function...
**	@return
*/
void InitTimer67_OCx()
{
    /* turn off Timer6/7 */
    T6CONbits.TON = 0;          /* timer6 OFF */        
    T7CONbits.TON = 0;          /* timer7 OFF */        
    IFS0bits.T6IF = 0;          /* clear timer6 interrupt flag */    
    IFS1bits.T7IF = 0;          /* clear timer7 interrupt flag */    
    T6CONbits.TCS = 0;          /* internal PBCLK */
    T6CONbits.T32 = 1;          /* 32-bit timer */
    T6CONbits.TCKPS = 0b111;    /* 1:256 pre-scale */ 
    // 16-bit resolution, 10hz
    PR6 = OC9_HEATER_PERIOD;    /* 16-bits */

    // return
    return;        
};
 /**/
 /***********************************************************************************/



/********************************************************************
**  ---------------  MILLISECOND TIMER --------------------------- **
**  @brief SetRunTimer89 the function for setting Timer8/9 and running it.
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return
*/
uint32_t SetRunTimer89(uint32_t uiTimeoutValue, bool bWaitOnTimer)
{
    uint32_t uiRetVal = 0;         
    
    /* turn off Timer8 & 9 */
    T8CONbits.TON = 0;          /* timer8 OFF */    
    T9CONbits.TON = 0;          /* timer9 OFF */
    IFS1bits.T8IF = 0;          /* clear timer8 interrupt flag */
    IFS1bits.T9IF = 0;          /* clear timer9 interrupt flag */
    T8CONbits.TCS = 0;          /* internal PBCLK */
    T8CONbits.T32 = 1;          /* 32-bit timer */
    T8CONbits.TCKPS = 0b110;    /* 1:64 pre-scale */    
    
    /* clear TIMER8 value */
    TMR8 = 0x00;        
    if (uiTimeoutValue == 0)
        goto exit;
    
    /* load Timer2 with specified value */   
    PR8 = (uiTimeoutValue * 1562);      
    
    /* turn *ON* timer 8/9!! */
    T8CONbits.TON = 1;          /* timer8/9 !!ON!! */
    
    // if waiting on the timer here, 
    // then loop until interrupt fires..
    if (bWaitOnTimer == TRUE)
    {
        /* wait for timer 9 to fire */
        while (IFS1bits.T9IF == 0 ) {
            Nop();
        }

        // timer 8/9 clear
        T8CONbits.TON = 0;     
        IFS1bits.T9IF = 0;    
    }
    
exit:
    // return
    return uiRetVal;    
};         
 /**/
 /***********************************************************************************/



/*
 *
 * *** NOTE ***:
 * This delay is SET based on CP0 Clk rate, which is SYSCLK/2, currently
 * 200Mhz/2.  If the SYSCLK is changed, the multiplier MUST be adjusted
 * below!!
 */
//void __longramfunc__ _Delay_uS(unsigned int Delay)
void _Delay_uS(uint32_t Delay)
{
	uint32_t Start;             // With unsigned arithmetic this should work
	uint32_t Wait;              // also when interval timer roll around.    

	Start = _CP0_GET_COUNT();	// MIPS Interval Counter
	Wait  =  (Delay * 100);     // Interval Counter ticks.
	
    // loop until counter matches
	while ((_CP0_GET_COUNT() - Start) < Wait);  // 
	return;
 }



/**
**  @brief function to delay xxx nano-seconds
**	@param[in]     _inArg1 Description of first function argument.
**	@return 
*/
void delay_ns(uint32_t ns)
{
    
    // nothing here as of now, ns delay not needed...
    
    return;
}
/**/
/***********************************************************************************/