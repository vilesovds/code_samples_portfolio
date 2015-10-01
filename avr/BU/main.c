//ICC-AVR application builder : 03.09.2009 10:45:18
// Target : M128
// Crystal: 16.000Mhz
#include "main.h"
#include <iom128v.h>
#include <macros.h>
#include <PLATFORM.h>
#include <types.h>
#include <timer0.h>
#include <eeprom.h>
#include <mcp2510.h>
#include "atadc.h"
#include "PWM.h"
/*PROTOCOL*/
#include "protocol.h"
/*DS18s20*/
#include "onewire.h"
#include "ds18S20.h"

/*��������*/
#include "algoritm.h"
/********************************************************************************
*		DEFINES
*********************************************************************************/
#define MAX_DCNT	20


/******************************************************************************
*	 GLOBAL DATA
*******************************************************************************/

/*������� �������������� ������*/
uint8_t en_side_doors=0;
/*
b o - ���� �����

b 1 - ���� ������
*/

/*DS18Sxx*/
extern uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];
uint16_t ts_meas=0,ts_conv=0;
/*�������� �����������*/
int16_t rez[2];
uint8_t err_flag=FALSE;
uint8_t was_send=FALSE;
uint8_t nSensors=0;

/*������� ��������� �����������*/

uint8_t ts_mez_timeout=0;
/*������� ������� ������� ������������ ���-������*/
uint16_t ts_debcan=0;
/*���� ������������� ������� ������*/
uint8_t send_deb=FALSE;
/*�������� ������ ������� �� ����...*/
uint16_t ts_send_deb=0;
/*����� ��������� �������*/
uint16_t buz_time=0;
/*���-�� ��������� �� ������� "���������"*/
uint16_t count_signal_position=0;



/*������� ��������������*/
//uint16_t bug_reg=0;
uint16_t bug_reg=0xFFFF;

/*from PROTOCOL*/
/*������� �������*/
extern uint8_t events_flags;

/*������� ��������� �����*/
extern uint8_t state_reg;
/*��������� ���������� �� ���� ��������*/
/*������ ��� ��������*/
extern uint8_t buff_sts[MAX_COUNT_OFF_SETTINGS];
/*������ ��� �������� ���������*/
extern uint8_t buff_state[MAX_COUNT_OFF_STATE];
extern uint8_t last_cmd;
extern uint16_t ts_delm1;

/*��������� ������*/
extern uint8_t lamps_reg;
	
/*���������� �������*/
struct {
	uint16_t time;
	uint8_t output;
	uint8_t mode;
	uint16_t ts_buz;
	uint8_t level;
}Buzzer = {0,0,CONTINUOUS,0,0};

uint16_t deb_trr=0;
uint16_t deb_td=0;
uint8_t deb_Vd=0;
uint16_t deb_tmp=0;

/*��������� �������� ��� ������ ������� ���������� ������*/
uint16_t ts_1sec_mig=0;
uint16_t ts_start_mig=0;


/*��� ��������� M1*/
uint16_t I_M1=0x00;

/*��������������� ������� �����*/
uint8_t dcnt=0;
/*���� ���������� ���������� �����*/
uint8_t flag_dcnt=0x00;
/*�����*/
TDoor Door;
/*���*/
TPWM PWM={OPEN,0};
extern USER_CAN_MESSAGE deb_msg;

/*������� ������ �� ��*/
extern uint8_t comand_reg;

/*����������� ������*/
//#define BUFLEN 10
//uint16_t ADC_buff[BUFLEN];

/*������� ������� ������� ���������� �� ����*/
uint16_t ts_CAN_man;

/*���� ���������� �� ����*/
uint8_t overcurrent=0x00;

/*��� ��������� ����*/
uint8_t en_Imez=0x00;



/*���� ������� ���������� �� ����*/
extern CAN_management;

extern uint8_t door_B_open_step;
extern uint8_t door_B_close_step;
extern uint8_t door_T_open_step;
extern uint8_t door_T_close_step;
/*������� ���������� ���������� �� ������� "���������", %*/
extern uint8_t door_currentS;

extern uint16_t ts_5_0;

