
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "common_defs.h"
/* local headers */
#include "onewire.h"
#include "max31855.h"
#include "nvstorage.h"
#include "sensor.h"



/*
* Normally the control input is the average of the first two TCs.
* By defining this any TC that has a readout 5C (or more) higher
* than the TC0 and TC1 average will be used as control input instead.
* Use if you have very sensitive components. Note that this will also
* kick in if the two sides of the oven has different readouts, as the
* code treats all four TCs the same way.
*/
//#define MAXTEMPOVERRIDE



// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //


/*
 *
 */
void Sensor_DoConversion(void) 
{
    int16_t temp = 0;
    int16_t coldjunc = 0;
    uint8_t numtemps = 0;
    int i = 0;   
    double temptotal = 0;
    
    
	/*
	* These are the temperature readings we get from the thermocouple interfaces.
    * The on-board 'DS18B20' is used ONLY for the cpu-fan control, it is NOT used
    * as any feedback or control for the system temp, as the max31855s have their
    * own on-die CJ calculation and compensation, no need to do anything further..
	*/        
    // collect COLD-JUNCTION temp of DS18B20 device...
    g_Config.TempSensors.CJSensor.temperature = OneWire_GetTempSensorReading();
    
    // collect the temps from ALL attached SPI TC sensors...
    // *** NOTE ***
    // The resolution of the max31855 is:
    // '0.25c' for the 14-bit Temperature
    // '0.0625c' for the 12-bit internal temp
    // all resolution is retained in the calculations below...
    for (i = 0; i < g_Config.numSensorsFound; i++)
    {
        // check current sensor if valid
        if ( (g_Config.TempSensors.TCSensor[i].ispresent == 1) && 
             (g_Config.TempSensors.TCSensor[i].tempvalid == 1) )
        {
            // extract out the top 14 bits for the 'TC-temp' reading
            // if it's signed, then convert to 16-bit negative int
            temp = (g_Config.TempSensors.TCSensor[i].devreadout >> 2);
            coldjunc = (g_Config.TempSensors.TCSensor[i].extrareadout >> 4);
            if (temp & BIT_14) {
                temp |= 0xC000;
            }
            if (coldjunc & BIT_12) {
                coldjunc |= 0xF000;
            }
            // calculate the exact temperature for each reading...
            g_Config.TempSensors.TCSensor[i].temperature = (float)(temp * 0.25);  
            g_Config.TempSensors.TCSensor[i].internal = (float)(coldjunc * 0.0625);     
            temptotal += g_Config.TempSensors.TCSensor[i].temperature;
            numtemps++;                                  
        }        
    }
    // if at least 1 SPI device found, calculate temp...
    if (numtemps > 0) {
        g_Config.TempSensors.avgtemp = (float)(temptotal / numtemps);                
    }
    // *** This may be a PIC32 errata thing, every so often an SPI bus
    //     transaction completely fails, and NO readings come out... 
    //     therefore, for now, just do nothing when temp updates come
    //     back empty, will have to investigate this further....
    // ***
    /*
    else {
        // otherwise set invalid temp...
        g_Config.TempSensors.avgtemp = 999.0f;        
    } 
    */  
   
    // return nothing...
    return;
}
/**/
/**********************************************************************************************/



/*
 *
 */
uint8_t Sensor_ColdjunctionPresent(void)
{
	return g_Config.TempSensors.CJSensor.ispresent;
}
/**/
/********************************************************/



/*
 *
 */
float Sensor_GetTemp(TempSensor_t sensor)
{
    float retval = 0;
    
	if (sensor == TC_COLD_JUNCTION) {
		retval = g_Config.TempSensors.CJSensor.temperature;
	} else if(sensor == TC_AVERAGE) {
		retval = g_Config.TempSensors.avgtemp;
	} else if(sensor < TC_NUM_ITEMS) {
		retval = g_Config.TempSensors.TCSensor[sensor - TC_LEFT].temperature;
	} else {
		retval = 0.0f;
	}
    
    // return temp
    return retval;
}
/**/
/*****************************************************/


/*
 * 
 */
uint8_t Sensor_IsValid(TempSensor_t sensor)
{
    uint8_t retval = 0;
    
    
	if (sensor == TC_COLD_JUNCTION) {
		retval = g_Config.TempSensors.CJSensor.ispresent;
	} else if(sensor == TC_AVERAGE) {
		retval = 1;
	} else if(sensor >= TC_NUM_ITEMS) {
		retval = 0;
	}
    else {
        retval = (g_Config.TempSensors.TCSensor[sensor-TC_LEFT].tempvalid);
    }
    
    // return result
    return retval;
}
/**/
/*******************************************************************/


/*
 *
 */
void Sensor_ListAll(void) 
{
	int count = 5;
    int i = 0;
	char* names[] = {"Left", "Right", "Pcb 1", "Pcb 2", "Cold junction"};
	TempSensor_t sensors[] = {TC_LEFT, TC_RIGHT, TC_PCB1, TC_PCB2, TC_COLD_JUNCTION};
	char* format = "\n%13s: %4.1fdegC";

    
	for (i = 0; i < count; i++)
    {
		if (Sensor_IsValid(sensors[i])) {
			printf(format, names[i], Sensor_GetTemp(sensors[i]));
		}
	}
	if (!Sensor_IsValid(TC_COLD_JUNCTION)) {
		printf("\nNo cold-junction sensor on PCB");
	}
}
/**/
/*********************************************************************************/
