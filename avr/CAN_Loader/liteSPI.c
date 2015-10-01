// *****************************************
// Atmega162, Atmega128 SPI interface driver
// *****************************************
//     Version 1.00 / 23.08.2006
// *****************************************
#include "liteSPI.h"


// Инициализация SPI в мастер-режиме 
// частота синхронизации = 1 000 000 hz
void SPI_masterInit(void)
{
 	// Ставим пины MOSI и SCK на выход, MISO на вход
    DDR_SPI &= ~BIT(DD_MISO); 				  // обнуляем MISO (на вход)
    DDR_SPI |=  BIT(DD_MOSI) | BIT(DD_SCK);   // устанавливаем MOSI и SCK
	
    DDR_SPI |=  BIT(DD_SS);   // устанавливаем SS на выход (для at128 - Обязательно!)



#ifdef PLATFORM_A128_SVIO_0
   	// Включаем SPI, режим Master, частота синхронизации = fck/128 (1MHz)
	SPCR = 0x5f;

#else
       	// Включаем SPI, режим Master, частота синхронизации = fck/16 (1MHz) 
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
#endif 

	

 	// Включаем SPI, режим Master, частота синхронизации = fck/4  (4MHz) 
	//SPCR = (1<<SPE)|(1<<MSTR);
}

// Добавить работу с прерываниями (?), установку частоты синхронизации
/*
void SPI_masterInitExt(char clrate)
{
 	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR);
}
*/

void SPI_MasterTransmit(char cData)
{
 	CLI();	 
	// Start transmission
	SPDR = cData;	 
	// Wait for transmission complete	 
#ifndef DEBUG_BUILD 
	while(!(SPSR & (1<<SPIF)));	 
#else
#warning Debug version!!!
#endif	
	SEI();
}

