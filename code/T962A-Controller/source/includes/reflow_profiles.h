#ifndef REFLOW_PROFILES_H_
#define REFLOW_PROFILES_H_


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */




// Number of temperature settings in a reflow profile
#define NUMPROFILETEMPS         (48)
// Total size of a profile (ie 96 bytes)
#define PROFILESIZE             (NUMPROFILETEMPS * sizeof(uint16_t))

// resolution of profile temperature settings (in secs)
#define PROFILE_TEMP_TIMESCALE  (10)

#define YAXIS (57)
#define XAXIS (12)


// Amtech 4300 63Sn/37Pb leaded profile
#define am4300profile \
    { "4300 63SN/37PB", \
	{  50, 50, 50, 60, 73, 86,100,113,126,140,143,147,150,154,157,161,\
	   164,168,171,175,179,183,195,207,215,207,195,183,168,154,140,125,\
	   111, 97, 82, 68, 54, 50, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0\
	} \
    }

// NC-31 low-temp lead-free profile
#define nc31profile \
    { "NC-31 LOW-TEMP LF", \
    {	50, 50, 50, 50, 55, 70, 85, 90, 95,100,102,105,107,110,112,115, \
		117,120,122,127,132,138,148,158,160,158,148,138,130,122,114,106,\
		98, 90, 82, 74, 66, 58,  50, 50,  0,  0,  0,  0,  0,  0,  0,  0  \
	} \
    }

// SynTECH-LF normal temp lead-free profile
#define syntechlfprofile \
    { "AMTECH SYNTECH-LF", \
    {   50, 50, 50, 50, 60, 70, 80, 90,100,110,120,130,140,149,158,166, \
		175,184,193,201,210,219,230,240,245,240,230,219,212,205,198,191,\
		184,177,157,137,117, 97, 77, 57, 50, 50,  0,  0,  0,  0,  0,  0 \
	} \
    }

// T962A original 'WAVE3', generic lead/lead-free profile
#define wave3_lead_leadfree \
    { "WAVE3 LEAD/LEAD-FREE", \
    {   50, 60, 70, 80, 90, 100,110,120,130,135,140,145,145,155,155,155, \
		155,160,160,170,170,175,175,185,185,190,190,200,210,220,230,240, \
		250,250,245,240,235,230,210,190,170,150,130,110, 90, 80, 70, 50  \
	} \
    }

// T962A original 'WAVE8', generic lead/lead-free profile
#define wave8_lead_leadfree \
    { "WAVE8 LEAD/LEAD-FREE", \
    {   50, 50, 50, 65, 80, 95, 110,125,140,155,155,155,165,165,165,165, \
		165,165,170,185,200,215,230,245,248,245,230,215,200,185,170,155, \
		140,125,110, 95, 80, 65, 50, 50, 50,  0,  0,  0,  0,  0,  0,  0  \
	} \
    }

// Ramp speed test temp profile
#define rampspeed_testprofile \
    { "RAMP SPEED TEST", \
    {   50, 50, 50, 50,245,245,245,245,245,245,245,245,245,245,245,245, \
		245,245,245,245,245,245,245,245,245, 50, 50, 50, 50, 50, 50, 50,\
		 50, 50, 50, 50, 50, 50, 50, 50,  0,  0,  0,  0,  0,  0,  0,  0 \
	}\
    }

// PID gain adjustment test profile (5% setpoint change)
#define pidcontrol_testprofile \
   { "PID CONTROL TEST", \
   {	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171, \
		180,180,180,180,180,180,180,180,171,171,171,171,171,171,171,171, \
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  \
	}\
    }


// struct for profile
typedef struct 
{
	const char* name;
	const uint16_t temperatures[NUMPROFILETEMPS];
} profile;

// struct for profile
typedef struct
{
	const char* name;
	uint16_t temperatures[NUMPROFILETEMPS];
} ramprofile;





// function declarations
void Reflow_LoadCustomProfiles(void);
void Reflow_ValidateNV(void);
void Reflow_PlotProfile(int highlight);

int Reflow_GetProfileIdx(void);
int Reflow_SelectProfileIdx(int idx);
int Reflow_SelectEEProfileIdx(int idx);

int Reflow_GetEEProfileIdx(void);
int Reflow_SaveEEProfile(void);
void Reflow_ListProfiles(void);
const char* Reflow_GetProfileName(void);
uint16_t Reflow_GetSetpointAtIdx(uint8_t idx);
void Reflow_SetSetpointAtIdx(uint8_t idx, uint16_t value);
void Reflow_DumpProfile(int profile);


#endif /* REFLOW_PROFILES_H */
