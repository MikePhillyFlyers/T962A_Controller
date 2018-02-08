//---------- start real program ---------


#include <xc.h>
#include <cp0defs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
/* custom includes */
#include "initialize.h"
#include "common_defs.h"
#include "p32mz2048efh064.h"








/**
**  @brief Initialize_Pic32 function to initialize the PIC32
**	@return Description of returned value.
*/
Initialize_Pic32(void)
{      
    
    // Unlock Sequence
    //_asm
    asm("di");         // disable interrupts
    //#endasm        
   
    
    // unlock the PPS lock
    PPSUnLock;   
    
    /* enable predictive prefetch for any address
     * Flash Wait State = 2 */
    PRECONbits.PREFEN = 3;
    PRECONbits.PFMWS = 2;   
    
    // setup Output Compares to use Timer45/67 for OCx
    CFGCONbits.OCACLK = 1;
    
	 // PB1DIV (100Mhz)
	PB1DIVbits.PBDIV = 0b00000001;  // Peripheral Bus 1 Clock Divisor Control (PBCLK1 is SYSCLK divided by 2)

	// PBCLK2 = UARTS (100Mhz)
	PB2DIVbits.ON = 1;              // Peripheral Bus 2 Output Clock Enable (Output clock is enabled)
  	while(!PB2DIVbits.PBDIVRDY); 	// Wait for allowed write.
  	PB2DIVbits.PBDIV = 0b00000001; 	// Peripheral Bus 2 Clock Divisor Control (PBCLK2 is SYSCLK divided by 2)
    
    
    // PBCLK3 = TIMERS (100Mhz)
	PB3DIVbits.ON = 1;              // Peripheral Bus 3 Output Clock Enable (Output clock is enabled)
  	while(!PB3DIVbits.PBDIVRDY); 	// Wait for allowed write.
  	PB3DIVbits.PBDIV = 0b00000001; 	// Peripheral Bus 3 Clock Divisor Control (PBCLK3 is SYSCLK divided by 2)
    

	// PB4DIV (PORTS = 200Mhz)
  	PB4DIVbits.ON = 1;              // Peripheral Bus 4 Output Clock Enable (Output clock is enabled)
  	while(!PB4DIVbits.PBDIVRDY); 	// Wait for allowed write.
  	PB4DIVbits.PBDIV = 0b00000000; 	// Peripheral Bus 4 Clock Divisor Control (PBCLK4 is SYSCLK divided by 1)

	// configure PMD1, enable none
	PMD1bits.ADCMD		= 1;
	PMD1bits.CVRMD		= 1;

	// configure PMD2, enable none
	PMD2bits.CMP1MD		= 1;
	PMD2bits.CMP2MD		= 1;

	// configure PMD3
	PMD3bits.IC1MD		= 1;
	PMD3bits.IC2MD		= 1;
	PMD3bits.IC3MD		= 1;
	PMD3bits.IC4MD		= 1;
	PMD3bits.IC5MD		= 1;
	PMD3bits.IC6MD		= 1;
	PMD3bits.IC7MD		= 1;
	PMD3bits.IC8MD		= 1;
	PMD3bits.IC9MD		= 1;
	PMD3bits.OC1MD		= 0;        /* ENABLE OC1 */
	PMD3bits.OC2MD		= 1;
	PMD3bits.OC3MD		= 1;
	PMD3bits.OC4MD		= 1;
	PMD3bits.OC5MD		= 1;
	PMD3bits.OC6MD		= 1;
	PMD3bits.OC7MD		= 1;
	PMD3bits.OC8MD		= 1;
	PMD3bits.OC9MD		= 0;        /* ENABLE OC9 */
    
	// configure PMD4, enable *ALL TIMERS*
	PMD4bits.T1MD		= 1;
	PMD4bits.T2MD		= 0;
	PMD4bits.T3MD		= 0;
	PMD4bits.T4MD		= 0;
	PMD4bits.T5MD		= 0;
	PMD4bits.T6MD		= 0;
	PMD4bits.T7MD		= 0;
	PMD4bits.T8MD		= 0;
	PMD4bits.T9MD		= 0;

	// configure PMD5, enable UART1, SPI2
	PMD5bits.U1MD		= 0;        /* ENABLE UART1 */
	PMD5bits.U2MD		= 1;
	PMD5bits.U3MD		= 1;
	PMD5bits.U4MD		= 1;
	PMD5bits.U5MD		= 1;
	PMD5bits.U6MD		= 1;
	PMD5bits.SPI1MD		= 1;
	PMD5bits.SPI2MD		= 0;        /* ENABLE SPI2 */
	PMD5bits.SPI3MD		= 1;
	PMD5bits.SPI4MD		= 1;        
//	PMD5bits.SPI5MD		= 1;        /* not on 64pin chip */
//	PMD5bits.SPI6MD		= 1;        /* not on 64pin chip */
	PMD5bits.I2C1MD		= 1;
//	PMD5bits.I2C2MD		= 1;        /* not on 64pin chip */
	PMD5bits.I2C3MD		= 1;
	PMD5bits.I2C4MD		= 1;
	PMD5bits.I2C5MD		= 0;        /* ENABLE I2C5*/
	PMD5bits.USBMD		= 1;
	PMD5bits.CAN1MD		= 1;
	PMD5bits.CAN2MD		= 1;

	// configure PMD6, enable none
	PMD6bits.RTCCMD		= 0;        /* enable RTC module */
	PMD6bits.REFO1MD	= 1;
	PMD6bits.REFO2MD	= 1;
	PMD6bits.REFO3MD	= 1;
	PMD6bits.REFO4MD	= 1;
	PMD6bits.PMPMD		= 1;
//	PMD6bits.EBIMD		= 1;        /* not on 64pin chip */
	PMD6bits.SQI1MD		= 1;
	PMD6bits.ETHMD		= 1;

	// configure PMD7, enable
	PMD7bits.DMAMD		= 1;
	PMD7bits.RNGMD		= 1;  

	// configure ANSEL bits
//	ANSELA = 0;                     /* not on 64pin chip */
	ANSELB = 0; 
//	ANSELC = 0; 
//	ANSELD = 0; 
	ANSELE = 0; 
//	ANSELF = 0; 
	ANSELG = 0; 

	// configure port values to '0'
//	LATA = 0x0000;                  /* not on 64pin chip */
	LATB = 0x0000;
	LATC = 0x0000;
	LATD = 0x0000;
	LATE = 0x0000;
	LATF = 0x0000;
	LATG = 0x0000;
    
    // configure all ports as startup outputs
	TRISB = 0;
	TRISC = 0;
	TRISD = 0;
	TRISE = 0;
	TRISF = 0;
	TRISG = 0;	

	// set output drain config	
	ODCB = 0;
	ODCC = 0;
	ODCD = 0;
	ODCE = 0;
	ODCF = 0;
	ODCG = 0;
    
    // clear all int enables
	IEC0SET		= 0;
	IEC1SET		= 0;
	IEC2SET		= 0;
	IEC3SET		= 0;
	IEC4SET		= 0;
	IEC5SET		= 0;
	IEC6SET		= 0;	 
    
   
    // go init all the PPS/IO/etc...
    InitDevices(); 

	// lock the PPS back up
    PPSLock;
    

    return;        
}
/**/
/********************************************************************************/



 /* InitDevices */