/*����������� ��������� ������� �� ����*/
extern uint8_t Tnar_EPVS;

extern uint16_t t_smeny;
extern uint16_t ts_tr;
extern uint16_t Tp;
extern uint16_t Ti;

extern uint8_t deb_delV;
extern uint8_t deb_errT;

extern uint16_t adc_buf;
/*========================================================================
		�����������
*=========================================================================*/
uint8_t debounce[N_SIGNAL_BUF_COUNT]; // ��������� ������ ����� ��������������� ���������
uint8_t bufval[N_SIGNAL_BUF_COUNT];   // �������� ������������ 
/*****************************************************************************
*	PROCEDURES
****************************************************************************/
void 
port_init(void)
{
 PORTA = 0x0F;
 DDRA  = 0xF0;
 //PORTB = 0x00;
 PORTB = 0xF0;
 DDRB  = 0x0B;
 
 PORTC = 0xFF; //m103 output only
 DDRC  = 0x00;
 
 PORTD = (0x3C^0xFF);
 DDRD  = 0x3C;
 
 PORTE = (0xC2^0xFF);
 DDRE  = 0xC2;
 
 PORTF = 0xF0;
 DDRF  = 0x00;
 
 PORTG = 0x00;
 DDRG  = 0x0F;
}


void
deb_msg_send(void)
{
//	uint16_t ftmp;
	//if(time_interval(ts_debcan)<100) return;
	//DEBUG
	if(time_interval(ts_debcan)<20) return;
	deb_msg.data[0]=PWM.power;
	deb_msg.data[1]=count_signal_position;
	deb_msg.data[2]=(uint8_t )(Tp>>8);
	//deb_msg.data[2]=door_B_open_step;
	deb_msg.data[3]=(uint8_t )(Tp&0x00FF);
	//deb_msg.data[3]= Door.distance;
	deb_msg.data[4]=(uint8_t )(Ti>>8);
	deb_msg.data[5]=(uint8_t )(Ti&0x00FF);
	deb_msg.data[6]=(uint8_t )(I_M1>>8);
	deb_msg.data[7]=(uint8_t )(I_M1&0x00FF);
	
	/*switch(deb_msg.data[0]){
		case 0:{
			deb_msg.data[1]=Door.state;
			deb_msg.data[2]=door_T_open_step;
			deb_msg.data[3]=door_T_close_step;
			deb_msg.data[4]=door_B_open_step;
			deb_msg.data[5]=door_B_close_step;
			
			ftmp=CAN_management|(PWM.direction<<1)|overcurrent|(err_flag<<3);
			deb_msg.data[6]=ftmp;
			deb_msg.data[7]=PWM.power;
			
		}break;
		case 1:{
			deb_msg.data[1]=(uint8_t)I_M1;
			deb_msg.data[2]=(uint8_t )(count_signal_position>>8);
			deb_msg.data[3]=(uint8_t )(count_signal_position&0x00FF);
			deb_msg.data[4]=Door.distance;//door_currentS;
			//deb_msg.data[5]=(uint8_t )(deb_trr>>8);
			//deb_msg.data[6]=(uint8_t )(deb_trr&0x00FF);
			deb_msg.data[5]=(uint8_t )(Ti>>8);
			deb_msg.data[6]=(uint8_t )(Ti&0x00FF);
			deb_msg.data[7]=deb_Vd;
			
			
		}break;
		case 2:{
			//deb_msg.data[1]=(uint8_t )(deb_td>>8);
			//deb_msg.data[2]=(uint8_t )(deb_td&0x00FF);
			
			deb_msg.data[1]=(uint8_t )(Tp>>8);
			deb_msg.data[2]=(uint8_t )(Tp&0x00FF);
			
			//deb_msg.data[3]=(uint8_t )(deb_tmp>>8);
			//deb_msg.data[4]=(uint8_t )(deb_tmp);
			
			deb_msg.data[3]=(uint8_t )((time_interval(ts_tr))>>8);
			deb_msg.data[4]=(uint8_t )(time_interval(ts_tr));
			//������������
			deb_msg.data[5]=(uint8_t )(rez[0]>>8);
			deb_msg.data[6]=(uint8_t )(rez[0]);
			deb_msg.data[7]=nSensors;
			
		}break;
	}
	*/
	CAN_Msg_Send(CAN_PARAMS,&deb_msg);
	ts_debcan=TIME_NOW;
	/*if(deb_msg.data[0]<DEBUG_OUT_STR_MAX) deb_msg.data[0]++;
	else							deb_msg.data[0]=0;
	
	*/
	return ;
}



