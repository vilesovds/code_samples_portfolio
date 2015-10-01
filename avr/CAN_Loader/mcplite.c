// ************************************************
// MCP2510 (CAN controller) device lite  driver interface
// Version 1.0 / 10.07.2008
// ************************************************

#include "mcplite.h"

/*********************************************************/
/*                   I N C L U D E    		             */
/*********************************************************/	
//#include "PLATFORM.h"

#include <macros.h>
#include <string.h>

/*************************************************************/
/*           Р Е Г И С Т Р Ы    M C P 2 5 1 0                */
/*************************************************************/



//Status & Control
#define CANSTAT 0x0E 	//CAN Status Register (ADDRESS: xEh)
#define CANCTRL 0x0F	//CAN Control Register (ADDRESS: xFh)
//PIN Configuration
#define BFPCTRL 0x0C	//RXnBF Pin Control and Status Register (ADDRESS: 0Ch)
#define TXRTSCTRL 0x0D	//TXnRTS Pin Control and Status Register (ADDRESS: 0Dh)
//Error Counters
#define TEC 0x1C		//Transmitter Error Counter (ADDRESS: 1Ch)
#define REC 0x1D		//Receiver Error Counter (ADDRESS: 1Dh)
//Configuration Registers
#define CNF3 0x28		//Configuration Register3 (ADDRESS: 28h)
#define CNF2 0x29		//Configuration Register2 (ADDRESS: 29h)
#define CNF1 0x2A		//Configuration Register1 (ADDRESS: 2Ah)
//Interrupt Registers
#define CANINTE 0x2B	//Interrupt Enable Register (ADDRESS: 2Bh)
#define CANINTF 0x2C	//Interrupt FLAG Register (ADDRESS: 2Ch)
//Error Register
#define EFLG 0x2D	//Error Flag Register (ADDRESS: 2Dh)
//Transmit&Receive Buffer Control Register
#define TXB0CTRL 0x30	//Transmit Buffer N Control Register (ADDRESS: 30h, 40h, 50h)
#define TXB1CTRL 0x40	//
#define TXB2CTRL 0x50	//
#define RXB0CTRL 0x60	//Receive Buffer 0 Control Register (ADDRESS: 60h)
#define RXB1CTRL 0x70	//Receive Buffer 1 Control Register (ADDRESS: 70h)
//Filters
#define RXF0SIDH 0x00	//Acceptance Filter N Standard Identifier High (Address: 00h, 04h, 08h, 10h, 14h, 18h)
#define RXF1SIDH 0x04	//SID10 - SID3
#define RXF2SIDH 0x08	
#define RXF3SIDH 0x10	
#define RXF4SIDH 0x14	
#define RXF5SIDH 0x18	
#define RXF0SIDL 0x01	//Acceptance Filter N Standard Identifier Low (Address: 01h, 05h, 09h, 11h, 15h, 19h)
#define RXF1SIDL 0x05	//SID2	SID1	SID0	xxx	EXIDE	xxx	EID17	EID16
#define RXF2SIDL 0x09	
#define RXF3SIDL 0x11	
#define RXF4SIDL 0x15	
#define RXF5SIDL 0x19	
#define RXF0EID8 0x02	//Acceptance Filter N Extended Identifier High (Address: 02h, 06h, 0Ah, 12h, 16h, 1Ah)
#define RXF1EID8 0x06	//EID15 - EID8
#define RXF2EID8 0x0A	
#define RXF3EID8 0x12	
#define RXF4EID8 0x16	
#define RXF5EID8 0x1A	
#define RXF0EID0 0x03	//Acceptance Filter N Extended Identifier Low (Address: 03h, 07h, 0Bh, 13h, 17h, 1Bh)
#define RXF1EID0 0x07	//EID7 - EID0
#define RXF2EID0 0x0B	
#define RXF3EID0 0x13	
#define RXF4EID0 0x17	
#define RXF5EID0 0x1B	
//Masks
#define RXM0SIDH 0x20	//Acceptance Filter Mask N Standard Identifier High (Address: 20h, 24h)
#define RXM1SIDH 0x24	//SID10 - SID3
#define RXM0SIDL 0x21	//Acceptance Filter Mask N Standard Identifier Low (Address: 21h, 25h)
#define RXM1SIDL 0x25	//SID2	SID1	SID0	xxx	 xxx	 xxx	EID17	EID16
#define RXM0EID8 0x22	//Acceptance Filter Mask N Extended Identifier High (Address: 22h, 26h)
#define RXM1EID8 0x26	//EID15 - EID8
#define RXM0EID0 0x23	//Acceptance Filter Mask N Extended Identifier Low (Address: 23h, 27h)
#define RXM1EID0 0x27	//EID7 - EID0
//RECIEVE BUFFERS
#define RXB0SIDH 0x61	//Receive Buffer N Standard Identifier High (ADDRESS: 61h, 71h)
#define RXB1SIDH 0x71	//SID10-SID3
#define RXB0SIDL 0x62	//Receive Buffer N Standard Identifier Low (ADDRESS: 62h, 72h)
#define RXB1SIDL 0x72	//SID10-SID3

