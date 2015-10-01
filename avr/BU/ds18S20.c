/*********************************************************************************
Title:    DS18S20_-Functions via One-Wire-Bus
Author:   Martin Thomas <eversmith@heizung-thomas.de>   
          http://www.siwawi.arubi.uni-kl.de/avr-projects

**********************************************************************************/
#include <types.h>
#include "ds18S20.h"
#include "onewire.h"
#include "crc8.h"

/*id's buffer*/
uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];
/**************************************************************
Sign mudak convert

   input is: 
   - scratchpad-buffer
   output is:
   - temp *10 (sign mudak convert)
-**************************************************************/
int16_t
DS18S20_meas_to_smc(uint8_t *sp)
{	uint8_t subzero;
	uint16_t meas;
	 /* LSB*/
	meas = sp[0]; 
	/* MSB*/
    meas |= ((uint16_t)sp[1])<<8; 
	/* check for negative */
	if ( meas & 0x8000 )  { 
		/* mark negative*/
		subzero=1;      
		/* convert to positive => (twos complement)++*/
		meas ^= 0xffff;  
		meas++;
	}
	else subzero=0;
	/* multiply by 5 */
	if (subzero)meas=(int16_t)meas*-5;
	else meas=(int16_t)meas*5;
	/*and set MSB*/
	//meas|=subzero<<15;
	/* that's all*/
	return meas;
}

/*************************************************
 find DS18S20 Sensors on 1-Wire-Bus
   input/ouput: diff is the result of the last rom-search
   output: id is the rom-code of the sensor found 
   -************************************************/
void 
DS18S20_find_sensor(uint8_t *diff, uint8_t id[])
 { 
	for (;;) {
		*diff = ow_rom_search( *diff, &id[0] );
		if ( *diff==OW_PRESENCE_ERR || *diff==OW_DATA_ERR || *diff == OW_LAST_DEVICE ) return;
		if ( id[0] == DS18S20_ID ) return;
    }
 }

/****************************************************************
 get power status of DS18S20 
   input  : id = rom_code 
   returns: DS18S20_POWER_EXTERN or DS18S20_POWER_PARASITE 
-****************************************************************/
uint8_t	
DS18S20_get_power_status(uint8_t id[])
{	uint8_t pstat;
	ow_reset();
	ow_command(DS18S20_READ_POWER_SUPPLY, id);
	/* pstat 0=is parasite/ !=0 ext. powered*/
	pstat=ow_bit_io(1); 
	ow_reset();
	return (pstat) ? DS18S20_POWER_EXTERN: DS18S20_POWER_PARASITE;
}

/***********************************************************
 start measurement (CONVERT_T) for all sensors if input id==NULL 
   or for single sensor. then id is the rom-code 
  -***********************************************************/
uint8_t 
DS18S20_start_meas( uint8_t id[])
{ 	uint8_t with_power_extern;
	ow_reset(); 
	if( ow_input_pin_state() ) { 
		/* only send if bus is "idle" = high*/
		ow_command( DS18S20_CONVERT_T, id );
		with_power_extern=DS18S20_get_power_status(id);
		if (with_power_extern != DS18S20_POWER_EXTERN) ow_parasite_enable();
		return DS18S20_OK;
    } 
	else return DS18S20_START_FAIL;
}

/****************************************************************
measurement temperature in sign style(*10)
input:		- id - idendificator of device(use the search...)
		- *rezult - buffer for rezult
return:	-DS18S20_OK - good measure
		-DS18S20_ERROR_CRC - some problem
		- *result - temperature in SignMudak Convert style
****************************************************************/
uint8_t 
DS18S20_read_temp(uint8_t id[], int16_t *rezult)
{	uint8_t i;
	uint8_t sp[DS18S20_SP_SIZE];
	ow_command(DS18S20_READ, id);
	for ( i=0 ; i< DS18S20_SP_SIZE; i++ ) sp[i]=ow_byte_rd();
	if ( ds_crc8( &sp[0], DS18S20_SP_SIZE ) ) return DS18S20_ERROR_CRC;
	*rezult=DS18S20_meas_to_smc(&sp[0]);
	return DS18S20_OK;
}

/**************************************************************
for search all ds18S20  connected to wire
return - count of sensors
***************************************************************/
uint8_t 
DS18S20_search_sensors(void)
{	uint8_t i;
	uint8_t id[OW_ROMCODE_SIZE];
	uint8_t diff, nSensors;
	nSensors = 0;
	for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && nSensors < MAXSENSORS ;){ 
		DS18S20_find_sensor( &diff, &id[0] );
		if(( diff == OW_PRESENCE_ERR )||( diff == OW_DATA_ERR ))break;
		for (i=0;i<OW_ROMCODE_SIZE;i++)
			gSensorIDs[nSensors][i]=id[i];
		nSensors++;
    }
	return nSensors;
}

