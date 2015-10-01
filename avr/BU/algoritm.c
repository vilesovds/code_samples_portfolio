/*****************************************************
	***	 �������� ������ �� (�������� �������) 	***
*****************************************************/
#include "main.h"
//#include "PWM.h"
#include <types.h>
#include "protocol.h"
#include "algoritm.h"
#include <PLATFORM.h>
#include <timer0.h>

/*****************************************************
	***	Global DATA 	***
*****************************************************/
//{
uint16_t ts_smoothbraking=0;
uint8_t v_smoothbraking=0;

uint16_t ts_sec2=0;
/*��� ������� ������� ����������� �����*/
uint16_t ts_nr=0;

/*�������� �������*/
uint16_t fatevent_buf;
uint16_t remevent_buf;

/*��� �������*/
uint16_t ts_buz=0;
uint8_t buz_on=0;

/*��� ������: ������� ��� ��������*/
extern uint8_t door_type;
/*������ ��� �������� ���������*/
extern uint8_t buff_state[MAX_COUNT_OFF_STATE];

extern uint8_t debounce[N_SIGNAL_BUF_COUNT]; // ��������� ������ ����� ��������������� ���������

/*���-�� ��������� �� ������� "���������"*/
extern uint16_t count_signal_position;

/*�������� �����������*/
extern int16_t rez[2];

/*����������� ��������� ������� �� ����*/
extern uint8_t Tnar_EPVS;

/*������� �������*/
extern uint8_t events_flags;

/*������� ������� ����*/
extern uint8_t events_flag1;

/*������� ���������� ���������� �� ������� "���������", %*/
uint8_t door_currentS=0x00;

/*��� ��������� �������� ����� �������*/
uint8_t door_B_open_step=0x00;
/*��� ��������� �������� ����� �������*/
uint8_t door_B_close_step=0x00;
/*��� ��������� �������� ����� ��������*/
uint8_t door_T_open_step=0x00;
/*��� ��������� �������� ����� ��������*/
uint8_t door_T_close_step=0x00;
/*��� ������ ���������� ������� �����*/
uint8_t door_B_blok_step=0x00;
/*������� ��������*/
uint8_t revers_count=0x00;
/*������� ���������� ������� "�������"*/
uint8_t close_out_count=0x00;

/*������� ��������������*/
extern uint16_t bug_reg;

/*������� ������ �� ��*/
uint8_t comand_reg=0x00;

// ���� �������� ���������
uint8_t sr_pol=0;

/*****************************************************
	***	��������� �������� 	***
*****************************************************/
//{
/*��� ������� ������ t�*/
uint16_t ts_tr=0;
/*��� ������� ������ t�*/
uint16_t ts_tf=0;
/*��� ������� t�*/
uint16_t ts_ts=0;
/*��� ������� t��*/
uint16_t ts_tpd=0;

/*t��*/
uint16_t ts_tzz=0;
/*��� ������� t��*/
uint16_t ts_top=0;
/*��� ������� t�*/
uint16_t ts_tz=0;
/*��� ������� 1.5 ���*/
uint16_t ts_1_5=0;
/*��� ������� 2 ���*/
uint16_t ts_2_0=0;
/*��� ������� 5 ������*/
uint16_t ts_5_0=0;
/*1 ���*/
uint16_t ts_1_0=0;
/*15 ���*/
uint16_t ts_15_0=0;

uint16_t ts_tzp=0;
/*����� ����� ������ � �1*/
uint16_t ts_MM1=0;
//}
//�������� ������� "�������" ��� ������������ ���������� �� ����
uint16_t ts_wClose=0;

/*����� �������� � ��������*/
uint16_t ts_time_open=0;
uint16_t ts_time_close=0;
uint16_t time_opcls=0;
/*������� ��������*/
uint8_t lamps_reg=0x00;
/*  
	0 - ����
	1 - ���.
	______________________________
	|������	 |    ����         |	���          |
	|_________|_________|__________|
	|		    / �������  - 	0	     |
	|<>(�������)  -|				     |
	|		    \ ������ -	1              |
	|_____________________________|
	|		    / ������� - 	2             |
	| ><(�������) -|				     |
	|		    \  ������ - 	3	     |
	|_____________________________|
*/

/*���� ������ ������� �������� ��� �������� �� ������� � �������������� � �� ��������������� � ������*/
uint8_t flag_start_mig=0;

uint16_t ts_delm1=0;
//
uint16_t t_smeny=0;
uint16_t t_smeny_old=0;

uint8_t megaflag=0;
/*���������� �������*/
extern struct {
	uint16_t time;
	uint8_t output;
	uint8_t mode;
	uint16_t ts_buz;
	uint8_t level;
}Buzzer;

/*��� ��������� M1*/
extern uint16_t I_M1;

extern TDoor Door;

/*������� ��������������*/
extern uint16_t bug_reg;

/*���� ���������� �� ����*/
extern uint8_t overcurrent;

/**/
extern uint8_t err_flag;
extern uint16_t deb_trr;
extern uint16_t deb_td;
extern uint8_t deb_Vd;
extern uint16_t deb_tmp;

/*��������� �������� ��� ������ ������� ���������� ������*/
extern uint16_t ts_start_mig;
//}

//{
uint16_t T=0;
uint16_t t1=0;

uint8_t new_tick=FALSE;
uint8_t Vn1=0;

uint16_t Ti=0;
uint16_t Tp=0;

int16_t errT=0;

uint8_t count_mez_tiks=0;
uint16_t start_ts=0;

extern TPWM PWM;
extern uint16_t adc_buf;

uint8_t deb_delV=0;
uint8_t deb_errT=0;

uint16_t ts_wait_logjams=0;


//������ 
uint16_t bufT[2]={0,0};
//}
/*****************************************************
	***	FUNCTIONS 	***
*****************************************************/

/****************************************************
*
****************************************************/
void
FatalError(uint8_t flag,uint8_t num_err)
{
	if (flag)	fatevent_buf|=BIT(num_err);
	else 		fatevent_buf&=~BIT(num_err);
}
/***********************************************************
*
***********************************************************/
void
RemError(uint8_t flag,uint8_t num_err)
{
	if (flag)	remevent_buf|=BIT(num_err);
	else 		remevent_buf&=~BIT(num_err);
}
/***********************************************************************************************
	������� ��������� ���-�� ��������� �� ������� ��������� � ����������, ���������� � ���������
************************************************************************************************/
void
Recalc_Position(void)
{	uint32_t temp=(count_signal_position*100)/MAX_IMPULS_COUNT;
	door_currentS=(uint8_t)temp;
	Door.distance=door_currentS;
	
	/*�������� ������ ���������� ���������*/
	if(count_signal_position>1)
		T=time_interval(t1);
	t1=TIME_NOW;
	//������� ������
	bufT[0]=bufT[1];
	bufT[1]=T;
	new_tick=0x01;
}

/*
*/
void
SwapPosition(void)
{
	door_currentS =100 - door_currentS;
	Door.distance=door_currentS;
}

/*****************************************************
	������� ������ ������� ������� 	
	������ ���������� � ������� ���������
*****************************************************/
void 
Reset_Position(void)
{
	door_currentS=0x00;
	count_signal_position=0;
	count_mez_tiks=0;
	new_tick=FALSE;
	megaflag=0x00;
	
}
/********************************************
	������� ��������� ��������� �����
*******************************************/
void
Door_AlarmClose(void)
{
	/*��� ������ ������� ��� �����*/
	if (Door.type==DOOR_TYPE_SIDE){
		/*��� ������� �����*/
		/*����� ������� � �������� � ������������ ������� ���������� Y1*/
		CY1U_OFF;
		CY1P_OFF;
		/*����. ������� �1*/
		M1_OFF;
		/*���� �2 � �3*/
		CM2_OFF;
		CM3_OFF;
		/*���� ����� ��1*/
		CM_OFF;
		/*���������� ������� �������� � ������������ ������� ��������� Y1*/
		/*������������ ������*/
		//SET_STATE_B_FATAL_ERR;
		//SET_STATE_T_REMOVABLE_ERR;
		Set_Buzzer(0,UNLIMITED);
	}
	else if(Door.type==DOOR_TYPE_FRONT){
	/*��� �������� �����*/
		/*����. ������� �1*/
		M1_OFF;
		/*���� �2 � �3*/
		CM2_OFF;
		CM3_OFF;
		/*���� ����� ��1*/
		CM_OFF;
		/*���������� ������� �������� � ������������ ������� ��������� Y1*/
		/*������������ ������*/
		//SET_STATE_T_FATAL_ERR;
		//SET_STATE_T_REMOVABLE_ERR;
		Set_Buzzer(0,UNLIMITED);
	}
}

