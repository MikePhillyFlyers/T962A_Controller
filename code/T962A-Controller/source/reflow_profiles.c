#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "common_defs.h"
/* local defines */
#include "lcd.h"
#include "nvstorage.h"
#include "eeprom.h"
#include "reflow.h"
#include "reflow_profiles.h"




// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;
extern uint8_t graphbmp[];

// ********************************************** //
// ********************************************** //



#define NUM_STOCK_PROFILES      (7)         // *TOTAL* number of 'stock'/builtin profiles
#define NUM_CUSTOM_PROFILES     (10)        // *TOTAL* number of custom profiles currently
#define NUMPROFILES             (NUM_STOCK_PROFILES+NUM_CUSTOM_PROFILES)

// start index of STOCK/CUSTOM PROFILES
#define STOCK_PROFILE_INDEX     0
#define CUSTOM_PROFILE_INDEX    NUM_STOCK_PROFILES


// struct containing all of the 'custom' reflow profiles
ramprofile reflowprofiles[NUMPROFILES] =
{
    am4300profile,              // stock profile #1
    nc31profile,                // stock profile #2
    syntechlfprofile,           // stock profile #3
    wave3_lead_leadfree,        // stock profile #4
    wave8_lead_leadfree,        // stock profile #5
    rampspeed_testprofile,      // stock profile #6
    pidcontrol_testprofile,     // stock profile #7
    { .name = "CUSTOM #1" },    // custom profile #1
    { .name = "CUSTOM #2" },    // custom profile #2
    { .name = "CUSTOM #3" },    // custom profile #3
    { .name = "CUSTOM #4" },    // custom profile #4
    { .name = "CUSTOM #5" },    // custom profile #5
    { .name = "CUSTOM #6" },    // custom profile #6
    { .name = "CUSTOM #7" },    // custom profile #7
    { .name = "CUSTOM #8" },    // custom profile #8
    { .name = "CUSTOM #9" },    // custom profile #9
    { .name = "CUSTOM #10" },   // custom profile #10
};




/**
**  @brief ByteswapTempProfile the function to byte
**  swap the profile temp data
**	@return 
*/
static void ByteswapTempProfile(uint16_t* buf, uint16_t len)
{
    int i = 0;
    uint16_t word = 0;
    
    // byte swap the temp bytes
	for (i = 0; i < len; i++) {
		word = buf[i];
		buf[i] = ( (word >> 8) | (word << 8) );
	}
}
/**/
/**************************************************************8*/


/**
**  @brief Reflow_LoadCustomProfiles the function to load into memory
**  the user-defined/custom profiles from eeprom...
**	@return 
*/
void Reflow_LoadCustomProfiles(void)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint32_t startpos = 0;
    
    // iterate over the eeprom custom profiles, 
    // and read them in...
    for (i = 0, j = CUSTOM_PROFILE_INDEX; i < NUM_CUSTOM_PROFILES; i++, j++)
    {
        // read the profile from eeprom into ram buffer
        startpos = (PROFILES_START_ADDR + (i*NV_PAGE_SIZE));
        EEPROM_Read((uint8_t*)&reflowprofiles[j].temperatures, startpos, PROFILESIZE);
        ByteswapTempProfile(reflowprofiles[j].temperatures, NUMPROFILETEMPS);
    }	
}
/**/
/****************************************************************/


