
#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H




#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "buzzer.h"
#include "circbuffer.h"
#include "eeprom.h"
#include "i2c.h"
#include "lcd.h"
#include "main_work.h"
#include "max31855.h"
#include "nvstorage.h"
#include "onewire.h"
#include "reflow.h"
#include "rtc.h"
#include "sensor.h"
#include "serial.h"
#include "schedule.h"
#include "timers.h"
#include "uarts.h"




// disable printf statements
#define printf(...)


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef HI
#define HI 1
#endif

#ifndef LO
#define LO 0
#endif




/* lowest start address of pic32 'RAM' */
#define RAM_BASE_ADDRESS            0x80000000

/* pic3mz WATCHDOG KICK VALUE */
#define PIC32_WATCHDOG_KICK         0x5743


/* peripheral lock/unluck function */
#define PPSUnLock {SYSKEY=0x0;SYSKEY=0xAA996655;SYSKEY=0x556699AA;CFGCONbits.IOLOCK=0;} 
#define PPSLock {CFGCONbits.IOLOCK=1;SYSKEY=0x0;} 


/* setup defines for clk values */
#define SYS_FREQ                    200E6
#define PBCLK_SET_100MHZ            100E6
#define PBCLK_SET_200MHZ            200E6
#define OSC_CLK_RATE                24E6


/* setup defines to PERIPHERAL CLOCK rates */
#define PORTS_CLK_RATE              PBCLK_SET_200MHZ
#define TIMERS_CLK_RATE             PBCLK_SET_100MHZ
#define UARTS_CLK_RATE              PBCLK_SET_100MHZ



/* individual BIT defines */
#define BIT_0                       0b00000001
#define BIT_1                       0b00000010
#define BIT_2                       0b00000100
#define BIT_3                       0b00001000
#define BIT_4                       0b00010000
#define BIT_5                       0b00100000
#define BIT_6                       0b01000000
#define BIT_7                       0b10000000
/* bits 8 - 15 */
#define BIT_8                       0x0100
#define BIT_9                       0x0200
#define BIT_10                      0x0400
#define BIT_11                      0x0800
#define BIT_12                      0x1000
#define BIT_13                      0x2000
#define BIT_14                      0x4000
#define BIT_15                      0x8000
#define BIT_16                      0x10000


/* bit set/clr/inv functions */
#define PORTB_SETBITS(mask)         LATBSET=mask
#define PORTC_SETBITS(mask)         LATCSET=mask
#define PORTD_SETBITS(mask)         LATDSET=mask
#define PORTE_SETBITS(mask)         LATESET=mask
#define PORTF_SETBITS(mask)         LATFSET=mask
#define PORTG_SETBITS(mask)         LATGSET=mask

/* bit set/clr/inv functions */
#define PORTB_CLRBITS(mask)         LATBCLR=mask
#define PORTC_CLRBITS(mask)         LATCCLR=mask
#define PORTD_CLRBITS(mask)         LATDCLR=mask
#define PORTE_CLRBITS(mask)         LATECLR=mask
#define PORTF_CLRBITS(mask)         LATFCLR=mask
#define PORTG_CLRBITS(mask)         LATGCLR=mask

/* bit set/clr/inv functions */
#define PORTB_INVBITS(mask)         LATBINV=mask
#define PORTC_INVBITS(mask)         LATCINV=mask
#define PORTD_INVBITS(mask)         LATDINV=mask
#define PORTE_INVBITS(mask)         LATEINV=mask
#define PORTF_INVBITS(mask)         LATFINV=mask
#define PORTG_INVBITS(mask)         LATGINV=mask

/* TRIS set functions */
#define TRISB_SETBITS(mask)         TRISBSET=mask
#define TRISC_SETBITS(mask)         TRISCSET=mask
#define TRISD_SETBITS(mask)         TRISDSET=mask
#define TRISE_SETBITS(mask)         TRISESET=mask
#define TRISF_SETBITS(mask)         TRISFSET=mask
#define TRISG_SETBITS(mask)         TRISGSET=mask