/******************************************************************************************
	������� �������� ���������� �������� �� �������, �� ������...
*******************************************************************************************/
/*
uint8_t
Recalc_A(uint16_t ts,uint16_t dT,uint8_t V1,uint8_t V2)
{
	int16_t deltaV=0;
	deltaV=V2-V1;
	deltaV=deltaV/dT;
	return (uint8_t)(V1+deltaV*time_interval(ts));
	
}
*/
/******************************************************************************************
*			������� ������� �������� ��� �������� ����������
*******************************************************************************************/
uint8_t
SmoothBraking(uint16_t dt, uint8_t V)
{
	int16_t tmp=0;
	int8_t res=0;

	if (time_interval(ts_smoothbraking)>=dt){
		M1_OFF;
		return 0;
	}else{
		tmp=(v_smoothbraking-V)*time_interval(ts_smoothbraking);
		tmp/=dt;
		res = (int8_t)((int16_t)v_smoothbraking-tmp);
		if ((res<=V)||(res>PWM.power)) res = 0;
		Set_PWM(PWM.direction,(uint8_t)res);
	}
	return 1;
	
}
/**/
void
Vals2(void)
{
	Tp=bufT[0]+bufT[1];
	Tp/=2;
}
/*
uint8_t 
Vals(void)
{uint8_t res=0;
	
	
	
	count_mez_tiks++;
	switch(count_mez_tiks){
		case 1:{
		}break;
		case 2:{
			Tp+=T;
		}break;
		case 3:{
			Tp+=T;
			Tp/=2;
			count_mez_tiks=0;
			start_ts=TIME_NOW;
			res=0x01;
		}break;
		default: break;
	}
	return res;
}
*/

//{ �������� �����
//������ �������� � ������ �������� ����������
uint8_t
Recalc_V(uint8_t _Va,uint8_t _Vb,uint16_t t){

int32_t V=0;

	V=(int32_t)_Vb-(int32_t)_Va;
	V=V*(int32_t)t;
	V=V/500;
	V=(int32_t)_Va+V;
return (uint8_t)V;
}

/////////////////////
uint8_t
Door_RecVd(void){
uint16_t tmp=0;
uint8_t maxdelV=10;
int8_t delV=0;
	//if(time_interval(ts_tr)<t_smeny){
	if(Door.distance<Door.sts_Srp){
		if (new_tick){
			new_tick=0;
			if (count_signal_position==3){
				Ti=Door.sts_tdp-time_interval(ts_tr)-500;
				Ti=Ti/(MAX_IMPULS_COUNT-3);
				count_mez_tiks=0;
				//��������� �������������
				//Vn0=PWM.power;
				//Vn1=Vn0;
				Vn1=PWM.power;
				return 0;
			}
			else if(count_signal_position>3){
					//������������ ������ ���������
						Vals2();
						//������������ ������ �� �������
						errT=Tp-Ti;
						//������� �� ��������
						if(errT<0) tmp=((~errT)+1);
						else tmp=(uint16_t)errT;
						//���� ���� ���������, �� ������������ � 0
						if (tmp<(Ti/10)) errT=0;
						//for DEBUG
						deb_errT=errT;
						
						//������������ �������� � ��������
						delV=(int8_t)(errT/10);
						//����������� 
						if(delV>0){
							if(delV>maxdelV) delV=maxdelV;
						}
						else{
							tmp=(~delV)+1;
							if(tmp>(maxdelV)) delV=(~maxdelV)+1;
						}
						//for DEBUG
						deb_delV=delV;
						//����������� ������� ��������
						Vn1=PWM.power+delV;
						// �������� ��  "������������"
						if(Vn1>100) Vn1=100;
						if(Door.type==DOOR_TYPE_SIDE){
							// ��� ������� ����������� ��� - 30%
							if(Vn1<30) Vn1=30;
						}
						else {
							// ��� �������� ����������� ��� - 10%
							if(Vn1<10) Vn1=10;
						}
						// �� ���� �������� �����������
						if(Vn1<Door.sts_Vd) Vn1=Door.sts_Vd;
						return 0;
			}
			else return 0;
		}
		else{
			if((count_signal_position>3)){
				Set_PWM(PWM.direction,Vn1);
				return 0;
			}
			else return 0;
		}
	}else{
		//Set_Buzzer(50,CONTINUOUS);
		return 1;
	}
}

