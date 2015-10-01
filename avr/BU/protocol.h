/*============================================================================================
*	�������� ������ ����� �� � �� ������, header
*=============================================================================================*/
//#ifndef PROTOCOL_BU_H_
//#define PROTOCOL_BU_H_
#include <types.h>
#include "main.h"
/*****************************************************
	***	 �������������� ��� ������ 	***
*****************************************************/
/*��������� ����� ����� �������*/
#define BRAKE_END_LEFT_SIDE 0x1A
/*��������� ����� ������ �������*/
#define BRAKE_END_RIGHT_SIDE 0x1B
/*��������� ����� ��������*/
#define BRAKE_END_FRONT 0x1C

/*����������� ����� ��������*/
#define NONBRAKE_END_FRONT 0x1F
/*����������� ����� ����� �������*/
#define NONBRAKE_END_LEFT_SIDE 0x1D
/*����������� ����� ������ �������*/
#define NONBRAKE_END_RIGHT_SIDE 0x1E

/*����������� ����� �� ����*/
#define DEBUG_IN_ID 0x2C

/*����������� ����� �� ��������*/
#define DEBUG_OUT_ID 0x100
#define DEBUG_OUT_STR_MAX 2


/*****************************************************
	***	���� ������: ������� ��� ��������	***
*****************************************************/
/*�������*/
#define DOOR_TYPE_SIDE	0x61
/*��������*/
#define DOOR_TYPE_FRONT 0x54
/*****************************************************
	***	�������������� ������� �� �� 	***
*****************************************************/
/*����� � ��������� ��� ��*/
#define PACKAGE_COMMAND				0x2A
/*����� � ����������� ������*/
#define PACKAGE_SETTINGS			0x2B
/*****************************************************
	***	���� ��� ����� ������� 	***
*****************************************************/
/*����� �������*/
#define NEW_CMD			0
/*��������� �������*/
#define STS_TAKEN		1
/*��������� ������*/
#define COM_STS			2
/*����� ������ �� ���������*/
#define GET_STS			3
/*������� � ������ �����*/		
#define MAN_MODE		4
/*****************************************************
	***	����� ������ ��� ������ 	***
*****************************************************/
/*cmd	-	����� ������ ��� ������		
		1	-	�������
		2	-	�������
		3	-	��������������
		4	-	�������������
		5	-	�����������
		6	-	���������� ����� ��������� �����
		7	-	������ �������� �����
		8	-	����� ��������
		9	-	�� ������ ��������
*/

#define CMD_OPEN_DOOR 0x01
#define CMD_CLOSE_DOOR 0x02
#define CMD_UNLOCK 0x03
#define CMD_LOCK 0x04
#define CMD_NEW_STS 0x06
#define CMD_GET_STS 0x07
#define CMD_NEW_FIRMWARE 0x08
#define CMD_BU_NOW_SETUP 0x09

/*****************************************************
	***	HZ	***
*****************************************************/
/*������������ ������ ������� ��� ��������*/
#define MAX_COUNT_OFF_SETTINGS		21

/*������������ ������ ������� ��� �������� ��������� �����*/
#define MAX_COUNT_OFF_STATE			7
/*�������� �������� ���������, ��*/
#define INTERVAL_BETWEEN_SENDING	300

/*****************************************************
	***	������� 	***
*****************************************************/

/*��������� ����� "����� �������"*/
#define SET_FLAG_NEW_CMD	events_flags|=BIT(NEW_CMD)
/*����� ����� "����� �������"*/
#define CLEAR_FLAG_NEW_CMD	events_flags&=~BIT(NEW_CMD)

/*��������� ����� "��������� �������"*/
#define SET_FLAG_STS_TAKEN	events_flags|=BIT(STS_TAKEN)
/*����� ����� "��������� �������"*/
#define CLEAR_FLAG_STS_TAKEN	events_flags&=~BIT(STS_TAKEN)

/*��������� ����� "���� ���������"*/
#define SET_FLAG_COM_STS	events_flags|=BIT(COM_STS)
/*����� ����� "���� ��������� "*/
#define CLEAR_FLAG_COM_STS	events_flags&=~BIT(COM_STS)

/*��������� ����� ������� � ������ �����*/	
#define SET_FLAG_MAN_MODE	events_flags|=BIT(MAN_MODE)	
/*����� ����� ������� � ������ �����*/	
#define CLEAR_FLAG_MAN_MODE	events_flags&=~BIT(MAN_MODE)