/*============================= ����������� ================================*/
//{
/********************************************************************
// ������ ������������
// debounce_time - ����� �������� ������������  (��)
**********************************************************************/
void 
checkbuf(uint8_t sig_num, uint8_t sig_val, uint8_t debounce_time)
{
	uint8_t tmpbufval = bufval[sig_num]; 
	if (sig_val){
		tmpbufval++;
	}
	else{
		if (tmpbufval > 0) tmpbufval--;
	}
	if (tmpbufval == 0) debounce[sig_num] = 0;
	if (tmpbufval >= debounce_time){
		debounce[sig_num] = 1;
		tmpbufval = debounce_time;
	}
	bufval[sig_num] = tmpbufval; 
}

/**********************************************************
* ����� � ����������� ��� ���� ������ 
* ���������� �� ������� (1 ms)
************************************************************/
void 
check_pins(void)
{
	checkbuf(N_SIGNAL_D6_7,SIGNAL_D6_7,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D6_6,SIGNAL_D6_6,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D6_2,SIGNAL_D6_2,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D4_2,SIGNAL_D4_2,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D4_3,SIGNAL_D4_3,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D4_6,SIGNAL_D4_6,2/*DEF_DEB_TIME*/);
	checkbuf(N_SIGNAL_D4_7,SIGNAL_D4_7,2/*DEF_DEB_TIME*/);
	checkbuf(N_SIGNAL_D5_2,SIGNAL_D5_2,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D5_3,SIGNAL_D5_3,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D5_6,SIGNAL_D5_6,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D5_7,SIGNAL_D5_7,DEF_DEB_TIME);
	//checkbuf(N_SIGNAL_D6_3,SIGNAL_D6_3,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_DV45,SIGNAL_DV45,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_DV5,SIGNAL_DV5,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_KO,SIGNAL_KO,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_KZ,SIGNAL_KZ,DEF_DEB_TIME);
	
	checkbuf(N_SIGNAL_KR1,SIGNAL_KR1,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_KR2,SIGNAL_KR2,DEF_DEB_TIME);
	
	checkbuf(N_SIGNAL_D7_7,SIGNAL_D7_7,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D7_6,SIGNAL_D7_6,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D7_3,SIGNAL_D7_3,DEF_DEB_TIME);
	checkbuf(N_SIGNAL_D7_2,SIGNAL_D7_2,DEF_DEB_TIME);
}
//}
/*====================== ����� ������������ ================================*/

