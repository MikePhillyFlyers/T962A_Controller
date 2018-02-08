#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */


#ifndef SPI_H
#define SPI_H



// total # of delay loops before rx/tx timeout
#define MAX_SPI_WAITS       10

/* function to INIT SPI2 */
extern InitSPI2(void);

/* function to send out via SPI2 */
extern TransmitSPI2(uint32_t TransmitWord);

/* function to receive via SPI2 */
extern uint32_t ReceiveSPI2(uint8_t SensorNumber);

/* function to set the main cooling fan speed */
void SetCoolFanSpeed(uint8_t FanSpeed);

/**
**  @brief TsSensorReceive the function to receive via SPI2
**	@return Description of returned value.
*/
uint32_t TsSensorReceive(uint8_t SensorNumber);

#endif // TIMERS_H