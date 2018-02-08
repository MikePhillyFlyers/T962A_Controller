#ifndef NVSTORAGE_H_
#define NVSTORAGE_H_



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */



// --------------------------------------------- //
//          EEPROM LAYOUT                        //
//                                               //
// 'CONFIG PAGE' = 256 bytes (extra in case move to 5 sec spacing)
//
// PG0:  ***CONFIG ITEMS***
// PG1:  CUSTOM PROFILE#1
// PG2:  CUSTOM PROFILE#2
// PG3:  CUSTOM PROFILE#3
// PG4:  CUSTOM PROFILE#4...
// .
// .
// .
// PG31  CUSTOM PROFILE#31

// magic eeprom header identifier, 
// ie 'UE' for UnifiedEngineering
#define NVMAGIC          (('E' << 8) | 'U')          // 'UE' == "UNIFIED ENGINEERING" :)
#define NVMAGIC_SIZE     (2)                         // size of this 'UE' ident
#define NV_PAGE_SIZE     (256)                       // 256 bytes per 'nv' storage page
#define TOTAL_NV_PAGES   EEPROM_SIZE/NV_PAGE_SIZE    // total num of nv pages in eeprom

// start pages for 'config' and 'custom profiles'
#define NV_START_ADDR       (0 * NV_PAGE_SIZE)  // eeprom start addr of 'config/nv' data
#define PROFILES_START_ADDR (1 * NV_PAGE_SIZE)  // eeprom start addr of 'custom profiles'

//
// --------------------------------------------- //
// --------------------------------------------- //



// Right now we only have 28 bytes in total for NV storage, and 3 of those bytes
// are used up for housekeeping so max 25 items will fit
// Only append to the end of this list to avoid backwards incompatibilities
typedef enum eNVItem 
{
	REFLOW_BEEP_DONE_LEN=0,
	REFLOW_PROFILE=1,
//	TC_LEFT_GAIN,
//	TC_LEFT_OFFSET,
//	TC_RIGHT_GAIN,
//	TC_RIGHT_OFFSET,
	REFLOW_MIN_FAN_SPEED,
	REFLOW_BAKE_SETPOINT_H,
	REFLOW_BAKE_SETPOINT_L,
	NVITEM_NUM_ITEMS // Last value
} NVItem_t;


// 'UE' signature bytes
typedef uint16_t MAGIC;
typedef struct __attribute__ ((__packed__)) 
{
    MAGIC magic;
    uint8_t numitems;
    uint8_t config[NVITEM_NUM_ITEMS];
} NV_t;


typedef struct __attribute__ ((__packed__)) 
{
    NV_t MyNV;
    uint8_t spare[NV_PAGE_SIZE - sizeof(NV_t) - sizeof(MAGIC)];
    MAGIC magic;     // ending 'UE' signature
} NV_Config_Page;


/* NV storage structure
 * 
 */
typedef struct
{
    NV_t myNV;
    uint8_t nvupdatepending;
} _NV_STORAGE_;



// function declarations
void NV_Init(void);
uint8_t NV_GetConfig(NVItem_t item);
void NV_SetConfig(NVItem_t item, uint8_t value);
int32_t NV_Work(void);

#endif /* NVSTORAGE_H_ */
