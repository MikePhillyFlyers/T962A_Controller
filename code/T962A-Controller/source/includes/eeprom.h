#ifndef EEPROM_H_
#define EEPROM_H_

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"



// ------------ SIZE OF EEPROM ------------ //
#define EEPROM_SIZE         8192        // size in bytes (64 Kbit)
#define EEPROM_PAGE_SIZE    32          // eeprom page size (in bytes)



// function declarations
void EEPROM_Init(void);
void EEPROM_Dump(void);
void EEPROM_Initialize(void);
int32_t EEPROM_Read(uint8_t* dest, uint32_t startpos, uint32_t len);
int32_t EEPROM_Write(uint32_t startdestpos, uint8_t* src, uint32_t len);

#endif /* EEPROM_H_ */