/**
**  @brief Reflow_ValidateNV the function to
**  validate the current NV data
**	@return 
*/
void Reflow_ValidateNV(void)
{
    // validate the 'beep length'
	if (NV_GetConfig(REFLOW_BEEP_DONE_LEN) == 255) {
		// Default 1 second beep length
		NV_SetConfig(REFLOW_BEEP_DONE_LEN, 10);
	}
    // validate the 'min fan speed'
	if (NV_GetConfig(REFLOW_MIN_FAN_SPEED) == 255) {
		// Default fan speed is now 8
		NV_SetConfig(REFLOW_MIN_FAN_SPEED, 8);
	}
    // validate the 'bake high' and 'bake low'
	if (NV_GetConfig(REFLOW_BAKE_SETPOINT_H) == 255 || NV_GetConfig(REFLOW_BAKE_SETPOINT_L) == 255) {
		NV_SetConfig(REFLOW_BAKE_SETPOINT_H, SETPOINT_DEFAULT >> 8);
		NV_SetConfig(REFLOW_BAKE_SETPOINT_L, (uint8_t)SETPOINT_DEFAULT);
		printf("Resetting bake setpoint to default.");
	}
    // select the NV profile
	Reflow_SelectProfileIdx(NV_GetConfig(REFLOW_PROFILE));
}
/**/
/****************************************************************************/


/**
**  @brief Reflow_GetProfileIdx the function to
**  return the current reflow index
**	@return 
*/
int Reflow_GetProfileIdx(void)
{
	return (g_Config.Reflow.profileidx);
}
/**/
/********************************************************************************/



/**
**  @brief Reflow_SelectProfileIdx the function to
**  select the correct profile index
**	@return 
*/
int Reflow_SelectProfileIdx(int idx)
{
    uint8_t* pindex = &g_Config.Reflow.profileidx;
    
    // check index for wrap arounds...
	if (idx < 0) {
		*pindex = (NUMPROFILES - 1);
	} else if(idx >= NUMPROFILES) {
		*pindex = 0;
	} else {
		*pindex = idx;
	}
    // update the NV storage with the currently
    // selected profile...
	NV_SetConfig(REFLOW_PROFILE, *pindex);
	return *pindex;
}
/**/
/***************************************************************/


/**
**  @brief Reflow_SelectEEProfileIdx the function to
**  select the correct eeprom index location
**	@return 
*/
int Reflow_SelectEEProfileIdx(int idx) 
{
    uint8_t* pindex = &g_Config.Reflow.profileidx;
    
    // if index is in the 'stock' profiles, then
    // just return 1st custom profile index
    if (idx < NUM_STOCK_PROFILES)
        *pindex = NUM_STOCK_PROFILES;
    
	return *pindex;
}
/**/
/*************************************************************************/



/**
**  @brief Reflow_GetEEProfileIdx the function to
**  check the index, if it's in 'custom' funcs,
**  then return 1.  Otherwise, return 0
**	@return 
*/
int Reflow_GetEEProfileIdx(void) 
{
    uint8_t profileidx = g_Config.Reflow.profileidx;

    // if index is in 'eeprom'/custom range, then
    // return 1, otherwise return 0
	if (profileidx >= NUM_STOCK_PROFILES) {
		return 1;
	} else {
		return 0;
	}
}
/**/
/*****************************************************************************/



/**
**  @brief Reflow_SaveEEProfile the function to
**  save the current custom profile to eeprom....
**	@return 
*/
int Reflow_SaveEEProfile(void)
{
	int retval = 0;
    uint16_t temperatures[NUMPROFILETEMPS] = {0};
    uint8_t profileidx = g_Config.Reflow.profileidx;
    uint8_t ee_index = (g_Config.Reflow.profileidx - NUM_STOCK_PROFILES);
    uint32_t startpos = (PROFILES_START_ADDR + (ee_index * NV_PAGE_SIZE));
    
    // just a sanity check to not go negative or out of bounds
    if ( (ee_index < 0) || (ee_index >= NUMPROFILES) ) {
        retval = -1;
        goto exit;    
    }
           
    // copy over the profile to temp buffer,
    // and write the buffer to the eeprom...
    memcpy(&temperatures, &reflowprofiles[profileidx].temperatures, PROFILESIZE);
    ByteswapTempProfile(temperatures, NUMPROFILETEMPS);
    retval = EEPROM_Write(startpos, (uint8_t*)temperatures, PROFILESIZE);       
exit:    
	return retval;
}
/**/
/*************************************************************************/


