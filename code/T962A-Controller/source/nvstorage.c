/*
 * nvstorage.c - Persistent settings storage for T-962 reflow controller
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
/* local includes */
#include "nvstorage.h"
#include "eeprom.h"
#include "schedule.h"





// ********************************************** //
// ****  global variables  declarations here **** //

// declare the main config global structure
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //





/**
**  @brief SetNVUpdatePending the function to
**  set a pending NV update...
**	@return 
*/
static void SetNVUpdatePending(void)
{
	g_Config.NvStorage.nvupdatepending = 1;
	Sched_SetState(NV_WORK, 1, 0);
}
/**/
/********************************************************/



/**
**  @brief NV_Init the function to init the NV storage
**  
**	@return 
*/
void NV_Init(void)
{
    NV_t* pMyNV = &g_Config.NvStorage.myNV;            
            
    // read nvstorage entry #1
	EEPROM_Read((uint8_t*)pMyNV, NV_START_ADDR, sizeof(NV_t));
	if ( pMyNV->magic != NVMAGIC ) 
    {   
		pMyNV->magic = NVMAGIC;
		pMyNV->numitems = NVITEM_NUM_ITEMS;
		memset(pMyNV->config, 0xff, NVITEM_NUM_ITEMS);
		printf("\nNV initialization cleared %d items", NVITEM_NUM_ITEMS);
		SetNVUpdatePending();
	} 
    else if (pMyNV->numitems < NVITEM_NUM_ITEMS)
    {
		uint8_t bytestoclear = (NVITEM_NUM_ITEMS - pMyNV->numitems);
		memset((pMyNV->config + pMyNV->numitems), 0xff, bytestoclear);
		pMyNV->numitems = NVITEM_NUM_ITEMS;
		printf("\nNV upgrade cleared %d new items", bytestoclear);
		SetNVUpdatePending();
	}
    #ifndef MINIMALISTIC
	Sched_SetWorkfunc(NV_WORK,NV_Work);
	Sched_SetState(NV_WORK, 1, 0);
    #endif
}
/**/
/************************************************************************/


/**
**  @brief NV_Init the function to init the NV storage
**  
**	@return 
*/
uint8_t NV_GetConfig(NVItem_t item)
{
    NV_t* pMyNV = &g_Config.NvStorage.myNV;     
    
	if (item < NVITEM_NUM_ITEMS) {
		return (pMyNV->config[item]);
	} else {
		return 0;
	}
}
/**/
/*******************************************************************/


/**
**  @brief NV_Init the function to init the NV storage
**  
**	@return 
*/
void NV_SetConfig(NVItem_t item, uint8_t value)
{
    NV_t* pMyNV = &g_Config.NvStorage.myNV;    
    
    // see if ram copy needs updating...
    // and set a pending update..
	if (item < NVITEM_NUM_ITEMS)
    {
		if (value != pMyNV->config[item])
        {
			pMyNV->config[item] = value;
			SetNVUpdatePending();
		}
	}
}
/**/
/*********************************************************************/



/**
**  @brief NV_Work the Periodic updater of NV
**  
**	@return 
*/
int32_t NV_Work(void)
{
	static uint8_t count = 0;
    _NV_STORAGE_* pNVStorage = &g_Config.NvStorage;
    
    // if an update is pending, inc count
	if (pNVStorage->nvupdatepending) count ++;
    
    // wait for for nv work updates, then flush to eeprom...
	if (count == 4) 
    {
		pNVStorage->nvupdatepending = count = 0;
		printf("\nFlushing NV copy to EE...");
		EEPROM_Write(NV_START_ADDR, (uint8_t*)&pNVStorage->myNV, sizeof(NV_t));
	}
    // if we have an update, callback in 2secs, otherwise no callback
	return pNVStorage->nvupdatepending ? (TICKS_SECS(2)) : -1;
}
/**/
/******************************************************************/