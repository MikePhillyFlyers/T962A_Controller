#ifndef BUZZER_H_
#define BUZZER_H_



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <sys/attribs.h>



typedef enum eBuzzFreq {
	BUZZ_NONE = 0,
	BUZZ_2KHZ = 3,
	BUZZ_1KHZ = 4,
	BUZZ_500HZ = 5,
	BUZZ_250HZ = 6
} BuzzFreq_t;


/*   buzzer item structure
 *
 */
typedef struct
{
    BuzzFreq_t requested_buzz_freq;
    uint8_t requested_buzz_volume;
    int32_t requested_buzz_length;
} _BUZZER_;


void Buzzer_Init(void);
void Buzzer_Beep(BuzzFreq_t freq, uint8_t volume, int32_t ticks);

#endif /* BUZZER_H_ */