/*****************************************************
	***	���������� �� ������� 	***
*****************************************************/
void
Timer0_1ms_int(void){
	/*�������� ������������*/
	check_pins();
	/*��������� ����� ��������� �������*/
	if(Buzzer.time) Buzzer.time--;
	
	/*��������*/
	if((ts_start_mig==0)||(Door.sts_zmig==0))	LED_PORT=((lamps_reg<<6)|(LED_PORT&0x3F));
	else{
		if(time_interval(ts_1sec_mig)<TMIG)
			lamps_reg=0b00000011;
		else if((time_interval(ts_1sec_mig)>=TMIG)
				&&(time_interval(ts_1sec_mig)<TMIG*2))
			lamps_reg=0b00000000;
		else if(time_interval(ts_1sec_mig)>=TMIG*2)
		    ts_1sec_mig=TIME_NOW;
		LED_PORT=((lamps_reg<<6)|(LED_PORT&0x3F));
	}
	if(ts_start_mig) ts_start_mig--;
	/*������� ������ "���������"*/
	if (((PIND&BIT(0))==0)&&(dcnt<MAX_DCNT)&&(flag_dcnt)) dcnt++;
	else if(dcnt)dcnt--;
	if (dcnt>=MAX_DCNT){
		//dcnt=0;
		//dcnt=++;
		flag_dcnt=FALSE;
		count_signal_position++;
		Recalc_Position();
	}
	
	/*��������� ������ ���������� �����*/
	if(ts_5_0){
		ts_5_0--;
		if (ts_5_0==0x01) CM_OFF;
	}
	
	if(ts_mez_timeout) ts_mez_timeout--;
	
	if(ts_CAN_man) ts_CAN_man--;
	else CAN_management=FALSE;
	
	if(ts_send_deb) ts_send_deb--;
	else send_deb=FALSE;
	
	if(ts_delm1) ts_delm1--;
}
/***************************************************************************
*	��������� �����������
****************************************************************************/
void
temp_meas_tread(void)
{	uint8_t  i;
	if (was_send==FALSE){
		if(nSensors==0)	nSensors = DS18S20_search_sensors();
		if(nSensors==0){err_flag=TRUE;return;}
		if ( DS18S20_start_meas( NULL/*&gSensorIDs[i-1][0]*/ ) == DS18S20_OK ) {
			was_send=TRUE;
			err_flag=FALSE;
		}
		else {
			err_flag=TRUE;
			return;
		}
	}	
	if (ts_conv==0)	ts_conv=timer0_getCounter();
	//timer0_Delay(DS18S20_TCONV_9BIT);
	/*wait for convert*/
	if (time_interval(ts_conv)<DS18S20_TCONV_9BIT) return;
	else{	
		ts_conv=0;
		for ( i=nSensors; i>0; i-- ){ 
			if ( DS18S20_read_temp( &gSensorIDs[i-1][0], &rez[i-1]) == DS18S20_OK ){ 
				was_send=FALSE;
				err_flag=FALSE;
				ts_mez_timeout=DALAY_MEZ;
			}
			else{
				was_send=FALSE;
				/*ERROR*/
				err_flag=TRUE;
			}
		}
	}
}




