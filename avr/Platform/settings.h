/*
 * settings.h
 *
 *  Created on: 12.12.2014
 *      Author: Vilesov.DS
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <avr/eeprom.h>


typedef struct{
	uint16_t addr;
	void * p;
	uint16_t len;
}Setting_t;

enum {
	SETTINGS_BPC_INDX = 0,
	SETTINGS_START_PPS_INDX,
	SETTINGS_STOP_PPS_INDX,
	SETTINGS_MARCH_PPS_INDX,
	SETTINGS_START_TIME_INDX,
	SETTINGS_STOP_TIME_INDX,
	SETTINGS_PAUSE_BEFORE_SHOT_INDX,
	SETTINGS_PAUSE_AFTER_SHOT_INDX,
	SETTINGS_DIR_INV_INDX ,
	SETTINGS_MICROSTEP_INDX,
	SETTINGS_TOTAL_STEPS_INDX,
	SETTINGS_CAL_PPS_INDX,
	SETTINGS_TBL_COUNT
};

uint8_t SettingsCheck(void) ;
void SettingsLoadAll(void);
void SettingsLoadDefaults(void);
void SettingsSaveAll(void);

void SettingsSave(uint16_t indx, void * p);
uint8_t SettingsLoadValue(uint16_t indx, void * dst);
void SettingsUpdateValue(uint16_t indx, void * p );
uint8_t SettingsGetCurrent(uint16_t indx, void * dst);
#endif /* SETTINGS_H_ */
