// ************************************************
// MCP2510 (CAN controller) device lite  driver interface
// Version 1.0 / 10.07.2008
// ************************************************

// ************************************************

#ifndef _MCPLITE_H
#define _MCPLITE_H

// **********************************
// CONFIGURATION
// *************


/// Set PARAMS OF PLATFORM 

// ���� ����������� ������
#ifdef PLATFORM_A128BDIAG
  // ��������� ���
  #define MCU_ATMEGA128
  // ��������� ���������
  #define PLATFORM 0x02
  #define MOD 0x00
  #define SOFT 0x00
  // ��������� ������������� ���������
  #define port_init() {PORTB = 0xFF; DDRB = 0xFF;}
  // ���������� ��� ������
  #define CSREG PORTB
  #define CSPIN 4
//  #define CSPIN 0
//  #define CSDDR DDRB
  #define CANOUT_ID 0x82

  #define BAUDRATE_125
  #define CAN_INPUTID 2

  #define PLATFORM_SET_OK
#endif

// ���� ���������� �������
#ifdef PLATFORM_A128BU
  // ��������� ���
  #define MCU_ATMEGA128
  
   // �����
  #define KEY 0 
  // ��������� ���������
	#define	_IS_ 		0x8F
	#define	_S_ 		0x66
	#define	_M_ 		0x6B
	#define	_P_ 		0x19
	#define	_K_ 		0x00
  // ��������� ������������� ���������
  #define port_init() {  PORTA = 0x0F; DDRA  = 0xF0; PORTB = 0xF0; DDRB  = 0x0B; PORTC = 0xFF; DDRC  = 0x00; PORTD = (0x3C^0xFF); DDRD  = 0x3C; PORTE = (0xC2^0xFF); DDRE  = 0xC2; PORTF = 0xF0; DDRF  = 0x00; PORTG = 0x00; DDRG  = 0x0F;}
  // ���������� ��� ������
  #define CSREG PORTB
  #define CSPIN 0
//  #define CSDDR DDRB
  #define CANOUT_ID 0x55

  #define BAUDRATE_125
  #define CAN_INPUTID 0x44

  #define PLATFORM_SET_OK
#endif


#ifdef PLATFORM_BUPUI
  // ��������� ���
  #define MCU_ATMEGA128
   // �����
  #define KEY 0 
  // ��������� ���������
	#define	_IS_ 		0xD0
	#define	_S_ 		0x3D
	#define	_M_ 		0x39
	#define	_P_ 		0x2D
	#define	_K_ 		0x00

  // ��������� ������������� ��������� 
  //  #define port_init() { PORTA = 0; DDRA  = 0x81;PORTB = 0x60; DDRB = 0x96;DDRC  = 0xFC;PORTC = ~0xFC;DDRD  = 0xE1;PORTD = ~0xE1; DDRE  = 0x02;PORTE = ~0x02;}
   #define port_init() {   PORTA = 0;   DDRA  = 0x81; PORTB = 0x60; DDRB  = 0x97;  DDRD  = 0xF9; PORTD = ~0xF9; }
  // ���������� ��� ������
  #define CSREG PORTB
  #define CSPIN 7
  #define CANOUT_ID 0x55
  #define BAUDRATE_500
  #define CAN_INPUTID 0x44
  #define PLATFORM_SET_OK
#endif


#ifdef TESTPALTFORM162
  // ��������� ���
  #define MCU_ATMEGA162
  
  #define port_init() {  PORTA = 0xFC; DDRA  = 0xff; PORTB = 0x06; DDRB  = 0xb9; PORTC = 0x0F; DDRC  = 0x0F; PORTD = 0xFF; DDRD  = 0xFF; PORTE = 0x03; DDRE  = 0x03;}
  // �����
  #define KEY 0 
  // ��������� ���������
	#define	_IS_ 		0x32
	#define	_S_ 		0x2C
	#define	_M_ 		0xCA
	#define	_P_ 		0x27
	#define	_K_ 		0x00

  // ���������� ��� ������
  #define CSREG PORTB
  #define CSPIN 0
//  #define CSDDR DDRB
  #define CANOUT_ID 0x55

  #define BAUDRATE_500

  #define CAN_INPUTID 0x44

  #define PLATFORM_SET_OK
#endif




#ifdef PLATFORM_ULSI
#define MCU_ATMEGA162
 // �����
   #define KEY 0
   // ��������� ���������
	#define	_IS_ 		0x5C
	#define	_S_ 		0xF1
	#define	_M_ 		0x6A
	#define	_P_ 		0x19
	#define	_K_ 		0x00


// PORTB = 0x00;
// DDRB  = 0x08;
#define port_init() { PORTB = 0x00; DDRB = 0x08;}
#define CSREG PORTB
#define CSPIN 3
//#define CSDDR DDRB
#define CANOUT_ID 0x55
#define CAN_INPUTID 0x44
#define BAUDRATE_500



  #define PLATFORM_SET_OK
#endif