#define PCAN_FF_EXTENDED 0x08

//Constants
#define MCP_RESET 	0xC0
#define MCP_READ 0x03
#define MCP_WRITE 0x02
#define MCP_RTS 0x80
#define MCP_Bit_Modify 0x05
#define MCP_Read_Status 0xA0


//unsigned char mcp_cs_reg;
//unsigned char mcp_cs_pin; 

//MCP_CAN_MESSAGE TX_MSG, RX_MSG;
//USER_CAN_MESSAGE BUF_RX_MSG;
//char can_rx_buf_empty = 1;

//************************************************************
////lite spi ===============================

//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

// *****************************************
// Atmega162, Atmega128 SPI interface driver
// *****************************************
//     Version 1.00 / 23.08.2006
// *****************************************
//#include "liteSPI.h"


// Инициализация SPI в мастер-режиме 
// частота синхронизации = 1 000 000 hz
void SPI_masterInit(void)
{
 	// Ставим пины MOSI и SCK на выход, MISO на вход
	
    DDR_SPI &= ~BIT(DD_MISO); 				  // обнуляем MISO (на вход)
    DDR_SPI |=  BIT(DD_MOSI) | BIT(DD_SCK);   // устанавливаем MOSI и SCK
	
    DDR_SPI |=  BIT(DD_SS);   // устанавливаем SS на выход (для at128 - Обязательно!)

       	// Включаем SPI, режим Master, частота синхронизации = fck/16 (1MHz) 
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
 	CLI();	 
	// Start transmission
	SPDR = cData;	 
	// Wait for transmission complete	 
  while(!(SPSR & (1<<SPIF)));	 
	SEI();
}

//*******************************************


/*****************************************************************************/
/* Programing Code								                             */
/*****************************************************************************/

// ***************************
// CAN функции высокого уровня
// ***************************

