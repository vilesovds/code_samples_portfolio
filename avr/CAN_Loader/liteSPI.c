// *****************************************
// Atmega162, Atmega128 SPI interface driver
// *****************************************
//     Version 1.00 / 23.08.2006
// *****************************************
#include "liteSPI.h"


// ������������� SPI � ������-������ 
// ������� ������������� = 1 000 000 hz
void SPI_masterInit(void)
{
 	// ������ ���� MOSI � SCK �� �����, MISO �� ����
    DDR_SPI &= ~BIT(DD_MISO); 				  // �������� MISO (�� ����)
    DDR_SPI |=  BIT(DD_MOSI) | BIT(DD_SCK);   // ������������� MOSI � SCK
	
    DDR_SPI |=  BIT(DD_SS);   // ������������� SS �� ����� (��� at128 - �����������!)



#ifdef PLATFORM_A128_SVIO_0
   	// �������� SPI, ����� Master, ������� ������������� = fck/128 (1MHz)
	SPCR = 0x5f;

#else
       	// �������� SPI, ����� Master, ������� ������������� = fck/16 (1MHz) 
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
#endif 

	

 	// �������� SPI, ����� Master, ������� ������������� = fck/4  (4MHz) 
	//SPCR = (1<<SPE)|(1<<MSTR);
}

// �������� ������ � ������������ (?), ��������� ������� �������������
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