/*****************************************************
	***	 ��� �������� ���������	***
******************************************************/
//{
/*=== ������� ����� ===*/
//{
/*�������*/
#define SET_STATE_B_OPEN	buff_state[0]|=BIT(0)
#define CLR_STATE_B_OPEN	buff_state[0]&=~BIT(0)

/*�������*/
#define SET_STATE_B_CLOSE	buff_state[0]|=BIT(1)
#define CLR_STATE_B_CLOSE	buff_state[0]&=~BIT(1)

/*����� �������*/
#define SET_STATE_B_INWARD	buff_state[0]|=BIT(2)
#define CLR_STATE_B_INWARD	buff_state[0]&=~BIT(2)

/*������� �� ����*/
#define SET_STATE_B_LOCK	buff_state[0]|=BIT(3)
#define CLR_STATE_B_LOCK	buff_state[0]&=~BIT(3)

/*������� "��������"*/
#define SET_STATE_B_SEKRETKA	buff_state[0]|=BIT(4)
#define CLR_STATE_B_SEKRETKA	buff_state[0]&=~BIT(4)

/*�������� �������*/
#define SET_STATE_B_FOOTBOARD	buff_state[0]|=BIT(5)
#define CLR_STATE_B_FOOTBOARD	buff_state[0]&=~BIT(5)

/*����� ��������������*/
#define SET_STATE_B_FREEZING	buff_state[0]|=BIT(6)
#define CLR_STATE_B_FREEZING	buff_state[0]&=~BIT(6)

/*������ � ������*/
#define SET_STATE_B_PARASITE	buff_state[0]|=BIT(7)
#define CLR_STATE_B_PARASITE	buff_state[0]&=~BIT(7)

/*������������ ����� ������ - ������*/
#define SET_STATE_B_SUPP_MODE_MANUAL	buff_state[1]|=BIT(0)
#define CLR_STATE_B_SUPP_MODE_MANUAL	buff_state[1]&=~BIT(0)

/*������������ ����� ������ - ��������������*/
#define SET_STATE_B_SUPP_MODE_AUTO	buff_state[1]|=BIT(1)
#define CLR_STATE_B_SUPP_MODE_AUTO	buff_state[1]&=~BIT(1)

/*�������� ����� 5 ��/� */
#define SET_STATE_B_V5	buff_state[1]|=BIT(2)
#define CLR_STATE_B_V5	buff_state[1]&=~BIT(2)

/*�������� ����� 45 ��/� */
#define SET_STATE_B_V45	buff_state[1]|=BIT(3)
#define CLR_STATE_B_V45	buff_state[1]&=~BIT(3)

/*������� ����� ������: 0 � ������, 1 � ��������������*/
#define SET_STATE_B_MANUAL_MODE	buff_state[1]|=BIT(4)
#define CLR_STATE_B_MANUAL_MODE	buff_state[1]&=~BIT(4)

/*************/
/*������� �������������*/
#define SET_STATE_B_REMOVABLE_ERR	buff_state[1]|=BIT(5)
#define CLR_STATE_B_REMOVABLE_ERR	buff_state[1]&=~BIT(5)
/**************/
/*������ ������ �������*/
#define SET_STATE_B_KO				buff_state[1]|=BIT(6)
#define CLR_STATE_B_KO				buff_state[1]&=~BIT(6)

/*������ ������ �������*/
#define SET_STATE_B_KZ				buff_state[1]|=BIT(7)
#define CLR_STATE_B_KZ				buff_state[1]&=~BIT(7)




/*�������� ���������������� ������� ����� �1*/
#define CLR_STATE_B_OPERABLE_M1	buff_state[2]|=BIT(0)
#define SET_STATE_B_OPERABLE_M1	buff_state[2]&=~BIT(0)

/*�������� ���������������� ���������� �������� �2*/
#define CLR_STATE_B_OPERABLE_M2	buff_state[2]|=BIT(1)
#define SET_STATE_B_OPERABLE_M2	buff_state[2]&=~BIT(1)

/*�������� ���������������� ���������� �������� �3*/
#define CLR_STATE_B_OPERABLE_M3	buff_state[2]|=BIT(2)
#define SET_STATE_B_OPERABLE_M3	buff_state[2]&=~BIT(2)

/*�������� ����� MF1 ���������������� ������� �����*/
#define CLR_STATE_B_OPERABLE_MF1	buff_state[2]|=BIT(3)
#define SET_STATE_B_OPERABLE_MF1	buff_state[2]&=~BIT(3)