/*****************************************************
	�������������� ������� ������ � ����������� � ������
	������������� ������ � ����������� � ����������� 
	�� ��������� � ����������
*****************************************************/
void
Unpack_STS(void)
{
/**/
	/*��� ������� �����*/
	if (Door.type==DOOR_TYPE_SIDE){
		Door.sts_ts=(uint16_t)buff_sts[0]*10;
		Door.sts_Vr=buff_sts[1];
		Door.sts_Vd=buff_sts[2];
		Door.sts_tdp=(uint16_t)buff_sts[3]*100;
		Door.sts_Srp=buff_sts[4];
		Door.sts_tf=(uint16_t)buff_sts[5]*100;
		Door.sts_tzp=(uint16_t)buff_sts[6]*10;
		
		Door.sts_tr=(((uint16_t)(buff_sts[7])<<8)|(buff_sts[8])) *10;
		Door.sts_tzz=(uint16_t)buff_sts[9]*10;
		/*���, ��� �� ���� ��������� � ����������*/
		Door.sts_Im=buff_sts[10];
		Door.sts_tpd=(uint16_t)buff_sts[11]*1000;
		/*� ���������������� ���� ���-������ ���������*/
		Door.sts_Tr1=(int16_t)buff_sts[12]*(-10);
		Door.sts_Tr2=(int16_t)buff_sts[13]*(-10);
		Door.sts_sb=(buff_sts[14]&BIT(0));
		Door.sts_zum=(buff_sts[14]&BIT(1))>>1;
		Door.sts_zmig=(buff_sts[14]&BIT(2))>>2;
		Door.sts_max_impuls=((((uint16_t)buff_sts[15])<<8)|(buff_sts[16]));
	}
	/*��� �������� �����*/
	else if (Door.type==DOOR_TYPE_FRONT){
		Door.sts_Vr=buff_sts[0];
		Door.sts_Vd=buff_sts[1];
		Door.sts_tdp=(uint16_t)buff_sts[2]*100;
		Door.sts_Srp=buff_sts[3];
		Door.sts_tr=((((uint16_t)buff_sts[4])<<8)|(buff_sts[5])) *10;
		Door.sts_top=(uint16_t)buff_sts[6]*100;
		
		Door.sts_tpd=(uint16_t)buff_sts[7]*1000;
		/*���, ��� �� ���� ��������� � ����������*/
		Door.sts_Im=buff_sts[8];
		//Door.sts_Im=buff_sts[8]+20;
		Door.sts_tz=(uint16_t)buff_sts[9]*10;
		/*� ���������������� ���� ���-������ ���������*/
		Door.sts_Tr1=(int16_t)buff_sts[10]*(-10);
		Door.sts_Tr2=(int16_t)buff_sts[11]*(-10);
		
		Door.sts_kzz=(buff_sts[12]&BIT(0))>>0;
		Door.sts_zum=(buff_sts[12]&BIT(1))>>1;
		Door.sts_zmig=(buff_sts[12]&BIT(2))>>2;
//#warning Debug
		//Door.sts_zmig=1;(buff_sts[12]&BIT(2))>>2;
		
		Door.sts_max_impuls=((((uint16_t)buff_sts[13])<<8)|(buff_sts[14]));
	}
	Buzzer.output=Door.sts_zum;
}
/*****************************************************
	������� ������ � �����������
	INPUT: operation
		0 - ��������� �� ������
		1 - �������� � ���������
		2 - ��������� � ������
	RETURN 1 -Ok,0 - error
*****************************************************/
uint8_t 
Work_STS(uint8_t operation)
{
	uint8_t res=0;
	switch (operation){
		case 0:{
			/*��������� ���������� ������ � ������*/
			if (EEPROM_ReadChar(ADR_STS+MAX_COUNT_OFF_SETTINGS)==MG_STS){
				/*���� ��������� ������� - ����������*/
				EEPROM_Read(ADR_STS,&buff_sts[0],MAX_COUNT_OFF_SETTINGS);
				Unpack_STS();
				res=0x01;
			}
			else res=0x00;
		}break;
		case 1:{
			if (Door.type==DOOR_TYPE_FRONT){
			/*����� � ���������*/
				buff_sts[0]=100;
				//buff_sts[0]=80;
				buff_sts[1]=50;
				buff_sts[2]=50;
				buff_sts[3]=50;
				buff_sts[4]=0x01;
				buff_sts[5]=0x90;
				buff_sts[6]=120;
				buff_sts[7]=10;
				//buff_sts[8]=50;
				#warning DEBUG �������� �������� 6 �����
				//buff_sts[8]=120;
				buff_sts[8]=60;
				buff_sts[9]=5;
				buff_sts[10]=25;
				buff_sts[11]=25;
				buff_sts[12]=0;
				buff_sts[13]=0;
				buff_sts[14]=30;
				buff_sts[15]=0;
				buff_sts[16]=0;
				buff_sts[17]=0;
				buff_sts[18]=0;
				buff_sts[19]=0;
				buff_sts[20]=0;
			}
			else if(Door.type==DOOR_TYPE_SIDE){
					buff_sts[0] = 100;
					buff_sts[1] = 100;
					buff_sts[2] = 50;
					buff_sts[3] = 50;
					buff_sts[4] = 50;
					buff_sts[5] = 20;
					buff_sts[6] = 10;
					buff_sts[7] = 1;
					buff_sts[8] = 144;
					buff_sts[9] = 10;
					#warning DEBUG �������� �������� 6 �����
					buff_sts[10] = 60;
					buff_sts[11] = 10;
					buff_sts[12] = 25;
					buff_sts[13] = 25;
					buff_sts[14] = 0;
					buff_sts[15] = 0;
					buff_sts[16] = 30;
					buff_sts[17] = 0;
					buff_sts[18] = 0;
					buff_sts[19] = 0;
					buff_sts[20] = 0;
			}
			Unpack_STS();
			
			
			res=0x01;
		}break;
		case 2:{
			EEPROM_Write(ADR_STS,&buff_sts[0],MAX_COUNT_OFF_SETTINGS);
			EEPROM_WriteChar(ADR_STS+MAX_COUNT_OFF_SETTINGS,MG_STS);
			Unpack_STS();
			res=0x01;
		}break;
		default: res=0x00;
	}
	return res;
}