/* TRIS clr functions */
#define TRISB_CLRBITS(mask)         TRISBCLR=mask
#define TRISC_CLRBITS(mask)         TRISCCLR=mask
#define TRISD_CLRBITS(mask)         TRISDCLR=mask
#define TRISE_CLRBITS(mask)         TRISECLR=mask
#define TRISF_CLRBITS(mask)         TRISFCLR=mask
#define TRISG_CLRBITS(mask)         TRISGCLR=mask

/* TRIS inv functions */
#define TRISB_INVBITS(mask)         TRISBINV=mask
#define TRISC_INVBITS(mask)         TRISCINV=mask
#define TRISD_INVBITS(mask)         TRISDINV=mask
#define TRISE_INVBITS(mask)         TRISEINV=mask
#define TRISF_INVBITS(mask)         TRISFINV=mask
#define TRISG_INVBITS(mask)         TRISGINV=mask


/* -------------------------------------------------------------- */
/* ---------------- BEGIN BOARD SPECIFIC DEFINES ---------------- */
/* -------------------------------------------------------------- */


/* LCD DISPLAY DEFINES */
#define LCD_RS_PIN(value)           ( (value==0) ? (PORTB_CLRBITS(BIT_2)) : (PORTB_SETBITS(BIT_2)) )
#define LCD_RW_PIN(value)           ( (value==0) ? (PORTB_CLRBITS(BIT_3)) : (PORTB_SETBITS(BIT_3)) )
#define LCD_EN_PIN(value)           ( (value==0) ? (PORTB_CLRBITS(BIT_4)) : (PORTB_SETBITS(BIT_4)) )
#define LCD_CS1_PIN(value)          ( (value==0) ? (PORTB_CLRBITS(BIT_13)) : (PORTB_SETBITS(BIT_13)) )
#define LCD_CS2_PIN(value)          ( (value==0) ? (PORTB_CLRBITS(BIT_14)) : (PORTB_SETBITS(BIT_14)) )
#define LCD_BACKLIGHT_PIN(value)    ( (value==0) ? (PORTB_CLRBITS(BIT_15)) : (PORTB_SETBITS(BIT_15)) )

/* LCD DATA BITS*/
#define LCD_DATA_BITS               (BIT_5 | BIT_6 | BIT_7 | BIT_8 | BIT_9 | BIT_10 | BIT_11 | BIT_12)
#define LCD_CLEAR_DATA              PORTB_CLRBITS(0xFF << 5);
#define LCD_DATA_INPUT              TRISB_SETBITS(0xFF << 5);
#define LCD_DATA_OUTPUT             TRISB_CLRBITS(0xFF << 5);
#define LCD_STATUS_BUSY             (PORTB & BIT_12)

/* CPU FAN */
#define CPUFAN_ENABLE(value)        ( (value==0) ? (PORTD_CLRBITS(BIT_0)) : (PORTD_SETBITS(BIT_0)) )

/* PIEZO BUZZER */
#define BUZZER_ENABLE(value)        ( (value==0) ? (PORTD_CLRBITS(BIT_3)) : (PORTD_SETBITS(BIT_3)) )

/* POWER LED */
#define POWER_LED_PORTBIT           BIT_4
#define POWER_LED_ENABLE(value)     ( (value==0) ? (PORTD_CLRBITS(POWER_LED_PORTBIT)) : (PORTD_SETBITS(POWER_LED_PORTBIT)) )  

/* DS18B20 1-WIRE PIN */
#define TEMP_SENSOR_DQ(value)       ( (value==0) ? (PORTD_CLRBITS(BIT_11)) : (PORTD_SETBITS(BIT_11)) )
#define TEMP_SENSOR_INPUT           TRISD_SETBITS(BIT_11);
#define TEMP_SENSOR_OUTPUT          TRISD_CLRBITS(BIT_11);
#define TEMP_SENSOR_DQ_STATE        (PORTD & BIT_11) 

