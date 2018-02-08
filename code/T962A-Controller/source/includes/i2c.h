#ifndef I2C_H_
#define I2C_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <string.h>
/* custom includes */
#include "common_defs.h"




// I2C READ/WRITE 'BIT 0' VALUE
#define I2C_READ_BIT0       TRUE        // for a read, bit0 of address is '1'
#define I2C_WRITE_BIT0      FALSE       // for a write, bit0 of address is '0'

#define I2CSTART            (0x08)
#define I2CRSTART           (0x10)

// LPC213x I2C status values
#define I2CWAACK            (0x18)
#define I2CWANOACK          (0x20)
#define I2CWDACK            (0x28)
#define I2CWDNOACK          (0x30)
#define I2CARBLOST          (0x38)
#define I2CRAACK            (0x40)
#define I2CRANOACK          (0x48)
#define I2CRDACK            (0x50)
#define I2CRDNOACK          (0x58)

// PIC32MZ I2C status values
#define I2C_NOACK_BIT       (BIT_15)    // acknowledge/no ack bit
#define I2C_XMT_BIT         (BIT_14)    // transmit bit
#define I2C_ACKTIM_BIT      (BIT_13)    // ack sequence timing bit
#define I2C_COLL_BIT        (BIT_10)    // collision detection bit
#define I2C_GENCALL_BIT     (BIT_9)     // general call status bit
#define I2C_ADD10_BIT       (BIT_8)     // 10-bit address status bit
#define I2C_WRCOLL_BIT      (BIT_7)     // write collision detection bit
#define I2C_RCVOVFW_BIT     (BIT_6)     // rcv overflow bit
#define I2C_DATADDR_BIT     (BIT_5)     // data/address bit
#define I2C_STOP_BIT        (BIT_4)     // stop bit
#define I2C_START_BIT       (BIT_3)     // start bit
#define I2C_RDWR_BIT        (BIT_2)     // read/write bit
#define I2C_RCVFULL_BIT     (BIT_1)     // rcv buffer full
#define I2C_XMTFULL_BIT     (BIT_0)     // transmit buffer full

// I2C 'REN/PEN' must-be-clear bits (ignore 'S' & 'P' bit??))
#define I2C_PEN_REN_CHECK   (BIT_0 | BIT_1 | BIT_2) // || BIT_3 | BIT_4)
#define I2C_READ            0x01        // read operation
#define I2C_WRITE           0x00        // write operation

// defines for the 'Atmel 24Cxx eeprom 
#define I2C_24CXX_ADDRESS   0x00                                        // in this case, device is at address '0'
#define I2C_EEPROM_ADDRESS  (0xA0 | I2C_24CXX_ADDRESS)                  // always high '0x0A' | address



// function declarations
void I2C_Init(void);
int32_t I2C_BusOperation(uint16_t slaveaddr, uint8_t* theBuffer, uint32_t theLength, uint8_t readwrite);

#endif /* I2C_H_ */
