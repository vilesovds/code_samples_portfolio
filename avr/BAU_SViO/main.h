

#define DATA_OUT       7      // ???? ?
#define LOKCH_OUT      3      // ???? ?
#define CS_DATA_OUT    4      // ???? ?
#define RESET_DATA_OUT 5      // ???? ?
#define CLOCK_DATA_OUT 6      // ???? ?


/************************************************
	TIME BUFFERS
************************************************/
/*��������*/
#define SIGNAL_J1 (PINC&BIT(2))
#define SIGNAL_J2 (PINC&BIT(1))
#define SIGNAL_J3 (PINC&BIT(0))
#define SIGNAL_J4 (PINF&BIT(0))
#define SIGNAL_J5 (PINF&BIT(1))
#define SIGNAL_BUT1 	(PINA&BIT(0)^BIT(0))
#define SIGNAL_BUT2 	(PINA&BIT(1)^BIT(1))
#define SIGNAL_JUMPER 	(PINA&BIT(2)^BIT(2))
// ������ ��� ��������������� ����������� ������
enum{
	N_SIGNAL_J1,
	N_SIGNAL_J2,
	N_SIGNAL_J3,
	N_SIGNAL_J4,
	N_SIGNAL_J5,
	N_SIGNAL_BUT1,
	N_SIGNAL_BUT2,
	N_SIGNAL_JUMPER,
	N_SIGNAL_BUF_COUNT
};
#define DEBOUNCE_TIME  1500

/*********************************************/

/*��������*/
#define J1 (debounce[N_SIGNAL_J1])
#define J2 (debounce[N_SIGNAL_J2])
#define J3 (debounce[N_SIGNAL_J3])
#define J4 (debounce[N_SIGNAL_J4])
#define J5 (debounce[N_SIGNAL_J5])

#define DBUT1		(debounce[N_SIGNAL_BUT1])
#define DBUT2		(debounce[N_SIGNAL_BUT2])
//��������� ��� ��� �� ������
#define TEST_JAMP	(debounce[N_SIGNAL_JUMPER])

#define LED_RUNN_ON	  	PORTG|=BIT(2)
#define LED_RUNN_OFF	PORTG&=~BIT(2)
#define LED_RUNN_SWITCH	PORTG^=BIT(2)


#define LED_ERR_ON	  	PORTG|=BIT(4)
#define LED_ERR_OFF	  	PORTG&=~BIT(4)
#define LED_ERR_SWITCH	PORTG^=BIT(4)

#define LED_SET_ON	  	PORTG|=BIT(3)
#define LED_SET_OFF	  	PORTG&=~BIT(3)
#define LED_SET_SWITCH	PORTG^=BIT(3)


enum{
	ENABLE_CAL,
	START_CAL,
	END_CAL,
	OUT_CAL
};


/*------------------------------------------------------------------------------------------------------------------
	����� ������� ������
---------------------------------------------------------------------------------------------------------------------*/
#define STARTING_ADDRES_P1	0x0000
#define STARTING_ADDRES_P2	0x001E
#define STARTING_ADDRES_DP	0x003C
#define ADDR_TUST  			0x0044
#define ADDR_VVK1			0x0045
#define ADDR_VVK2			0x0047
#define ADDR_MOTOA			0x0049
#define ADDR_MOTOM			0x004D
/*==============================================================================

FROM PD

==============================================================================*/
#define NumIT   60

// ������� �������
#define Heating_End      0
#define Event_Led_SET    1
#define SendMSG          2      
#define ENA_DATA_ADC     3
#define EnaDispley       4
#define But_plus   7//6/*7*/
#define But_minus  5//7/*5*/
#define But_prog   6//5/*6*/

// ������� EnaStandByInfo
#define WIEW             0 

//�������� �������� ������
#define CLOCK_DATA_IN   3  // ���� �
#define LOKCH_IN        0  // ���� �
#define RESET_DATA_IN   2  // ���� �
#define CS_DATA_IN      1  // ���� �
#define DATA_IN         4  // ���� �

/*#define DATA_OUT       5      // ���� �
#define LOKCH_OUT      4      // ���� �
#define CS_DATA_OUT    5      // ���� �
#define RESET_DATA_OUT 6      // ���� �
#define CLOCK_DATA_OUT 7      // ���� �
*/

// ���������� ���������������� �����������
#define CS_PROC       6
#define IN_PROC       4
#define OUT_PROC      3
#define SCK_PROC      5

 
// ������� ������ ButtonReg
#define EnaBut_plus   0
#define EnaBut_minus  1
#define EnaBut_prog   2

#define FlagButP   4
// �������� ����� G
#define Led_RUN      2
#define Led_SET      3
#define Led_ERR      4