/* 
**
**  
*/
InitDevices(void)
{    
    uint32_t temp = 0;
    
    // ***************** DS18B20 - COLD JUNC. SENSOR **************** //
       
    TRISDbits.TRISD11 = 1;      // input RD11
    ODCDbits.ODCD11 = 1;        // PD11: open drain    
    
    // **************** I2C INTERFACE ***************************** //
    
    ODCFbits.ODCF4 = 1;         // PF4:  I2C5-DATA open drain        
    ODCFbits.ODCF5 = 1;         // PF5:  I2C5-CLK  open drain      
    
    // **************** SPI INTERFACE ***************************** //
    // setup SPI - (VIA SPI2)
    SDI2R = 0b0001;             // enable RG7 for SDI2
    TRISGbits.TRISG7 = 1;       // SDI input RG7
    RPG8R = 0b0110;             // enable RG8 for SDO2
    
    // **************** OUTPUT COMPARE 1, 9 *********************** //
    // setup OC1
    RPD0R = 0b1100;             // enable RD0 for OC1
    // setup OC9
    RPD1R = 0b1101;             // enable RD1 for OC9   
    
    // ************* UART1 - DEBUG OUTPUT/CONSOLE INPUT *********** //
    // setup UART1 RX/TX
    
    // setup U1TX
    ODCFbits.ODCF0 = 1;         // PF0:  U1TX open drain        
    RPF0R = 0b0001;             // enable RF0 for U1TX
    // setup U1RX
    TRISFbits.TRISF1 = 1;       // PF1:  input RF1
    U1RXR = 0b0100;             // enable RF1 for U1RX
    
    // **************** KEYPAD INPUTS ***************************** //
    
    // porte, set open drain (not needed on inputs, but who cares)
    ODCEbits.ODCE0 = 1;         // PE0: keypad - 'F1' key   
    ODCEbits.ODCE1 = 1;         // PE0: keypad - 'F2' key   
    ODCEbits.ODCE2 = 1;         // PE0: keypad - 'F3' key   
    ODCEbits.ODCE3 = 1;         // PE0: keypad - 'F4' key   
    ODCEbits.ODCE4 = 1;         // PE0: keypad - 'S' key     
    
    // keyboard bits = input
    TRISEbits.TRISE0 = 1;       // keypad 'F1' key
    TRISEbits.TRISE1 = 1;       // keypad 'F2' key
    TRISEbits.TRISE2 = 1;       // keypad 'F3' key
    TRISEbits.TRISE3 = 1;       // keypad 'F4' key
    TRISEbits.TRISE4 = 1;       // keypad 'S' key
    
    
    /* Assign PIC32MZ shadow register sets to specific CPU IPLs */
    #ifdef KEYBOARD_INTERRUPTS
    PRISS = 0x76543210;    
    /* Set Interrupt Controller for multi-vector mode */
    INTCONSET = _INTCON_MVEC_MASK;    
    
    // enable PORTE I/O trig bits
    CNENEbits.CNIEE0 = 1;
    CNENEbits.CNIEE1 = 1;
    CNENEbits.CNIEE2 = 1;
    CNENEbits.CNIEE3 = 1;
    CNENEbits.CNIEE4 = 1;
    
    IPC30bits.CNEIP = 7;        // Interrupt priority 7   
    IFS3bits.CNEIF = 0;         // Clear the interrupt status flag
    IEC3bits.CNEIE =  1;        // Enable CN interrupts 
    CNCONEbits.EDGEDETECT = 1;  // Port Edge Detects (in CNFE reg)
    temp = PORTE;
    Nop();
    CNCONEbits.ON = 1;
    #endif
    

    // ************** REAL TIME CLOCK (EXTERNAL) ****************** //
    
    // initialize SOSCI/O ports
    // as 'inputs' for no loading
    TRISCbits.TRISC13 = 1;          // SOSCI input RC13
    TRISCbits.TRISC14 = 1;          // SOSCO input RC14 *** see PIC32MZEFH 'ERRATA'
                                    //                  *** for details on why SOSCO is used...
    /* enable the RTC clock */
    RTCCONbits.RTCWREN = 1;         /* rtc write enable */ 
    RTCCONbits.RTCCLKSEL = 0b01;    /* external 32.768khz clock */
    RTCCONbits.ON = 1;              /* turn on the clock */
    RTCCONbits.RTCWREN = 0;         /* rtc write disable */
    
    // return...
	return;
}
/**/
/****************************************************************************************************/



/*----------------------------------------------------------------------------
    SystemUnlock() perform a system unlock sequence
  --------------------------------------------------------------------------*/
void SystemUnLock()
{
    SYSKEY = 0;				// ensure OSCCON is locked
    SYSKEY = 0xAA996655;	// Write Key1 to SYSKEY
    SYSKEY = 0x556699AA;	// Write Key2 to SYSKEY
}

/*----------------------------------------------------------------------------
    SystemLock() relock OSCCON by relocking the SYSKEY
  --------------------------------------------------------------------------*/
void SystemLock()
{
    SYSKEY = 0x12345678;	// Write any value other than Key1 or Key2
}

