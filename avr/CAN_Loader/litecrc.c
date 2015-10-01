/*
 * $Id$
 */
//#include <types.h>
#include "litecrc.h"

/*uint16_t
_raw_crc16(unsigned char *bitstream, uint16_t len, uint16_t poly)
{
	uint16_t crc = 0xffff;
	uint8_t i;

	while (len--) {

		crc ^= *bitstream++;

		for (i = 0; i < 8; i++)
			crc = crc & 0x8000 ? (crc << 1) ^ poly : crc << 1;
	}
 
    return crc;
}
*/


uint16_t
crc16(unsigned char *bitstream, uint16_t len)
{ uint16_t crc = 0xffff;
	signed char i;

	//return(_raw_crc16(bitstream, len, CRC16_POLY));
	
	while (len--) {

		crc ^= *bitstream++;

		for (i = 0; i < 8; i++)
			crc = crc & 0x8000 /*MSB*/ ? (crc << 1) ^ CRC16_POLY : crc << 1;
	}
 
    return crc;
	
	
}

