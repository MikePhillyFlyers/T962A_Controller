#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */


#ifndef INITIALIZE_H
#define INITIALIZE_H





/* function to initialize pic32 chip */
extern Initialize_Pic32(void);


/* function to init the on-board chips */
extern InitDevices(void);


// lock/unlock functions
extern void SystemLock();
extern void SystemUnLock();



#endif // INITIALIZE_H