// �������� �������� ������ ErrorReg
#define Err_SET      0
/*==================================================================
 PROTOTYPES
*====================================================================*/
/*================================================================================
*	������ � ������ ���������  ������
*=================================================================================*/
void
Save_motoA(void);
/*=======================================================================================
*	������ � ������ ���������  ������ ����� � ������ ������
*========================================================================================*/
void
Save_motoM(void);
/*======================================================================================
*	���������� �������� 6 ����� ��� �������� ���������� ������ �����
*	INPUT: 
*	 operation - ��� ��������: 	0 - ���������
*						1 - ���������
*=======================================================================================*/
void
Set_motoa_time(uint8_t operation);

/*================================================================================
*	���������� �������� 6 ����� ��� �������� ��������� � ������ ������
*	INPUT: 
*	 operation - ��� ��������: 	0 - ���������
*						1 - ���������
*=================================================================================*/
void
Set_motom_time(uint8_t operation);
/*==================================================================
// ������ ������������
// debounce_time - ����� �������� ������������  (��)
*===================================================================*/
void 
checkbuf(uint8_t sig_num, uint8_t sig_val, uint8_t debounce_time);
/* ========================================================
* ����� � ����������� ��� ���� ������ 
* ���������� �� ������� (1 ms)
* =========================================================*/
void 
check_pins(void);
/*============================================
���������������� �������, 
���������� �� ���������� �� ������� ������ ��
==============================================*/
void
Timer0_1ms_int(void);
/*============================================
���������������� �������, 
���������� �� ���������� �� ������� ������ �������
==============================================*/
void
Timer0_1s_int(void);
/*================================================================
*	��������� 1�� ���� ��� ���������
*=================================================================*/
void 
SetBitForDataOut(uint16_t data);
/*====================================================================
*	��������� �������� �� ����� ����������� ���������� ��������
*=====================================================================*/
void 
OutLed(uint8_t Led);
/*=============================================================================
*		�� ��� � ��� ������� �� ���������� ))
*=============================================================================*/
void port_init(void);
/*=====================================================================================
*	�������� ���������� ���������� �� �������
*======================================================================================*/
void 
on_timer(void);
/*===========================================================================
*	������ ����� 
*============================================================================*/
void 
NopLine(void);
/*============================================================================
*	������ signed int
*=============================================================================*/
uint16_t 
Mod16(int16_t idata);
/*=============================================================================
*	 � ������� ���� ������� ������������ ����� �������
*==============================================================================*/
void 
ErrorLine(void);
/*======================================================================
*	��������� ����� � ������� ��� ����������� �� ������ ������
*=======================================================================*/
void 
LoadDataInLine(signed int Zdata);
/*==========================================================================
*	� ����������� �� � ������ ��������� � ����������� ����������� ������
*==========================================================================*/
void SelectScreen(void);
/*==========================================================================
*	������� ������� �� ������
*===========================================================================*/
void 
OnClickButton(unsigned char button);
/*============================================================================
*	� ����������� �� ������ �������� ����� ������
*=============================================================================*/
void 
LoadScreenError(void);
/*=============================================================================
*	��������� ���������������� ������� �� ������ "P"
*===============================================================================*/
void 
LochButP (void);
/*===============================================================================
*	������ ������������� ���������
*================================================================================*/
void 
ReadDataADC(void);

/*================================================================================
*	�������� ���������� ���������� ����������
*=================================================================================*/
void 
CheckDataADC(void);

/*���������� ��� ���������*/
void 
SendCANPacketMessage(void);
/*=====================================================================
*	��������� � ��������� ��� ���������
*======================================================================*/
void 
ReciveCAN (void);
/*=======================================================================
*  ��������� ������ 
*========================================================================*/
void
ReadDataDIN(void);
/*============================================================================
*	������ ���� ��� ���������� ��  ������������� ������� ��������
*=============================================================================*/
void
Read_DamperIN(void);
/*==============================================================================
*	���������� ����������� ������� ��� ��������.
*===============================================================================*/
void
Write_DamperOUT(void);
/*==============================================================================
*	������� ���������� ���� �� ������ �����
*================================================================================*/
void 
Saw(void);
/*=====================================================================================
*	������� �������� ������������� ������
*=======================================================================================*/
void 
Clear_Calibrate(void);
/*=====================================================================================
*	������������� CAN
*=====================================================================================*/
void 
CAN_SetUp(void);
/*=====================================================================================
*	������������� ���������
*=====================================================================================*/
void
Init_Periphery(void);
/* =====================================================================================
*	������ � �����������
*=====================================================================================*/
void 
Do_Load_Or_Clear_Calibrate(void);
/*=====================================================================================
*	�������� �������� ������������ �������.
*=====================================================================================*/
void
Load_Count_VVK(void);
/*=====================================================================================
* �������� ��������� �� ������
*=====================================================================================*/
void
LoadMotoTime(void);