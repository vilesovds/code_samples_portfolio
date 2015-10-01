/*=========================================================================================================
*		�������� ������ ����� �� � �� ������
* - v.0.1  - ������
*=========================================================================================================*/
/***********************************************************************************************************
*	***	���������	***
***********************************************************************************************************/
#include "protocol.h"
#include "main.h"
#include "algoritm.h"
/*��������*/
#include "PWM.h"
#include <mcp2510.h>
#include <SPI.h>
#include <types.h>
#include <timer0.h>
#include <crc.h>
/*****************************************************
	***	Defines 	***
*****************************************************/
/*************************************************************************************************************
*	***	Global DATA	***
*************************************************************************************************************/
/*���������� ���������*/
USER_CAN_MESSAGE deb_msg,in_msg,out_msg;

/*����� ������� �������� ������������� ���������*/
uint8_t current_out_str=0;
/*������� �������� ������� */
uint8_t count_in_str=0;
/*����������� ��������� ������� �� ����*/
uint8_t Tnar_EPVS=0;

/*��������� ���������� �� ���� ��������*/
uint8_t last_cmd=0;

/*������ ��� ��������*/
uint8_t buff_sts[MAX_COUNT_OFF_SETTINGS];

/*������ ��� �������� ���������*/
uint8_t buff_state[MAX_COUNT_OFF_STATE]/*={1,2,3,4,5,6,7}*/;



/*������� ��������� �����*/
uint8_t state_reg=0;
/*****************************************************
	***	��������� �������� 	***
*****************************************************/
/*������� �������� ���������*/
uint16_t ts_sendcan=0;

/*****************************************************
	***	����� � ��������		***
*****************************************************/
/*������� �������*/
uint8_t events_flags=0;

/*���� ������� ���������� �� ����*/
uint8_t CAN_management=FALSE;
extern TDoor Door;


/*�������� �����������*/
extern int16_t rez[2];
/*========================================================================
		�����������
*=========================================================================*/
extern uint8_t debounce[N_SIGNAL_BUF_COUNT]; // ��������� ������ ����� ��������������� ���������


/*������� ��������������*/
extern uint16_t bug_reg;

/*�������� �������*/
extern uint16_t fatevent_buf;
extern uint16_t remevent_buf;

/*����� ��������/�������� */
extern uint16_t time_opcls;
/*������� ������� ������� ���������� �� ����*/
extern uint16_t ts_CAN_man;

extern uint8_t door_T_close_step;

/*������� �������������� ������*/
extern uint8_t en_side_doors;


/*���� ������������� ������� ������*/
extern uint8_t send_deb;
/*�������� ������ ������� �� ����...*/
extern uint16_t ts_send_deb;

extern uint16_t ts_5_0;
/*****************************************************
	***	��������� � ������� 	***
*****************************************************/

