#ifndef KEYPAD_H_
#define KEYPAD_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "common_defs.h"




#define KEY_F1  (BIT_0)
#define KEY_F2  (BIT_1)
#define KEY_F3  (BIT_2)
#define KEY_F4  (BIT_3)
#define KEY_S   (BIT_4)
#define KEY_ANY (KEY_F1 | KEY_F2 | KEY_F3 | KEY_F4 | KEY_S)

uint32_t Keypad_Get(void);
uint32_t Keypad_GetRaw(void);
void Keypad_Init(void);

#endif /* KEYPAD_H_ */
