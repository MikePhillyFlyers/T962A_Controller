#ifndef REFLOW_H_
#define REFLOW_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "PID_v1.h"



#define SETPOINT_MIN (30)
#define SETPOINT_MAX (300)
#define SETPOINT_DEFAULT (30)

// 36 hours max timer
#define BAKE_TIMER_MAX (60 * 60 * 36)




/* enum of the REFLOW STATES */
typedef enum eReflowMode 
{
	REFLOW_INITIAL=0,
	REFLOW_STANDBY,
	REFLOW_BAKE,
	REFLOW_REFLOW,
	REFLOW_STANDBYFAN
} ReflowMode_t;




/* reflow data structure
 * 
 */
typedef struct
{
    ReflowMode_t mymode;
    uint8_t reflowdone;    
    PidType PID;
    int standby_logging;
    int bake_timer;    
    uint16_t intsetpoint;
    uint16_t numticks;
    float avgtemp;    
    // current selected profile
    uint8_t profileidx;
    
} _REFLOW_;




// function declarations....
void Reflow_Init(void);
void Reflow_SetMode(ReflowMode_t themode);
void Reflow_SetSetpoint(uint16_t thesetpoint);
void Reflow_LoadSetpoint(void);
int16_t Reflow_GetActualTemp(void);
uint8_t Reflow_IsDone(void);
int Reflow_IsPreheating(void);
uint16_t Reflow_GetSetpoint(void);
void Reflow_SetBakeTimer(int seconds);
int Reflow_GetTimeLeft(void);
int32_t Reflow_Run(uint32_t thetime, float meastemp, uint8_t* pheat, uint8_t* pfan, int32_t manualsetpoint);
void Reflow_ToggleStandbyLogging(void);

#endif /* REFLOW_H_ */
