//---------- start real program ---------


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"
#include "uarts.h" 
 
 



// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //










/*
 * 
 */
InitUART1(uint32_t uiBaudRate)
{    
    uint32_t uiCalcBaud = 0;

    
    // setup U1 UART
    //((PBCLK2 / 115200) / 16) - 1; // Set Baud rate
    U1MODE = 0x00;
    uiCalcBaud = (UARTS_CLK_RATE/16/uiBaudRate)-1;
    U1BRG = uiCalcBaud;           
    
    /* enable the Tx/Rx modules */
    /* default: RX IS OFF */
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXISEL0 = 0b01;    
    
    /* configure 8,N,1 */
    U1MODEbits.PDSEL = 0b00;        /* 8bits, no parity */
    U1MODEbits.STSEL = 0;           /* 1 stop bit */   
    
    /* turn on the module */
    U1MODEbits.ON = 1;              /* enable enable UART */	
	
	return;
}
/**/
/****************************************************************************************************/




/* AdjustUART1 - function to adjust UART1 baudrate
/* @params      uiBaudRate      parameter to specify desired 
**                              baudrate for ISO, ie 9600, 115200, etc
*/
AdjustUART1(uint32_t uiBaudRate)
{
    uint32_t uiCalcBaud = 0;        
    
	
	// setup U2 UART
    //((PBCLK2 / 115200) / 16) - 1; // Set Baud rate       
    uiCalcBaud = (UARTS_CLK_RATE/16/uiBaudRate)-1;	
    U1BRG = uiCalcBaud;           
        
	return;
}
/**/
/****************************************************************************************************/



/**
**  @brief ReceiveFromHost the function for receiving bytes
**         from the host.
**	@param[in]     _inArg1 Description of first function argument.
**	@return 
*/
uint32_t ReceiveFromHost(unsigned char* pRcvdChar)
{
    unsigned char ucRawChar = '\0';
    uint32_t uiReturnVal = 0;    
    
    
    /* if UART tx is busy transmitting, wait */
    while (U1STAbits.TRMT == 0) {
        Nop();
    };
    
    /* set 250ms timeout, no wait */    
    SetRunTimer89(250, FALSE);     
   
    /* timeout if nothing received */
    while (IFS0bits.T3IF == 0)
    {    
        /* if UART recv'd byte, get it */
        if (U1STAbits.URXDA) {            
            *pRcvdChar = U1RXREG;                   // get UART byte        
            uiReturnVal = 1;                        // return success
            break;
        }
    }
    
    /* return status */
    return uiReturnVal;        
};
/**/
/******************************************************************/




/**
**  @brief SendToHost the function for sending bytes
**         to the host.
**	@param[in]     _inArg1 Description of first function argument.
**	@param[out]    _outArg2 Description of second function argument.
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
SendToHost(unsigned char ucSendChar)
{      
    
    /* if UART tx is busy transmitting, wait */
    while (U1STAbits.TRMT == 0) {
        Nop();
    };         
    
    /* send out the char */
    U1TXREG = ucSendChar;   
    
    
    /* return status */
    return;        
};
/**/
/******************************************************************/


