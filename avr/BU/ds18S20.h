#ifndef DS18S20_H_
#define DS18S20_H_

#include <stdlib.h>




/* return values */
#define DS18S20_OK          0x00
#define DS18S20_ERROR       0x01
#define DS18S20_START_FAIL  0x02
#define DS18S20_ERROR_CRC   0x03

#define DS18S20_POWER_PARASITE 0x00
#define DS18S20_POWER_EXTERN   0x01

/* DS18S20 specific values (see datasheet) */
#define DS18S20_ID 0x10


#define DS18S20_CONVERT_T	0x44
//#define DS18S20_CONVERT_T	0x55
#define DS18S20_READ		0xBE
#define DS18S20_WRITE		0x4E
#define DS18S20_EE_WRITE	0x48
#define DS18S20_EE_RECALL	0xB8
#define DS18S20_READ_POWER_SUPPLY 0xB4


//#define DS18S20_TCONV_9BIT       750
#define DS18S20_TCONV_9BIT       1200
#define DS18S20_TCONV            DS18B20_TCONV_12_BIT

// constant to convert the fraction bits to cel*(10^-4)
#define DS18S20_FRACCONV         625

#define DS18S20_SP_SIZE  9



/* for description of functions see DS18S20.c */
int16_t DS18S20_meas_to_smc( uint8_t *sp);
uint8_t DS18S20_read_temp(uint8_t id[], int16_t *rezult);
uint8_t DS18S20_search_sensors(void);
extern void DS18S20_find_sensor(uint8_t *diff, uint8_t id[]);
	
extern uint8_t	DS18S20_get_power_status(uint8_t id[]);

extern uint8_t DS18S20_start_meas( uint8_t id[]);

#endif
