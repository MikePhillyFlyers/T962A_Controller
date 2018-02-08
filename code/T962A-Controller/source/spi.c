//---------- start real program ---------


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"
#include "spi.h" 
#include "timers.h"
 
 


/* declare globals */





/**
**  @brief InitSPI2 the function for initialize SPI#2
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
InitSPI2(void)
{        
    uint32_t uiTemp = 0;
    
    
    // setup SPI BRG
    //((pbclk/( 2*( SPI1BRG+1)) // Set Baud rate
    SPI2CON = 0x00;
    SPI2CON2 = 0x00;
    
    /* clear any SPI2 ints */
    uiTemp = SPI2BUF;
    IFS4bits.SPI2EIF = 0;
    IFS4bits.SPI2TXIF = 0;
    IFS4bits.SPI2RXIF = 0;  
    
    /* setup SPI2 */
    SPI2CONbits.MODE32 = 1;
    SPI2CONbits.MODE16 = 0;
    SPI2CONbits.MSTEN = 1;           /* master mode (MSTEN = 1) */
    SPI2CONbits.DISSDO = 0;          /* SDO ouput enabled */
    SPI2CONbits.DISSDI = 0;          /* SDI input enabled */
    SPI2BRG = 0b1100;                /* 3.60Mhz baud rate */
    
    /* turn on the module */
    SPI2CONbits.ON = 1;              /* enable SPI2 */
    delay_ms(50);                    /* delay just for safe keeping */	
	
	return;
}
/**/
/****************************************************************************************************/




/**
**  @brief TransmitSPI2 the function to transmit out via SPI2
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
TransmitSPI2(uint32_t TransmitWord)
{        
    uint32_t uiRetVal = 0;
    uint32_t temp = 0;
    
    
    // if there is anything in Rx, empty now...
    if (SPI2STATbits.SPIRBE == 0) {
        temp = SPI2BUF;
    }
    
    /* wait for SPI buffer to be avail */
    while (SPI2STATbits.SPITBF == 1) {
        Nop();
    }

    /* wait for SPI to be idle */
    while (SPI2STATbits.SPIBUSY == 1) {
        Nop();
    }
    
    /* transmit the char */
    SPI2BUF = TransmitWord;
    
    /* wait for SPI shift to start */
    while (SPI2STATbits.SRMT == 1) {
        Nop();
    }
    
    /* wait for SPI shift to finish */
    while (SPI2STATbits.SRMT == 0) {
        Nop();
    }
    
    /* wait for SPI to be idle */
    while (SPI2STATbits.SPIBUSY == 1) {
        Nop();
    }
	
    // return
	return uiRetVal;
}
 /**/
 /*********************************************************************************************************/


/**
**  @brief ReceiveSPI2 the function to receive via SPI2
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
uint32_t ReceiveSPI2(uint8_t SensorNumber)
{        
    uint32_t ReturnWord = 0;
    int i = 0;
      
    
    /* wait for SPI to be idle */
    while (SPI2STATbits.SPIBUSY == 1) {
        Nop();
    } 
    
    // delay loop, waiting for rx
    for (i = 0; i < 10; i++)
    {
        /* wait for SPI buffer to be avail */
        if (SPI2STATbits.SPIRBF == 1) {
            ReturnWord = SPI2BUF;
            Nop();
            break;
        }
        // delay 1ms and try again
        delay_ms(1);   /* delay 1ms */
    }             
    
	// return char
	return ReturnWord;
}
 /**/
 /*********************************************************************************************************/



/**
**  @brief SetCoolFanSpeed the function to set the desired 8-bit speed
**  of the main/system cooling fan.
**	@return Description of returned value.
*/
void SetCoolFanSpeed(uint8_t FanSpeed)
{        
    uint32_t DacWriteCmd = 0;
    uint16_t cmd = 0;
    
   
    // populate the MCP4901 dac write cmd
    // ** set gain to: 1x
    // ** set buff to: buffered
    // ** shutdown mode:  DAC Active
    //cmd = (SPI_DAC_WRITE(TRUE) | SPI_DAC_BUFFERED(TRUE) | SPI_DAC_GAIN_1X(TRUE) | SPI_DAC_ACTIVE(TRUE));
    // ** WTF?  This chip setup is not working according to the datasheet,
    // **       ..these bit settings got it right...
    cmd = (0 << 15 | 0 << 14 | 1 << 13 | 0 << 12);
    cmd |= SPI_DAC_DATA(FanSpeed);
    
    // now create the 32-bit value to transmit via SPI in 32-bit mode...
    DacWriteCmd = (cmd << 16);
    
    // enable the SPI DAC CS
    // delay per chip spec (only 10ns really needed..)
    SPI_DAC_ENABLE(TRUE);
    delay_us(10);
    
    // send the new DAC value to the cool fan dac
    TransmitSPI2(DacWriteCmd);
    
    // disable the SPI DAC CS
    // delay per chip spec (only 10ns really needed..)
    delay_us(10);
    SPI_DAC_ENABLE(FALSE);
    delay_us(10);    
    
	// nothing to return
	return;
}
/**/
/*********************************************************************************************************/





/**
**  @brief TsSensorReceive the function to receive via SPI2
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
uint32_t TsSensorReceive(uint8_t SensorNumber)
{        
    uint32_t ReturnWord = 0;
    uint32_t uiRetVal = 0;
    int i = 0;
    
    
    // setup the Sensor Select
    // small delay for data hold...
    SPI_ADDR_SEL(SensorNumber);
    delay_us(10);
    
    // enable the cs demux
    // *MUST* be at least 100ns before SCK...
    SPI_CS_ENABLE(TRUE);
    delay_us(10);
    
    // send a dummy 32-bit word
    TransmitSPI2((uint32_t)-1);    
    
    // receive the TC value
    ReturnWord = ReceiveSPI2(SensorNumber);
    
    // disable the cs demux
    // small delay just to assure clear timing...
    SPI_CS_ENABLE(FALSE);  
    delay_us(10);
    
	// return char
	return ReturnWord;
}
 /**/
 /*********************************************************************************************************/