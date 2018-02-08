#ifndef ONEWIRE_H_
#define ONEWIRE_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */




// only 1 one-wire device for this board!!!
#define MAX_OW_DEVICES      (1)

#define OW_SEARCH_ROM       (0xf0)
#define OW_READ_ROM         (0x33)
#define OW_MATCH_ROM        (0x55)
#define OW_SKIP_ROM         (0xcc)
#define OW_CONVERT_T        (0x44)
#define OW_WRITE_SCRATCHPAD (0x4e)
#define OW_COPY_SCRATCHPAD  (0x48)
#define OW_READ_SCRATCHPAD  (0xbe)
#define OW_READ_POWERSUPPLY (0xb4)
#define OW_FAMILY_TEMP1     (0x22) // DS1822
#define OW_FAMILY_TEMP2     (0x28) // DS18B20
#define OW_FAMILY_TC        (0x3b)


#define OW_SEARCH_FIRST     0xFF /* start new search */
#define OW_LAST_DEVICE      0x00 /* last device found */
#define OW_ROMCODE_SIZE     8    /* rom-code size including CRC */

#define OW_ERR_SUCCESS      0x00
#define OW_ERR_SHORT        0x01
#define OW_ERR_UART         0x02
#define OW_ERR_PRESENCE     0xFF
#define OW_ERR_DATA         0xFE



/*/
 *
 */
typedef struct
{
	// run-time info
    bool owdevicefound;                         // bool to indicate DS18B20 found
	int numowdevices;
	int8_t tcidmapping[16]; 					// Map TC ID to ROM ID index
	int8_t tempidx; 							// Which ROM ID index that contains the temperature sensor
	
	// one wire device info
	uint8_t owdeviceids[8];                     // uint64_t results in really odd code
	int16_t devreadout;                         // Keeps last readout
	int16_t extrareadout;                       // Keeps last readout		
	
} _ONEWIRE_;




// function declarations
uint32_t OneWire_Init( void );
int32_t OneWire_Work(void);
float OneWire_GetTempSensorReading(void);




#endif /* ONEWIRE_H_ */