/**
**  @brief Reflow_ListProfiles the function to
**  print the profiles list to debug output...
**	@return 
*/
void Reflow_ListProfiles(void)
{
    int i = 0;    
    
	for (i = 0; i < NUMPROFILES; i++) {
		printf("%d: %s\n", i, profiles[i]->name);
	}
}
/**/
/******************************************************************************/



/**
**  @brief Reflow_GetProfileName the function to
**  return the current profile name
**	@return 
*/
const char* Reflow_GetProfileName(void)
{
    uint8_t profileidx = g_Config.Reflow.profileidx;
    
    // validate the current profile index
	return reflowprofiles[profileidx].name;
}
/**/
/******************************************************************************/



/**
**  @brief Reflow_GetSetpointAtIdx the function to
**  return the current temp point
**	@return 
*/
uint16_t Reflow_GetSetpointAtIdx(uint8_t idx)
{
    uint8_t profileidx = g_Config.Reflow.profileidx;
    uint16_t retval = 0;
    
    // if index is out of range, return 0
	if (idx > (NUMPROFILETEMPS - 1)) {
		retval = 0;
	}
    else {
        retval = reflowprofiles[profileidx].temperatures[idx];
    }        
        
	return retval;
}
/**/
/******************************************************************************/


/**
**  @brief Reflow_SetSetpointAtIdx the function to
**  set a point in the current profile
**	@return 
*/
void Reflow_SetSetpointAtIdx(uint8_t idx, uint16_t value) 
{
    uint8_t profileidx = g_Config.Reflow.profileidx;
    
    // check temp index, make sure it's valid...
	if (idx > (NUMPROFILETEMPS - 1)) { return; }
	if (value > SETPOINT_MAX) { return; }

    // read in temp value
	uint16_t* temp = (uint16_t*) &reflowprofiles[profileidx].temperatures[idx];
	if (temp >= (uint16_t*)RAM_BASE_ADDRESS) {
		*temp = value; // If RAM-based
	}
}
/**/
/*******************************************************************************/


/**
**  @brief Reflow_PlotProfile the function to
**  plot the current profile on the LCD display
**	@return 
*/
void Reflow_PlotProfile(int highlight)
{
    int x = 0;
    uint8_t profileidx = g_Config.Reflow.profileidx;
    
    // print the 'graph' bmp to the lcd display
	LCD_BMPDisplay(graphbmp, 0, 0);

	// No need to plot first value as it is obscured by Y-axis
	for(x = 1; x < NUMPROFILETEMPS; x++)
    {
		int realx = (x << 1) + XAXIS;
		int y = reflowprofiles[profileidx].temperatures[x] / 5;
		y = YAXIS - y;
		LCD_SetPixel(realx, y);

		if (highlight == x) {
			LCD_SetPixel(realx - 1, y - 1);
			LCD_SetPixel(realx + 1, y + 1);
			LCD_SetPixel(realx - 1, y + 1);
			LCD_SetPixel(realx + 1, y - 1);
		}
	}
}
/**/
/*************************************************************************/


/**
**  @brief Reflow_DumpProfile the function to
**  dump the current profile to dbg output
**	@return 
*/
void Reflow_DumpProfile(int profile)
{
    int i = 0;
    int current = 0;
    uint8_t profileidx = g_Config.Reflow.profileidx;
    
    // if out of range, bail out...
	if (profile > NUMPROFILES) {
		printf("\nNo profile with id: %d\n", profile);
		return;
	}

	current = profileidx;
	profileidx = profile;

    // iterate over the temps, and print the temp value...
	for (i = 0; i < NUMPROFILETEMPS; i++) 
    {
		printf("%4d,", Reflow_GetSetpointAtIdx(i));
		if (i == 15 || i == 31) {
			printf("\n ");
		}
	}
	printf("\n");
	profileidx = current;
}
/**/
/***************************************************************************/