/*  ��� ���� - 03*/
#ifdef PLATFORM_BAU_SVIO_3
#define MCU_ATMEGA128
 // �����
   #define KEY 0
   // ��������� ���������
	#define	_IS_ 		0x5C
	#define	_S_ 		0xF1
	#define	_M_ 		0x6A
	#define	_P_ 		0x19
	#define	_K_ 		0x00


#define port_init() { PORTB = 0x00; DDRB  = 0x16;}
#define CSREG PORTB
#define CSPIN 4

#define CANOUT_ID 0x55
#define CAN_INPUTID 0x44
#define BAUDRATE_500



  #define PLATFORM_SET_OK
#endif
#ifndef PLATFORM_SET_OK
 #error ���������� ��������� ���������! 
#endif


// ������������ �������� ����� � ��������
// ������� 0-5
#define CAN_maxFilterNum 5
// ����� 0 � 1
#define CAN_maxMaskNum 1

// max ����� ������� ��������� �������� � ��������� mcp2510
#define CAN_MAX_TRY 100



/*********************************************************/
/*                 S T R U C T U R E    	             */
/*********************************************************/	
typedef struct 
{
    unsigned char CNTRL;	 // 
	unsigned char SIDH;      // CAN_ID H
    unsigned char SIDL;      // CAN_ID L
    unsigned char EID8;      // ������������� ������������ ������
    unsigned char EID0;      //
	unsigned char DLEN;		 // �����
    unsigned char data[8];   // ������
} MCP_CAN_MESSAGE;

/*
typedef struct 
{
	unsigned int  ID;		 // CAN_ID 11 bit align to right (0x000 - 0x7FF)
	unsigned char DLEN;		 // ����� (0-8)
    unsigned char data[8];   // ������
} USER_CAN_MESSAGE;

#define USER_CAN_MESSAGE_SIZE sizeof(USER_CAN_MESSAGE)
*/
// *****************************************************
//                D E C L R A T I O N S
// *****************************************************




//===================   liteSPI=============================

#ifdef MCU_ATMEGA162    
	// Atmega 162 SPI pins configuration
	#include <iom162v.h>
	#define DDR_SPI DDRB
	#define DD_MOSI DDB5
	#define DD_MISO DDB6
	#define DD_SCK DDB7
	#define DD_SS DDB4
	#define MCU_SET_OK
#endif

#ifdef MCU_ATMEGA128    
	// Atmega 128 SPI pins configuration
	#include <iom128v.h>
	#define DDR_SPI DDRB
	#define DD_MOSI DDB2
	#define DD_MISO DDB3
	#define DD_SCK DDB1
	#define DD_SS DDB0
	#define MCU_SET_OK
#endif



#ifndef MCU_SET_OK
 #error �������� MCU �� �������������� ���� ������� ��������! 
#endif




// SPI initialisation
// Master, clock rate: 4 000 000hz
void SPI_masterInit(void);

// �������� ����� ������ ����� SPI
// (�� �������� ������������� CS!)
void SPI_MasterTransmit(char cData);


#endif





//****************************************************************









// ***************************
// CAN ������� �������� ������
// ***************************

//-----------------------------------------
// �������������� CAN ���������� 
//-----------------------------------------

//-----------------------------------------
// RETURN: 1 - Init ok 
// 		   0 - Error
//-----------------------------------------
unsigned char CAN_init(void);

// -----------------------------------
// ������� TX_MSG ����� CAN controller
// -----------------------------------
// ���������: CAN_MESSAGE
// -----------------------------------
// RETURN: 1 - ��������� �������� � ��������� ������ 
//         0 - ��� ��������� TX ��������
// -----------------------------------   
unsigned char CAN_Msg_Send(MCP_CAN_MESSAGE *TX_MSG);


// -------------------------------------------------
// ��������� ��������� ��������� �� �������� ������ MCP2510
// -------------------------------------------------
// ���������: ���
// -------------------------------------------------
// RETURN: 0 - ��� ������
// 		   1 - ��������� �� ������� RX0
//         2 - ��������� �� ������� RX1 
// 		   RX_MSG - �������� ���������
// -------------------------------------------------
unsigned char CAN_msg_Read(MCP_CAN_MESSAGE *RX_MSG);


// =================================================================
// ��������� �������
// =================================================================

// �������� ����� ����������������
char CAN_ConfigMode(void);

// �������� ���������� �����
char CAN_NormalMode(void);

// =================================================================
// ��������� ������� ������� ������
// =================================================================

// ����� �����������
void CAN_Reset(void);

// ������ ������� �� ������ cAdr
unsigned char CAN_READ(unsigned char cAdr);

// ������ � ������� �� ������ cAdr, ����� ������ cDATA
void CAN_WRITE(unsigned char cAdr, unsigned char cDATA);

// 

// ��������� ��������� ������ ��������
// cAddr - ����� ��������, cMask - ����� ��������� (��� 1 - ������, 0 - �� ������), cData - ����� ��������
void CAN_bit_modify (unsigned char cAdr, unsigned char cMask, unsigned char cDATA);

// ��������� ������ CAN �����������
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
unsigned char CAN_read_status (void);