/*�������� ������������� ����������� ����������� �������� S11*/
#define CLR_STATE_B_OPERABLE_S11	buff_state[2]|=BIT(4)
#define SET_STATE_B_OPERABLE_S11	buff_state[2]&=~BIT(4)

/*�������� ������������� ����������� ����������� ���������� S9*/
#define CLR_STATE_B_OPERABLE_S9	buff_state[2]|=BIT(5)
#define SET_STATE_B_OPERABLE_S9	buff_state[2]&=~BIT(5)

/*�������� ������������� ����������� ����������� �������� S10*/
#define CLR_STATE_B_OPERABLE_S10	buff_state[2]|=BIT(6)
#define SET_STATE_B_OPERABLE_S10	buff_state[2]&=~BIT(6)

/*�������� ������������� ����������� ����������� �������� 1� S1*/
#define CLR_STATE_B_OPERABLE_S1	buff_state[2]|=BIT(7)
#define SET_STATE_B_OPERABLE_S1	buff_state[2]&=~BIT(7)


/*�������� ������������� ����������� ����������� �������� 2� S2*/
#define CLR_STATE_B_OPERABLE_S2	buff_state[3]|=BIT(0)
#define SET_STATE_B_OPERABLE_S2	buff_state[3]&=~BIT(0)

/*�������� �������� Y1*/
#define CLR_STATE_B_OPERABLE_Y1	buff_state[3]|=BIT(1)
#define SET_STATE_B_OPERABLE_Y1	buff_state[3]&=~BIT(1)

/*�������� �������� Y2*/
#define CLR_STATE_B_OPERABLE_Y2	buff_state[3]|=BIT(2)
#define SET_STATE_B_OPERABLE_Y2	buff_state[3]&=~BIT(2)

/**!!!**************!!!*************!!!**/
/*������������ ������*/
#define SET_STATE_B_FATAL_ERR	buff_state[3]|=BIT(3)
#define CLR_STATE_B_FATAL_ERR	buff_state[3]&=~BIT(3)
/**!!!**************!!!*************!!!**/

/*�������� ���� �������� ������ S12, S13*/
#define CLR_STATE_B_OPERABLE_S12	buff_state[3]|=BIT(4)
#define SET_STATE_B_OPERABLE_S12	buff_state[3]&=~BIT(4)

/*�������� ������ �̸����� ����� 1� S3*/
#define CLR_STATE_B_OPERABLE_S3	buff_state[3]|=BIT(5)
#define SET_STATE_B_OPERABLE_S3	buff_state[3]&=~BIT(5)

/*�������� ������ �̸����� ����� 2� S4*/
#define CLR_STATE_B_OPERABLE_S4	buff_state[3]|=BIT(6)
#define SET_STATE_B_OPERABLE_S4	buff_state[3]&=~BIT(6)

/*������ �������� �� "����(""��������")*/
#define CLR_STATE_B_LOCKERR	buff_state[3]|=BIT(7)
#define SET_STATE_B_LOCKERR	buff_state[3]&=~BIT(7)
/*���������/������� ��� ����� ������ �������	*/
#define SET_STATE_B_ACCEPTED	buff_state[5]|=BIT(0)
#define CLR_STATE_B_ACCEPTED	buff_state[5]&=~BIT(0)


/*����� ��������/�������� �����*/
#define TIME_TO_OPEN_CLOSE				buff_state[6]

/*������ ��������
���� �� main.h FIRMWARE_VERSION*/
//}
/*=== �������� ����� ===*/
//{
/*�������*/
#define SET_STATE_T_OPEN	buff_state[0]|=BIT(0)
#define CLR_STATE_T_OPEN	buff_state[0]&=~BIT(0)

/*�������*/
#define SET_STATE_T_CLOSE	buff_state[0]|=BIT(1)
#define CLR_STATE_T_CLOSE	buff_state[0]&=~BIT(1)

/*������� �� ����*/
#define SET_STATE_T_LOCK	buff_state[0]|=BIT(2)
#define CLR_STATE_T_LOCK	buff_state[0]&=~BIT(2)

/*������� "��������"*/
#define SET_STATE_T_SEKRETKA	buff_state[0]|=BIT(3)
#define CLR_STATE_T_SEKRETKA	buff_state[0]&=~BIT(3)

/*������� ����� ������: 0 � ������, 1 � ��������������*/
#define SET_STATE_T_MANUAL_MODE	buff_state[0]|=BIT(4)
#define CLR_STATE_T_MANUAL_MODE	buff_state[0]&=~BIT(4)

