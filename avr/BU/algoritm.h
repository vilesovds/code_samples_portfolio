/*�������� ������ ��*/
#ifndef	_ALGORITM_BU_H_
#define _ALGORITM_BU_H_

#include <types.h>
#include <macros.h>
#include "protocol.h"

/*****************************************************
	***	DEFINES 	***
*****************************************************/
#warning ��� ����� ���� ����������(� ��������� ������...)
/*������������ ���-�� ���������*/
#define MAX_IMPULS_COUNT  Door.sts_max_impuls

/*�������� ��� ���*/
#define M1A 'A',7 
#define M1B 'A',6 

// �������� ��������� ����, ��
#define I_DEL  100

/*������������ ��������� ��*/
# define OVERHEATING_T	800

/*���� �������� � �������� lamps_reg*/
#define LED_O_GREEN	1
#define LED_Z_GREEN	0

// ������� ������
#define LED_PWR_ON	PORTD|=BIT(5) 
#define LED_PWR_OFF	PORTD&=~BIT(5)

/*���� ��� �������� ������*/
#define C_OPEN 0
#define C_CLOSE 1
#define C_LOCK 2
#define C_UNLOCK 3

#define ON	1
#define OFF	0

/*������ ������*/
enum {
	MODE_FIRST_BLOOD=0,// ������ ������
	MODE_MANUAL,// ������ �����
	MODE_OPENING, // ��������
	MODE_CLOSING,// ��������
	MODE_OPEN, //��������
	MODE_CLOSE,// ���������
	MODE_BREAKAGE, //��������� 
	MODE_BLOKING, // ����������
	MODE_BLOKED, // ��������������
	MODE_INTERMEDIATELY, // � ������������� ���������
	MODE_KEYLOCK, // ������� �� ���� ��� ��������
	MODE_SMOOTHBRAKING // ������� ����������
};

/*��������������� ������*/
enum {
	FAT_BUG_B1,
	FAT_BUG_B2,
	FAT_BUG_B3,
	FAT_BUG_B4,
	FAT_BUG_B5,
	FAT_BUG_B6,
	FAT_BUG_B7,
	FAT_BUG_B8,
	FAT_BUG_T1,
	FAT_BUG_T2,
	FAT_BUG_T3,
	FAT_BUG_T4,
	FAT_BUG_T5,
	FAT_BUG_T6,
	FAT_BUG_T7,
	FAT_BUG_T8
};
enum {
	REM_BUG_B1,
	REM_BUG_B2,
	REM_BUG_B3,
	REM_BUG_B4,
	REM_BUG_B5,
	REM_BUG_B6,
	REM_BUG_B7,
	REM_BUG_B8,
	REM_BUG_T1,
	REM_BUG_T2,
	REM_BUG_T3,
	REM_BUG_T4,
	REM_BUG_T5,
	REM_BUG_T6,
	REM_BUG_T7,
	REM_BUG_T8
};
/*��������� �������� ���� ��������� �1, ���� �������� ����� ���������� � ������������ */
#define MIN_IM1	10
/*�������� ����� ���������� ����� � ��������� �1, ��*/
#define PAUSE_MF	300

/*������ ������� ���������� ������*/
#define PERIOD_ZMIG 10000

/*===  �������===*/
//{
/********************************************
	�����������
*******************************************/
/*��������� �2*/
#define CM2_ON	PORTA|=BIT(4)
#define CM2_OFF	PORTA&=~BIT(4)

/*��������� �3*/
#define CM3_ON	PORTA|=BIT(5)
#define CM3_OFF	PORTA&=~BIT(5)

/*����������� ������� ��������� Y1*/
#define CY1U_ON	 PORTD|=BIT(2)
#define CY1U_OFF PORTD&=~BIT(2)

/*�������� ������� ��������� Y1*/
#define CY1P_ON	 PORTD|=BIT(4)
#define CY1P_OFF PORTD&=~BIT(4)

/*����������� ������� ��������� Y2*/
#define CY2U_ON	 PORTD|=BIT(3)
#define CY2U_OFF PORTD&=~BIT(3)

/*�������� ������� ��������� Y2*/
#define CY2P_ON	 PORTD|=BIT(5)
#define CY2P_OFF PORTD&=~BIT(5)

/*�����*/
#define CM_ON	{PORTG|=BIT(3);ts_5_0=0;}
#define CM_OFF	PORTG&=~BIT(3)

/*���������� ������*/
#define CB_INSIDE_ON	PORTG|=BIT(2)
#define CB_INSIDE_OFF	PORTG&=~BIT(2)

/*������� ������*/
#define CB_OUTSIDE_ON	PORTG|=BIT(0)
#define CB_OUTSIDE_OFF	PORTG&=~BIT(0)


/*����������� ��������� �1*/
#define M1_OFF Stop_PWM()
//}

/*****************************************************
	***	��������� 	***
*****************************************************/
/***********************************************************************************************
	������� ��������� ���-�� ��������� �� ������� ��������� � ����������, ���������� � ���������
************************************************************************************************/
void
Recalc_Position(void);

/****************************************************************
	***	���� ������� �������, ����������� ������� 	***
*****************************************************************/
void 
Door_Manager(void);
/***********************************************************************
	������������ ��������/��������  ����� �� �������� ������� "���������"
RETURN:	0 - ��� �� ��
		1 - �� ������, ���������
*************************************************************************/
uint8_t
Door_WorkPosition(void);

/*************************************************************
 ������������ ��������/�������� ����� ��� ���������� ������� "���������"
 Return: 	0 - ��� �� ��
		1- ��,���������
**************************************************************/
uint8_t 
Door_WorkWithoutPosition(void);

uint8_t
Door_RecVd(void);
void
Chech_Bugs(void);
#endif