/********************************************
	������� �������� ����� �������
*******************************************/
void
Door_T_Open(void)
{
	switch(door_T_open_step){
		case 0:{
			/*���������� �������� "���������"*/
			Reset_Position();
			/*���������� ���� ����������*/
			overcurrent=0x00;
			//��������� ������� ���������
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*������������ ������� "�����" ��� "��������"?*/
			if((S_LOCK)||(S_SEKRETKA)){
				/*����������� ������ 3 ���*/
				Set_Buzzer(3000,INTERRUPTED);
				/*�����*/
				/*�������� ����� �������������  - ����� ������*/
				Door.state=MODE_CLOSE;
				door_T_open_step=0x00;
			}
			else{
				/*������ 1..2 ���*/
				Set_Buzzer(1000,CONTINUOUS);
				/*������ ������� �� ����� MF1*/
				CM_ON;
				ts_MM1=TIME_NOW;
				door_T_open_step++;
			}
		}break;
		case 1:{
			if(time_interval(ts_MM1)>=PAUSE_MF){	
				/*������ ������� �� M1 (� ������� ��������)*/
				//Set_PWM(OPEN,Door.sts_Vr);
				Set_PWM(OPEN,Door.sts_Vd);
				ts_tpd=timer0_getCounter();
				ts_tr=timer0_getCounter();
				door_T_open_step++;
				ts_time_open=TIME_NOW;
			}
		}break;
		case 2:{
			/* 0) */
			/*������������ ������ "�������" ?*/
			if(S_CLOSED==0){
				/*������������ ������� "�����" ��� "��������"*/
				if((S_SEKRETKA)||(S_LOCK)){
					/*���������� ������*/
					//SET_STATE_T_REMOVABLE_ERR;
					/*������� ������� � M1*/
					M1_OFF;
					door_T_open_step=0x05;
				}
			}
			/* 1)*/
			//���� ����������� �� �� �������� ���������,��
			if(sr_pol==0){
			/*��������� 2 ������� �� ������� "���������"*/
				if(count_signal_position>=2){
					if(Door_WorkPosition()) door_T_open_step++;
					else return;
				}
				else {
					if(Door_WorkWithoutPosition()) door_T_open_step++;
					else return;
				}
			}
			else{
				Set_PWM(OPEN,50);
				door_T_open_step++;
			}
		}break;
		case 3:{
			/*�������� ������ "�������"?*/
			if(S_OPEN){
				/*�������� ������ "�������"*/
				bug_reg|=BIT(BUG_OPEN);
				/*������ "���������" ��������?*/
				if(count_signal_position){
					/*���������  ��������*/
					bug_reg|=BIT(BUG_POS);
				}
				else {
					/*���������  ����������*/
					bug_reg&=~BIT(BUG_POS);
					/*���������� ������*/
					//SET_STATE_T_REMOVABLE_ERR;
				}
				/*��������� ������� � M1*/
				M1_OFF;
				/*������� � �����*/
				door_T_open_step=0x04;
			}
			else{
				/*� ������� ��������� M1 ������ ����� t�� ?*/
				if(time_interval(ts_tpd)<Door.sts_tpd) return;
				else {
					/*�� �������� ������ "�������"*/
					bug_reg&=~BIT(BUG_OPEN);
					door_T_open_step=0;
					/*������������ ������*/
					FatalError(ON,FAT_BUG_T1);
					Door_AlarmClose();
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
		
		}break;
		case 4:{
			/*�������� t��*/
			ts_top=TIME_NOW;
			/*����� ��������*/
			//time_opcls=(uint8_t)(time_interval(ts_time_open)/1000);
			time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
			/*DEBUG*/
			deb_tmp=time_opcls;
			door_T_open_step=0;
			/*����� ������*/
			Door.state=MODE_OPEN;
		}break;
		case 5:{
			/*����������� ������*/
			Buzzer.mode=UNLIMITED;
		}break;
	}
}

/********************************************
	������������ �������� ��������
*******************************************/
void
Door_T_Revers(void)
{
	M1_OFF;
	revers_count++;
	if(revers_count>10){
		#warning ����� ��� ��-���� ��������� ��������
		Door_AlarmClose();
		Door.state=MODE_BREAKAGE;
		return;
	}
	else {
		
		if((Door.distance>97)||(count_signal_position==0)){
			/*����� ������� � ����� MF1*/
			CM_OFF;
			/*������ ��� �������� ... � ����� �. 1.4.3.1.3*/
			door_T_open_step=0;
			Reset_Position();
		}
		else{
			/*������ 1..2 ���*/
			Set_Buzzer(1000,CONTINUOUS);
			SwapPosition();
			door_T_open_step=1;
			sr_pol=0;
		}
		
		/*������� � �������� ������*/
		Door.state=MODE_OPENING;
		return;
	}
}

/*************************************************************
 ������������ ��������/�������� ����� ��� ���������� ������� "���������"
 Return: 	0 - ��� �� ��
		1- ��,���������
**************************************************************/
uint8_t 
Door_WorkWithoutPosition(void)
{	uint8_t res=0x00;
	uint8_t tmp=0;
		//���� ����������� - ������� �� ������ �������
	if(PWM.direction==CLOSE){
		/*����� ���� ��� ���������?*/
		if(S_CLOSED) {
			return 0x01;
		}
	}
	else{//�����  - ������� �� ������ �������
		/*����� ���� ��� ���������?*/
		if(S_OPEN) {
			return 0x01;
		}
	}
	//������
	if((time_interval(ts_tr)>=0)&&(time_interval(ts_tr)<=500)){
		tmp=Recalc_V(Door.sts_Vd,Door.sts_Vr,time_interval(ts_tr));
		if(tmp) Set_PWM(PWM.direction,tmp);
	}
	else {
		//����������
		t_smeny=Door.sts_tr-500;
		if((time_interval(ts_tr)>=t_smeny)&&(time_interval(ts_tr)<Door.sts_tr)){
			tmp=Recalc_V(Door.sts_Vr,Door.sts_Vd,time_interval(ts_tr)-t_smeny);
			if(tmp) Set_PWM(PWM.direction,tmp);
		}
	}
	/*������ ����� tr?*/
	if(time_interval(ts_tr)<Door.sts_tr) res=0x00;
	else{
		/*��������� ���, ��� ����� ���������� ����������������� �1*/
		if (I_M1<MIN_IM1)	bug_reg&=~BIT(BUG_M1);
		else 				bug_reg|=BIT(BUG_M1);
		/*������ ������� �� �1 ��������������� �������� Vd, � �������, �������� ��������*/
		Set_PWM(PWM.direction,Door.sts_Vd);
		/*��������� �������*/
		res=0x01;
	}
	return res;
}

/********************************************
	������� �������� ����� ��������
*******************************************/
void
Door_T_Close(void)
{
	switch (door_T_close_step){
		case 0:{
			/*���������� �������� "���������"*/
			Reset_Position();
			/*���������� ���� ����������*/
			overcurrent=0x00;
			//��������� ������� ���������
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*������ ����� t�� � ������� ������� ������� �����?*/
			if (time_interval(ts_top)<Door.sts_top) return;
			else {
				/*������ 1..2 ���*/
				Set_Buzzer(1500,CONTINUOUS);
				/*������ ������� �� M1 (� ������� �������� ��������)*/
				Set_PWM(CLOSE,Door.sts_Vd);
				
				ts_delm1=I_DEL;
				
				/*�������� t��*/
				ts_tr=timer0_getCounter();
				/*�������� t��*/
				ts_tpd=timer0_getCounter();
				door_T_close_step++;
				ts_time_close=TIME_NOW;
			}
		}break;
		case 1:{
			/*0) */
			/*������������ ������ "������ � ������ 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2))	{
				//Door_T_Revers();
				/*������ � ������� �����������*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;

				return;
			}
			if ((overcurrent)&&(S_CLOSED==0)){
				/*�������� ����� t�*/
				ts_tz=timer0_getCounter();
				/*��� �� ��������� ���*/
				door_T_close_step=3;
				return;
			}
			/* 1)*/
			//���� ����������� �� �� �������� ���������,��
			if(sr_pol==0){
				/*��������� 2 �������� �� ������� "��������� ?"*/
				if(count_signal_position>=2){
					if(Door_WorkPosition()) door_T_close_step++;
					else	return;
				}
				else {
					if(Door_WorkWithoutPosition()) door_T_close_step++;
					else return;
				}
			}
			else{
				Set_PWM(CLOSE,50);
				door_T_open_step++;
			}
		}break;
		case 2:{
			/*0)*/
			/*������������ ������ "������ � ������ 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2)){
				Door_T_Revers();
				/*������� � �������� ������*/
				Door.state=MODE_OPENING;
				return;
			}
			/*1)
			�������� ������������ ����*/
			//if (I_M1>Door.sts_Im){
			//if ((overcurrent)&&(Door.sts_kzz)&&(S_CLOSED==0)){
			if ((overcurrent)&&(S_CLOSED==0)){
				/*�������� ����� t�*/
				ts_tz=timer0_getCounter();
				/*��� �� ��������� ���*/
				door_T_close_step++;
				return;
			}
			
			/*2)
			C ������� ��������� �1 ������ ����� t��, � ������ "������� "�����������?*/
			//if((time_interval(ts_tpd)<Door.sts_tpd)&&(S_CLOSED==0)) return;
			if((time_interval(ts_tpd)<12000)&&(S_CLOSED==0)) return;
			else door_T_close_step=0x04;
			
		}break;
		case 3:{
			/*������ ����� t�?*/
			if((time_interval(ts_tz)<Door.sts_tz)&&(S_CLOSED==0)) return;
			else door_T_close_step++;
		}break;
		case 4:{
			/*������������ ������ "������ � ������ 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2)){
				//Door_T_Revers();
				/*������ � ������� �����������*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;
				door_T_close_step=0x00;
				return;
			}
			/*C ������� ��������� �1 ������ ����� t��, � ������ "������� "�����������?*/
			//if((time_interval(ts_tpd)<Door.sts_tpd)&&(S_CLOSED==0)) return;
			/*������������ ������ "�������" ?*/
			if(S_CLOSED){
				close_out_count=0x00;
				/*�������� ������ "�������"*/
				bug_reg|=BIT(BUG_CLOSE);
				/*����� ������ "��������� ������"*/
				//CLR_STATE_T_REMOVABLE_ERR;
				RemError(OFF,REM_BUG_T3);
				/*����� ������� � �1 � MF1*/
				CM_OFF;
				M1_OFF;
				/*������� � ���������� ����*/
				door_T_close_step=0x05;
			}
			else {
				/*���������� ������ "�������"*/
				bug_reg&=~BIT(BUG_CLOSE);

				close_out_count++;
				if(close_out_count>1){
					/*����� ������ ��� ������ ���=> "������������ ������"*/
					/*����� ������� � M1*/
					M1_OFF;
					/*������������ ������*/
					FatalError(ON,FAT_BUG_T2);
					Door_AlarmClose();
					Door.state=MODE_BREAKAGE;
					return;
				}
				else{
					/*���������� ������*/
					//SET_STATE_T_REMOVABLE_ERR;
					RemError(ON,REM_BUG_T3);
					//Door_T_Revers();
					/*������ � ������� �����������*/
					ts_smoothbraking = TIME_NOW;
					v_smoothbraking = PWM.power;
					Door.state = MODE_SMOOTHBRAKING;
					door_T_close_step=0x00;
					/*������� ������� t��*/
					//ts_top=timer0_getCounter();
					/*������� � �������� ������*/
					//Door.state=MODE_OPENING;
				}
			}
		}break;
		case 5:{
			/*����� ��������*/
			//time_opcls=(uint8_t)(time_interval(ts_time_close)/1000);
			time_opcls=(uint8_t)(time_interval(ts_time_close)/100);
			/*DEBUG*/
			deb_tmp=time_opcls;
			door_T_close_step=0;
			/*���������  - ����� ������*/
			Door.state=MODE_CLOSE;
		}break;
	}
}



