/*
 * $Id$
 */
#ifndef _COMMON_AVR_CRC_H_
#define _COMMON_AVR_CRC_H_

//#include <types.h>

typedef unsigned int	uint16_t;

/*
 * Some common polynomials:
 *  CRC8-CCITT	- 0x8d
 *  CRC8-DALLAS	- 0x31
 *  CRC15-CAN	- 0x4599
 *  CRC16-CCITT	- 0x1021
 *  CRC16-IBM	- 0x8005
 */
#define CRC8_POLY	0x31	/* x^8 + x^5 + x^4 + 1 */
#define CRC16_POLY	0x1021

/*
 * These functions take pointer to some data flow.
 * 'len' is number of *bytes* in flow that should be signed:
 *   - crc8() max len is 15 (127 bits);
 *   - crc16() max len is 4095 (32767 bits);
 */
/* default polynomial */
uint16_t	crc16(unsigned char *bitstream, uint16_t len);

/* controlled polynomial */

//uint16_t	_raw_crc16(unsigned char *bitstream, uint16_t len, uint16_t poly);




#endif /* !_COMMON_AVR_CRC_H_ */

