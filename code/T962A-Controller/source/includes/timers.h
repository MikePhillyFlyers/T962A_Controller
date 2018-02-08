#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <sys/attribs.h>
#include "common_defs.h"


#ifndef TIMERS_H
#define TIMERS_H




/* FUNCTION FOR 'BUSY_WAIT' function */
void InitTimer23();

/* function to delay xxxx */
void InitTimer4_OCx();
void InitTimer67_OCx();


/* FUNCTION FOR 'USEC DELAY */
void _Delay_uS(uint32_t Delay);
#define delay_us(value) _Delay_uS(value);

/* FUNCTION FOR 'MSEC DELAY */
uint32_t SetRunTimer89(uint32_t uiTimeoutValue, bool bWaitOnTimer);
#define delay_ms(value) SetRunTimer89(value, TRUE);



#endif // TIMERS_H