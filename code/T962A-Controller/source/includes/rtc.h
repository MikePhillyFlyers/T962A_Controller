#ifndef RTC_H_
#define RTC_H_



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */


// PIC32MZ RTC TIME MASKS
#define RTC_HOURS_TENS(value)      ( (value & (0x0F << 28)) >> 28 )
#define RTC_HOURS_ONES(value)      ( (value & (0x0F << 24)) >> 24 )
#define RTC_MINS_TENS(value)       ( (value & (0x0F << 20)) >> 20 )
#define RTC_MINS_ONES(value)       ( (value & (0x0F << 16)) >> 16 )
#define RTC_SECS_TENS(value)       ( (value & (0x0F << 12)) >> 12 )
#define RTC_SECS_ONES(value)       ( (value & (0x0F << 8)) >> 8 )

#define RTCINTDIV ((PCLKFREQ / 32768)-1)
#define RTCFRACDIV (PCLKFREQ-((RTCINTDIV+1)*32768))



// ** RTC functions **
void RTC_Init(void);
uint32_t RTC_Read(void);
void RTC_Zero(void);

// ** bcd functions **
uint8_t bcd2bin(uint8_t bcd);
uint8_t bin2bcd(uint8_t dec);



#endif /* RTC_H_ */