/********************************************************************
*	***	�������� ������	***
*********************************************************************/
void
CheckAlarms(void)
{
	/*�������� �����*/
	if (fatevent_buf&0xFF00){
		SET_STATE_T_FATAL_ERR;
	}
	else {
		CLR_STATE_T_FATAL_ERR;
	}
	if (remevent_buf&0xFF00){
		SET_STATE_T_REMOVABLE_ERR;
	}
	else{
		CLR_STATE_T_REMOVABLE_ERR;
	}
	/*������� �����*/
	if (fatevent_buf&0x00FF){
		SET_STATE_B_FATAL_ERR;
	}
	else {
		CLR_STATE_B_FATAL_ERR;
	}
	if (remevent_buf&0x00FF){
		SET_STATE_B_REMOVABLE_ERR;
	}
	else{
		CLR_STATE_B_REMOVABLE_ERR;
	}	
}
/********************************************************************
	������� ������������ ������ ��� ��������
********************************************************************/
void
Formation_Of_Data_To_Send(void)
{
	CheckAlarms();
	/*������� ������� ��� �����*/
	if(Door.type==DOOR_TYPE_SIDE){
		/*
		//�������
		if (Door.state==MODE_OPEN)  SET_STATE_B_OPEN;
		else						CLR_STATE_B_OPEN;
		
		//�������
		if ((Door.state==MODE_CLOSE)
			||(Door.state==MODE_BLOKING)
			||(Door.state==MODE_BLOKED)) 	SET_STATE_B_CLOSE;
		else						 		CLR_STATE_B_CLOSE;
		*/
		
		 //��� ������������� �������� �� ���
		//�������
		if (S_OPEN)  	SET_STATE_B_OPEN;
		else			CLR_STATE_B_OPEN;
		
		//������� 
		if ((S_CLOSED)&&(S_COMPRESSED_2)&&(S_COMPRESSED_1) )	SET_STATE_B_CLOSE;
		else			CLR_STATE_B_CLOSE;
		
		
		
		/*����� �������*/
		if((S_COMPRESSED_2)&&(S_COMPRESSED_1))  SET_STATE_B_INWARD;
		else 								 	CLR_STATE_B_INWARD;
		
		/*������� �� ����*/
		if(S_LOCK)	SET_STATE_B_LOCK;
		else		CLR_STATE_B_LOCK;
		
		/*������� "��������"*/
		if(S_SEKRETKA)	SET_STATE_B_SEKRETKA;
		else			CLR_STATE_B_SEKRETKA;
		
		/*�������� �������*/
		if(S_FOOTBOARD)	SET_STATE_B_FOOTBOARD;
		else			CLR_STATE_B_FOOTBOARD;
		
		/*����� ��������������*/
		if ((Door.state==MODE_BLOKED)||(Door.state==MODE_KEYLOCK))//||(S_LOCK)||(S_SEKRETKA))	
				SET_STATE_B_FREEZING;
		else	CLR_STATE_B_FREEZING;
		
		/*������ � ������*/
		if((S_DIST_AL_1)||(S_DIST_AL_2))	SET_STATE_B_PARASITE;
		else								CLR_STATE_B_PARASITE;
		
		#warning ��������
		/*������������ ����� ������ - ������*/
		
		/*������������ ����� ������ - ��������������*/
		
		/*�������� ����� 5 ��/� */
		if(S_V5)	SET_STATE_B_V5;
		else		CLR_STATE_B_V5;
		
		/*�������� ����� 45 ��/� */
		if(S_V45)	SET_STATE_B_V45;
		else 		CLR_STATE_B_V45;
		
		/*������� ����� ������: 0 � ������, 1 � ��������������*/
		if(Door.state!=MODE_MANUAL)	SET_STATE_B_MANUAL_MODE;
		else						CLR_STATE_B_MANUAL_MODE;
		
		/*������ ������ �������*/
		if(S_KO)	SET_STATE_B_KO;
		else 		CLR_STATE_B_KO;
		
		/*������ ������ �������*/
		if(S_KZ)	SET_STATE_B_KZ;
		else 		CLR_STATE_B_KZ;
		
		/********/
		/*������������ */
		/*��������� �1*/
		if(bug_reg&BIT(BUG_M1))	SET_STATE_B_OPERABLE_M1;
		else 					CLR_STATE_B_OPERABLE_M1;
		
		/*��������� �2*/
		if(bug_reg&BIT(BUG_M2))	SET_STATE_B_OPERABLE_M2;
		else 					CLR_STATE_B_OPERABLE_M2;
		
		/*��������� �3*/
		if(bug_reg&BIT(BUG_M3))	SET_STATE_B_OPERABLE_M3;
		else 					CLR_STATE_B_OPERABLE_M3;
		
		/*�������� ����� MF1 ���������������� ������� �����*/
		if(bug_reg&BIT(BUG_MF1))	SET_STATE_B_OPERABLE_MF1;
		else 						CLR_STATE_B_OPERABLE_MF1;
		
		/*�������� ������������� ����������� ����������� �������� S11*/
		if(bug_reg&BIT(BUG_OPEN))	SET_STATE_B_OPERABLE_S11;
		else 						CLR_STATE_B_OPERABLE_S11;
		
		/*�������� ������������� ����������� ����������� ���������� S9*/
		if(bug_reg&BIT(BUG_POS))	SET_STATE_B_OPERABLE_S9;
		else 						CLR_STATE_B_OPERABLE_S9;
		
		/*�������� ������������� ����������� ����������� �������� S10*/
		if(bug_reg&BIT(BUG_CLOSE))	SET_STATE_B_OPERABLE_S10;
		else 						CLR_STATE_B_OPERABLE_S10;
		
		/*�������� ������������� ����������� ����������� �������� 1� S1*/
		if(bug_reg&BIT(BUG_COMPRES1))	SET_STATE_B_OPERABLE_S1;
		else 							CLR_STATE_B_OPERABLE_S1;
		
		/*�������� ������������� ����������� ����������� �������� 2� S2*/
		if(bug_reg&BIT(BUG_COMPRES1))	SET_STATE_B_OPERABLE_S2;
		else 							CLR_STATE_B_OPERABLE_S2;
		
		/*�������� �������� Y1*/
		if(bug_reg&BIT(BUG_Y1))	SET_STATE_B_OPERABLE_Y1;
		else 					CLR_STATE_B_OPERABLE_Y1;
		
		/*�������� �������� Y2*/
		if(bug_reg&BIT(BUG_Y2))	SET_STATE_B_OPERABLE_Y2;
		else 					CLR_STATE_B_OPERABLE_Y2;
		
		/*�������� ���� �������� ������ S12, S13*/
		if(bug_reg&BIT(BUG_PROTECTIVE_EDGE))	SET_STATE_B_OPERABLE_S12;
		else 									CLR_STATE_B_OPERABLE_S12;
				
		/*�������� ������ �̸����� ����� 1� S3*/
		if(bug_reg&BIT(BUG_LOGJAM1))	SET_STATE_B_OPERABLE_S3;
		else 							CLR_STATE_B_OPERABLE_S3;
		
		/*�������� ������ �̸����� ����� 2� S4*/
		if(bug_reg&BIT(BUG_LOGJAM2))	SET_STATE_B_OPERABLE_S4;
		else 							CLR_STATE_B_OPERABLE_S4;
			
		/*������ �������� �� ����*/
	/*	if(bug_reg&BIT(BUG_LOCK))		SET_STATE_B_LOCKERR;
		else 							CLR_STATE_B_LOCKERR;
	*/
		
		/*����� ��������/�������� �����*/
		buff_state[4]=time_opcls;

		
		/*������ ��������*/
		buff_state[6]=FIRMWARE_VERSION;
	}
	/*===��� �������� �����===*/
	else{
		/*
		//�������
		if ((Door.state==MODE_OPEN)
			||((Door.state==MODE_CLOSING)&&(door_T_close_step==0)))  SET_STATE_T_OPEN;
		else						CLR_STATE_T_OPEN;
		
		//�������
		if (Door.state==MODE_CLOSE)  SET_STATE_T_CLOSE;
		else						 CLR_STATE_T_CLOSE;		
		*/
		
		// ��� ������������� �������� �� ���
		//�������
		if (S_OPEN)  	SET_STATE_T_OPEN;
		else			CLR_STATE_T_OPEN;
		
		//�������
		if (S_CLOSED )	SET_STATE_T_CLOSE;
		else			CLR_STATE_T_CLOSE;
		
		
		
		/*������� �� ����*/
		if(S_LOCK)	SET_STATE_T_LOCK;
		else		CLR_STATE_T_LOCK;
		
		/*������� "��������"*/
		if(S_SEKRETKA)	SET_STATE_T_SEKRETKA;
		else			CLR_STATE_T_SEKRETKA;
		
		/*������� ����� ������: 0 � ������, 1 � ��������������*/
		if(Door.state!=MODE_MANUAL)	SET_STATE_T_MANUAL_MODE;
		else						CLR_STATE_T_MANUAL_MODE;
		
		/*������� �������������*/
		#warning TODO
		/*������ ������ �������*/
		if(S_KO)	SET_STATE_T_KO;
		else 		CLR_STATE_T_KO;
		
		#warning ��������
		/*������������ ����� ������ - ������*/
		
		/*������������ ����� ������ - ��������������*/
		
		/*������ � ������*/
		if((S_DIST_AL_1)||(S_DIST_AL_2))	SET_STATE_T_PARASITE;
		else								CLR_STATE_T_PARASITE;
		
		/*����� ��������������*/
		if (Door.state==MODE_BLOKED)	SET_STATE_T_FREEZING;
		else						 	CLR_STATE_T_FREEZING;
		
		/*�������������*/
		/*�������� ���������������� ������� ����� �1*/
		if(bug_reg&BIT(BUG_M1))	SET_STATE_T_OPERABLE_M1;
		else					CLR_STATE_T_OPERABLE_M1;
		
		/*�������� ����� MF1 ���������������� ������� �����*/
		if(bug_reg&BIT(BUG_MF1))	SET_STATE_T_OPERABLE_MF1;
		else						CLR_STATE_T_OPERABLE_MF1;
		
		/*�������� ������������� ����������� ����������� �������� S3*/
		if(bug_reg&BIT(BUG_OPEN))	SET_STATE_T_OPERABLE_S3;
		else						CLR_STATE_T_OPERABLE_S3;
		
		/*�������� ������������� ����������� ����������� ���������� S1*/
		if(bug_reg&BIT(BUG_POS))	SET_STATE_T_OPERABLE_S1;
		else						CLR_STATE_T_OPERABLE_S1;
		
		/*�������� ������������� ����������� ����������� �������� S2*/
		if(bug_reg&BIT(BUG_CLOSE))	SET_STATE_T_OPERABLE_S2;
		else						CLR_STATE_T_OPERABLE_S2;
		
		/*�������� ���� �������� ������ S6*/
		if(bug_reg&BIT(BUG_PROTECTIVE_EDGE))	SET_STATE_T_OPERABLE_S6;
		else									CLR_STATE_T_OPERABLE_S6;
		
		/*����������� � �������*/
	//	if (rez[0]<0 ){
	//		buff_state[3]=-((int8_t)(rez[0]/10));
	//	}
	//	else{
			buff_state[3]=(int8_t)(rez[0]/10);
	//	}
		
		//buff_state[3]=-10;
		/*����� ��������/��������*/
		buff_state[4]=time_opcls;
		/*������ ��������*/
		buff_state[6]=FIRMWARE_VERSION;
	}
}

