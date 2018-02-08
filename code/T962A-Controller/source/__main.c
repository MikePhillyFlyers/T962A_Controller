



//#define SYSTEM_TESTS


//---------- start real program ---------



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <sys/attribs.h>
 /* custom includes */
#include "common_defs.h"
#include "initialize.h"
// T962A source headers
#include "buzzer.h"
#include "circbuffer.h"
#include "eeprom.h"
#include "i2c.h"
#include "lcd.h"
#include "main_work.h"
#include "onewire.h"
#include "rtc.h"
#include "schedule.h"
#ifdef SYSTEM_TESTS
#include "testing\__tests.h"
#endif
#include "timers.h"
#include "uarts.h"




/*** DEVCFG0 ***/
#pragma config DEBUG = OFF
#pragma config JTAGEN = OFF
#pragma config ICESEL = ICS_PGx1
#pragma config TRCEN = OFF
#pragma config BOOTISA = MIPS32
#pragma config FECCCON = OFF_UNLOCKED
#pragma config FSLEEP = OFF
#pragma config DBGPER = PG_ALL
#pragma config SMCLR = MCLR_NORM
#pragma config SOSCGAIN = GAIN_2X
#pragma config SOSCBOOST = ON
#pragma config POSCGAIN = GAIN_2X
#pragma config POSCBOOST = ON
#pragma config EJTAGBEN = NORMAL
#pragma config CP = OFF

/*** DEVCFG1 ***/
#pragma config FNOSC = SPLL	//SPLL
#pragma config DMTINTV = WIN_127_128
#pragma config FSOSCEN = OFF        // secondary oscillator
#pragma config IESO = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = OFF
#pragma config FCKSM = CSDCMD 		// ???? CSECME??
#pragma config WDTPS = PS4096       // 4sec timeout
#pragma config WDTSPGM = STOP
#pragma config FWDTEN = OFF
#pragma config WINDIS = NORMAL
#pragma config FWDTWINSZ = WINSZ_25
#pragma config DMTCNT = DMT31
#pragma config FDMTEN = OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV = DIV_3
#pragma config FPLLRNG = RANGE_5_10_MHZ
#pragma config FPLLICLK = PLL_POSC
#pragma config FPLLMULT = MUL_50
#pragma config FPLLODIV = DIV_2
#pragma config UPLLFSEL = FREQ_24MHZ

/*** DEVCFG3 ***/
#pragma config USERID = 0xffff
#pragma config FMIIEN = OFF
#pragma config FETHIO = ON
#pragma config PGL1WAY = OFF
#pragma config PMDL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = OFF

/*** BF1SEQ0 ***/
#pragma config TSEQ = 0xffff
#pragma config CSEQ = 0xffff





// ********************************************** //
// ****  global variables  declarations here **** //


extern uint8_t logobmp[];
extern uint8_t stopbmp[];
extern uint8_t selectbmp[];
extern uint8_t editbmp[];
extern uint8_t f3editbmp[];

// declare the main config global structure
_REFLOW_CONFIG_ g_Config = {0};


// ********************************************** //
// ********************************************** //





// No version.c file generated for LPCXpresso builds, fall back to this
__attribute__((weak)) const char* Version_GetGitVersion(void) {
	return "no version info";
}    

char* format_about = \
"\nT-962-controller open source firmware (%s)" \
"\n" \
"\nSee https://github.com/UnifiedEngineering/T-962-improvement for more details." \
"\n" \
"\nInitializing improved reflow oven...";

char* help_text = \
"\nT-962-controller serial interface.\n\n" \
" about                   Show about + debug information\n" \
" bake <setpoint>         Enter Bake mode with setpoint\n" \
" bake <setpoint> <time>  Enter Bake mode with setpoint for <time> seconds\n" \
" help                    Display help text\n" \
" list profiles           List available reflow profiles\n" \
" list settings           List machine settings\n" \
" quiet                   No logging in standby mode\n" \
" reflow                  Start reflow with selected profile\n" \
" setting <id> <value>    Set setting id to value\n" \
" select profile <id>     Select reflow profile by id\n" \
" stop                    Exit reflow or bake mode\n" \
" values                  Dump currently measured values\n" \
"\n";



/**
    This is the main program entry point.
	@param[in]     _inArg1 Description of first function argument.
	@param[out]    _outArg2 Description of second function argument.
	@param[in,out] _inoutArg3 Description of third function argument.
	@return Description of returned value.
*/
int main(int argc, char** argv) {
     
    uint32_t RetVal = 0;
    char buf[22];
 	int len = 0;

  
       
    
    
    
    /* initialize the pic32 chip */
    Initialize_Pic32();
    
    /* make sure DAC !CS is OFF */
    SPI_DAC_ENABLE(FALSE);
    
    
    // if testing is defined..
    #ifdef SYSTEM_TESTS
    delay_ms(1000);
    run_system_tests();
    #endif
    
   
    
    /************************************************************************************/
    /************************************************************************************/
    /*                                                                                  */
    /* ------------------ REAL MAIN PROGRAM STARTUP ----------------------------------- */
    /*                                                                                  */
    /*                                                                                  */
    /*                                                                                  */    
    /*                                                                                  */

	VIC_Init();
	Sched_Init();
	IO_Init();
	Set_Heater(0);
	Set_Fan(0);
	//Serial_Init();
	//printf(format_about, Version_GetGitVersion());

    // init eeprom & storage
	I2C_Init();
	EEPROM_Init();
	NV_Init();

    // init the LCD display
	LCD_Init();
	LCD_BMPDisplay(logobmp, 0, 0);

    // init/start watchdog
	IO_InitWatchdog();
	IO_PrintResetReason();

    // display 'micro/part' version info..
	len = IO_Partinfo(buf, sizeof(buf), "%s rev %s");
	LCD_disp_str((uint8_t*)buf, len, 0, 64 - 6, FONT6X6);
	printf("\nRunning on an %s", buf);

	//len = snprintf(buf, sizeof(buf), "%s", Version_GetGitVersion());
	//LCD_disp_str((uint8_t*)buf, len, 128 - (len * 6), 0, FONT6X6);

    // update the lcd buffer to display
	LCD_FB_Update();
    // init all of the peripherals, etc
	Keypad_Init();
	Buzzer_Init();
	RTC_Init();
	OneWire_Init();
	SPI_TC_Init();
	Reflow_Init();
	SystemFan_Init();

    // schedule the main work item...
	Sched_SetWorkfunc(MAIN_WORK, Main_Work);
	Sched_SetState(MAIN_WORK, 1, TICKS_SECS(2)); // Enable in 2 seconds

    // initial buzzer beep for program startup....
	Buzzer_Beep(BUZZ_1KHZ, 255, TICKS_MS(100));
    
    // do the initial program work, or sleep loop...
	while (1)
    {
        #ifdef ENABLE_SLEEP
        int32_t sleeptime;
        sleeptime = Sched_Do(0); // No fast-forward support
        //printf("\n%d ticks 'til next activity"),sleeptime);
        #else
        Sched_Do(0); // No fast-forward support
        #endif
	}
    
    // return 
	return 0;        
}
/**/
/************************************************************************/