/*
 * buzzer.c - Simple buzzer interface for T-962 reflow controller
 *
 * Copyright (C) 2011,2014 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include <sys/attribs.h>
#include "common_defs.h"
// local defines
#include "buzzer.h"
#include "keypad.h"
#include "lcd.h"
#include "main_work.h"
#include "nvstorage.h"
#include "reflow.h"
#include "schedule.h"
#include "sensor.h"



// ********************************************** //
// ****  global variables  declarations here **** //

// declare externs....
extern uint8_t logobmp[];
extern uint8_t stopbmp[];
extern uint8_t selectbmp[];
extern uint8_t editbmp[];
extern uint8_t f3editbmp[];

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //





/**
**  @brief Main_Work
**	@param[in,out]
**	@return 
*/
int32_t Main_Work(void)
{
	static MainMode_t mode = MAIN_HOME;
	static uint16_t setpoint = 0;
    static int timer = 0;
    // profile editing
	static uint8_t profile_time_idx = 0;
	static uint8_t current_edit_profile = 0;
	int32_t retval = 0;
    uint32_t keyspressed = 0;
    char serial_cmd[255] = "";
	char* cmd_select_profile = "select profile %d";
	char* cmd_bake = "bake %d %d";
	char* cmd_dump_profile = "dump profile %d";
	char* cmd_setting = "setting %d %f";
	char buf[22] = {0};
	int len = 0;
    int i = 0;
    
    
    
    
	if (setpoint == 0) {
		Reflow_LoadSetpoint();
		setpoint = Reflow_GetSetpoint();
	}		

    #ifdef KEYPAD_INTERRUPTS
    // disable ints...
    __builtin_enable_interrupts();
    #endif
   
    // get keys pressed...
    retval = TICKS_MS(500);
	keyspressed = Keypad_Get();

    /*
	if (uart_isrxready())
    {
		int len = uart_readline(serial_cmd, 255);

		if (len > 0) {
			int param, param1;
			float paramF;

			if (strcmp(serial_cmd, "about") == 0) {
				printf(format_about, Version_GetGitVersion());
				len = IO_Partinfo(buf, sizeof(buf), "\nPart number: %s rev %c\n");
				printf(buf);
				EEPROM_Dump();

				printf("\nSensor values:\n");
				Sensor_ListAll();

			} else if (strcmp(serial_cmd, "help") == 0 || strcmp(serial_cmd, "?") == 0) {
				printf(help_text);

			} else if (strcmp(serial_cmd, "list profiles") == 0) {
				printf("\nReflow profiles available:\n");

				Reflow_ListProfiles();
				printf("\n");

			} else if (strcmp(serial_cmd, "reflow") == 0) {
				printf("\nStarting reflow with profile: %s\n", Reflow_GetProfileName());
				mode = MAIN_HOME;
				// this is a bit dirty, but with the least code duplication.
				keyspressed = KEY_S;

			} else if (strcmp(serial_cmd, "list settings") == 0) {
				printf("\nCurrent settings:\n\n");
				for (int i = 0; i < Setup_getNumItems() ; i++) {
					printf("%d: ", i);
					Setup_printFormattedValue(i);
					printf("\n");
				}

			} else if (strcmp(serial_cmd, "stop") == 0) {
				printf("\nStopping bake/reflow");
				mode = MAIN_HOME;
				Reflow_SetMode(REFLOW_STANDBY);
				retval = 0;

			} else if (strcmp(serial_cmd, "quiet") == 0) {
				Reflow_ToggleStandbyLogging();
				printf("\nToggled standby logging\n");

			} else if (strcmp(serial_cmd, "values") == 0) {
				printf("\nActual measured values:\n");
				Sensor_ListAll();
				printf("\n");

			} else if (sscanf(serial_cmd, cmd_select_profile, &param) > 0) {
				// select profile
				Reflow_SelectProfileIdx(param);
				printf("\nSelected profile %d: %s\n", param, Reflow_GetProfileName());

			} else if (sscanf(serial_cmd, cmd_bake, &param, &param1) > 0) {
				if (param < SETPOINT_MIN) {
					printf("\nSetpoint must be >= %ddegC\n", SETPOINT_MIN);
					param = SETPOINT_MIN;
				}
				if (param > SETPOINT_MAX) {
					printf("\nSetpont must be <= %ddegC\n", SETPOINT_MAX);
					param = SETPOINT_MAX;
				}
				if (param1 < 1) {
					printf("\nTimer must be greater than 0\n");
					param1 = 1;
				}

				if (param1 < BAKE_TIMER_MAX) {
					printf("\nStarting bake with setpoint %ddegC for %ds after reaching setpoint\n", param, param1);
					timer = param1;
					Reflow_SetBakeTimer(timer);
				} else {
					printf("\nStarting bake with setpoint %ddegC\n", param);
				}

				setpoint = param;
				Reflow_SetSetpoint(setpoint);
				mode = MAIN_BAKE;
				Reflow_SetMode(REFLOW_BAKE);

			} else if (sscanf(serial_cmd, cmd_dump_profile, &param) > 0) {
				printf("\nDumping profile %d: %s\n ", param, Reflow_GetProfileName());
				Reflow_DumpProfile(param);

			} else if (sscanf(serial_cmd, cmd_setting, &param, &paramF) > 0) {
				Setup_setRealValue(param, paramF);
				printf("\nAdjusted setting: ");
				Setup_printFormattedValue(param);

			} else {
				printf("\nCannot understand command, ? for help\n");
			}
		}
	}
    */
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- MAIN SETUP ---------------------------------- //
    // --------------------------------------------------------------------- //
    //
	// main menu state machine
	if (mode == MAIN_SETUP) 
    {
		static uint8_t selected = 0;
		int y = 0;

		int keyrepeataccel = keyspressed >> 17; // Divide the value by 2
		if (keyrepeataccel < 1) keyrepeataccel = 1;
		if (keyrepeataccel > 30) keyrepeataccel = 30;

		if (keyspressed & KEY_F1)
        {
			if (selected > 0) { // Prev row
				selected--;
			} else { // wrap
				selected = Setup_getNumItems() - 1;
			}
		}
		if (keyspressed & KEY_F2)
        {
			if (selected < (Setup_getNumItems() - 1)) { // Next row
				selected++;
			} else { // wrap
				selected = 0;
			}
		}

		if (keyspressed & KEY_F3) {
			Setup_decreaseValue(selected, keyrepeataccel);
		}
		if (keyspressed & KEY_F4) {
			Setup_increaseValue(selected, keyrepeataccel);
		}

		LCD_FB_Clear();
		len = snprintf(buf, sizeof(buf), "Setup/calibration");
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), y, FONT6X6);
		y += 7;

		for (i = 0; i < Setup_getNumItems() ; i++) 
        {
			len = Setup_snprintFormattedValue(buf, sizeof(buf), i);
			LCD_disp_str((uint8_t*)buf, len, 0, y, FONT6X6 | (selected == i) ? INVERT : 0);
			y += 7;
		}

		// buttons
		y = 64 - 7;
		LCD_disp_str((uint8_t*)" < ", 3, 0, y, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)" > ", 3, 20, y, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)" - ", 3, 45, y, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)" + ", 3, 65, y, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)" DONE ", 6, 91, y, FONT6X6 | INVERT);

		// Leave setup
		if (keyspressed & KEY_S) {
			mode = MAIN_HOME;
			Reflow_SetMode(REFLOW_STANDBY);
			retval = 0; // Force immediate refresh
		}
	}
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //    
    
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- MAIN ABOUT- --------------------------------- //
    // --------------------------------------------------------------------- //
    //
    else if (mode == MAIN_ABOUT) 
    {
		LCD_FB_Clear();
		LCD_BMPDisplay(logobmp, 0, 0);

		len = snprintf(buf, sizeof(buf), "T-962 controller");
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), 0, FONT6X6);

		len = snprintf(buf, sizeof(buf), "%s", Version_GetGitVersion());
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), 64 - 6, FONT6X6);

		LCD_BMPDisplay(stopbmp, 127 - 17, 0);

		// Leave about with any key.
		if (keyspressed & KEY_ANY) {
			mode = MAIN_HOME;
			retval = 0; // Force immediate refresh
		}
	} 
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //
    
    
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- MAIN REFLOW --------------------------------- //
    // --------------------------------------------------------------------- //
    //
    else if (mode == MAIN_REFLOW)
    {
		uint32_t ticks = RTC_Read();

		len = snprintf(buf, sizeof(buf), "%03u", Reflow_GetSetpoint());
		LCD_disp_str((uint8_t*)"SET", 3, 110, 7, FONT6X6);
		LCD_disp_str((uint8_t*)buf, len, 110, 13, FONT6X6);

		len = snprintf(buf, sizeof(buf), "%03u", Reflow_GetActualTemp());
		LCD_disp_str((uint8_t*)"ACT", 3, 110, 20, FONT6X6);
		LCD_disp_str((uint8_t*)buf, len, 110, 26, FONT6X6);

		len = snprintf(buf, sizeof(buf), "%03u", (unsigned int)ticks);
		LCD_disp_str((uint8_t*)"RUN", 3, 110, 33, FONT6X6);
		LCD_disp_str((uint8_t*)buf, len, 110, 39, FONT6X6);

		// Abort reflow
		if (Reflow_IsDone() || keyspressed & KEY_S) {
			printf("\nReflow %s\n", (Reflow_IsDone() ? "done" : "interrupted by keypress"));
			if (Reflow_IsDone()) {
				Buzzer_Beep(BUZZ_1KHZ, 255, TICKS_MS(100) * NV_GetConfig(REFLOW_BEEP_DONE_LEN));
			}
			mode = MAIN_HOME;
			Reflow_SetMode(REFLOW_STANDBY);
			retval = 0; // Force immediate refresh
		}
	} 
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //
    
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- SELECT PROFILE ------------------------------ //
    // --------------------------------------------------------------------- //
    //
    else if (mode == MAIN_SELECT_PROFILE) 
    {
		int curprofile = Reflow_GetProfileIdx();
		LCD_FB_Clear();

		// Prev profile
		if (keyspressed & KEY_F1) {
			curprofile--;
		}
		// Next profile
		if (keyspressed & KEY_F2) {
			curprofile++;
		}
        // select the current profile
		Reflow_SelectProfileIdx(curprofile);

		Reflow_PlotProfile(-1);
		LCD_BMPDisplay(selectbmp, 127 - 17, 0);
		int eeidx = Reflow_GetEEProfileIdx();
		if (eeidx) { // Display edit button
			LCD_BMPDisplay(f3editbmp, 127 - 17, 29);
		}
		len = snprintf(buf, sizeof(buf), "%s", Reflow_GetProfileName());
		LCD_disp_str((uint8_t*)buf, len, 13, 0, FONT6X6);

		if (eeidx && keyspressed & KEY_F3) { // Edit ee profile
			mode = MAIN_EDIT_PROFILE;
			current_edit_profile = eeidx;
			retval = 0; // Force immediate refresh
		}

		// Select current profile
		if (keyspressed & KEY_S) {
			mode = MAIN_HOME;
			retval = 0; // Force immediate refresh
		}

	} 
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //
    
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- BAKE MODE ----------------------------------- //
    // --------------------------------------------------------------------- //
    //
    else if (mode == MAIN_BAKE)
    {
		LCD_FB_Clear();
		LCD_disp_str((uint8_t*)"MANUAL/BAKE MODE", 16, 0, 0, FONT6X6);

		int keyrepeataccel = keyspressed >> 17; // Divide the value by 2
		if (keyrepeataccel < 1) keyrepeataccel = 1;
		if (keyrepeataccel > 30) keyrepeataccel = 30;

		// Setpoint-
		if (keyspressed & KEY_F1) {
			setpoint -= keyrepeataccel;
			if (setpoint < SETPOINT_MIN) setpoint = SETPOINT_MIN;
		}

		// Setpoint+
		if (keyspressed & KEY_F2) {
			setpoint += keyrepeataccel;
			if (setpoint > SETPOINT_MAX) setpoint = SETPOINT_MAX;
		}

		// timer --
		if (keyspressed & KEY_F3) {
			if (timer - keyrepeataccel < 0) {
				// infinite bake
				timer = -1;
			} else {
				timer -= keyrepeataccel;
			}
		}
		// timer ++
		if (keyspressed & KEY_F4) {
			timer += keyrepeataccel;
		}

		int y = 10;
		// display F1 button only if setpoint can be decreased
		char f1function = ' ';
		if (setpoint > SETPOINT_MIN) {
			LCD_disp_str((uint8_t*)"F1", 2, 0, y, FONT6X6 | INVERT);
			f1function = '-';
		}
		// display F2 button only if setpoint can be increased
		char f2function = ' ';
		if (setpoint < SETPOINT_MAX) {
			LCD_disp_str((uint8_t*)"F2", 2, LCD_ALIGN_RIGHT(2), y, FONT6X6 | INVERT);
			f2function = '+';
		}
		len = snprintf(buf, sizeof(buf), "%c SETPOINT %d` %c", f1function, (int)setpoint, f2function);
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), y, FONT6X6);


		y = 18;
		if (timer == 0) {
			len = snprintf(buf, sizeof(buf), "inf TIMER stop +");
		} else if (timer < 0) {
			len = snprintf(buf, sizeof(buf), "no timer    stop");
		} else {
			len = snprintf(buf, sizeof(buf), "- TIMER %3d:%02d +", timer / 60, timer % 60);
		}
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), y, FONT6X6);

		if (timer >= 0) {
			LCD_disp_str((uint8_t*)"F3", 2, 0, y, FONT6X6 | INVERT);
		}
		LCD_disp_str((uint8_t*)"F4", 2, LCD_ALIGN_RIGHT(2), y, FONT6X6 | INVERT);

		y = 26;
		if (timer > 0)
        {
			int time_left = Reflow_GetTimeLeft();
			if (Reflow_IsPreheating()) {
				len = snprintf(buf, sizeof(buf), "PREHEAT");
			} else if (Reflow_IsDone() || time_left < 0) {
				len = snprintf(buf, sizeof(buf), "DONE");
			} else {
				len = snprintf(buf, sizeof(buf), "%d:%02d", time_left / 60, time_left % 60);
			}
			LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_RIGHT(len), y, FONT6X6);
		}

		len = snprintf(buf, sizeof(buf), "ACT %3.1f`", Sensor_GetTemp(TC_AVERAGE));
		LCD_disp_str((uint8_t*)buf, len, 0, y, FONT6X6);

		y = 34;
		len = snprintf(buf, sizeof(buf), "  L %3.1f`", Sensor_GetTemp(TC_LEFT));
		LCD_disp_str((uint8_t*)buf, len, 0, y, FONT6X6);
		len = snprintf(buf, sizeof(buf), "  R %3.1f`", Sensor_GetTemp(TC_RIGHT));
		LCD_disp_str((uint8_t*)buf, len, LCD_CENTER, y, FONT6X6);

		if (Sensor_IsValid(TC_PCB1) || Sensor_IsValid(TC_PCB2))
        {
			y = 42;
			if (Sensor_IsValid(TC_PCB1)) {
				len = snprintf(buf, sizeof(buf), " X1 %3.1f`", Sensor_GetTemp(TC_PCB1));
				LCD_disp_str((uint8_t*)buf, len, 0, y, FONT6X6);
			}
			if (Sensor_IsValid(TC_PCB2)) {
				len = snprintf(buf, sizeof(buf), " X2 %3.1f`", Sensor_GetTemp(TC_PCB2));
				LCD_disp_str((uint8_t*)buf, len, LCD_CENTER, y, FONT6X6);
			}
		}

		y = 50;
		len = snprintf(buf, sizeof(buf), "COLDJUNCTION");
		LCD_disp_str((uint8_t*)buf, len, 0, y, FONT6X6);

		y += 8;
		if (Sensor_IsValid(TC_COLD_JUNCTION)) {
			len = snprintf(buf, sizeof(buf), "%3.1f`", Sensor_GetTemp(TC_COLD_JUNCTION));
		} else {
			len = snprintf(buf, sizeof(buf), "NOT PRESENT");
		}
		LCD_disp_str((uint8_t*)buf, len, (12 * 6) - (len * 6), y, FONT6X6);

		LCD_BMPDisplay(stopbmp, 127 - 17, 0);

		Reflow_SetSetpoint(setpoint);

		if (timer > 0 && Reflow_IsDone()) {
			Buzzer_Beep(BUZZ_1KHZ, 255, TICKS_MS(100) * NV_GetConfig(REFLOW_BEEP_DONE_LEN));
			Reflow_SetBakeTimer(0);
			Reflow_SetMode(REFLOW_STANDBY);
		}

		if (keyspressed & KEY_F3 || keyspressed & KEY_F4)
        {
			if (timer == 0) {
				Reflow_SetMode(REFLOW_STANDBY);
			} else {
				if (timer == -1) {
					Reflow_SetBakeTimer(0);
				} else if (timer > 0) {
					Reflow_SetBakeTimer(timer);
					printf("\nSetting bake timer to %d\n", timer);
				}
				Reflow_SetMode(REFLOW_BAKE);
			}
		}

		// Abort bake
		if (keyspressed & KEY_S)
        {
			printf("\nEnd bake mode by keypress\n");

			mode = MAIN_HOME;
			Reflow_SetBakeTimer(0);
			Reflow_SetMode(REFLOW_STANDBY);
			retval = 0; // Force immediate refresh
		}
	} 
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- EDIT PROFILE -------------------------------- //
    // --------------------------------------------------------------------- //
    //
    //
    else if (mode == MAIN_EDIT_PROFILE) 
    { // Edit ee1 or 2
		LCD_FB_Clear();
		int keyrepeataccel = keyspressed >> 17; // Divide the value by 2
		if (keyrepeataccel < 1) keyrepeataccel = 1;
		if (keyrepeataccel > 30) keyrepeataccel = 30;

		int16_t cursetpoint;
		Reflow_SelectEEProfileIdx(current_edit_profile);
		//if (keyspressed & KEY_F1 && profile_time_idx > 0) { // Prev time
        if (keyspressed & KEY_F1) { // Prev time
			profile_time_idx--;
            if (profile_time_idx > 47)
                profile_time_idx = 47;
		}
		//if (keyspressed & KEY_F2 && profile_time_idx < 47) { // Next time
        if (keyspressed & KEY_F2) { // Next time
			profile_time_idx++;
            if (profile_time_idx > 47)
                profile_time_idx = 0;     
		}
        
		cursetpoint = Reflow_GetSetpointAtIdx(profile_time_idx);

		if (keyspressed & KEY_F3) { // Decrease setpoint
			cursetpoint -= keyrepeataccel;
		}
		if (keyspressed & KEY_F4) { // Increase setpoint
			cursetpoint += keyrepeataccel;
		}
		if (cursetpoint < 0) cursetpoint = 0;
		if (cursetpoint > SETPOINT_MAX) cursetpoint = SETPOINT_MAX;
		Reflow_SetSetpointAtIdx(profile_time_idx, cursetpoint);

		Reflow_PlotProfile(profile_time_idx);
		LCD_BMPDisplay(editbmp, 127 - 17, 0);

		len = snprintf(buf, sizeof(buf), "%02u0s %03u`", profile_time_idx, cursetpoint);
		LCD_disp_str((uint8_t*)buf, len, 13, 0, FONT6X6);

		// Done editing
		if (keyspressed & KEY_S) {
			Reflow_SaveEEProfile();
			mode = MAIN_HOME;
			retval = 0; // Force immediate refresh
		}
	}
    // --------------------------------------------------------------------- //
    // --------------------------------------------------------------------- //
    
    
    
    // --------------------------------------------------------------------- //
    // ----------------------- MAIN MENU ----------------------------------- //
    // --------------------------------------------------------------------- //
    //
    //
    else
    { // Main menu
		LCD_FB_Clear();

		len = snprintf(buf, sizeof(buf),"MAIN MENU");
		LCD_disp_str((uint8_t*)buf, len, 0, 6 * 0, FONT6X6);
		LCD_disp_str((uint8_t*)"F1", 2, 0, 8 * 1, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)"ABOUT", 5, 14, 8 * 1, FONT6X6);
		LCD_disp_str((uint8_t*)"F2", 2, 0, 8 * 2, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)"SETUP", 5, 14, 8 * 2, FONT6X6);
		LCD_disp_str((uint8_t*)"F3", 2, 0, 8 * 3, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)"BAKE/MANUAL MODE", 16, 14, 8 * 3, FONT6X6);
		LCD_disp_str((uint8_t*)"F4", 2, 0, 8 * 4, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)"SELECT PROFILE", 14, 14, 8 * 4, FONT6X6);
		LCD_disp_str((uint8_t*)"S", 1, 3, 8 * 5, FONT6X6 | INVERT);
		LCD_disp_str((uint8_t*)"RUN REFLOW PROFILE", 18, 14, 8 * 5, FONT6X6);

		len = snprintf(buf, sizeof(buf), "%s", Reflow_GetProfileName());
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), 8 * 6, FONT6X6 | INVERT);

		len = snprintf(buf,sizeof(buf), "OVEN TEMPERATURE %d`", Reflow_GetActualTemp());
		LCD_disp_str((uint8_t*)buf, len, LCD_ALIGN_CENTER(len), 64 - 6, FONT6X6);

		// Make sure reflow complete beep is silenced when pressing any key
		if (keyspressed) {
			Buzzer_Beep(BUZZ_NONE, 0, 0);
		}

		// About
		if (keyspressed & KEY_F1) {
			mode = MAIN_ABOUT;
			retval = 0; // Force immediate refresh
		}
		if (keyspressed & KEY_F2) { // Setup/cal
			mode = MAIN_SETUP;
			Reflow_SetMode(REFLOW_STANDBYFAN);
			retval = 0; // Force immediate refresh
		}

		// Bake mode
		if (keyspressed & KEY_F3) {
			mode = MAIN_BAKE;
			Reflow_Init();
			retval = 0; // Force immediate refresh
		}

		// Select profile
		if (keyspressed & KEY_F4) {
			mode = MAIN_SELECT_PROFILE;
			retval = 0; // Force immediate refresh
		}

		// Start reflow
		if (keyspressed & KEY_S) {
			mode = MAIN_REFLOW;
			LCD_FB_Clear();
			printf("\nStarting reflow with profile: %s", Reflow_GetProfileName());
			Reflow_Init();
			Reflow_PlotProfile(-1);
			LCD_BMPDisplay(stopbmp, 127 - 17, 0);
			len = snprintf(buf, sizeof(buf), "%s", Reflow_GetProfileName());
			LCD_disp_str((uint8_t*)buf, len, 13, 0, FONT6X6);
			Reflow_SetMode(REFLOW_REFLOW);
			retval = 0; // Force immediate refresh
		}
	} // end if 'MAIN MENU'...
    
    #ifdef KEYPAD_INTERRUPTS
    __builtin_disable_interrupts();
    #endif
    
    // update the LCD display...
	LCD_FB_Update();

	return retval;
}
// --------------------------------------------------------------------- //
// --------------------------------------------------------------------- //
//
/**/
/**************************************************************************************************/