/********************************************
	���������� ������
*******************************************/
void
Headler_Commands(void)
{
	/*�������, � �� ������ �� ����� ������� �����-������*/
	if(events_flags&BIT(NEW_CMD)){
		/*������ �� �� ������� ������������ ��-�������*/
		switch(last_cmd) {
			case 0:{
			//comand_reg
				events_flags&=~BIT(GET_STS);
				CLEAR_FLAG_NEW_CMD;
				CLR_STATE_B_ACCEPTED;
				//events_flag1&=~BIT(0);
				return;
			}break;
			case CMD_OPEN_DOOR:{
				comand_reg|=BIT(C_OPEN);
				SET_STATE_B_ACCEPTED;
			}break;
			case CMD_CLOSE_DOOR:{
				comand_reg|=BIT(C_CLOSE);
				SET_STATE_B_ACCEPTED;
			} break;
			case CMD_UNLOCK:{
				comand_reg|=BIT(C_UNLOCK);
				SET_STATE_B_ACCEPTED;
			} break;
			case CMD_LOCK:{
				comand_reg|=BIT(C_LOCK);
				SET_STATE_B_ACCEPTED;
			} break;
			case CMD_NEW_STS:{
				/*���� ���� ��� �� �������, ��� ��� ��������� ������*/
				if ((events_flags&BIT(STS_TAKEN))==0){
					/*������ ���� ����� ��������*/
					SET_FLAG_COM_STS;
				}
				else {
					CLEAR_FLAG_STS_TAKEN;
					CLEAR_FLAG_COM_STS;
					Work_STS(2);
					/*��������� ������� ��������� ����� ��������*/
					//events_flag1|=BIT(0);
					SET_STATE_B_ACCEPTED;
					/*������ ����, ��� �� ��������*/
					state_reg|=BIT(0);
					/*��������� � ������, ��� �� ��������*/
					EEPROM_WriteChar(ADR_STATE,0x01);
					//CLEAR_FLAG_NEW_CMD;
				}
			} break;
			case CMD_GET_STS:{
				events_flags|=BIT(GET_STS);
				/*��������� ������� ��������� ����� ��������*/
				SET_STATE_B_ACCEPTED;
				//events_flag1|=BIT(0);
				CLEAR_FLAG_NEW_CMD;
			} break;
			case 8:{
			} break;
			case CMD_BU_NOW_SETUP:{
				events_flags&=~BIT(GET_STS);
				/*������ ����, ��� �� ��������*/
				state_reg|=BIT(0);
				/*��������� � ������, ��� �� ��������*/
				EEPROM_WriteChar(ADR_STATE,0x01);
				/*��������� ������� ��������� ����� �������*/
				SET_STATE_B_ACCEPTED;
				//events_flag1|=BIT(0);
				//CLEAR_FLAG_NEW_CMD;
			} break;
		}
		//CLEAR_FLAG_NEW_CMD;
	}
	else return;/*�� ���� ��� ������ ����������, �� � ������ ��� ���*/
}

/********************************************
	������� ���������  "���������"
*******************************************/
#pragma interrupt_handler int0_isr:2
void 
int0_isr(void)
{
	//external interupt on INT0
	dcnt=0x00;
	flag_dcnt=TRUE;

}

//{ ������
/*****************************************************************
	������
������� ������������� �����, � ������� �������� ������ ������ ������
Input  - ����� ���������, �� 
*******************************************************************/
void
Set_Buzzer(uint16_t time,uint8_t mode)
{
	/*����� �����������, ������ ���� ����� ���������*/
	if((Buzzer.time==0)||(Buzzer.mode!=mode)){
		Buzzer.time+=time;
	}
	Buzzer.mode=mode;
}

