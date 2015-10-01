/*
 * settings.c
 *
 *  Created on: 12.12.2014
 *      Author: Vilesov.DS
 */

#include "settings.h"
#include "A4988.h"
#include <string.h>

#define SETTINGS_CHK_ADDR			0x0000
#define SETTINGS_CHK_DATA 			0xDEAD

#define SETTINGS_BPC_DEF					((uint16_t)10)
#define SETTINGS_BPC_POINTER				(&A4988bpc)

#define	SETTINGS_START_PPS_DEF				((uint16_t)200)
#define	SETTINGS_START_PPS_POINTER			(&A4988startPPS)

#define	SETTINGS_STOP_PPS_DEF				((uint16_t)100)
#define	SETTINGS_STOP_PPS_POINTER			(&A4988stopPPS)

#define	SETTINGS_MARCH_PPS_DEF				((uint16_t)500)
#define	SETTINGS_MARCH_PPS_POINTER			(&A4988marchPPS)

#define	SETTINGS_START_TIME_DEF				((uint16_t)500)
#define	SETTINGS_START_TIME_POINTER			(&A4988startTime)

#define	SETTINGS_STOP_TIME_DEF				((uint16_t)500)
#define	SETTINGS_STOP_TIME_POINTER			(&A4988stopTime)

#define	SETTINGS_PAUSE_BEFORE_SHOT_DEF		((uint16_t)1000)
#define	SETTINGS_PAUSE_BEFORE_SHOT_POINTER	(&A4988pauseBeforeShot)

#define	SETTINGS_PAUSE_AFTER_SHOT_DEF		((uint16_t)1000)
#define	SETTINGS_PAUSE_AFTER_SHOT_POINTER	(&A4988pauseAfterShot)

#define	SETTINGS_DIR_INV_DEF				((uint8_t)0)
#define	SETTINGS_DIR_INV_POINTER			(&A4988invdir)

#define	SETTINGS_MICROSTEP_DEF				((uint8_t)0)
#define	SETTINGS_MICROSTEP_POINTER			(&A4988microstep)

#define	SETTINGS_TOTAL_STEPS_DEF			((uint16_t)32000)
#define	SETTINGS_TOTAL_STEPS_POINTER		(&A4988totalSteps)

#define	SETTINGS_CAL_PPS_DEF			((uint16_t)300)
#define	SETTINGS_CAL_PPS_POINTER		(&A4988calibPPS)



Setting_t Settings[] = {
		{0x0002, (void*)SETTINGS_BPC_POINTER, 2},// bpc
		{0x0004, (void*)SETTINGS_START_PPS_POINTER, 2}, //start PPS
		{0x0006, (void*)SETTINGS_STOP_PPS_POINTER, 2}, // stop pps
		{0x0008, (void*)SETTINGS_MARCH_PPS_POINTER, 2}, // march pps
		{0x000A, (void*)SETTINGS_START_TIME_POINTER, 2}, // START_TIME
		{0x000C, (void*)SETTINGS_STOP_TIME_POINTER, 2}, // STOP_TIME
		{0x000E, (void*)SETTINGS_PAUSE_BEFORE_SHOT_POINTER, 2}, //PAUSE_BEFORE_SHOT
		{0x0010, (void*)SETTINGS_PAUSE_AFTER_SHOT_POINTER, 2}, //PAUSE_AFTER_SHOT
		{0x0012, (void*)SETTINGS_DIR_INV_POINTER, 1}, //DIR_INV
		{0x0013, (void*)SETTINGS_MICROSTEP_POINTER, 1}, //MICROSTEP
		{0x0014, (void*)SETTINGS_TOTAL_STEPS_POINTER, 2}, //TOTAL_STEPS
		{0x0016, (void*)SETTINGS_CAL_PPS_POINTER, 2}, //CAL_PPS
		};


void SettingsLoadDefaults(void) {
*SETTINGS_BPC_POINTER = SETTINGS_BPC_DEF;
*SETTINGS_START_PPS_POINTER = SETTINGS_START_PPS_DEF;
*SETTINGS_STOP_PPS_POINTER = SETTINGS_STOP_PPS_DEF;
*SETTINGS_MARCH_PPS_POINTER = SETTINGS_MARCH_PPS_DEF;
*SETTINGS_START_TIME_POINTER = SETTINGS_START_TIME_DEF;
*SETTINGS_STOP_TIME_POINTER = SETTINGS_STOP_TIME_DEF;
*SETTINGS_PAUSE_BEFORE_SHOT_POINTER = SETTINGS_PAUSE_BEFORE_SHOT_DEF;
*SETTINGS_PAUSE_AFTER_SHOT_POINTER = SETTINGS_PAUSE_AFTER_SHOT_DEF;
*SETTINGS_DIR_INV_POINTER = SETTINGS_DIR_INV_DEF;
*SETTINGS_MICROSTEP_POINTER = SETTINGS_MICROSTEP_DEF;
*SETTINGS_TOTAL_STEPS_POINTER = SETTINGS_TOTAL_STEPS_DEF;
*SETTINGS_CAL_PPS_POINTER = SETTINGS_CAL_PPS_DEF;
}

uint8_t SettingsCheck(void) {
	return (eeprom_read_word((uint16_t *) SETTINGS_CHK_ADDR) == SETTINGS_CHK_DATA);
}

void SettingsSave(uint16_t indx, void * p){
	if(indx < SETTINGS_TBL_COUNT){
		eeprom_write_block(p,(void *)Settings[indx].addr,Settings[indx].len);
	}
}

void SettingsSaveAll(void) {
	uint16_t i;
	for(i=0;i<SETTINGS_TBL_COUNT;i++){
		SettingsSave(i,Settings[i].p);
	}
	eeprom_write_word(SETTINGS_CHK_ADDR,SETTINGS_CHK_DATA);
}

uint8_t SettingsLoadValue(uint16_t indx, void * dst)
{
	if(indx < SETTINGS_TBL_COUNT){
		eeprom_read_block(dst,(void *)Settings[indx].addr,Settings[indx].len);
		return Settings[indx].len;
	}
	return 0;
}

void SettingsLoadAll(void)
{
	uint16_t i;
	for(i=0;i<SETTINGS_TBL_COUNT;i++){
		SettingsLoadValue(i,Settings[i].p);
	}
}

void SettingsUpdateValue(uint16_t indx, void * p ){
	SettingsSave(indx,p);
	SettingsLoadValue(indx,Settings[indx].p);
}

uint8_t SettingsGetCurrent(uint16_t indx, void * dst){
	if(indx < SETTINGS_TBL_COUNT){
		memcpy(dst, Settings[indx].p,Settings[indx].len);
		return Settings[indx].len;
	}
	return 0;
}


