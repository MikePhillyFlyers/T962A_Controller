#ifndef MAX31855_H_
#define MAX31855_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */


// const defines...
#define INVALID_VALUE (999.0f)
#define MAX_SPI_DEVICES (8)             // 8 maximum SPI devices avail


// device fault bits
#define TC_FAULT_BIT    BIT_16          // fault bit
#define TC_SCV_BIT      BIT_2           // short to vcc fault
#define TC_SCG_BIT      BIT_1           // short to gnd fault
#define TC_OC_BIT       BIT_0           // open circuit fault
#define TC_ALL_FAULTS   (TC_SCV_BIT | TC_SCG_BIT | TC_OC_BIT)



/*  SPI tc devices
 *
 */
typedef struct
{
    int16_t spidevreadout;
    int16_t spiextrareadout;
} _SPI_DEVICE_;


// function declarations
uint32_t SPI_TC_Init(void);
int SPI_IsTCPresent(uint8_t tcid);


#endif /* MAX31855_H_ */