/********************************************
*	������������� CAN
*	door_ID - ������������� �����
*******************************************/
void 
CAN_SetUp(uint8_t door_ID)
{
	/*�������������� SPI*/
	SPI_masterInit(); 
	/*�������������� mcp2510*/
	CAN_Init(CAN_PARAMS, CAN_BAUD_125);  
	/*��������� ��*/
	CAN_setAcceptFilter(CAN_PARAMS, 1, 0x7FF);
	CAN_setAcceptMask(CAN_PARAMS,0,0x0000);
	/*�������������� ���������� ���������*/
	out_msg.ID=door_ID;
	/*����� ���������� ��� �����*/
	if((out_msg.ID==BRAKE_END_LEFT_SIDE)||(out_msg.ID==BRAKE_END_RIGHT_SIDE)
							||(out_msg.ID==NONBRAKE_END_LEFT_SIDE)||(out_msg.ID==NONBRAKE_END_RIGHT_SIDE)){
		/*������� �����*/
		Door.type=DOOR_TYPE_SIDE;
	}
	else if((out_msg.ID==BRAKE_END_FRONT)||(out_msg.ID==NONBRAKE_END_FRONT)){
		/* �������� �����*/
		Door.type=DOOR_TYPE_FRONT;
	}
	out_msg.DLEN=8;
	
	deb_msg.ID=DEBUG_OUT_ID;
	deb_msg.DLEN=8;
}
/***********************************************
*	������� ����� � ������� ��� ���������
************************************************/
void
CAN_ReadMsg(void)
{
	/*��������� �������*/
	uint8_t i;
	/*�������, � ������ �� ���������?*/
	if(CAN_msg_Read(CAN_PARAMS, &in_msg)){
		/*�������� ��������� �� ��������*/
		switch(in_msg.ID){
			/*�������� �� ��*/
			case PACKAGE_COMMAND:{
				/*������ ������ �������, ���������� ���*/
				//if(last_cmd!=in_msg.data[out_msg.ID-BRAKE_END_LEFT_SIDE]){
					last_cmd=in_msg.data[out_msg.ID-BRAKE_END_LEFT_SIDE];
					SET_FLAG_NEW_CMD;
				//}
				/*��������� �����������*/
				Tnar_EPVS=in_msg.data[6];
				/*���������� ����*/
				
			}break;
			/*��������� �� ��*/
			case PACKAGE_SETTINGS:{
				/*���� ��������� �������� ��� ���*/	
				if(events_flags&BIT(COM_STS)){
						
					/*������� � ��������*/
					if (in_msg.data[0]==0x00){
						/*�������� ���������*/
						for (i=0;i<7;i++){	
							buff_sts[i]=in_msg.data[i+1];
						}
						/*������ �����, ��� ������� ������� ��������*/
						count_in_str|=BIT(0);
					}
					else if(in_msg.data[0]==0x01){
						/*�������� ���������*/
						for (i=0;i<7;i++){	
							buff_sts[i+7]=in_msg.data[i+1];
						}
						/*������ �����, ��� ������� ������ ��������*/
						count_in_str|=BIT(1);
					}
					else if(in_msg.data[0]==0x02){
						/*�������� ���������*/
						for (i=0;i<7;i++){	
							buff_sts[i+14]=in_msg.data[i+1];
						}
						/*������ �����, ��� ������� ������ ��������*/
						count_in_str|=BIT(2);
					}
					/*���������� ����, ���� ������� ��� ��������*/
					if (count_in_str==0x07){
						SET_FLAG_STS_TAKEN;
						/*���������� �������*/
						count_in_str=0x00;
					}
				}
			}break;
			/*CAN_MAN*/
			case DEBUG_IN_ID:{
				/*���� ���� ����� �� ���, �� �������*/
				if(in_msg.data[0]!=out_msg.ID) return;
				else{
						/*������ ����, ��� ����� ���������� �����-������*/
						send_deb=TRUE;
						/*�������� �������� ������, �� ��������� ����� ������� ����, ������� ���� ������ =0*/
						ts_send_deb=1000;
						/*���� �� ��� ����� ������ ���������� �� ����, ��...*/
						if(in_msg.data[1]&BIT(0)){
							/*�������� ������� ���������� �� ����. ����� �� �������, �������� ���� ������ = 0*/
							ts_CAN_man=1000;
							/*������ ����, ��� ����������� �� ����...*/
							CAN_management=TRUE;
							/* ����������� � ���*/
							Set_PWM(in_msg.data[1]&BIT(1),in_msg.data[2]);
							/*�����*/
							if(in_msg.data[1]&BIT(2)) 	CM_ON
							else 						CM_OFF;
							/*��������� �2*/
							if(in_msg.data[1]&BIT(3))	CM2_ON;
							else						CM2_OFF;
							/*��������� �3*/
							if(in_msg.data[1]&BIT(4))	CM3_ON;
							else						CM3_OFF;
							/*����������� ������� ��������� Y1*/
							if(in_msg.data[1]&BIT(5))	CY1U_ON;
							else						CY1U_OFF;
							/*�������� ������� ��������� Y1*/
							if(in_msg.data[1]&BIT(6))	CY1P_ON;
							else						CY1P_OFF;
							/*�������� ������� ��������� Y1*/
							if(in_msg.data[1]&BIT(6))	CY1P_ON;
							else						CY1P_OFF;
							/*������������ ������� ��������� Y2*/
							if(in_msg.data[1]&BIT(7))	CY2U_ON;
							else						CY2U_OFF;
						}
						else CAN_management=FALSE;
					}
			}break;
			/*����������� ������ �� ������������*/
			default:return;
		}
	}
	//else CLEAR_FLAG_NEW_CMD;
}
/********************************************
*	������� ��������� ���- ���������
*******************************************/
void
CAN_ResetMSG(void)
{	uint8_t i;
	for(i=1;i<out_msg.DLEN;i++)
		out_msg.data[i]=0x00;

}
/********************************************
*	������� ������������ ���- ���������
*******************************************/
void
CAN_FormationPackage(void)
{
	uint8_t i;
	uint16_t temp;
	CAN_ResetMSG();
//	out_msg.data[0]=current_out_str;
	switch(out_msg.data[0]){
		case 0:{
			for(i=0;i<4;i++){
				out_msg.data[i+1]=buff_state[i];
			}
			/*��������������� ������*/
			/* 5� ���� ��� ������������*/
			out_msg.data[5] = (uint8_t)((fatevent_buf&0x00FF)|( (fatevent_buf&0xFF00)>>8 ));
			/* 6� ���� ��� ����������*/
			out_msg.data[6] = (uint8_t)((remevent_buf&0x00FF)|( (remevent_buf&0xFF00)>>8 ));
			out_msg.data[7] =0x00;

		} break;
		case 1:{
			/*����������� ����� �������� ��� �����*/
			temp=crc16(&buff_sts[0],21);
			/*CRC.h*/
			out_msg.data[1]=(uint8_t)(temp>>8);
			/*CRC.l*/
			out_msg.data[2]=(uint8_t)(temp&0x00FF);
			/*�������� �� ��*/
			out_msg.data[3]=state_reg;
			
			/*�������������� �����*/
			if(en_side_doors) {
				out_msg.data[4]=en_side_doors;
				if (S_HAND_LS_LOCK) 	out_msg.data[4]|=BIT(2);
				else					out_msg.data[4]&=~BIT(2);
				if (S_HAND_LS_SECRET)	out_msg.data[4]|=BIT(3);
				else					out_msg.data[4]&=~BIT(3);
				if (S_HAND_RS_LOCK)		out_msg.data[4]|=BIT(4);
				else					out_msg.data[4]&=~BIT(4);
				if (S_HAND_RS_SECRET) 	out_msg.data[4]|=BIT(5);
				else 					out_msg.data[4]&=~BIT(5);
			}
			out_msg.data[5] = buff_state[4];
			out_msg.data[6] = buff_state[5];
			out_msg.data[7] = buff_state[6];

		} break;
		/*���������*/
		case 2:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i];
			}
		}break;
		case 3:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i+7];
			}
		}break;
		case 4:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i+14];
			}
		}break;
		default: return;
	}
}
/********************************************
*	������� �������� ��������� ��� ��
*******************************************/
void
CAN_Thread(void)
{
	/*���� ��������� �� ��(� ����� � �� ������ �� ��*/
	CAN_ReadMsg();
	/*�������� �������� �������� ���������*/
	if(time_interval(ts_sendcan)<INTERVAL_BETWEEN_SENDING) return; 

	/*�������� ������ ��� ������������ ���������� ������*/
	Formation_Of_Data_To_Send();
	/*��������� ��������� �����*/
	CAN_FormationPackage();
	/*�� � ������� ��� � ����*/

	CAN_Msg_Send(CAN_PARAMS,&out_msg);
	ts_sendcan=timer0_getCounter();
	/*������ ����� ������������ ��������*/
//	current_out_str++;
out_msg.data[0]++;
	/*���������*/
	if(events_flags&BIT(GET_STS)){
		//if (current_out_str>4)	current_out_str=0;
		if (out_msg.data[0]>4)	out_msg.data[0]=0;
	}
	//else if(current_out_str>1) current_out_str=0;
	else if(out_msg.data[0]>1) out_msg.data[0]=0;
	
}