/**********************************************************************************************
	������ 
 ������� � ����������� �� ���� �������(����������/�������) ������ ������ ��� � ������� ������,
 ����� ��������� ��� � ������ �������
***********************************************************************************************/
void
Check_Buzzer(void)
{
	/*���� ������ ������ �� �������������, �� ���� �������*/
	if (Buzzer.mode==UNLIMITED){
		if (Buzzer.output==0){
			CB_INSIDE_ON;
			CB_OUTSIDE_OFF;
		}
		else{
			CB_OUTSIDE_ON;
			CB_INSIDE_OFF;
		}
	}
	else if(Buzzer.mode==CONTINUOUS){
		/*� ������ ����������� � ����� �������*/
		if (Buzzer.output==0){
			CB_OUTSIDE_OFF;
			/*����������*/
			/*� ������ ���� �� ������. ������� �� �����*/
			if(Buzzer.time){
				/*�� ������  - ������� �������*/
				CB_INSIDE_ON;
			}
			else {
				/*������ - ������*/
				CB_INSIDE_OFF;
			}
		}
		else{
			CB_INSIDE_OFF;
			/*�������*/
			/*� ������ ���� �� ������. ������� �� �����*/
			if(Buzzer.time){
				/*�� ������  - ������� �������*/
				CB_OUTSIDE_ON;
				
			}
			else {
				/*������ - ������*/
				CB_OUTSIDE_OFF;
			}
		}
	}
	else if(Buzzer.mode==INTERRUPTED){
		/*� ������ ����������� � ����� �������*/
		if (Buzzer.output==0){
			CB_OUTSIDE_OFF;
			/*����������*/
			/*� ������ ���� �� ������. ������� �� �����*/
			if(Buzzer.time){
				/*�� ������  */
				if (time_interval(Buzzer.ts_buz)<BUZZ_CYCLE){
					if(Buzzer.level)CB_INSIDE_OFF;
					else			CB_INSIDE_ON;
				}
				else{
					Buzzer.level^=0x01;
					Buzzer.ts_buz=timer0_getCounter();
				}
			}
			else {
				Buzzer.level=0x00;
				/*������ - ������*/
				CB_INSIDE_OFF;
			}
		}
		else{
			CB_INSIDE_OFF;
			/*�������*/
			/*� ������ ���� �� ������. ������� �� �����*/
			if(Buzzer.time){
				/*�� ������ */
				if (time_interval(Buzzer.ts_buz)<BUZZ_CYCLE){
					if(Buzzer.level)CB_OUTSIDE_OFF;
					else			CB_OUTSIDE_ON;
				}
				else{
					Buzzer.level^=0x01;
					Buzzer.ts_buz=timer0_getCounter();
				}
			}
			else {
				Buzzer.level=0x00;
				/*������ - ������*/
				CB_OUTSIDE_OFF;
			}
		}
		
	}
} 
//}

/********************************************
	�������� ���
*******************************************/
void
Get_I(void)
{	uint32_t temp=0;

	/*������� ���������� �� ���������*/
	//if(en_Imez){
		//temp=ATADC_GetValue();
		temp=adc_buf;
		/*����������� � ���*/
		/*���� ���������� ������ 2.5 � (512 ��������) , �� ��� =0-*/
		if (temp<512) {
			I_M1=0;
		}
		else{
			temp=250*5*temp;
			I_M1=(uint16_t)(temp/1024-625);
			//I_M1=(uint16_t)(temp-625);
		}
		/*��� �� ����� ��������� �� ���������� �������� �������� � ��������� ����*/
		//if (I_M1>Door.sts_Im) overcurrent=0x01;
//#warning Debug
		if (((I_M1>Door.sts_Im)||(I_M1>150))&&(ts_delm1==0)) overcurrent=0x04;
		//else overcurrent=0x00;
	//}
}

//{         ���
/************************************************
	������� ��������� ������ � ����������� ����
	dir - ����������� �������� (�������� - 0 /�������� - 1)
	pow - ������� ���� � %
*************************************************/
void 
Set_PWM(uint8_t dir, uint8_t pow)
{	uint8_t ch=FALSE;
	if (PWM.power!=pow){
		PWM.power=pow;
		ch=TRUE;
	}
	if (PWM.direction!=dir){
		PWM.direction=dir;
		ch=TRUE;
	}
	if(ch) {
		//PWM_Stop();
		//if(PWM.direction)	PWM_Start(M1B,PWM.power);
		//else				PWM_Start(M1A,PWM.power);
		if(PWM.direction)	PWM_Set_Div(M1B,PWM.power);
		else				PWM_Set_Div(M1A,PWM.power);
	}
}