/* keyboard inputs - PORTE[0-4]*/
#define KEYBOARD                    (BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4) 

/* temp sensor - SPI chip sel */
#define SPI_CS_ENABLE(value)        ( (value==0) ? (PORTG_CLRBITS(BIT_9)) : (PORTG_SETBITS(BIT_9)) ) 
#define SPI_ADDR_SEL(value)         ( (PORTE_CLRBITS(0x07 << 5)), (PORTE_SETBITS((value & 0x07) << 5)) )   

/* COOLING FAN DAC CONTROL */
#define SPI_DAC_ENABLE(value)       ( (value==0) ? (PORTD_SETBITS(BIT_2)) : (PORTD_CLRBITS(BIT_2)) )    
#define SPI_DAC_WRITE(value)        ( (value==0) ? (1 << 15) : (0 << 15) )  // write = 0
#define SPI_DAC_BUFFERED(value)     ( (value==0) ? (0 << 14) : (1 << 14) )  // buffered = 1
#define SPI_DAC_GAIN_1X(value)      ( (value==0) ? (0 << 13) : (1 << 13) )  // gain1x = 1
#define SPI_DAC_ACTIVE(value)       ( (value==0) ? (0 << 12) : (1 << 12) )  // active = 1
#define SPI_DAC_DATA(value)         ( (value & 0xFF) << 4)                  // data = bits [11:4]


// TEMP sensor enums
enum {
    SPI_SENSOR_ONE = 0,     // TS#2 = 0b000
    SPI_SENSOR_TWO,         // TS#2 = 0b001
    SPI_SENSOR_THREE,       // TS#3 = 0b010
    SPI_SENSOR_FOUR,        // TS#4 = 0b011
    SPI_SENSOR_FIVE,        // TS#5 = 0b100
    SPI_SENSOR_SIX,         // TS#6 = 0b101
    SPI_SENSOR_SEVEN,       // TS#7 = 0b110
    SPI_SENSOR_EIGHT        // TS#8 = 0b111    
} MAX38155_SENSORS;

/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */


// define for the UART1-SERIAL (ext dbg/comm interface)
// *baudrate*
//#define SERIAL_2MBs
#ifdef SERIAL_2MBs
#define SERIAL_BAUDRATE 2000000
#else
#define SERIAL_BAUDRATE 115200
#endif


// runtime timer tick register
#define RUNTIME_TICK        TMR2

// OC1,OC9 period defines
#define OC1_CPUFAN_PERIOD   0xFF            // 255 8-bit PR value
#define OC9_HEATER_PERIOD   0x9896          // 39026 - 16-bit PR value
#define OC9_HEATER_MULTIPLIER   (OC9_HEATER_PERIOD/255)


// define **IF USING** interrupts for keypad...
// #define KEYBOARD_INTERRUPTS

// define if 'LCD BUSY' polling needed
// #define LCD_BUSY_POLLING

/*
 *
 */
/* REFLOW OVEN GLOBAL CONFIG DATA */
typedef struct
{
    // buzzer globals
    _BUZZER_ Buzzer;    
    
    // keypad keys
    uint32_t latchedkeypadstate;
    
    // LCD data
    _LCD_DATA_ LcdData;
    
    // NV Storage
    _NV_STORAGE_ NvStorage;
    
    // reflow data
    _REFLOW_ Reflow;
    
    // schedule data
    _SCHEDULE_ Schedule;

    // serial cmd port
    _SERIAL_ SerialPort;
    
    // temperature sensors/data
    int numSensorsFound;
    _TEMP_SENSORS_ TempSensors;

} _REFLOW_CONFIG_;
/**/
/**************************************************/



#endif // COMMON_DEFS_H