const char can_init_reg_info[][2] = {
	 { BFPCTRL,   0},
 { TXRTSCTRL, 0},
// BaudRate
#ifdef BAUDRATE_1000   
	   { CNF1, 0 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif

#ifdef BAUDRATE_800   
	   { CNF1, 192 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif

#ifdef BAUDRATE_500 	   
	   { CNF1, 1 },  		  // CNF1: Baud Rate Prescaler = 1; Sync Jump = 0 
	   { CNF2, 226 },		 // CNF2: phase; sampling... 
	   { CNF3, 2 },			  // CNF3: wake-up filter disabled; phase...
#endif

#ifdef BAUDRATE_250   
	   { CNF1, 3 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif

#ifdef BAUDRATE_125   
	   { CNF1, 7 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif

#ifdef BAUDRATE_50   
	   { CNF1, 19 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif

#ifdef BAUDRATE_25   
	   { CNF1, 39 },  		   
	   { CNF2, 226 },		  
	   { CNF3, 2 },			
#endif


 { CANINTE,   0},
 { CANINTF, 0x1C},
 { TXB0CTRL,  0},
 { TXB1CTRL,  0},
 { TXB2CTRL,  0},
 { RXB0CTRL, 0x24},
 { RXB1CTRL, 0x20},			
 { RXM0SIDH,  0xFF},
 { RXM0SIDL,  0xE0},
 { RXM1SIDH,  0xFF},
 { RXM1SIDL,  0xE0},                        
 { RXF0SIDH,  (char)((CAN_INPUTID >> 3) & 0x00FF)},                         
 { RXF0SIDL,  (char)((CAN_INPUTID << 5) & 0x00E0)},
 { RXF1SIDH, 0},                         
 { RXF1SIDL, 0},
 { RXF2SIDH, 0},                         
 { RXF2SIDL, 0},
 { RXF3SIDH, 0},                         
 { RXF3SIDL, 0},
 { RXF4SIDH, 0},                         
 { RXF4SIDL, 0},
 { RXF5SIDH, 0},                         
 { RXF5SIDL, 0}
};

//-----------------------------------------
// Инициализирует CAN контроллер 
//-----------------------------------------
//-----------------------------------------
// RETURN: 1 - Init ok 
// 		   0 - Error
//-----------------------------------------
unsigned char CAN_init(void)
{
	register unsigned char r;	

    CAN_Reset();    	   			 	  // сброс контроллера

	r = CAN_ConfigMode();
	if (r)
	{	
		CAN_bit_modify(CANCTRL,0x07,0x04);	  // CANCTRL, CLKOUT = System CLK / 1 ; CLKOUT pin enabled (?)

	for (r=0; r<23; r++) CAN_WRITE(can_init_reg_info[r][0], can_init_reg_info[r][1]);
	
    // * Configuration registers
	
   CAN_bit_modify(EFLG, 0xff,0);		  // Clear error flags 
   
   /*r =*/return CAN_NormalMode();
	}	
	
	return r;
}


// -----------------------------------
// Послать TX_MSG через CAN controller
// -----------------------------------
// ПАРАМЕТРЫ: CAN_MESSAGE
// -----------------------------------
// RETURN: 1 - сообщение помещено в исходящий буффер 
//         0 - нет свободных TX буфферов
// -----------------------------------   
unsigned char CAN_Msg_Send(MCP_CAN_MESSAGE *TX_MSG)
{
    unsigned char TX_flags, TX_buf_addr, n;	

	// ищем свободный выходной буффер
	TX_flags = CAN_read_status() & 0xA8; // TX buffer empty flags: TX2, TX1, TX0 (b10101000)
	
	if (TX_flags) // есть свободные
    {
       if (TX_flags & 0x08) TX_buf_addr = 0x30; 
       else if (TX_flags & 0x20) TX_buf_addr = 0x40;
            else TX_buf_addr = 0x50;
    }
    else return 0; // нет свободных TX буфферов
	
	if (CAN_READ(TX_buf_addr) & BIT(3)) return 0; // не смотря ни на что, буфер занят 
	
//	TX_MSG.SIDH = (char)((USER_TX_MSG->ID >> 3) & 0x00FF); 
//	TX_MSG.SIDL = (char)((USER_TX_MSG->ID << 5) & 0x00E0);
//	TX_MSG.DLEN  = USER_TX_MSG->DLEN;
//	memcpy(&(TX_MSG.data[0]), &(USER_TX_MSG->data[0]), 8);
	
    for (n = 0x0d; n != 0xFF; n--) CAN_WRITE(TX_buf_addr+n, *((unsigned char*)TX_MSG + n));

    return 1; // сообщение помещено в исходящий буфер
}

// -------------------------------------------------
// Прочитать пришедшее сообщение из входного буфера MCP2510
// -------------------------------------------------
// ПАРАМЕТРЫ: нет
// -------------------------------------------------
// RETURN: 0 - нет данных
// 		   1 - сообщение из буффера RX0
//         2 - сообщение из буффера RX1 
// 		   RX_MSG - принятое сообщение
// -------------------------------------------------
unsigned char CAN_msg_Read(MCP_CAN_MESSAGE *RX_MSG)
{   
	unsigned char n, rx_flags, ret_val = 0; //,RX_buf_addr, can_eflg;

    if (CAN_READ(CANINTF) & 0x80) CAN_bit_modify(CANINTF, 0x80, 0);   // антон - сброс флага MERRF		
	
    rx_flags = CAN_read_status() & 0x03;  // FLAGS: xxxx x x RX1 RX0

	if (rx_flags == 0) return 0;	   		  // Ничего не пришло

	if (rx_flags & 1) // Есть данные в буфере RX0
	{
	  if ((CAN_READ(TXB0CTRL) & BIT(3)) == 0) // check clear TXB0CTRL.TXREQ ! (ERRATA #6) // -
	   if ((CAN_READ(EFLG) & 0x40) == 0)  // RX0OVR // ошибки нет, читаем сообщение 
	   { 
	     // читаем все содержимое буфера в RX_MSG
	     for (n = 0; n<14; n++) *((unsigned char*)RX_MSG + n) = CAN_READ(0x60 + n);
	     ret_val = 1; // сообщение принято буфером 1
	   }
	  CAN_bit_modify(EFLG, 0x40, 0x0);    // сброс флага RX0OVR	   
	  CAN_bit_modify(CANINTF, 0x81, 0);   // сброс флага RX0IF & MERRF		
	}

	if (rx_flags & 2)	// Есть данные в буфере RX1	
    {
	  if ((CAN_READ(TXB1CTRL) & BIT(3)) == 0) // check clear TXB1CTRL.TXREQ ! (ERRATA #6) // -
	   if ((CAN_READ(EFLG) & 0x80) == 0)  // RX1OVR // ошибки нет, читаем сообщение 
	   { 
	    // читаем все содержимое буфера в RX_MSG
	    for (n = 0; n<14; n++) *((unsigned char*)RX_MSG + n) = CAN_READ(0x70 + n);        	   
	    ret_val = 2; // сообщение принято буфером 2
	   }
	  CAN_bit_modify(EFLG, 0x80, 0);      // сброс флага RX1OVR	   
	  CAN_bit_modify(CANINTF, 0x82, 0);   // сброс флага RX1IF & MERRF		
	}
	
	// иногда в бите RB1 ставится 1 !!!
	RX_MSG->DLEN &= 0x0F;  		  // длина принятого сообщения 
		
	return ret_val;   	
}


// =================================================================
// ЛОКАЛЬНЫЕ ФУНКЦИИ
// =================================================================


// Включить режим конфигурирования
char CAN_ConfigMode(void)
{
//	register char can_antifreeze = 0;    

	while (!(CAN_READ(CANSTAT)&0x80)) // ждем пока режим установится 
	{
	  CAN_bit_modify(CANCTRL, 0xE0,0x80);	  // CANCTRL: REQOP = b100 - set config mode
//	  if (can_antifreeze > CAN_MAX_TRY) return 0;	   
//	  can_antifreeze++;
	}  
	return 1;
}

// Включить нормальный режим (ERRATA #1)
char CAN_NormalMode(void)
{
//	register char can_antifreeze = 0;
    
    // ждем когда контроллер перейдет в нормальный режим
	while (CAN_READ(CANSTAT) & 0xE0) 
	{
	  CAN_bit_modify(CANCTRL,0xE0,0x80);	  // CANCTRL: REQOP = b100 - set config mode	 
	  CAN_bit_modify(CANCTRL,0xE0,0x00);	  // CANCTRL: REQOP = b000 - set normal mode
//	  if (can_antifreeze > CAN_MAX_TRY) return 0;	   
//	  can_antifreeze++;
	} 
	return 1;
}

// =================================================================
// СЛУЖЕБНЫЕ ФУНКЦИИ НИЗКОГО УРОВНЯ
// =================================================================

/*void litechipSelect(void )
{
  CSREG&=~BIT(CSPIN);
}


void litechipUnSelect(void)
{
  CSREG|=BIT(CSPIN);  
}
*/

// Сброс контроллера
void CAN_Reset(void)
{
 CSREG&=~BIT(CSPIN); //  litechipSelect();//mcp_cs_reg, mcp_cs_pin); 

	SPI_MasterTransmit(MCP_RESET);

    CSREG|=BIT(CSPIN);  //litechipUnSelect();//mcp_cs_reg, mcp_cs_pin); 
}

// читаем регистр по адресу cAdr
unsigned char CAN_READ(unsigned char cAdr)
{
  CSREG&=~BIT(CSPIN); // litechipSelect();//mcp_cs_reg, mcp_cs_pin); 

	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(cAdr);
	SPI_MasterTransmit(' ');

   CSREG|=BIT(CSPIN);  // litechipUnSelect();
	 
	return SPDR; // возвращаем регистр данных SPI
}

// Запись в регистр по адресу cAdr, байта данных cDATA
void CAN_WRITE(unsigned char cAdr, unsigned char cDATA)
{
  CSREG&=~BIT(CSPIN); // litechipSelect();//mcp_cs_reg, mcp_cs_pin); 

	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(cAdr);
	SPI_MasterTransmit(cDATA);

    CSREG|=BIT(CSPIN);  //litechipUnSelect();
}

// 

// побитовое изменение данных регистра
// cAddr - адрес регистра, cMask - маска изменения (бит 1 - меняем, 0 - не меняем), cData - новое значение
void CAN_bit_modify (unsigned char cAdr, unsigned char cMask, unsigned char cDATA)
{
  CSREG&=~BIT(CSPIN); // litechipSelect();//mcp_cs_reg, mcp_cs_pin); 

	SPI_MasterTransmit(MCP_Bit_Modify);
	SPI_MasterTransmit(cAdr);
	SPI_MasterTransmit(cMask);
	SPI_MasterTransmit(cDATA);

   CSREG|=BIT(CSPIN);  // litechipUnSelect();
}

// Прочитать статус CAN контроллера
// --------------------------------
// 0 CANITF.RX0IF
// 1 CANITF.RX1IF
// 2 TXB0CNTRL.TXREQ
// 3 CANITF.TX0IF
// 4 TXB1CNTRL.TXREQ
// 5 CANITF.TX1IF
// 6 TXB2CNTRL.TXREQ 
// 7 CANITF.TX2IF  
// --------------------------------

unsigned char CAN_read_status (void)
{
   CSREG&=~BIT(CSPIN);// litechipSelect(); 

	SPI_MasterTransmit(MCP_Read_Status);
	SPI_MasterTransmit(' ');

   CSREG|=BIT(CSPIN);  // litechipUnSelect();

	return SPDR;
}