/********************************************
	������� ��������� ����
*******************************************/
void
Stop_PWM(void)
{
	PWM_Stop();
	PWM.power=0;
}
//}
/********************************************
	����������� ������
*******************************************/
uint8_t
Parsing_Addresses(void)
{
	if(((ADDR1)&&(ADDR2==0))&&((ADDR4==0)&&(ADDR3==0))){
		return BRAKE_END_RIGHT_SIDE;
	}
	else if(((ADDR1==0)&&(ADDR2))&&((ADDR4==0)&&(ADDR3==0))){
		return BRAKE_END_LEFT_SIDE;
	}
	else if((ADDR4)&&(ADDR3==0)){
		return BRAKE_END_FRONT;
	}
	else if(((ADDR4)&&(ADDR3))&&((ADDR4==0)&&(ADDR3==0))){
		return NONBRAKE_END_RIGHT_SIDE;
	}
	else if(((ADDR1)&&(ADDR2))&&((ADDR4==0)&&(ADDR3==0))){
		return NONBRAKE_END_LEFT_SIDE;
	}
	else if((ADDR4==0)&&(ADDR3)){
		if(ADDR1)en_side_doors|=BIT(1);
		if(ADDR2)en_side_doors|=BIT(0);
		return NONBRAKE_END_FRONT;
	}
	/*������� ��� - ����� �����*/
	else {
		while(1){
			/*����� �������� "�����-�����!!!"*/
			CB_INSIDE_ON;
			//CB_OUTSIDE_ON;
		}
	}
}


/****************************************************************
* 	   			   MAIN
****************************************************************/
void
main(void)
{	uint16_t tmp_ts=0;
	/*init perephery*/
	Door.state=0;
	port_init();
	timer0_init();
	tmp_ts=TIME_NOW;
	while(time_interval(tmp_ts)<1000) {
		Door.state=0;
	};
	LED_RUN_ON;
	//DEBUG
	//while(1) Set_PWM(OPEN,100);
	PWM_Start(M1A,0);
	ATADC_Init(0);
	CAN_SetUp(Parsing_Addresses());/*BRAKE_END_LEFT_SIDE);*/

	EICRA |= 0x02; //extended ext ints
	EIMSK = 0x01;
	
	/*������� ��������� ���������*/
	if (Work_STS(0)==0) Work_STS(1);
	/*������� ��������� ���������*/
//	if (EEPROM_ReadChar(ADR_STATE)==0x01)  state_reg|=BIT(0);
	//if (EEPROM_ReadChar(ADR_STATE)==0x01) //state_reg|=BIT(0);
	state_reg=EEPROM_ReadChar(ADR_STATE);
	state_reg++;
	//deb_msg.data[0]=EEPROM_ReadChar(ADR_STATE);
	//deb_msg.data[1]=state_reg;
	
	was_send=FALSE;
	err_flag=TRUE;
	/*Forever loop*/
	while(1){
		//���������� ������ �� ����(�������� �����)
		if((FB)||(D24)){
			Door_AlarmClose();
			//Set_Buzzer(10000,INTERRUPTED);
			Door.state=MODE_BREAKAGE;
		}
		if(((Door.state==MODE_MANUAL)
			||(Door.state==MODE_OPEN)
			||(Door.state==MODE_CLOSE)
			||(Door.state==MODE_BLOKED)
			||(Door.state==MODE_BREAKAGE))&&(ts_mez_timeout==0))
		temp_meas_tread();
		/*�������� ���*/
		Get_I();
		Check_Buzzer();
		Chech_Bugs();
		CAN_Thread();
		if(CAN_management==FALSE){
			/*������������ ��������� ��������*/
			Headler_Commands();
			/*����� ������ � ������������ � ���������� ������*/
			Door_Manager();
		}
		/*���� ���� �������������, �� ���������� ����������� �����*/
		if(send_deb) deb_msg_send();
	}
}