/*************/
/*������� �������������*/
#define SET_STATE_T_REMOVABLE_ERR	buff_state[0]|=BIT(5)
#define CLR_STATE_T_REMOVABLE_ERR	buff_state[0]&=~BIT(5)
/**************/
/*������ ������ �������*/
#define SET_STATE_T_KO				buff_state[0]|=BIT(6)
#define CLR_STATE_T_KO				buff_state[0]&=~BIT(6)

/*������������ ����� ������ - ������*/
#define SET_STATE_T_SUPP_MODE_MANUAL	buff_state[1]|=BIT(0)
#define CLR_STATE_T_SUPP_MODE_MANUAL	buff_state[1]&=~BIT(0)

/*������������ ����� ������ - ��������������*/
#define SET_STATE_T_SUPP_MODE_AUTO	buff_state[1]|=BIT(1)
#define CLR_STATE_T_SUPP_MODE_AUTO	buff_state[1]&=~BIT(1)

/*������ � ������*/
#define SET_STATE_T_PARASITE	buff_state[1]|=BIT(2)
#define CLR_STATE_T_PARASITE	buff_state[1]&=~BIT(2)

/*����� ��������������*/
#define SET_STATE_T_FREEZING	buff_state[1]|=BIT(3)
#define CLR_STATE_T_FREEZING	buff_state[1]&=~BIT(3)


/*�������� ���������������� ������� ����� �1*/
#define CLR_STATE_T_OPERABLE_M1	buff_state[2]|=BIT(0)
#define SET_STATE_T_OPERABLE_M1	buff_state[2]&=~BIT(0)

/*�������� ����� MF1 ���������������� ������� �����*/
#define CLR_STATE_T_OPERABLE_MF1	buff_state[2]|=BIT(1)
#define SET_STATE_T_OPERABLE_MF1	buff_state[2]&=~BIT(1)

/*�������� ������������� ����������� ����������� �������� S3*/
#define CLR_STATE_T_OPERABLE_S3	buff_state[2]|=BIT(2)
#define SET_STATE_T_OPERABLE_S3	buff_state[2]&=~BIT(2)

/*�������� ������������� ����������� ����������� ���������� S1*/
#define CLR_STATE_T_OPERABLE_S1	buff_state[2]|=BIT(3)
#define SET_STATE_T_OPERABLE_S1	buff_state[2]&=~BIT(3)

/*�������� ������������� ����������� ����������� �������� S2*/
#define CLR_STATE_T_OPERABLE_S2	buff_state[2]|=BIT(4)
#define SET_STATE_T_OPERABLE_S2	buff_state[2]&=~BIT(4)

/*�������� ���� �������� ������ S6*/
#define CLR_STATE_T_OPERABLE_S6	buff_state[2]|=BIT(5)
#define SET_STATE_T_OPERABLE_S6	buff_state[2]&=~BIT(5)

/**!!!**************!!!*************!!!**/
/*������������ ������*/
#define SET_STATE_T_FATAL_ERR	buff_state[2]|=BIT(6)
#define CLR_STATE_T_FATAL_ERR	buff_state[2]&=~BIT(6)
/**!!!**************!!!*************!!!**/


/*����������� ���������� ������*/


/*���������/������� ��� ����� ������ �������	*/
#define SET_STATE_T_ACCEPTED	buff_state[5]|=BIT(0)
#define CLR_STATE_T_ACCEPTED	buff_state[5]&=~BIT(0)

/*���� ������ ��������*/
#define SET_STATE_T_TEST_COMPLETED	buff_state[5]|=BIT(1)
#define CLR_STATE_T_TEST_COMPLETED	buff_state[5]&=~BIT(1)

/*����� ��������/�������� �����*/
#define TIME_TO_OPEN_CLOSE				buff_state[6]

/*������ ��������
���� �� main.h FIRMWARE_VERSION*/
//}
//}

/*****************************************************
	***	PROTOTYPES 	***
*****************************************************/

/********************************************************************
	������� ������������ ������ ��� ��������
********************************************************************/
void
Formation_Of_Data_To_Send(void);
/********************************************
*	������������� CAN
*	door_ID - ������������� �����
*******************************************/
void CAN_SetUp(uint8_t door_ID);
/********************************************
*	������� �������� ��������� ��� ��
*******************************************/
void
CAN_Thread(void);
/***********************************************
*	������� ����� � ������� ��� ���������
************************************************/
void
CAN_ReadMsg(void);

/********************************************
*	������� ������������ ���- ���������
*******************************************/
void
CAN_FormationPackage(void);
//#endif