//}
//{������� �����
/********************************************
	������� �������� ����� �������
*******************************************/
void
Door_B_Open(void)
{
	uint8_t tmp=0;
	switch(door_B_open_step){
		case 0:{
			/*���������� ���� ����������*/
			overcurrent=0x00;
			
			Reset_Position();
			
			//��������� ������� ���������
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*������������ ������� "������ Y2" ��� V>5 ��� V>45 ��� "�����" ��� "��������"*/
			if((S_Y2)||(S_LOCK)||(S_SEKRETKA)||(S_V45)||(S_V5)){
				return;
			}
			else{
				/*������ 1..2 ���*/
				
				Set_Buzzer(1000,CONTINUOUS);
				//Reset_Position();
				/*��������� ���*/
				door_B_open_step++;
				ts_time_open=TIME_NOW;
				return;
			}
		}break;
		case 1:{
			/*������������ ������� "̸����� ����� 1"� "̸����� ����� 2"?*/
			if((S_LOGJAM_2)||(S_LOGJAM_1)){
				/*���. �2 ��� �3 ��������������*/
				if(S_LOGJAM_1){
					CM2_ON;
				}
				if(S_LOGJAM_2){
					CM3_ON;
				}
				/*�������� t�*/
				ts_tf=timer0_getCounter();
				/*��������� ���*/
				door_B_open_step++;
				return;
			}
			else{
				/*������������� �� ��� "����"*/
				door_B_open_step+=3;
				return;
			}
		}break;
		case 2:{
			/*�������� ��������� �������� t�?*/
			if(time_interval(ts_tf)<Door.sts_tf){
				/*��� - ���������  "���"*/
				door_B_open_step++;
				return;
			}
			else {
				/*�� -*/
				/*���������� ������*/
				RemError(ON,REM_BUG_B4);
				/*��������� �2 � �3*/
				CM2_OFF;
				CM3_OFF;
				/*������������  "���"*/
				door_B_open_step+=2;
				return;
			}
		}break;
		case 3:{
			/*������������ ������ "̸����� ����� 1"*/
			if(S_LOGJAM_1==0){
				/*���� �2*/
				CM2_OFF;
			}
			/*������������ ������ "̸����� ����� 2"*/
			if(S_LOGJAM_2==0){
				/*���� �3*/
				CM3_OFF;
			}
			/*��������� ��� ��������� �2 � �3?*/
			tmp=PORTA&0x30;
			if(tmp==0x00){
				/*��������� ���*/
				door_B_open_step++;
				/*������� ���������� ������*/
				RemError(OFF,REM_BUG_B4);
				return;
			}
			else{
				/*�� ��� �����*/
				door_B_open_step--;
				return;
			}
		}break;
		case 4:{
			overcurrent=0x00;
			/*������ ������� �� �������� � ������������ ������� ��������� Y1*/
			CY1U_ON;
			CY1P_ON;
			/*�������� t�*/
			ts_ts=timer0_getCounter();
			/*��������� ���*/
			door_B_open_step++;
			return;
		}break;
		case 5:{
			/*������ ����� t�?*/
			if(time_interval(ts_ts)<Door.sts_ts) return;
			else {
				/*����� ������� � �������� ������� Y1*/
				CY1P_OFF;
				/*������ ������� �� ����� MF1 */
				CM_ON;
				ts_MM1=TIME_NOW;
				door_B_open_step++;
			}
		}break;
		case 6:{
				if(time_interval(ts_MM1)<PAUSE_MF) return;
				
				/*������ ������� �� �������� ��������� M1 ��������������� �������� V�*/
				//Reset_Position();
				Set_PWM(OPEN,Door.sts_Vd);
				//������� ������ ��������� ����
				ts_delm1=I_DEL;
				/*�������� ������ 1.5 ���...*/
				ts_1_5=timer0_getCounter();
				/*�������� ������ 2 ���...*/
				ts_2_0=timer0_getCounter();
				/*�������� ������ t��*/
				ts_tpd=timer0_getCounter();
				/*��������� ���*/
				/*�������� t�*/
				ts_tr=TIME_NOW;
				door_B_open_step++;
				return;
		}break;
		case 7:{
			/*1)*/
			/*������������ ������ �������� "������� 1 � "������� 2 "*/
			if((S_COMPRESSED_2)||(S_COMPRESSED_1)){
				/*������ 1.5 ���?*/
				if(time_interval(ts_1_5)>=1500){
					if (S_COMPRESSED_1){				
						bug_reg&=~BIT(BUG_COMPRES1);
					}
					else {
						bug_reg|=BIT(BUG_COMPRES1);
					}
					if (S_COMPRESSED_2){				
						bug_reg&=~BIT(BUG_COMPRES2);
					}
					else {
						bug_reg|=BIT(BUG_COMPRES2);
					}
					FatalError(ON,FAT_BUG_B1);
					Door_AlarmClose();
					/*������� �� �������� ���*/
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			else {
				/*��� �������� �� "�������" - ������� ���������� ������*/
				RemError(OFF,REM_BUG_B3);
			}
			/*2)*/
			/*������������ ����������� ������� "�������?"*/
			if(S_CLOSED){
				/*������ 2 ���?*/
				if(time_interval(ts_1_5)>=1500){
					FatalError(ON,FAT_BUG_B2);
					Door_AlarmClose();
					/*������� �� �������� ���*/
					//door_B_open_step=0x09;
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
//			else{
//				/*������������ ������ "������ Y2" ��� "�����" ��� "��������"?*/
//				if((S_Y2)||(S_LOCK)||(S_SEKRETKA)){
//					/*�� �� ������� ������ ���������� ������*/
//					SET_STATE_B_REMOVABLE_ERR;
//				}
//			}
			/*3)*/
			//���� ����������� �� �� �������� ���������,��
			if(sr_pol==0){
			/*��������� 2 ������� �� ������� ���������?*/
				if(count_signal_position>2){
					/*�� */
					/*�������� ������������ �������� ����� �� �������� �� ������� "���������"*/
					if(Door_WorkPosition()){
						/*���� ������ ����������� - ���� ������*/
						/*��������� ���*/
						door_B_open_step++;
						return;
					}
				}
				else{
					/*��� */
					/*�������� ������������ �������� ����� ��� ���������� ������� �� ������� "���������"*/
					if(Door_WorkWithoutPosition()){
						/*���� ������ ����������� - ���� ������*/
						/*��������� ���*/
						door_B_open_step++;
						return;
					}
				}
			}
			else{
				Set_PWM(OPEN,70);
				door_B_open_step++;
			}
		}break;
		case 8:{
		/*1)*/
			/*������������ ������ �������� "������� 1 � "������� 2 "*/
			if((S_COMPRESSED_2)||(S_COMPRESSED_1)){
				/*������ 1.5 ���?*/
				if(time_interval(ts_1_5)>=1500){
					if (S_COMPRESSED_1){				
						bug_reg&=~BIT(BUG_COMPRES1);
					}
					else {
						bug_reg|=BIT(BUG_COMPRES1);
					}
					if (S_COMPRESSED_2){				
						bug_reg&=~BIT(BUG_COMPRES2);
					}
					else {
						bug_reg|=BIT(BUG_COMPRES2);
					}
					FatalError(ON,FAT_BUG_B1);
					Door_AlarmClose();
					/*������� �� �������� ���*/
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			else {
				/*��� �������� �� "�������" - ������� ���������� ������*/
				RemError(OFF,REM_BUG_B3);
			}
			/*2)*/
			/*������������ ����������� ������� "�������?"*/
			if(S_CLOSED){
				/*������ 2 ���?*/
				if(time_interval(ts_1_5)>=1500){
					/*������������ ������*/
					FatalError(ON,FAT_BUG_B2);
					Door_AlarmClose();
					/*������� �� �������� ���*/
					//door_B_open_step=0x09;
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			/*�������� ������ "�������"?*/
			if(S_OPEN){
				/*��*/
				/*����� ��������*/
				time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
				/*�������� ������ "�������"*/
				bug_reg|=BIT(BUG_OPEN);
				/*������� ������������ ������*/
				FatalError(OFF,FAT_BUG_B3);
				/*������ "��������� ��������?"*/
				if(count_signal_position){
					/*��*/
					/*���������  ��������*/
					bug_reg|=BIT(BUG_POS);
					RemError(OFF,REM_BUG_B1);
				}
				else{
					/*��� */
					/*���������  ����������*/
					bug_reg&=~BIT(BUG_POS);
					/*�� �� ������� ������ "���������� ������"*/
					//SET_STATE_B_REMOVABLE_ERR;
					RemError(ON,REM_BUG_B1);
				}
				/*��������� ������� � �1*/
				M1_OFF;
				/*��������� ������� � ��������� Y1*/
				CY1U_OFF;
				CY1P_OFF;
				/*�������� 5 ���*/
				ts_5_0=timer0_getCounter();
				/*��������� ���*/
				door_B_open_step++;
				return;
			}
			else{
				/*���, �� ��������*/
				/* � ������� ��������� �1 ������ ����� t��?*/
				if(time_interval(ts_tpd)>=Door.sts_tpd){
					/*�� �������� ������ "�������"*/
					bug_reg&=~BIT(BUG_OPEN);
					FatalError(ON,FAT_BUG_B3);
					Door_AlarmClose();
					/*������ ������ - ����������*/
					Door.state=MODE_BREAKAGE;
					return;
				}
				return;
			}
		}break;
		case 9:{
			/*����� ���������� ����� 5 ������*/
			ts_5_0=5000;
			door_B_open_step++;
		}break;
		case 10:{
			/*����� ��������*/
			//time_opcls=(uint8_t)(time_interval(ts_time_open)/1000);
			//������ ����			time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
			/*��� ��������� ���, ��� ������������ �� ��������, ����� ����� ������*/
			Door.state=MODE_OPEN;
			door_B_open_step=0;
			
		}break;
	};
}


/***********************************************
	������� ������� ��� �������� ����� �������
************************************************/
void
Door_B_Revers(void)
{
	// ������������� ��������� �1
	M1_OFF;
	/*�������� ������ "���������"?*/
	if((Door.distance>97)||(count_signal_position==0)/*||((S_OPEN==0)&&(S_CLOSED==0))*/){
		/*����� ������� � ����� MF1*/
		CM_OFF;
		/*������ ��� �������� ... � ����� �. 1.4.3.1.3*/
		door_B_open_step=4;
		Reset_Position();
	}
	else {
		SwapPosition();
		door_B_open_step=4;
		sr_pol=0;
	}
	door_B_close_step=0;
	/*�������� �����*/
	Door.state=MODE_OPENING;
}


/***********************************************************************
	������������ ��������/��������  ����� �� �������� ������� "���������"
RETURN:	0 - ��� �� ��
		1 - �� ������, ���������
*************************************************************************/
uint8_t
Door_WorkPosition(void)
{
 uint8_t tmp=0;
	//���� ����������� - ������� �� ������ �������
	if(PWM.direction==CLOSE){
		/*����� ���� ��� ���������? ��� ������ ����� t�� ?*/
		if((S_CLOSED)||(time_interval(ts_tpd)>=Door.sts_tpd)){
			megaflag=0x00;
			//t_smeny=TIME_NOW;
			return 0x01;
		}
	}
	else{//�����  - ������� �� ������ �������
		/*����� ���� ��� ���������? ��� ������ ����� t�� ?*/
		if((S_OPEN)||(time_interval(ts_tpd)>=Door.sts_tpd)) {
			megaflag=0x00;
			//t_smeny=TIME_NOW;
			return 0x01;
		}
	
	}
	
	if(megaflag==0){
		t_smeny=Door.sts_tdp-1000;
		if(Door_RecVd()) {
			megaflag=0x01;
			t_smeny=TIME_NOW;
			return 0;
		}
		else return 0;
	}else{
		//if((time_interval(ts_tr)>=t_smeny)&&(time_interval(ts_tr)<(t_smeny+500))){
		if((time_interval(t_smeny)>=0)&&(time_interval(t_smeny)<500)){
			tmp=Recalc_V(Vn1,Door.sts_Vd,time_interval(t_smeny));
			Set_PWM(PWM.direction,tmp);
			return 0;
		}
		//else if((time_interval(ts_tr)>=(t_smeny+500))){
		else if((time_interval(t_smeny)>=500)){
			/*��������� ���, ��� ����� ���������� ����������������� �1*/
			if (I_M1<MIN_IM1)	bug_reg&=~BIT(BUG_M1);
			else 				bug_reg|=BIT(BUG_M1);
			/*������ ������� �� �1 ��������������� �������� Vd, � �������, �������� ��������*/
			Set_PWM(PWM.direction,Door.sts_Vd);
			megaflag=0x00;
			return 0x01;
		}
		else return 0;
	}
	return 0;
}

/********************************************
	������� �������� ����� �������
*******************************************/
void
Door_B_Close(void)
{
	switch (door_B_close_step){
		case 0:{
			/*���������� ���� ����������*/
			overcurrent=0x00;
			//��������� ������� ���������
			if((S_OPEN==0)&&(S_CLOSED==0))sr_pol=0x01;
			else sr_pol=0;
			/*������������ ������ "�������� �������"?*/
			if(S_FOOTBOARD){
				/*������ 1..2 ���*/
				Set_Buzzer(1500,CONTINUOUS);
				/*�������� ����� ��������*/
				ts_time_close=TIME_NOW;
				/*������ � ������� "�������"*/
				if (S_CLOSED){
					/*��������� � ��������*/
					ts_tzz = TIME_NOW;
					door_B_close_step = 5;
					return;
				}
				//Reset_Position();
				/*������ ������� �� �������� � ������������ ������� ��������� Y1*/
				CY1U_ON;
				CY1P_ON;
				/*�������� tc*/
				ts_ts=timer0_getCounter();
				/*��������� ���*/
				door_B_close_step++;
			}else {
				/*�������� ������� �� �� "�������������" ��� V>5 ��� V>45?*/
				if ((comand_reg&BIT(C_LOCK))||(S_V5)||(S_V45)){
					FatalError(ON,FAT_BUG_B6);
					Door_AlarmClose();
					/*����������� ������*/
					Set_Buzzer(0,UNLIMITED);
					/*������ ����� "���������"*/
					Door.type=MODE_BREAKAGE;
				}
			}
		}break;
		case 1:{

			/*������ ����� tc?*/
			if(time_interval(ts_ts)<Door.sts_ts)return;
			else {
				/*����� ������� � �������� � ������������ ������� ���������� Y1*/
				CY1U_OFF;
				CY1P_OFF;
				/*������ ������� �� ����� MF1*/
				CM_ON;
				ts_MM1=TIME_NOW;
				door_B_close_step++;
			}
		}
		case 2:{		
				if(time_interval(ts_MM1)<PAUSE_MF) return;
				/*������ ������� �� ��������� M1(� �������, �������� ��������), ��������������� �������� V�*/
				Reset_Position();
				Set_PWM(CLOSE,Door.sts_Vd);
				ts_delm1=I_DEL;
				ts_tr=TIME_NOW;
				/*�������� t��*/
				ts_tpd=TIME_NOW;
				
				/*������� � ���������� ����*/
				door_B_close_step++;
		}break;
		case 3:{
			/*0)*/
			/*������������ ������� "V>5" ��� "V>45"*/
			if((S_V45==0)&&(S_V5==0)){
				/*�������� ��������� M2 � M3(���������� ��������)?*/
				if(((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0)){
					/*���*//*������������ ������ "������ � ������ 1" ��� "������ � ������ 2"?*/
					if((S_DIST_AL_1)||(S_DIST_AL_2)){
						/*������ 1..2 ���*/
						Set_Buzzer(1500,CONTINUOUS);
						/*������ � ������� �����������*/
						ts_smoothbraking = TIME_NOW;
						v_smoothbraking = PWM.power;
						Door.state = MODE_SMOOTHBRAKING;
						/*������*/
						//Door_B_Revers();
						return;
					}
				}
			}
			/*1)*/
			/*������������ ������ "�������� �������"?*/
			if(S_FOOTBOARD==0){
				/*���*/
				FatalError(ON,FAT_BUG_B7);
				/*��������� ���������*/
				Door_AlarmClose();
				/*����������� ������*/
				Set_Buzzer(0,UNLIMITED);
				/*������ ����� "���������"*/
				Door.state=MODE_BREAKAGE;
				return;
			}
			/*2)*/
			/*���������� �� ����*/
			if((overcurrent)&&(S_CLOSED==0)){
				Set_Buzzer(1500,CONTINUOUS);
				/*������ � ������� �����������*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;
				/**/
				/*���������� ������*/
				RemError(ON,REM_BUG_B5);
				/*������*/
				//Door_B_Revers();
				return;
			}
			else {
				/*������� ���������� ������*/
				RemError(OFF,REM_BUG_B5);
			}
			/*3)*/
			//���� ����������� �� �� �������� ���������
			if(sr_pol==0){
				/*��������� 2 ������� �� ������� "����������"?*/
				if(count_signal_position>=2){
					if(Door_WorkPosition()) door_B_close_step++;
					else return;
				}
				else {
					if(Door_WorkWithoutPosition()) door_B_close_step++;
					else return;
				}
			}
			else{
				// �� �������� ��������� ���� �� ���� ��������
				Set_PWM(CLOSE,50);
				door_B_close_step++;
			}
		}break;
		case 4:{
			
			
			/*0)*/
			/*������������ ������� "V>5" ��� "V>45"*/
			if((S_V45==0)&&(S_V5==0)){
				/*�������� ��������� M2 � M3(���������� ��������)?*/
				if(((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0)){
					/*���*//*������������ ������ "������ � ������ 1" ��� "������ � ������ 2"?*/
					if((S_DIST_AL_1)||(S_DIST_AL_2)){
						/*������ 1..2 ���*/
						Set_Buzzer(1500,CONTINUOUS);
						
						/*������ � ������� �����������*/
						ts_smoothbraking = TIME_NOW;
						v_smoothbraking = PWM.power;
						Door.state = MODE_SMOOTHBRAKING;
						
						/*������*/
						//Door_B_Revers();
						return;
					}
				}
			}
        	/*1)*/
			/*������������ ������ "�������� �������"?*/
			if(S_FOOTBOARD==0){
				/*���*/
				FatalError(ON,FAT_BUG_B7);
				/*��������� ���������*/
				Door_AlarmClose();
				/*����������� ������*/
				Set_Buzzer(0,UNLIMITED);
				/*������ ����� "���������"*/
				Door.state=MODE_BREAKAGE;
				return;
			}
			
			if((overcurrent)&&(S_CLOSED==0)){
				/*������*/
				//Door_B_Revers();
				//������ ��� ������ ������, ������� � ���������, ��� ������ "������� ��-���� ��������?"
				M1_OFF;
				ts_wClose=TIME_NOW;
				//door_B_close_step=10;
				door_B_close_step=14;
				return;
			}
			else		
			/*�������� ������ "�������"?*/
			if(S_CLOSED){
				/*����� ��������*/
				time_opcls=(uint8_t)(time_interval(ts_time_close)/100);
				/*�������� ������ "�������"*/
				bug_reg|=BIT(BUG_CLOSE);
//				CLR_STATE_B_REMOVABLE_ERR;
				/*�������� t��*/
				ts_tzz=timer0_getCounter();
				/*�������� ������ "���������"?*/
				if(count_signal_position==0){
					/*���������  ����������*/
					bug_reg&=~BIT(BUG_POS);
					/*�� �� ������ ���������� ������*/
					//SET_STATE_B_REMOVABLE_ERR;
					RemError(ON,REM_BUG_B1);
				}
				else{
					/*���������  ��������*/
					bug_reg|=BIT(BUG_POS);
					RemError(OFF,REM_BUG_B1);
				}
				/*��������� ���*/
				door_B_close_step++;
			}
			else{
				/*������ ����� t�� ?*/
				if(time_interval(ts_tpd)<Door.sts_tpd) return;
				else{
					/*�������� ������ ��������� ?*/
					if(count_signal_position==0){
						/*���������  �� ��������*/
						bug_reg&=~BIT(BUG_POS);
						/*�� �������� ������ "�������"*/
						bug_reg&=~BIT(BUG_CLOSE);
						/*��������� ���������*/
						Door_AlarmClose();
						/*����������� ������*/
						Set_Buzzer(0,UNLIMITED);
						/*������ ����� "���������"*/
						Door.state=MODE_BREAKAGE;
						return;
					}
					else{
						/*TODO ���� �� �������, ��� "������� �� ��������"*/
						/*�� �������� ������ "�������"*/
						bug_reg&=~BIT(BUG_CLOSE);
						/*���������� ������*/
//						SET_STATE_B_REMOVABLE_ERR;
						/*������ 1..2 ���*/
						Set_Buzzer(1500,CONTINUOUS);
						/*������*/
						Door_B_Revers();
					}
						
				}
			}
		}break;
		case 5:{
			/*������ ����� t�� ?*/
			if(time_interval(ts_tzz)<Door.sts_tzz) return;
			else{
				/*����� ������� � �1 � MF1*/
				//CM_OFF;
				M1_OFF;
				/*�������� M2 � M3(�������� �����)*/
				CM2_ON;
				CM3_ON;
				/*�������� t�*/
				ts_tf=timer0_getCounter();
				/*��������� ���*/
				door_B_close_step++;
			}
		}break;
		case 6:{
			/*��������� ������� "������� 1" � "������� 2"?*/
			if((S_COMPRESSED_2)&&(S_COMPRESSED_1)) {
				/*������� ���������� ������*/
				RemError(OFF,REM_BUG_B3);
				ts_tzp=TIME_NOW;
				door_B_close_step++;
			}
			/*�������� ��������� �������� t� ?*/
			if(time_interval(ts_tf)<Door.sts_tf) return;
			else {
				/*�� �������� ������� 1 � ������� 2*/
				if((S_COMPRESSED_1)==0){
					bug_reg&=~BIT(BUG_M2);
					
					/*���������� ������*/
					//SET_STATE_B_REMOVABLE_ERR;
				}
				if((S_COMPRESSED_2)==0){
					bug_reg&=~BIT(BUG_M3);
					/*���������� ������*/
					//SET_STATE_B_REMOVABLE_ERR;
				}

				/*���������� ������*/
				if (((S_COMPRESSED_2)==0)||((S_COMPRESSED_1)==0))	RemError(ON,REM_BUG_B3);
				else 	RemError(OFF,REM_BUG_B3);
				/*����� ���� �������� ��1 � ��2*/
				//....
				//���������� ������� �������
				//ts_tzp=TIME_NOW-Door.sts_tzp-1;
				/*�������� ���*/
				door_B_close_step+=2;
			}
		}break;
		case 7:{
			if(time_interval(ts_tzp)<Door.sts_tzp) {
				//ts_tf=TIME_NOW;
				return;
			}
			else{
				door_B_close_step++;
				return;
			}
		}break;
		case 8:{
			    /*������������ ������ "̸����� ����� 1"?*/
				if(S_LOGJAM_1) {
					CM2_ON;
					//ts_tf=TIME_NOW;
				}
				else CM2_OFF;
				/*������������ ������ "̸����� ����� 1"?*/
				if(S_LOGJAM_2){
					CM3_ON;
					//ts_tf=TIME_NOW;
				}
				else CM3_OFF;
				/*��������� ��� ��������� � ����������� ������� ��1 � ��2 ?*/
				if ((((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0))
						&&(((S_LOGJAM_1)==0)&&((S_LOGJAM_2)==0))){
					/*��������� ���*/
					ts_wait_logjams=TIME_NOW;
					door_B_close_step++;
					return;
				}
				if(time_interval(ts_tf)>Door.sts_tf){
					door_B_close_step+=2;
				}
		}break;
		case 9:{
			if (time_interval(ts_wait_logjams)<=200){
				if((S_LOGJAM_2)||(S_LOGJAM_1)){
					ts_tf=TIME_NOW;
					door_B_close_step--;
					return;
				}
				else return;
			}
			else{
				door_B_close_step++;
				return;
			}
		}break;
		case 10:{
			/*����. �2 � �3*/
			CM2_OFF;
			CM3_OFF;
			/*������������ ������ "̸����� ����� 1" ��� "̸����� ����� 2"?*/
			if(S_LOGJAM_2){
						    bug_reg&=~BIT(BUG_LOGJAM2);
							/*���������� ������*/
							//SET_STATE_B_REMOVABLE_ERR;
							RemError(ON,REM_BUG_B4);
			}
			else{
				 bug_reg|=BIT(BUG_LOGJAM2);
			}
			if(S_LOGJAM_1){ 
				/*���� �� �������� ������� ̸����� ����� 1 � ̸����� ����� 2*/
				bug_reg&=~BIT(BUG_LOGJAM1);
				/*���������� ������*/
				//SET_STATE_B_REMOVABLE_ERR;
				RemError(ON,REM_BUG_B4);
			}
			else{
				/* ������� ̸����� ����� 1 � ̸����� ����� 2 ��������*/
				bug_reg|=BIT(BUG_LOGJAM1);
			}
			/*���� ���� �� ������ � ���������, �� ������� ���������� ������*/
			if ((S_LOGJAM_1==0)&&(S_LOGJAM_1==0)) RemError(OFF,REM_BUG_B4);
			/*��������� ���*/
			door_B_close_step++;
		}break;
		case 11:{
#warning ������ ��������!!!			/*���������� ������*/
			Set_PWM(OPEN,10);
			ts_nr=TIME_NOW;
			/*��������� ���*/
			door_B_close_step++;
		}break;
		case 12:{
			if (time_interval(ts_nr)<250) return;
			else {
				/*��������� 200..2500 �� � �������� ��������� � �����*/
				M1_OFF;
				CM_OFF;
				/*��������� ���*/
				door_B_close_step++;
			}
		}break;
		case 13:{
			/*��� ���� ����� ������ */
			Door.state=MODE_CLOSE;
			door_B_close_step=0;
		}break;
		case 14:{
			if (S_CLOSED==0){
				if((time_interval(ts_wClose)<500)) return;
				/*���������� ������*/
				RemError(ON,REM_BUG_B4);
				Door_B_Revers();
			}
			else{
				//���������� �������
				//ts_tzz=TIE_NOW-Door.sts_tzz-1;
				
				overcurrent=0;
				/*������� ���������� ������*/
				RemError(OFF,REM_BUG_B5);
				door_B_close_step=4;
			}
		}break;
	};
}

/********************************************
	������� ���������� ������� �����
*******************************************/
void
Door_B_Bloking(void)
{
	switch(door_B_blok_step){
		case 0:{
			/*������ 15 ������ � ��������� ����������*/		
			if(time_interval(ts_15_0)<15000){ 
				Door.state=MODE_CLOSE;
				return;
			}
			else{
				/*�������� 15 ������*/
				ts_15_0=TIME_NOW;
				/*������ ������� �� "������� � ������������ ������� ��������� Y2"*/
				CY2U_ON;
				//CY2P_ON;
				/*�������� 1 ���*/
				ts_1_0=TIME_NOW;
				/*��������� ���*/
				door_B_blok_step++;
			}
		}break;
		case 1:{
			/*������ 1 ���?*/
			//if(time_interval(ts_1_0)<1000) return;
			//else{
				/*��������� ������� � �������� ������� Y2*/
			//	CY2P_OFF;
				/*��������� ������ V>5*/
				//if(S_V5){
					/*�������� ������ "������ Y2"*/
					if(S_Y2){
						/*����� ���*/
						door_B_blok_step=0x02;
					}
					else{
						/*������ ��� 1 ���?*/
						if(time_interval(ts_1_0)<2000) return;
						else {
							/*������ ����� "���������"*/
							Door.state=MODE_BREAKAGE;
							/*������*/
							FatalError(ON,FAT_BUG_B4);
							Door_AlarmClose();
							/*����������� ������*/
							Set_Buzzer(0,UNLIMITED);
							return;
						}
					}
				//}
				//else{
					/*��������� ������� � ������������ ������� Y2 ?*/
				//	CY2U_OFF;
					/*����� ���*/
				//	door_B_blok_step=0x02;
				//}	
			//}
		}break;
		case 2:{
			/*�������� 15 ������*/
			ts_15_0=TIME_NOW;
			Door.state=MODE_BLOKED;
			door_B_blok_step=0;
		}break;
	};
}
//}

/********************************************************************
	�������� ��������������
	
	����� ����������� �� �������������, ������� 
	������������� ��������� �� ����� ���������� ���������
**********************************************************************/
void
Chech_Bugs(void)
{
	/*��������� ������ � ������(�������� ������)*/
	if (Door.type==DOOR_TYPE_SIDE)
		if((S_KRERR1)||(S_KRERR1)){
			bug_reg&=~BIT(BUG_PROTECTIVE_EDGE);
			FatalError(ON,FAT_BUG_B8);
			/*��������� ���������*/
			Door_AlarmClose();
			/*����������� ������*/
			Set_Buzzer(0,UNLIMITED);
			/*������ ����� "���������"*/
			Door.state=MODE_BREAKAGE;
		}
		else{
			bug_reg|=BIT(BUG_PROTECTIVE_EDGE);
			FatalError(OFF,FAT_BUG_B8);
		}
	else {
		/* ��� �������� ���� �� ������� ��� ������*/
		if(S_KRERR1){
			bug_reg&=~BIT(BUG_PROTECTIVE_EDGE);
			
		}
		else{
			bug_reg|=BIT(BUG_PROTECTIVE_EDGE);
			
		}
	}
	
	
}


/*�������� ������� �������� � ����� �� ����� ��������*/
uint8_t 
CheckLock(void)
{
	// ����� ����� � �������� � ���������� ���������
	if	((S_LOCK)||(S_SEKRETKA)){
		/*���������� ������*/
		if(Door.type==DOOR_TYPE_SIDE){
			RemError(ON,REM_BUG_B2);
		}
		else {
			RemError(ON,REM_BUG_T2);
		}
		// ����� ��������
		LED_PWR_OFF;
		lamps_reg&=~BIT(LED_O_GREEN);
		lamps_reg&=~BIT(LED_Z_GREEN);
		return 0;
	}else{
		/*������� ������ "�����"*/
		if(Door.type==DOOR_TYPE_SIDE){
			RemError(OFF,REM_BUG_B2);
		}
		else{
			RemError(OFF,REM_BUG_T2);
		}
		return 1;
	}
}


/****************************************************************
	������� ������ ���� ������ � ������
****************************************************************/
void
Reset_errors(void){
	Reset_Position();
	revers_count=0x00;
	overcurrent=0x00;
	bug_reg=0xFFFF;
	fatevent_buf=0x00;
	remevent_buf = 0x00;
	CLR_STATE_B_REMOVABLE_ERR;
	CLR_STATE_B_REMOVABLE_ERR;
	CLR_STATE_T_REMOVABLE_ERR;
	CLR_STATE_T_FATAL_ERR;
	close_out_count=0x00;
	ts_15_0=TIME_NOW+15000;
	adc_buf=0;
}
/****************************************************************
	***	���� ������� �������, ����������� ������� 	***
*****************************************************************/
void 
Door_Manager(void)
{

	/*� ����������� �� ���� ����� ...*/
	if(Door.type==DOOR_TYPE_SIDE){
	//	if((S_MAN_AUTO)||(rez[0]<Door.sts_Tr1)||(Tnar_EPVS<Door.sts_Tr2)||(rez[0]>=OVERHEATING_T)||(events_flags&BIT(MAN_MODE))){
	//	if((S_MAN_AUTO)){//		/*������� � ������ �����*/					
	if((S_MAN_AUTO)||((rez[0]<Door.sts_Tr1)&&(err_flag==0))||((rez[0]>=OVERHEATING_T)&&(err_flag==0))||(events_flags&BIT(MAN_MODE))){
		if ((Door.state!=MODE_OPENING)&&(Door.state!=MODE_CLOSING)){
				Door.state=MODE_MANUAL;
			}
	}
		/*������� �����*/
		/*� ����������� �� �������� ������ �������� ��������...*/
		switch (Door.state){
			case MODE_FIRST_BLOOD:{
				LED_PWR_ON;
				/*�������� ������ ������*/
				Set_Buzzer(0,CONTINUOUS);
				/*������������� ����������*/
				door_B_close_step=0;
				door_B_open_step=0;
				door_B_blok_step=0;
				ts_15_0=TIME_NOW+15000;
				/*���� �� �������� ���������� ��� �� ...*/
				if ((S_CLOSED)&&(S_COMPRESSED_2)&&(S_COMPRESSED_1)){
					/*���� ������������ ������ "�������"*/
					Door.state=MODE_CLOSE;
				}
				else if ((S_OPEN)&&(S_V45==0)&&(S_V5==0)){
					/*���� ������������ ������ "�������" � ��� �������� V5 ��� V45*/
					Door.state=MODE_OPEN;
				}
				else if((S_OPEN)&&((S_V45)||(S_V5))){
					/*���� ������������ ������ "�������" � ������� ��� V>5 ���(�) V>45 */
					Door.state=MODE_CLOSING;
				}
				else if( (S_OPEN==0)&&((S_CLOSED==0)||(S_COMPRESSED_2==0)||(S_COMPRESSED_1==0)) ){
					/*���� ��������� � ������������� ���������*/
					Door.state=MODE_OPENING;
				}
			}break;
			case MODE_MANUAL:{
				// ���������� ������
				Reset_errors();
				LED_PWR_OFF;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				/*�������� ����������� ����������*/
				M1_OFF;
				CM_OFF;
				CY1U_OFF;
				CY1P_OFF;
				CY2U_OFF;
				/*�������� ������ ������*/
				Set_Buzzer(0,CONTINUOUS);
				/*���������� ����� ������� ������*/
				if ((flag_start_mig==0)&&((S_SEKRETKA==0)&&(S_LOCK==0)/*&&(S_CLOSED)*/)){
					flag_start_mig=0x01;
					ts_start_mig=PERIOD_ZMIG;
				}
				/*������� � �������������� �����*/
				if((S_MAN_AUTO==0)&&(rez[0]>=Door.sts_Tr1)&&((rez[0]<OVERHEATING_T))) {
					/*���������� ����� ������� ������*/
					flag_start_mig=0x00;
					ts_start_mig=PERIOD_ZMIG;
					Door.state=MODE_FIRST_BLOOD;
				}
			}break;
			case MODE_OPENING:{
				// ����� ����� � �������� � ���������� ���������
				if	(CheckLock()){
					LED_PWR_ON;
					ts_start_mig=0;
					lamps_reg&=~BIT(LED_O_GREEN);
					lamps_reg&=~BIT(LED_Z_GREEN);
				}
				Door_B_Open();
			}break;
			case MODE_CLOSING:{
				// ����� ����� � �������� � ���������� ���������
				if	(CheckLock()){
					LED_PWR_ON;
					ts_start_mig=0;
					lamps_reg&=~BIT(LED_O_GREEN);
					lamps_reg&=~BIT(LED_Z_GREEN);
					Door_B_Close();
				}
			}break;			
			case MODE_OPEN:{
				// ����� ����� � �������� � ���������� ���������
				if	(CheckLock()){
					LED_PWR_ON;
					/*������� ������� � ������������� ���������. ���� ���� ������ ������ "�������", 
					������ ����� � ������������� ���������*/
					if(S_OPEN==0){
						Door.state=MODE_INTERMEDIATELY;
						return;
					}
					lamps_reg&=~BIT(LED_O_GREEN);
					if((S_FOOTBOARD))
						lamps_reg|=BIT(LED_Z_GREEN);
					else
						lamps_reg&=~BIT(LED_Z_GREEN);
					/*����� ��������-��������*/
					TIME_TO_OPEN_CLOSE=time_opcls;
					if(comand_reg&BIT(C_OPEN)){
						/*���� ���� ������ ������� �� ��������, �� ������� � ... */
						comand_reg&=~BIT(C_OPEN);
						/*��������� ������� ��������� ����� ��������*/
						//SET_STATE_B_ACCEPTED;
					}
					/*������� �� �� ��� ������� �� ������... ��� ������� ���� "V>5"*/
					if ((((comand_reg&BIT(C_LOCK))||(comand_reg&BIT(C_CLOSE))
						||(S_KZ)||((S_V45)||(S_V5))))/*&&(S_FOOTBOARD)*/) Door.state=MODE_CLOSING;
				}
			}break;			
			case MODE_CLOSE:{
					LED_PWR_ON;
					lamps_reg&=~BIT(LED_Z_GREEN);
				/*������� ������� � ������������� ���������. ���� ���� ������ ������ "�������", 
				������ ����� � ������������� ���������*/
				if((S_CLOSED==0)||(S_COMPRESSED_2==0)||(S_COMPRESSED_1==0)){
					Door.state=MODE_INTERMEDIATELY;
					return;
				}
				TIME_TO_OPEN_CLOSE=time_opcls;
				if(comand_reg&BIT(C_CLOSE)){
					/*���� ���� ������ ������� �� ��������, �� ������� � ... */
					comand_reg&=~BIT(C_CLOSE);
					/*��������� ������� ��������� ����� ��������*/
					//SET_STATE_B_ACCEPTED;
				}
				
				/*��������� ������ �� ���������� ��� ������������ ������� "V>5 "���" V>45"?*/
				if(((S_V45)||(S_V5)||((comand_reg&BIT(C_LOCK))))
					&&(time_interval(ts_15_0)>=15000)){
					Door.state=MODE_BLOKING;
				}
				else {
					lamps_reg|=BIT(LED_O_GREEN);
				}
				
				// ����� ����� � ��������
				if((S_LOCK)||(S_SEKRETKA)) Door.state=MODE_KEYLOCK;
				/*�� ���� �����������*/
				if (((comand_reg&BIT(C_OPEN))||(S_KO))
				&& (S_V45==0)&&(S_V5==0)&&(S_LOCK==0)&&(S_SEKRETKA==0))
					Door.state=MODE_OPENING;
			}break;
			case MODE_BLOKED:{
				/*������� ������ "�����"*/
				RemError(OFF,REM_BUG_B2);
				//	bug_reg|=BIT(BUG_LOCK);
				LED_PWR_OFF;
				door_B_blok_step=0x00;
				if((S_V45==0)&&(S_V5==0)&&((comand_reg&BIT(C_LOCK))==0)){
					/*�������  ������� � ������������ ������� ��������� Y2*/
					CY2U_OFF;
					/*��������� � ����� "�������"*/
					Door.state=MODE_CLOSE;
				}
				
				if (comand_reg&BIT(C_UNLOCK)) {
					if(comand_reg&BIT(C_LOCK)){
						/*���� ���� ������ ������� �� ��������, �� ������� � ... */
						comand_reg&=~BIT(C_LOCK);				
					}
					/*�������  ������� � ������������ ������� ��������� Y2*/
					CY2U_OFF;
					/*��������� � ����� "�������"*/
					Door.state=MODE_CLOSE;
					/*��������� ������� ��������� ����� ��������*/
					comand_reg&=~BIT(C_UNLOCK);
				}
			}break;		
			case MODE_BLOKING:{
				LED_PWR_ON;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				if (Door.sts_sb){	
					/*���� ���� � ���������� "���������� ����������", �� ������ ���������*/
					Door_B_Bloking();
				}
				else {
					/*� ��� ���� ��� � ���������� "���������� ����������", 
					�� ������� "������������ ������" �� ��� ���, 
					���� ����� �� ����� ������� �� ����� �/��� ��������*/
					FatalError(ON,FAT_BUG_B5);
					/*����������� ������*/
					Set_Buzzer(0,UNLIMITED);
					if ((S_LOCK)||(S_SEKRETKA)){
						FatalError(OFF,FAT_BUG_B5);
						/*�������� ������ ������*/
						Set_Buzzer(0,CONTINUOUS);
						Door.state = MODE_KEYLOCK;
					}
				}
				
			}break;
			case MODE_INTERMEDIATELY:{
				if (CheckLock()){
					LED_PWR_ON;
					/*� ������������� ��������� �������� ��� ���������*/
					lamps_reg|=BIT(LED_O_GREEN);
					lamps_reg|=BIT(LED_Z_GREEN);
				}
				/*������� �� �� ��� ������� �� ������... ��� ������� ���� "V>5"*/
				if (((comand_reg&BIT(C_LOCK))||(comand_reg&BIT(C_CLOSE))
					||(S_KZ)||((S_V45)||(S_V5)))) Door.state=MODE_CLOSING;
				/*� �� ���� �����������*/
				if ((comand_reg&BIT(C_OPEN))||(S_KO)) Door.state=MODE_OPENING;
				/*���� ��������� ������� � ������� "�������" ��� "�������", �� ������ ����� ������� ��� ������� �������*/
				if((S_CLOSED)&&(S_COMPRESSED_2)&&(S_COMPRESSED_1)) 	Door.state=MODE_CLOSE;
				if(S_OPEN) 		Door.state=MODE_OPEN;
				
			}break;
			case MODE_BREAKAGE:{
				LED_PWR_ON;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				if(S_MAN_AUTO){
					Door.state=MODE_MANUAL;
				}
			}break;
			case MODE_KEYLOCK:{
				/*������� ������ "�����"*/
			//	bug_reg|=BIT(BUG_LOCK);
				RemError(OFF,REM_BUG_B2);
				// ����� ��������
				LED_PWR_OFF;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				if((S_LOCK==0)&&(S_SEKRETKA==0))
					Door.state=MODE_CLOSE;
			}break;
			case MODE_SMOOTHBRAKING:{
				CheckLock();
				if (SmoothBraking(500,0)>0) return;
				else Door_B_Revers();
				
			}break;
			default: {
				return;
			}
		}
	}
	else if(Door.type==DOOR_TYPE_FRONT){
		/*�������� �����*/
		if((S_MAN_AUTO)||((rez[0]<Door.sts_Tr1)&&(err_flag==0))||((rez[0]>=OVERHEATING_T)&&(err_flag==0))||(events_flags&BIT(MAN_MODE))){
			/*������� � ������ �����*/					
			
			//if ((Door.state!=MODE_OPENING)&&(Door.state!=MODE_CLOSING)){
				if(Door.state!=MODE_MANUAL){
					Set_Buzzer(10000,CONTINUOUS);
					Door.state=MODE_MANUAL;
				}
			//}
		}
		/*�������� �����/��������*/
		if ((S_CLOSED==0) &&(CheckLock()==0)){
				if (buz_on==0){
					Set_Buzzer(10000,INTERRUPTED);
					ts_buz = TIME_NOW;
					buz_on = 0x01;
				}
				/*������������� �����*/
				M1_OFF;
				return;
			}
			else {
				buz_on = 0;
				/*�������� ������*/
	//			Set_Buzzer(0,CONTINUOUS);
			}
		//}
		/*� ����������� �� �������� ������ �������� ��������...*/
		switch (Door.state){
			case MODE_FIRST_BLOOD:{
				/*�������� ������ ������*/
				//Set_Buzzer(1,CONTINUOUS);
				Buzzer.time=0;
				/*��� 2 ������� ��� �������� �� �������*/
				if(time_interval(ts_sec2)<2000) return;
				/*������������� ����������*/
				/*���� ��� ������� �������, �� �����������*/
				if (S_CLOSED==0)
					Door.state=MODE_OPENING;
				else
					Door.state=MODE_CLOSE;
			}break;
			case MODE_OPENING:{
				/*���������� � ��������*/
				ts_start_mig=0;
				lamps_reg&=~BIT(LED_O_GREEN);
				
				LED_PWR_OFF;
				Door_T_Open();
			}break;
			case MODE_CLOSING:{
				/*���������� � ��������*/
				ts_start_mig=0;
				if((S_OPEN)==0){
					lamps_reg|=BIT(LED_O_GREEN);
					LED_PWR_ON;
				}
				else {
					lamps_reg&=~BIT(LED_O_GREEN);
					LED_PWR_OFF;
				}
				//���� �� ����� �������� ������ ������ "�������", �� ������ ������
				if(S_KO){
					Door_T_Revers();
					Door.state=MODE_OPENING;
					return;
				}
				Door_T_Close();
			}break;
			case MODE_CLOSE:{
				/*���������*/
				if((S_SEKRETKA)||(S_LOCK)){
					/*���� �������, �� ����. ���������*/
					lamps_reg&=~BIT(LED_O_GREEN);
					LED_PWR_OFF;
				}
				else{
					/*����� ����� ����� � ���� ������, ������� ���� �������� ������� ����������
					� �� ������ ����������, ������� ������� ����-�� �������, � ��� �� �������� 
					�� ������-���������, ��������� ���������  � ������-�� ����� �������� ����...*/
					lamps_reg|=BIT(LED_O_GREEN);
					LED_PWR_ON;
				}
				if(comand_reg&BIT(C_CLOSE)){
					/*���� ���� ������ ������� �� ��������, �� ������� � ... */
					comand_reg&=~BIT(C_CLOSE);
					/*��������� ������� ��������� ����� ��������*/
					//SET_STATE_B_ACCEPTED;
				}
				if((comand_reg&BIT(C_OPEN))||(S_KO)){
					
					door_T_open_step=0;
					Door.state=MODE_OPENING;
					
				}
			}break;
			case MODE_MANUAL:{
				// ���������� ������
				Reset_errors();
				/*�����  ���������*/
				lamps_reg&=~BIT(LED_O_GREEN);
				LED_PWR_OFF;
				/*������� � M1 � MF1 ��������� */
				M1_OFF;
				CM_OFF;
				/*��� ������� �� ������ ������ ������������� ������ ������� ������������� 10 ���*/
				if((S_KO)||(S_KZ)) Set_Buzzer(10000,CONTINUOUS);
				/*������� � �������������� �����*/
				/*���������� ����� ������� ������*/
				if ((flag_start_mig==0)&&((S_SEKRETKA==0)&&(S_LOCK==0)&&(S_CLOSED))){
					flag_start_mig=0x01;
					ts_start_mig=PERIOD_ZMIG;
				}
				/*������� � �������������� �����*/
				if((S_MAN_AUTO==0)&&(rez[0]>=Door.sts_Tr1)) {
					/*���������� ����� ������� ������*/
					if ((S_SEKRETKA==0)&&(S_LOCK==0)&&(S_CLOSED)){
						flag_start_mig=0x00;
						ts_start_mig=PERIOD_ZMIG;
					}
					ts_sec2 = TIME_NOW;
					Door.state=MODE_FIRST_BLOOD;
				}
			}break;
			case MODE_OPEN:{
				/*���������� � ��������*/
				ts_start_mig=0;
				if(comand_reg&BIT(C_OPEN)){
					/*���� ���� ������ ������� �� ��������, �� ������� � ... */
					comand_reg&=~BIT(C_OPEN);
					/*��������� ������� ��������� ����� ��������*/
					//SET_STATE_B_ACCEPTED;
				}
				lamps_reg&=~BIT(LED_O_GREEN);
				LED_PWR_OFF;
				if (time_interval(ts_top)<Door.sts_top) return;
				else {
					door_T_close_step=0;
					Door.state=MODE_CLOSING;
				}
			}break;
			case MODE_BLOKED:{
				if(comand_reg&BIT(C_OPEN)){
					/*���� ���� ������ ������� �� ��������, �� ������� � ... */
					comand_reg&=~BIT(C_OPEN);
					/*��������� ������� ��������� ����� ��������*/
					//SET_STATE_B_ACCEPTED;
				}
				if(S_KO) Set_Buzzer(3000,INTERRUPTED);
			}break;
			case MODE_BREAKAGE:{
				lamps_reg&=~BIT(LED_O_GREEN);
				LED_PWR_OFF;
				if(S_MAN_AUTO){
					if(Door.state!=MODE_MANUAL){
						Set_Buzzer(10000,CONTINUOUS);
						Door.state=MODE_MANUAL;
					}
				}
			}break;
			case MODE_SMOOTHBRAKING:{
				if (SmoothBraking(500,0)>0) return;
				else Door_T_Revers();
				
			}break;
		}
	}
}