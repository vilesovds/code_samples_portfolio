// *****************************************
// Atmega162, Atmega128 SPI interface driver
// *****************************************
//     Version 1.00 / 23.08.2006
// *****************************************

#ifndef _SPIDRV_H
#define _SPIDRV_H



#ifdef MCU_ATMEGA162    
	// Atmega 162 SPI pins configuration
	#define DDR_SPI DDRB
	#define DD_MOSI DDB5
	#define DD_MISO DDB6
	#define DD_SCK DDB7
	#define DD_SS DDB4
	#define MCU_SET_OK
#endif

#ifdef MCU_ATMEGA128    
	// Atmega 128 SPI pins configuration
	#define DDR_SPI DDRB
	#define DD_MOSI DDB2
	#define DD_MISO DDB3
	#define DD_SCK DDB1
	#define DD_SS DDB0
	#define MCU_SET_OK
#endif



#ifndef MCU_SET_OK
 #error Заданный MCU не поддерживается этой версией драйвера! 
#endif

#include <macros.h>


// SPI initialisation
// Master, clock rate: 4 000 000hz
void SPI_masterInit(void);

// Передача байта данных через SPI
// (Не забываем устанавливать CS!)
void SPI_MasterTransmit(char cData);


#endif