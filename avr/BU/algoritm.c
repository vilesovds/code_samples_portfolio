/*****************************************************
	***	 Алгоритм работы БУ (основные функции) 	***
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
/*Для отсчёта времени натягивания ремня*/
uint16_t ts_nr=0;

/*Регистры событий*/
uint16_t fatevent_buf;
uint16_t remevent_buf;

/*для баззера*/
uint16_t ts_buz=0;
uint8_t buz_on=0;

/*Тип дверей: боковая или торцевая*/
extern uint8_t door_type;
/*буффер для отправки состояния*/
extern uint8_t buff_state[MAX_COUNT_OFF_STATE];

extern uint8_t debounce[N_SIGNAL_BUF_COUNT]; // состояние вводов после антидребезговой обработки

/*кол-во импульсов от датчика "Положение"*/
extern uint16_t count_signal_position;

/*Значения температуры*/
extern int16_t rez[2];

/*температура наружного воздуха из ЕПВС*/
extern uint8_t Tnar_EPVS;

/*Регистр событий*/
extern uint8_t events_flags;

/*Регистр событий типа*/
extern uint8_t events_flag1;

/*текущее пройденное расстояние по датчику "Положение", %*/
uint8_t door_currentS=0x00;

/*Шаг алгоритма открытия двери боковой*/
uint8_t door_B_open_step=0x00;
/*Шаг алгоритма закрытия двери боковой*/
uint8_t door_B_close_step=0x00;
/*Шаг алгоритма открытия двери торцевой*/
uint8_t door_T_open_step=0x00;
/*Шаг алгоритма закрытия двери торцевой*/
uint8_t door_T_close_step=0x00;
/*Шаг режима блокировки боковой двери*/
uint8_t door_B_blok_step=0x00;
/*Счётчик реверсов*/
uint8_t revers_count=0x00;
/*Сяётчик отсутствия сигнала "Закрыто"*/
uint8_t close_out_count=0x00;

/*Регистр неисправностей*/
extern uint16_t bug_reg;

/*Регистр команд от БД*/
uint8_t comand_reg=0x00;

// Флаг среднего положения
uint8_t sr_pol=0;

/*****************************************************
	***	Временные счётчики 	***
*****************************************************/
//{
/*для отсчёта врмени tр*/
uint16_t ts_tr=0;
/*для отсчёта врмени tф*/
uint16_t ts_tf=0;
/*для отсчёта tс*/
uint16_t ts_ts=0;
/*Для отсчёта tпд*/
uint16_t ts_tpd=0;

/*tзз*/
uint16_t ts_tzz=0;
/*Для отсчёта tоп*/
uint16_t ts_top=0;
/*Для отсчёта tз*/
uint16_t ts_tz=0;
/*для отсчёта 1.5 сек*/
uint16_t ts_1_5=0;
/*для отсчёта 2 сек*/
uint16_t ts_2_0=0;
/*для отсчёта 5 секунд*/
uint16_t ts_5_0=0;
/*1 сек*/
uint16_t ts_1_0=0;
/*15 сек*/
uint16_t ts_15_0=0;

uint16_t ts_tzp=0;
/*Пауза между муфтой и М1*/
uint16_t ts_MM1=0;
//}
//Ожидание сигнала "Закрыто" при срабатывании перегрузки по току
uint16_t ts_wClose=0;

/*Время закрытия и открытия*/
uint16_t ts_time_open=0;
uint16_t ts_time_close=0;
uint16_t time_opcls=0;
/*Регистр лампочек*/
uint8_t lamps_reg=0x00;
/*  
	0 - выкл
	1 - вкл.
	______________________________
	|кнопка	 |    цвет         |	бит          |
	|_________|_________|__________|
	|		    / красный  - 	0	     |
	|<>(открыть)  -|				     |
	|		    \ зелёный -	1              |
	|_____________________________|
	|		    / красный - 	2             |
	| ><(закрыть) -|				     |
	|		    \  зелёный - 	3	     |
	|_____________________________|
*/

/*Флаг начала мигания кнопками при переходе из ручного в автоматический и из автоматического в ручной*/
uint8_t flag_start_mig=0;

uint16_t ts_delm1=0;
//
uint16_t t_smeny=0;
uint16_t t_smeny_old=0;

uint8_t megaflag=0;
/*Структурка баззера*/
extern struct {
	uint16_t time;
	uint8_t output;
	uint8_t mode;
	uint16_t ts_buz;
	uint8_t level;
}Buzzer;

/*Ток двигателя M1*/
extern uint16_t I_M1;

extern TDoor Door;

/*Регистр неисправностей*/
extern uint16_t bug_reg;

/*Флаг перегрузки по току*/
extern uint8_t overcurrent;

/**/
extern uint8_t err_flag;
extern uint16_t deb_trr;
extern uint16_t deb_td;
extern uint8_t deb_Vd;
extern uint16_t deb_tmp;

/*Временные счётчики для режима мигания подсветкой кнопок*/
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


//Буффер 
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
	Функция пересчёта кол-ва импульсов от датчика положение в расстояние, измеряемое в процентах
************************************************************************************************/
void
Recalc_Position(void)
{	uint32_t temp=(count_signal_position*100)/MAX_IMPULS_COUNT;
	door_currentS=(uint8_t)temp;
	Door.distance=door_currentS;
	
	/*Измеряем период следования импульсов*/
	if(count_signal_position>1)
		T=time_interval(t1);
	t1=TIME_NOW;
	//Смещаем буффер
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
	Функция сброза текущей позиции 	
	Заодно сбрасывает и счётчик импульсов
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
	Функция аварийной остановки двери
*******************************************/
void
Door_AlarmClose(void)
{
	/*Как всегда смотрим тип двери*/
	if (Door.type==DOOR_TYPE_SIDE){
		/*для боковой двери*/
		/*Снять питание с пусковой и удерживающей катушек соленноида Y1*/
		CY1U_OFF;
		CY1P_OFF;
		/*откл. питания М1*/
		M1_OFF;
		/*Откл М2 и М3*/
		CM2_OFF;
		CM3_OFF;
		/*Откл муфты МФ1*/
		CM_OFF;
		/*отключение питания пусковой и удерживающей катушек соленоида Y1*/
		/*Неустранимая ошибка*/
		//SET_STATE_B_FATAL_ERR;
		//SET_STATE_T_REMOVABLE_ERR;
		Set_Buzzer(0,UNLIMITED);
	}
	else if(Door.type==DOOR_TYPE_FRONT){
	/*Для торцевой двери*/
		/*откл. питания М1*/
		M1_OFF;
		/*Откл М2 и М3*/
		CM2_OFF;
		CM3_OFF;
		/*Откл муфты МФ1*/
		CM_OFF;
		/*отключение питания пусковой и удерживающей катушек соленоида Y1*/
		/*Неустранимая ошибка*/
		//SET_STATE_T_FATAL_ERR;
		//SET_STATE_T_REMOVABLE_ERR;
		Set_Buzzer(0,UNLIMITED);
	}
}

/******************************************************************************************
	Функция плавного увеличения скорости от текущей, до нужной...
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
*			Функция расчёта скорости для плавного торможения
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

//{ Торцевая дверь
//Расчёт скорости с учётом плавного торможения
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
				//Начальная инициализация
				//Vn0=PWM.power;
				//Vn1=Vn0;
				Vn1=PWM.power;
				return 0;
			}
			else if(count_signal_position>3){
					//Рассчитываем период импульсов
						Vals2();
						//Рассчитываем ошибку по времени
						errT=Tp-Ti;
						//смотрим ее величину
						if(errT<0) tmp=((~errT)+1);
						else tmp=(uint16_t)errT;
						//типа если маленькая, то приравниваем к 0
						if (tmp<(Ti/10)) errT=0;
						//for DEBUG
						deb_errT=errT;
						
						//Рассчитываем прибавку к скорости
						delV=(int8_t)(errT/10);
						//Ограничение 
						if(delV>0){
							if(delV>maxdelV) delV=maxdelV;
						}
						else{
							tmp=(~delV)+1;
							if(tmp>(maxdelV)) delV=(~maxdelV)+1;
						}
						//for DEBUG
						deb_delV=delV;
						//Простенький рассчёт скорости
						Vn1=PWM.power+delV;
						// Проверка на  "Зашкаливание"
						if(Vn1>100) Vn1=100;
						if(Door.type==DOOR_TYPE_SIDE){
							// Для боковой минимальный ШИМ - 30%
							if(Vn1<30) Vn1=30;
						}
						else {
							// Для торцевой минимальный ШИМ - 10%
							if(Vn1<10) Vn1=10;
						}
						// Не ниже скорости дотягивания
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
	Функция открытия двери боковой
*******************************************/
void
Door_T_Open(void)
{
	switch(door_T_open_step){
		case 0:{
			/*Сбрасываем счётчики "Положение"*/
			Reset_Position();
			/*Сбрасываем флаг перегрузки*/
			overcurrent=0x00;
			//Проверяем среднее положение
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*Присутствуют сигналы "Замок" или "Секретка"?*/
			if((S_LOCK)||(S_SEKRETKA)){
				/*Прерывистый баззер 3 сек*/
				Set_Buzzer(3000,INTERRUPTED);
				/*Конец*/
				/*Открытие двери заблокировано  - смена режима*/
				Door.state=MODE_CLOSE;
				door_T_open_step=0x00;
			}
			else{
				/*Баззер 1..2 сек*/
				Set_Buzzer(1000,CONTINUOUS);
				/*Подать питание на муфту MF1*/
				CM_ON;
				ts_MM1=TIME_NOW;
				door_T_open_step++;
			}
		}break;
		case 1:{
			if(time_interval(ts_MM1)>=PAUSE_MF){	
				/*Подать питание на M1 (в сторону открытия)*/
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
			/*Присутствует сигнал "Закрыто" ?*/
			if(S_CLOSED==0){
				/*Присутствуют сигналы "Замок" или "Секретка"*/
				if((S_SEKRETKA)||(S_LOCK)){
					/*Устранимая ошибка*/
					//SET_STATE_T_REMOVABLE_ERR;
					/*Снимаем питание с M1*/
					M1_OFF;
					door_T_open_step=0x05;
				}
			}
			/* 1)*/
			//Если закрываемся не из среднего положения,то
			if(sr_pol==0){
			/*Поступило 2 сигнала от датчика "Положение"*/
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
			/*Поступил сигнал "Открыто"?*/
			if(S_OPEN){
				/*Исправен датчик "открыто"*/
				bug_reg|=BIT(BUG_OPEN);
				/*Сигнал "Положение" поступал?*/
				if(count_signal_position){
					/*положение  исправен*/
					bug_reg|=BIT(BUG_POS);
				}
				else {
					/*положение  неисправен*/
					bug_reg&=~BIT(BUG_POS);
					/*устранимая ошибка*/
					//SET_STATE_T_REMOVABLE_ERR;
				}
				/*Снимается питание с M1*/
				M1_OFF;
				/*переход в конец*/
				door_T_open_step=0x04;
			}
			else{
				/*С момента включения M1 прошло время tпд ?*/
				if(time_interval(ts_tpd)<Door.sts_tpd) return;
				else {
					/*Не исправен датчик "открыто"*/
					bug_reg&=~BIT(BUG_OPEN);
					door_T_open_step=0;
					/*Неустранимая ошибка*/
					FatalError(ON,FAT_BUG_T1);
					Door_AlarmClose();
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
		
		}break;
		case 4:{
			/*Засекаем tоп*/
			ts_top=TIME_NOW;
			/*Время открытия*/
			//time_opcls=(uint8_t)(time_interval(ts_time_open)/1000);
			time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
			/*DEBUG*/
			deb_tmp=time_opcls;
			door_T_open_step=0;
			/*Смена режима*/
			Door.state=MODE_OPEN;
		}break;
		case 5:{
			/*Бесконечный баззер*/
			Buzzer.mode=UNLIMITED;
		}break;
	}
}

/********************************************
	Подпрограмма подсчёта реверсов
*******************************************/
void
Door_T_Revers(void)
{
	M1_OFF;
	revers_count++;
	if(revers_count>10){
		#warning Думаю тут всё-таки аварийное закрытие
		Door_AlarmClose();
		Door.state=MODE_BREAKAGE;
		return;
	}
	else {
		
		if((Door.distance>97)||(count_signal_position==0)){
			/*Снять питание с муфты MF1*/
			CM_OFF;
			/*Ставим шаг открытия ... в соотв п. 1.4.3.1.3*/
			door_T_open_step=0;
			Reset_Position();
		}
		else{
			/*Баззер 1..2 сек*/
			Set_Buzzer(1000,CONTINUOUS);
			SwapPosition();
			door_T_open_step=1;
			sr_pol=0;
		}
		
		/*перейти к открытию дверей*/
		Door.state=MODE_OPENING;
		return;
	}
}

/*************************************************************
 Подпрограмма открытия/закрытия двери при отсутствии датчика "положение"
 Return: 	0 - ещё не всё
		1- всё,открылась
**************************************************************/
uint8_t 
Door_WorkWithoutPosition(void)
{	uint8_t res=0x00;
	uint8_t tmp=0;
		//Если закрываемся - смотрим на датчик закрыто
	if(PWM.direction==CLOSE){
		/*Может быть уже закрылись?*/
		if(S_CLOSED) {
			return 0x01;
		}
	}
	else{//Иначе  - смотрим на датчик открыто
		/*Может быть уже открылись?*/
		if(S_OPEN) {
			return 0x01;
		}
	}
	//Разгон
	if((time_interval(ts_tr)>=0)&&(time_interval(ts_tr)<=500)){
		tmp=Recalc_V(Door.sts_Vd,Door.sts_Vr,time_interval(ts_tr));
		if(tmp) Set_PWM(PWM.direction,tmp);
	}
	else {
		//Замедление
		t_smeny=Door.sts_tr-500;
		if((time_interval(ts_tr)>=t_smeny)&&(time_interval(ts_tr)<Door.sts_tr)){
			tmp=Recalc_V(Door.sts_Vr,Door.sts_Vd,time_interval(ts_tr)-t_smeny);
			if(tmp) Set_PWM(PWM.direction,tmp);
		}
	}
	/*Прошло время tr?*/
	if(time_interval(ts_tr)<Door.sts_tr) res=0x00;
	else{
		/*Проверяем ток, тем самым определяем работоспособность М1*/
		if (I_M1<MIN_IM1)	bug_reg&=~BIT(BUG_M1);
		else 				bug_reg|=BIT(BUG_M1);
		/*Подать питание на М1 соответствующее скорости Vd, в сторону, обратную открытию*/
		Set_PWM(PWM.direction,Door.sts_Vd);
		/*Результат удачный*/
		res=0x01;
	}
	return res;
}

/********************************************
	Функция закрытия двери торцевой
*******************************************/
void
Door_T_Close(void)
{
	switch (door_T_close_step){
		case 0:{
			/*Сбрасываем счётчики "Положение"*/
			Reset_Position();
			/*Сбрасываем флаг перегрузки*/
			overcurrent=0x00;
			//Проверяем среднее положение
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*Прошло время tоп с момента полного отрытия двери?*/
			if (time_interval(ts_top)<Door.sts_top) return;
			else {
				/*Баззер 1..2 сек*/
				Set_Buzzer(1500,CONTINUOUS);
				/*подать питание на M1 (в сторону обратную открытию)*/
				Set_PWM(CLOSE,Door.sts_Vd);
				
				ts_delm1=I_DEL;
				
				/*Засекаем tтр*/
				ts_tr=timer0_getCounter();
				/*Засекаем tпд*/
				ts_tpd=timer0_getCounter();
				door_T_close_step++;
				ts_time_close=TIME_NOW;
			}
		}break;
		case 1:{
			/*0) */
			/*Присутствует сигнал "Помеха в створе 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2))	{
				//Door_T_Revers();
				/*Реверс с плавным торможением*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;

				return;
			}
			if ((overcurrent)&&(S_CLOSED==0)){
				/*Засекаем время tз*/
				ts_tz=timer0_getCounter();
				/*Идём на следующий шаг*/
				door_T_close_step=3;
				return;
			}
			/* 1)*/
			//Если закрываемся не из среднего положения,то
			if(sr_pol==0){
				/*Поступило 2 импульса от датчика "Положение ?"*/
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
			/*Присутствует сигнал "Помеха в створе 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2)){
				Door_T_Revers();
				/*перейти к открытию дверей*/
				Door.state=MODE_OPENING;
				return;
			}
			/*1)
			Проверка перегрузкипо току*/
			//if (I_M1>Door.sts_Im){
			//if ((overcurrent)&&(Door.sts_kzz)&&(S_CLOSED==0)){
			if ((overcurrent)&&(S_CLOSED==0)){
				/*Засекаем время tз*/
				ts_tz=timer0_getCounter();
				/*Идём на следующий шаг*/
				door_T_close_step++;
				return;
			}
			
			/*2)
			C момента включения М1 прошло время tпд, а сигнал "Закрыто "отсутствует?*/
			//if((time_interval(ts_tpd)<Door.sts_tpd)&&(S_CLOSED==0)) return;
			if((time_interval(ts_tpd)<12000)&&(S_CLOSED==0)) return;
			else door_T_close_step=0x04;
			
		}break;
		case 3:{
			/*Прошло время tз?*/
			if((time_interval(ts_tz)<Door.sts_tz)&&(S_CLOSED==0)) return;
			else door_T_close_step++;
		}break;
		case 4:{
			/*Присутствует сигнал "Помеха в створе 1"?*/
			if((S_DIST_AL_1)||(S_DIST_AL_2)){
				//Door_T_Revers();
				/*Реверс с плавным торможением*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;
				door_T_close_step=0x00;
				return;
			}
			/*C момента включения М1 прошло время tпд, а сигнал "Закрыто "отсутствует?*/
			//if((time_interval(ts_tpd)<Door.sts_tpd)&&(S_CLOSED==0)) return;
			/*Присутствует сигнал "Закрыто" ?*/
			if(S_CLOSED){
				close_out_count=0x00;
				/*Исправен датчик "Закрыто"*/
				bug_reg|=BIT(BUG_CLOSE);
				/*Снять сигнал "Устрнимая ошибка"*/
				//CLR_STATE_T_REMOVABLE_ERR;
				RemError(OFF,REM_BUG_T3);
				/*Снять питание с М1 и MF1*/
				CM_OFF;
				M1_OFF;
				/*перейти к последнему шагу*/
				door_T_close_step=0x05;
			}
			else {
				/*Неисправен датчик "Закрыто"*/
				bug_reg&=~BIT(BUG_CLOSE);

				close_out_count++;
				if(close_out_count>1){
					/*Такая бадяга уже второй раз=> "Неустранимая ошибка"*/
					/*Снять питание с M1*/
					M1_OFF;
					/*Неустранимая ошибка*/
					FatalError(ON,FAT_BUG_T2);
					Door_AlarmClose();
					Door.state=MODE_BREAKAGE;
					return;
				}
				else{
					/*Устранимая ошибка*/
					//SET_STATE_T_REMOVABLE_ERR;
					RemError(ON,REM_BUG_T3);
					//Door_T_Revers();
					/*Реверс с плавным торможением*/
					ts_smoothbraking = TIME_NOW;
					v_smoothbraking = PWM.power;
					Door.state = MODE_SMOOTHBRAKING;
					door_T_close_step=0x00;
					/*Заводим счётчик tоп*/
					//ts_top=timer0_getCounter();
					/*перейти к открытию дверей*/
					//Door.state=MODE_OPENING;
				}
			}
		}break;
		case 5:{
			/*Время закрытия*/
			//time_opcls=(uint8_t)(time_interval(ts_time_close)/1000);
			time_opcls=(uint8_t)(time_interval(ts_time_close)/100);
			/*DEBUG*/
			deb_tmp=time_opcls;
			door_T_close_step=0;
			/*Закрылись  - смена режима*/
			Door.state=MODE_CLOSE;
		}break;
	}
}



//}
//{Боковая дверь
/********************************************
	Функция открытия двери боковой
*******************************************/
void
Door_B_Open(void)
{
	uint8_t tmp=0;
	switch(door_B_open_step){
		case 0:{
			/*Сбрасываем флаг перегрузки*/
			overcurrent=0x00;
			
			Reset_Position();
			
			//Проверяем среднее положение
			if((S_CLOSED==0)&&(S_OPEN==0))sr_pol=0x01;
			else sr_pol=0;
			/*присутствуют сигналы "Датчик Y2" или V>5 или V>45 или "Замок" или "Секретка"*/
			if((S_Y2)||(S_LOCK)||(S_SEKRETKA)||(S_V45)||(S_V5)){
				return;
			}
			else{
				/*Баззер 1..2 сек*/
				
				Set_Buzzer(1000,CONTINUOUS);
				//Reset_Position();
				/*Следующий шаг*/
				door_B_open_step++;
				ts_time_open=TIME_NOW;
				return;
			}
		}break;
		case 1:{
			/*Присутствуют сигналы "Мёртвая точка 1"и "Мёртвая точка 2"?*/
			if((S_LOGJAM_2)||(S_LOGJAM_1)){
				/*Вкл. М2 или М3 соответственно*/
				if(S_LOGJAM_1){
					CM2_ON;
				}
				if(S_LOGJAM_2){
					CM3_ON;
				}
				/*Засекаем tф*/
				ts_tf=timer0_getCounter();
				/*Следующий шаг*/
				door_B_open_step++;
				return;
			}
			else{
				/*перескакиваем на три "хода"*/
				door_B_open_step+=3;
				return;
			}
		}break;
		case 2:{
			/*Превышен временной интервал tф?*/
			if(time_interval(ts_tf)<Door.sts_tf){
				/*нет - следующий  "ход"*/
				door_B_open_step++;
				return;
			}
			else {
				/*Да -*/
				/*Устранимая ошибка*/
				RemError(ON,REM_BUG_B4);
				/*отключаем М2 и М3*/
				CM2_OFF;
				CM3_OFF;
				/*проскакиваем  "ход"*/
				door_B_open_step+=2;
				return;
			}
		}break;
		case 3:{
			/*Присутствует сигнал "Мёртвая точка 1"*/
			if(S_LOGJAM_1==0){
				/*Откл М2*/
				CM2_OFF;
			}
			/*Присутствует сигнал "Мёртвая точка 2"*/
			if(S_LOGJAM_2==0){
				/*Откл М3*/
				CM3_OFF;
			}
			/*отключены оба двигателя М2 и М3?*/
			tmp=PORTA&0x30;
			if(tmp==0x00){
				/*Следующий шаг*/
				door_B_open_step++;
				/*Убираем устранимую ошибку*/
				RemError(OFF,REM_BUG_B4);
				return;
			}
			else{
				/*На ход назад*/
				door_B_open_step--;
				return;
			}
		}break;
		case 4:{
			overcurrent=0x00;
			/*Подать питание на пусковую и удерживающую катушки соленоида Y1*/
			CY1U_ON;
			CY1P_ON;
			/*Засекаем tс*/
			ts_ts=timer0_getCounter();
			/*Следующий ход*/
			door_B_open_step++;
			return;
		}break;
		case 5:{
			/*Прошло время tс?*/
			if(time_interval(ts_ts)<Door.sts_ts) return;
			else {
				/*Снять питание с пусковой катушки Y1*/
				CY1P_OFF;
				/*Подать питание на муфту MF1 */
				CM_ON;
				ts_MM1=TIME_NOW;
				door_B_open_step++;
			}
		}break;
		case 6:{
				if(time_interval(ts_MM1)<PAUSE_MF) return;
				
				/*Подать питание на марфевый двигатель M1 соответствуещее скорости Vр*/
				//Reset_Position();
				Set_PWM(OPEN,Door.sts_Vd);
				//Заводим таймер измерения тока
				ts_delm1=I_DEL;
				/*Начинаем отсчёт 1.5 сек...*/
				ts_1_5=timer0_getCounter();
				/*Начинаем отсчёт 2 сек...*/
				ts_2_0=timer0_getCounter();
				/*Начинаем отсчёт tпд*/
				ts_tpd=timer0_getCounter();
				/*Следующий шаг*/
				/*Засекаем tр*/
				ts_tr=TIME_NOW;
				door_B_open_step++;
				return;
		}break;
		case 7:{
			/*1)*/
			/*Прекратилось подача сигналов "Поджато 1 и "Пождато 2 "*/
			if((S_COMPRESSED_2)||(S_COMPRESSED_1)){
				/*Прошло 1.5 сек?*/
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
					/*Переход на конечный шаг*/
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			else {
				/*нет сигналов от "Поджато" - убираем устранимую ошибку*/
				RemError(OFF,REM_BUG_B3);
			}
			/*2)*/
			/*Прекратилось поступление сигнала "Закрыто?"*/
			if(S_CLOSED){
				/*Прошло 2 сек?*/
				if(time_interval(ts_1_5)>=1500){
					FatalError(ON,FAT_BUG_B2);
					Door_AlarmClose();
					/*Переход на конечный шаг*/
					//door_B_open_step=0x09;
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
//			else{
//				/*Присутствует сигнал "Датчик Y2" или "Замок" или "Секретка"?*/
//				if((S_Y2)||(S_LOCK)||(S_SEKRETKA)){
//					/*На БД выдаётся сигнал устранимая ошибка*/
//					SET_STATE_B_REMOVABLE_ERR;
//				}
//			}
			/*3)*/
			//Если закрываемся не из среднего положения,то
			if(sr_pol==0){
			/*Поступило 2 сигнала от датчика положение?*/
				if(count_signal_position>2){
					/*Да */
					/*Вызываем подпрограмму открытия двери по сигналам от датчика "Положение"*/
					if(Door_WorkPosition()){
						/*если удачно завершилась - едем дальше*/
						/*Следующий шаг*/
						door_B_open_step++;
						return;
					}
				}
				else{
					/*нет */
					/*Вызываем подпрограмму открытия двери при отсутствии сигнала от датчика "Положение"*/
					if(Door_WorkWithoutPosition()){
						/*если удачно завершилась - едем дальше*/
						/*Следующий шаг*/
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
			/*Прекратилось подача сигналов "Поджато 1 и "Пождато 2 "*/
			if((S_COMPRESSED_2)||(S_COMPRESSED_1)){
				/*Прошло 1.5 сек?*/
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
					/*Переход на конечный шаг*/
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			else {
				/*нет сигналов от "Поджато" - убираем устранимую ошибку*/
				RemError(OFF,REM_BUG_B3);
			}
			/*2)*/
			/*Прекратилось поступление сигнала "Закрыто?"*/
			if(S_CLOSED){
				/*Прошло 2 сек?*/
				if(time_interval(ts_1_5)>=1500){
					/*Неустранимая ошибка*/
					FatalError(ON,FAT_BUG_B2);
					Door_AlarmClose();
					/*Переход на конечный шаг*/
					//door_B_open_step=0x09;
					Door.state=MODE_BREAKAGE;
					return;
				}
			}
			/*Поступил сигнал "Открыто"?*/
			if(S_OPEN){
				/*Да*/
				/*Время открытия*/
				time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
				/*Исправен датчик "открыто"*/
				bug_reg|=BIT(BUG_OPEN);
				/*Снимаем неустранимую ошибку*/
				FatalError(OFF,FAT_BUG_B3);
				/*Сигнал "положение поступал?"*/
				if(count_signal_position){
					/*Да*/
					/*положение  исправен*/
					bug_reg|=BIT(BUG_POS);
					RemError(OFF,REM_BUG_B1);
				}
				else{
					/*Нет */
					/*положение  неисправен*/
					bug_reg&=~BIT(BUG_POS);
					/*На БД подаётся сигнал "Устранимая ошибка"*/
					//SET_STATE_B_REMOVABLE_ERR;
					RemError(ON,REM_BUG_B1);
				}
				/*Снимается питание с М1*/
				M1_OFF;
				/*Снимается питание с соленоида Y1*/
				CY1U_OFF;
				CY1P_OFF;
				/*Засекаем 5 сек*/
				ts_5_0=timer0_getCounter();
				/*Следующий шаг*/
				door_B_open_step++;
				return;
			}
			else{
				/*Нет, не поступал*/
				/* с момента включения М1 прошло время tпд?*/
				if(time_interval(ts_tpd)>=Door.sts_tpd){
					/*Не исправен датчик "открыто"*/
					bug_reg&=~BIT(BUG_OPEN);
					FatalError(ON,FAT_BUG_B3);
					Door_AlarmClose();
					/*сиавим статус - сломалдись*/
					Door.state=MODE_BREAKAGE;
					return;
				}
				return;
			}
		}break;
		case 9:{
			/*Муфта отключится через 5 секунд*/
			ts_5_0=5000;
			door_B_open_step++;
		}break;
		case 10:{
			/*Время открытия*/
			//time_opcls=(uint8_t)(time_interval(ts_time_open)/1000);
			//перенёс выше			time_opcls=(uint8_t)(time_interval(ts_time_open)/100);
			/*Это последний шаг, тут ничегошеньки не делается, кроме смены режима*/
			Door.state=MODE_OPEN;
			door_B_open_step=0;
			
		}break;
	};
}


/***********************************************
	Функция реверса при закрытии двери боковой
************************************************/
void
Door_B_Revers(void)
{
	// Останавливаем двигатель М1
	M1_OFF;
	/*Исправен датчик "Положение"?*/
	if((Door.distance>97)||(count_signal_position==0)/*||((S_OPEN==0)&&(S_CLOSED==0))*/){
		/*Снять питание с муфты MF1*/
		CM_OFF;
		/*Ставим шаг открытия ... в соотв п. 1.4.3.1.3*/
		door_B_open_step=4;
		Reset_Position();
	}
	else {
		SwapPosition();
		door_B_open_step=4;
		sr_pol=0;
	}
	door_B_close_step=0;
	/*Открытие двери*/
	Door.state=MODE_OPENING;
}


/***********************************************************************
	Подпрограмма Открытия/Закрытия  двери по сигналам датчика "Положение"
RETURN:	0 - ещё не всё
		1 - всё ништяк, открылась
*************************************************************************/
uint8_t
Door_WorkPosition(void)
{
 uint8_t tmp=0;
	//Если закрываемся - смотрим на датчик закрыто
	if(PWM.direction==CLOSE){
		/*Может быть уже закрылись? или Прошло время tпд ?*/
		if((S_CLOSED)||(time_interval(ts_tpd)>=Door.sts_tpd)){
			megaflag=0x00;
			//t_smeny=TIME_NOW;
			return 0x01;
		}
	}
	else{//Иначе  - смотрим на датчик открыто
		/*Может быть уже открылись? или Прошло время tпд ?*/
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
			/*Проверяем ток, тем самым определяем работоспособность М1*/
			if (I_M1<MIN_IM1)	bug_reg&=~BIT(BUG_M1);
			else 				bug_reg|=BIT(BUG_M1);
			/*Подать питание на М1 соответствующее скорости Vd, в сторону, обратную открытию*/
			Set_PWM(PWM.direction,Door.sts_Vd);
			megaflag=0x00;
			return 0x01;
		}
		else return 0;
	}
	return 0;
}

/********************************************
	Функция закрытия двери боковой
*******************************************/
void
Door_B_Close(void)
{
	switch (door_B_close_step){
		case 0:{
			/*Сбрасываем флаг перегрузки*/
			overcurrent=0x00;
			//Проверяем среднее положение
			if((S_OPEN==0)&&(S_CLOSED==0))sr_pol=0x01;
			else sr_pol=0;
			/*Присутствует сигнал "Подножка сложена"?*/
			if(S_FOOTBOARD){
				/*Баззер 1..2 сек*/
				Set_Buzzer(1500,CONTINUOUS);
				/*Засекаем время закрытия*/
				ts_time_close=TIME_NOW;
				/*Сигнал с датчика "Закрыто"*/
				if (S_CLOSED){
					/*переходим к поджатию*/
					ts_tzz = TIME_NOW;
					door_B_close_step = 5;
					return;
				}
				//Reset_Position();
				/*Подать питание на пусковую и удерживающую катушки соленоида Y1*/
				CY1U_ON;
				CY1P_ON;
				/*Засекаем tc*/
				ts_ts=timer0_getCounter();
				/*Следующий шаг*/
				door_B_close_step++;
			}else {
				/*Получена команда от БД "Заблокировать" или V>5 или V>45?*/
				if ((comand_reg&BIT(C_LOCK))||(S_V5)||(S_V45)){
					FatalError(ON,FAT_BUG_B6);
					Door_AlarmClose();
					/*Бесконечный баззер*/
					Set_Buzzer(0,UNLIMITED);
					/*Ставим режим "Сломались"*/
					Door.type=MODE_BREAKAGE;
				}
			}
		}break;
		case 1:{

			/*Прошло время tc?*/
			if(time_interval(ts_ts)<Door.sts_ts)return;
			else {
				/*Снять питание с пусковой и удерживающей катушек соленноида Y1*/
				CY1U_OFF;
				CY1P_OFF;
				/*Подать питание на муфту MF1*/
				CM_ON;
				ts_MM1=TIME_NOW;
				door_B_close_step++;
			}
		}
		case 2:{		
				if(time_interval(ts_MM1)<PAUSE_MF) return;
				/*Подать питание на Двигатель M1(в сторону, обратную открытию), соответствующее скорости Vр*/
				Reset_Position();
				Set_PWM(CLOSE,Door.sts_Vd);
				ts_delm1=I_DEL;
				ts_tr=TIME_NOW;
				/*Засекаем tпд*/
				ts_tpd=TIME_NOW;
				
				/*Переход к следующему шагу*/
				door_B_close_step++;
		}break;
		case 3:{
			/*0)*/
			/*Присутствуют сигналы "V>5" или "V>45"*/
			if((S_V45==0)&&(S_V5==0)){
				/*Работают двигатели M2 и M3(происходит поджатие)?*/
				if(((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0)){
					/*Нет*//*Присутствует сигнал "Помеха в створе 1" или "Помеха в створе 2"?*/
					if((S_DIST_AL_1)||(S_DIST_AL_2)){
						/*Баззер 1..2 сек*/
						Set_Buzzer(1500,CONTINUOUS);
						/*Реверс с плавным торможением*/
						ts_smoothbraking = TIME_NOW;
						v_smoothbraking = PWM.power;
						Door.state = MODE_SMOOTHBRAKING;
						/*Реверс*/
						//Door_B_Revers();
						return;
					}
				}
			}
			/*1)*/
			/*Присутствует сигнал "Подножка сложена"?*/
			if(S_FOOTBOARD==0){
				/*нет*/
				FatalError(ON,FAT_BUG_B7);
				/*Аварийная остановка*/
				Door_AlarmClose();
				/*Бесконечный баззер*/
				Set_Buzzer(0,UNLIMITED);
				/*Ставим режим "Сломались"*/
				Door.state=MODE_BREAKAGE;
				return;
			}
			/*2)*/
			/*перегрузка по току*/
			if((overcurrent)&&(S_CLOSED==0)){
				Set_Buzzer(1500,CONTINUOUS);
				/*Реверс с плавным торможением*/
				ts_smoothbraking = TIME_NOW;
				v_smoothbraking = PWM.power;
				Door.state = MODE_SMOOTHBRAKING;
				/**/
				/*устранимуя ошибка*/
				RemError(ON,REM_BUG_B5);
				/*Реверс*/
				//Door_B_Revers();
				return;
			}
			else {
				/*Убираем устранимую ошибку*/
				RemError(OFF,REM_BUG_B5);
			}
			/*3)*/
			//Если закрываемся не из среднего положения
			if(sr_pol==0){
				/*Поступило 2 сигнала от датчика "Полложение"?*/
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
				// из среднего положения едем на этой скорости
				Set_PWM(CLOSE,50);
				door_B_close_step++;
			}
		}break;
		case 4:{
			
			
			/*0)*/
			/*Присутствуют сигналы "V>5" или "V>45"*/
			if((S_V45==0)&&(S_V5==0)){
				/*Работают двигатели M2 и M3(происходит поджатие)?*/
				if(((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0)){
					/*Нет*//*Присутствует сигнал "Помеха в створе 1" или "Помеха в створе 2"?*/
					if((S_DIST_AL_1)||(S_DIST_AL_2)){
						/*Баззер 1..2 сек*/
						Set_Buzzer(1500,CONTINUOUS);
						
						/*Реверс с плавным торможением*/
						ts_smoothbraking = TIME_NOW;
						v_smoothbraking = PWM.power;
						Door.state = MODE_SMOOTHBRAKING;
						
						/*Реверс*/
						//Door_B_Revers();
						return;
					}
				}
			}
        	/*1)*/
			/*Присутствует сигнал "Подножка сложена"?*/
			if(S_FOOTBOARD==0){
				/*нет*/
				FatalError(ON,FAT_BUG_B7);
				/*Аварийная остановка*/
				Door_AlarmClose();
				/*Бесконечный баззер*/
				Set_Buzzer(0,UNLIMITED);
				/*Ставим режим "Сломались"*/
				Door.state=MODE_BREAKAGE;
				return;
			}
			
			if((overcurrent)&&(S_CLOSED==0)){
				/*Реверс*/
				//Door_B_Revers();
				//Прежде чем делать реверс, подождём и посмотрим, мож сигнал "Закрыто всё-таки поступил?"
				M1_OFF;
				ts_wClose=TIME_NOW;
				//door_B_close_step=10;
				door_B_close_step=14;
				return;
			}
			else		
			/*Поступил сигнал "Закрыто"?*/
			if(S_CLOSED){
				/*Время закрытия*/
				time_opcls=(uint8_t)(time_interval(ts_time_close)/100);
				/*Исправен датчик "Закрыто"*/
				bug_reg|=BIT(BUG_CLOSE);
//				CLR_STATE_B_REMOVABLE_ERR;
				/*Засекаем tзз*/
				ts_tzz=timer0_getCounter();
				/*Исправен датчик "положение"?*/
				if(count_signal_position==0){
					/*положение  Неисправен*/
					bug_reg&=~BIT(BUG_POS);
					/*На БД сигнал устранимая ошибка*/
					//SET_STATE_B_REMOVABLE_ERR;
					RemError(ON,REM_BUG_B1);
				}
				else{
					/*положение  исправен*/
					bug_reg|=BIT(BUG_POS);
					RemError(OFF,REM_BUG_B1);
				}
				/*Следующий шаг*/
				door_B_close_step++;
			}
			else{
				/*Прошло время tпд ?*/
				if(time_interval(ts_tpd)<Door.sts_tpd) return;
				else{
					/*Исправен датчик положение ?*/
					if(count_signal_position==0){
						/*положение  Не исправен*/
						bug_reg&=~BIT(BUG_POS);
						/*Не исправен датчик "Закрыто"*/
						bug_reg&=~BIT(BUG_CLOSE);
						/*Аварийная остановка*/
						Door_AlarmClose();
						/*Бесконечный баззер*/
						Set_Buzzer(0,UNLIMITED);
						/*Ставим режим "Сломались"*/
						Door.state=MODE_BREAKAGE;
						return;
					}
					else{
						/*TODO надо бы сказать, что "Закрыто не исправен"*/
						/*Не исправен датчик "Закрыто"*/
						bug_reg&=~BIT(BUG_CLOSE);
						/*Устранимая ошибка*/
//						SET_STATE_B_REMOVABLE_ERR;
						/*Баззер 1..2 сек*/
						Set_Buzzer(1500,CONTINUOUS);
						/*Реверс*/
						Door_B_Revers();
					}
						
				}
			}
		}break;
		case 5:{
			/*Прошло время tзз ?*/
			if(time_interval(ts_tzz)<Door.sts_tzz) return;
			else{
				/*Снять питание с М1 и MF1*/
				//CM_OFF;
				M1_OFF;
				/*Включить M2 и M3(поджатие двери)*/
				CM2_ON;
				CM3_ON;
				/*засекаем tф*/
				ts_tf=timer0_getCounter();
				/*Следующий шаг*/
				door_B_close_step++;
			}
		}break;
		case 6:{
			/*Поступили сигналы "Поджато 1" и "Поджато 2"?*/
			if((S_COMPRESSED_2)&&(S_COMPRESSED_1)) {
				/*Снимаем устранимую ошибку*/
				RemError(OFF,REM_BUG_B3);
				ts_tzp=TIME_NOW;
				door_B_close_step++;
			}
			/*Превышен временной интервал tф ?*/
			if(time_interval(ts_tf)<Door.sts_tf) return;
			else {
				/*Не исправны поджато 1 и поджато 2*/
				if((S_COMPRESSED_1)==0){
					bug_reg&=~BIT(BUG_M2);
					
					/*Устранимая ошибка*/
					//SET_STATE_B_REMOVABLE_ERR;
				}
				if((S_COMPRESSED_2)==0){
					bug_reg&=~BIT(BUG_M3);
					/*Устранимая ошибка*/
					//SET_STATE_B_REMOVABLE_ERR;
				}

				/*Устранимая ошибка*/
				if (((S_COMPRESSED_2)==0)||((S_COMPRESSED_1)==0))	RemError(ON,REM_BUG_B3);
				else 	RemError(OFF,REM_BUG_B3);
				/*Сдесь мона запалить МТ1 и МТ2*/
				//....
				//Обманываем счётчик времени
				//ts_tzp=TIME_NOW-Door.sts_tzp-1;
				/*Перескок шаг*/
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
			    /*Присутствует сигнал "Мёртвая точка 1"?*/
				if(S_LOGJAM_1) {
					CM2_ON;
					//ts_tf=TIME_NOW;
				}
				else CM2_OFF;
				/*Присутствует сигнал "Мёртвая точка 1"?*/
				if(S_LOGJAM_2){
					CM3_ON;
					//ts_tf=TIME_NOW;
				}
				else CM3_OFF;
				/*Отключены оба двигателя и отсутствуют сигналы МТ1 и МТ2 ?*/
				if ((((PORTA&BIT(5))==0)&&((PORTA&BIT(4))==0))
						&&(((S_LOGJAM_1)==0)&&((S_LOGJAM_2)==0))){
					/*Следующий шаг*/
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
			/*Откл. М2 и М3*/
			CM2_OFF;
			CM3_OFF;
			/*Присутствует сигнал "Мёртвая точка 1" или "Мёртвая точка 2"?*/
			if(S_LOGJAM_2){
						    bug_reg&=~BIT(BUG_LOGJAM2);
							/*Устранимая ошибка*/
							//SET_STATE_B_REMOVABLE_ERR;
							RemError(ON,REM_BUG_B4);
			}
			else{
				 bug_reg|=BIT(BUG_LOGJAM2);
			}
			if(S_LOGJAM_1){ 
				/*Типа не исправны датчики Мёртвая точка 1 и Мёртвая точка 2*/
				bug_reg&=~BIT(BUG_LOGJAM1);
				/*Устранимая ошибка*/
				//SET_STATE_B_REMOVABLE_ERR;
				RemError(ON,REM_BUG_B4);
			}
			else{
				/* Датчики Мёртвая точка 1 и Мёртвая точка 2 исправны*/
				bug_reg|=BIT(BUG_LOGJAM1);
			}
			/*Если типа всё ништяк с датчиками, то убираем устранимую ошибку*/
			if ((S_LOGJAM_1==0)&&(S_LOGJAM_1==0)) RemError(OFF,REM_BUG_B4);
			/*Следующий шаг*/
			door_B_close_step++;
		}break;
		case 11:{
#warning Убрать комменты!!!			/*Натягиваем ремень*/
			Set_PWM(OPEN,10);
			ts_nr=TIME_NOW;
			/*Следующий шаг*/
			door_B_close_step++;
		}break;
		case 12:{
			if (time_interval(ts_nr)<250) return;
			else {
				/*подождали 200..2500 мс и вырубаем двигатель и муфту*/
				M1_OFF;
				CM_OFF;
				/*Следующий шаг*/
				door_B_close_step++;
			}
		}break;
		case 13:{
			/*Тут типа смена режима */
			Door.state=MODE_CLOSE;
			door_B_close_step=0;
		}break;
		case 14:{
			if (S_CLOSED==0){
				if((time_interval(ts_wClose)<500)) return;
				/*устранимая ошибка*/
				RemError(ON,REM_BUG_B4);
				Door_B_Revers();
			}
			else{
				//Обманываем счётчик
				//ts_tzz=TIE_NOW-Door.sts_tzz-1;
				
				overcurrent=0;
				/*Убираем устранимую ошибку*/
				RemError(OFF,REM_BUG_B5);
				door_B_close_step=4;
			}
		}break;
	};
}

/********************************************
	Функция блокировки боковой двери
*******************************************/
void
Door_B_Bloking(void)
{
	switch(door_B_blok_step){
		case 0:{
			/*Прошло 15 секунд с последней блокировки*/		
			if(time_interval(ts_15_0)<15000){ 
				Door.state=MODE_CLOSE;
				return;
			}
			else{
				/*Засекаем 15 секунд*/
				ts_15_0=TIME_NOW;
				/*Подать питание на "Пускоую и удерживающую катушки соленоида Y2"*/
				CY2U_ON;
				//CY2P_ON;
				/*Засекаем 1 сек*/
				ts_1_0=TIME_NOW;
				/*Следующий шаг*/
				door_B_blok_step++;
			}
		}break;
		case 1:{
			/*Прошла 1 сек?*/
			//if(time_interval(ts_1_0)<1000) return;
			//else{
				/*Отключить питание с пусковой катушки Y2*/
			//	CY2P_OFF;
				/*Поступает сигнал V>5*/
				//if(S_V5){
					/*Поступил сигнал "Датчик Y2"*/
					if(S_Y2){
						/*Хэппи энд*/
						door_B_blok_step=0x02;
					}
					else{
						/*Прошла ещё 1 сек?*/
						if(time_interval(ts_1_0)<2000) return;
						else {
							/*Ставим режим "Сломались"*/
							Door.state=MODE_BREAKAGE;
							/*авария*/
							FatalError(ON,FAT_BUG_B4);
							Door_AlarmClose();
							/*Бесконечный баззер*/
							Set_Buzzer(0,UNLIMITED);
							return;
						}
					}
				//}
				//else{
					/*Отключить питание с удерживающей катушки Y2 ?*/
				//	CY2U_OFF;
					/*Хэппи энд*/
				//	door_B_blok_step=0x02;
				//}	
			//}
		}break;
		case 2:{
			/*Засекаем 15 секунд*/
			ts_15_0=TIME_NOW;
			Door.state=MODE_BLOKED;
			door_B_blok_step=0;
		}break;
	};
}
//}

/********************************************************************
	Проверка неисправностей
	
	Здесь проверяются те неисправности, которые 
	необязательно проверять во время выполнения алгоритма
**********************************************************************/
void
Chech_Bugs(void)
{
	/*Проверяем помеху в створе(защитные кромки)*/
	if (Door.type==DOOR_TYPE_SIDE)
		if((S_KRERR1)||(S_KRERR1)){
			bug_reg&=~BIT(BUG_PROTECTIVE_EDGE);
			FatalError(ON,FAT_BUG_B8);
			/*Аварийная остановка*/
			Door_AlarmClose();
			/*Бесконечный баззер*/
			Set_Buzzer(0,UNLIMITED);
			/*Ставим режим "Сломались"*/
			Door.state=MODE_BREAKAGE;
		}
		else{
			bug_reg|=BIT(BUG_PROTECTIVE_EDGE);
			FatalError(OFF,FAT_BUG_B8);
		}
	else {
		/* для торцевой пока не понятно что делать*/
		if(S_KRERR1){
			bug_reg&=~BIT(BUG_PROTECTIVE_EDGE);
			
		}
		else{
			bug_reg|=BIT(BUG_PROTECTIVE_EDGE);
			
		}
	}
	
	
}


/*Проверка нажатия секретки и замка во время движения*/
uint8_t 
CheckLock(void)
{
	// Палим Замок и Секретку в незакрытом состоянии
	if	((S_LOCK)||(S_SEKRETKA)){
		/*Устранимая ошибка*/
		if(Door.type==DOOR_TYPE_SIDE){
			RemError(ON,REM_BUG_B2);
		}
		else {
			RemError(ON,REM_BUG_T2);
		}
		// Гасим Лампочки
		LED_PWR_OFF;
		lamps_reg&=~BIT(LED_O_GREEN);
		lamps_reg&=~BIT(LED_Z_GREEN);
		return 0;
	}else{
		/*Убираем ошибку "Ключа"*/
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
	Функция сброса всех ошибок и аварий
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
	***	Мега главная функция, разруливает дверями 	***
*****************************************************************/
void 
Door_Manager(void)
{

	/*В зависимости от типа двери ...*/
	if(Door.type==DOOR_TYPE_SIDE){
	//	if((S_MAN_AUTO)||(rez[0]<Door.sts_Tr1)||(Tnar_EPVS<Door.sts_Tr2)||(rez[0]>=OVERHEATING_T)||(events_flags&BIT(MAN_MODE))){
	//	if((S_MAN_AUTO)){//		/*Переход в ручной режим*/					
	if((S_MAN_AUTO)||((rez[0]<Door.sts_Tr1)&&(err_flag==0))||((rez[0]>=OVERHEATING_T)&&(err_flag==0))||(events_flags&BIT(MAN_MODE))){
		if ((Door.state!=MODE_OPENING)&&(Door.state!=MODE_CLOSING)){
				Door.state=MODE_MANUAL;
			}
	}
		/*Боковая дверь*/
		/*В зависимости от текущего режима выбираем действие...*/
		switch (Door.state){
			case MODE_FIRST_BLOOD:{
				LED_PWR_ON;
				/*Вырубаем нахрен баззер*/
				Set_Buzzer(0,CONTINUOUS);
				/*Инициализация переменных*/
				door_B_close_step=0;
				door_B_open_step=0;
				door_B_blok_step=0;
				ts_15_0=TIME_NOW+15000;
				/*Надо по датчикам определить где мы ...*/
				if ((S_CLOSED)&&(S_COMPRESSED_2)&&(S_COMPRESSED_1)){
					/*если присутствует сигнал "Закрыто"*/
					Door.state=MODE_CLOSE;
				}
				else if ((S_OPEN)&&(S_V45==0)&&(S_V5==0)){
					/*если присутствует сигнал "Открыто" и нет сигналов V5 или V45*/
					Door.state=MODE_OPEN;
				}
				else if((S_OPEN)&&((S_V45)||(S_V5))){
					/*если присутствует сигнал "Открыто" и сигналы или V>5 или(и) V>45 */
					Door.state=MODE_CLOSING;
				}
				else if( (S_OPEN==0)&&((S_CLOSED==0)||(S_COMPRESSED_2==0)||(S_COMPRESSED_1==0)) ){
					/*если находится в промежуточном положении*/
					Door.state=MODE_OPENING;
				}
			}break;
			case MODE_MANUAL:{
				// Сбрасываем ошибки
				Reset_errors();
				LED_PWR_OFF;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				/*Вырубаем электронные устройства*/
				M1_OFF;
				CM_OFF;
				CY1U_OFF;
				CY1P_OFF;
				CY2U_OFF;
				/*Вырубаем нахрен баззер*/
				Set_Buzzer(0,CONTINUOUS);
				/*Активируем режим мигания кнопок*/
				if ((flag_start_mig==0)&&((S_SEKRETKA==0)&&(S_LOCK==0)/*&&(S_CLOSED)*/)){
					flag_start_mig=0x01;
					ts_start_mig=PERIOD_ZMIG;
				}
				/*Переход в автоматический режим*/
				if((S_MAN_AUTO==0)&&(rez[0]>=Door.sts_Tr1)&&((rez[0]<OVERHEATING_T))) {
					/*Активируем режим мигания кнопок*/
					flag_start_mig=0x00;
					ts_start_mig=PERIOD_ZMIG;
					Door.state=MODE_FIRST_BLOOD;
				}
			}break;
			case MODE_OPENING:{
				// Палим Замок и Секретку в незакрытом состоянии
				if	(CheckLock()){
					LED_PWR_ON;
					ts_start_mig=0;
					lamps_reg&=~BIT(LED_O_GREEN);
					lamps_reg&=~BIT(LED_Z_GREEN);
				}
				Door_B_Open();
			}break;
			case MODE_CLOSING:{
				// Палим Замок и Секретку в незакрытом состоянии
				if	(CheckLock()){
					LED_PWR_ON;
					ts_start_mig=0;
					lamps_reg&=~BIT(LED_O_GREEN);
					lamps_reg&=~BIT(LED_Z_GREEN);
					Door_B_Close();
				}
			}break;			
			case MODE_OPEN:{
				// Палим Замок и Секретку в незакрытом состоянии
				if	(CheckLock()){
					LED_PWR_ON;
					/*Смотрим переход в промежуточное положение. Типа если пропал сигнал "Открыто", 
					значит дверь в промежуточном положении*/
					if(S_OPEN==0){
						Door.state=MODE_INTERMEDIATELY;
						return;
					}
					lamps_reg&=~BIT(LED_O_GREEN);
					if((S_FOOTBOARD))
						lamps_reg|=BIT(LED_Z_GREEN);
					else
						lamps_reg&=~BIT(LED_Z_GREEN);
					/*Время открытия-закрытия*/
					TIME_TO_OPEN_CLOSE=time_opcls;
					if(comand_reg&BIT(C_OPEN)){
						/*Типа если стояла команда на открытие, то убираем её ... */
						comand_reg&=~BIT(C_OPEN);
						/*Добавляем событие успешного приёма комманды*/
						//SET_STATE_B_ACCEPTED;
					}
					/*Команды от БД или нажатия на кнопки... или сигналы типа "V>5"*/
					if ((((comand_reg&BIT(C_LOCK))||(comand_reg&BIT(C_CLOSE))
						||(S_KZ)||((S_V45)||(S_V5))))/*&&(S_FOOTBOARD)*/) Door.state=MODE_CLOSING;
				}
			}break;			
			case MODE_CLOSE:{
					LED_PWR_ON;
					lamps_reg&=~BIT(LED_Z_GREEN);
				/*Смотрим переход в промежуточное положение. Типа если пропал сигнал "Закрыто", 
				значит дверь в промежуточном положении*/
				if((S_CLOSED==0)||(S_COMPRESSED_2==0)||(S_COMPRESSED_1==0)){
					Door.state=MODE_INTERMEDIATELY;
					return;
				}
				TIME_TO_OPEN_CLOSE=time_opcls;
				if(comand_reg&BIT(C_CLOSE)){
					/*Типа если стояла команда на закрытие, то убираем её ... */
					comand_reg&=~BIT(C_CLOSE);
					/*Добавляем событие успешного приёма комманды*/
					//SET_STATE_B_ACCEPTED;
				}
				
				/*Поступила комада на блокировку или присутствуют сигналы "V>5 "или" V>45"?*/
				if(((S_V45)||(S_V5)||((comand_reg&BIT(C_LOCK))))
					&&(time_interval(ts_15_0)>=15000)){
					Door.state=MODE_BLOKING;
				}
				else {
					lamps_reg|=BIT(LED_O_GREEN);
				}
				
				// Палим Замок и Секретку
				if((S_LOCK)||(S_SEKRETKA)) Door.state=MODE_KEYLOCK;
				/*от этих открываемся*/
				if (((comand_reg&BIT(C_OPEN))||(S_KO))
				&& (S_V45==0)&&(S_V5==0)&&(S_LOCK==0)&&(S_SEKRETKA==0))
					Door.state=MODE_OPENING;
			}break;
			case MODE_BLOKED:{
				/*Убираем ошибку "Ключа"*/
				RemError(OFF,REM_BUG_B2);
				//	bug_reg|=BIT(BUG_LOCK);
				LED_PWR_OFF;
				door_B_blok_step=0x00;
				if((S_V45==0)&&(S_V5==0)&&((comand_reg&BIT(C_LOCK))==0)){
					/*Снимаем  питание с удерживающей катушки соленоида Y2*/
					CY2U_OFF;
					/*переводим в режим "Закрыто"*/
					Door.state=MODE_CLOSE;
				}
				
				if (comand_reg&BIT(C_UNLOCK)) {
					if(comand_reg&BIT(C_LOCK)){
						/*Типа если стояла команда на закрытие, то убираем её ... */
						comand_reg&=~BIT(C_LOCK);				
					}
					/*Снимаем  питание с удерживающей катушки соленоида Y2*/
					CY2U_OFF;
					/*переводим в режим "Закрыто"*/
					Door.state=MODE_CLOSE;
					/*Добавляем событие успешного приёма комманды*/
					comand_reg&=~BIT(C_UNLOCK);
				}
			}break;		
			case MODE_BLOKING:{
				LED_PWR_ON;
				lamps_reg&=~BIT(LED_O_GREEN);
				lamps_reg&=~BIT(LED_Z_GREEN);
				if (Door.sts_sb){	
					/*Если есть в настройках "Устройство блокировки", то просто блокируем*/
					Door_B_Bloking();
				}
				else {
					/*А вот если нет в настройках "Устройства блокировки", 
					то светить "Неустронимая ошибка" до тех пор, 
					пока дверь не будет закрыта на замок и/или секретку*/
					FatalError(ON,FAT_BUG_B5);
					/*Бесконечный баззер*/
					Set_Buzzer(0,UNLIMITED);
					if ((S_LOCK)||(S_SEKRETKA)){
						FatalError(OFF,FAT_BUG_B5);
						/*Вырубаем нахрен баззер*/
						Set_Buzzer(0,CONTINUOUS);
						Door.state = MODE_KEYLOCK;
					}
				}
				
			}break;
			case MODE_INTERMEDIATELY:{
				if (CheckLock()){
					LED_PWR_ON;
					/*В промежуточном положении включены обе подсветки*/
					lamps_reg|=BIT(LED_O_GREEN);
					lamps_reg|=BIT(LED_Z_GREEN);
				}
				/*Команды от БД или нажатия на кнопки... или сигналы типа "V>5"*/
				if (((comand_reg&BIT(C_LOCK))||(comand_reg&BIT(C_CLOSE))
					||(S_KZ)||((S_V45)||(S_V5)))) Door.state=MODE_CLOSING;
				/*А от этих открываемся*/
				if ((comand_reg&BIT(C_OPEN))||(S_KO)) Door.state=MODE_OPENING;
				/*Если появились сигналы с дачиков "Открыто" или "Закрыто", то значит дверь открыли или закрыли ручками*/
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
				/*Убираем ошибку "Ключа"*/
			//	bug_reg|=BIT(BUG_LOCK);
				RemError(OFF,REM_BUG_B2);
				// Гасим Лампочки
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
		/*Торцевая дверь*/
		if((S_MAN_AUTO)||((rez[0]<Door.sts_Tr1)&&(err_flag==0))||((rez[0]>=OVERHEATING_T)&&(err_flag==0))||(events_flags&BIT(MAN_MODE))){
			/*Переход в ручной режим*/					
			
			//if ((Door.state!=MODE_OPENING)&&(Door.state!=MODE_CLOSING)){
				if(Door.state!=MODE_MANUAL){
					Set_Buzzer(10000,CONTINUOUS);
					Door.state=MODE_MANUAL;
				}
			//}
		}
		/*Проверка замка/секретки*/
		if ((S_CLOSED==0) &&(CheckLock()==0)){
				if (buz_on==0){
					Set_Buzzer(10000,INTERRUPTED);
					ts_buz = TIME_NOW;
					buz_on = 0x01;
				}
				/*Останавливаем дверь*/
				M1_OFF;
				return;
			}
			else {
				buz_on = 0;
				/*Вырубаем баззер*/
	//			Set_Buzzer(0,CONTINUOUS);
			}
		//}
		/*В зависимости от текущего режима выбираем действие...*/
		switch (Door.state){
			case MODE_FIRST_BLOOD:{
				/*Вырубаем нахрен баззер*/
				//Set_Buzzer(1,CONTINUOUS);
				Buzzer.time=0;
				/*Ждём 2 секунды при переводе из ручного*/
				if(time_interval(ts_sec2)<2000) return;
				/*Инициализация параметров*/
				/*Если нет сигнала Закрыто, то открываемся*/
				if (S_CLOSED==0)
					Door.state=MODE_OPENING;
				else
					Door.state=MODE_CLOSE;
			}break;
			case MODE_OPENING:{
				/*Завязываем с миганием*/
				ts_start_mig=0;
				lamps_reg&=~BIT(LED_O_GREEN);
				
				LED_PWR_OFF;
				Door_T_Open();
			}break;
			case MODE_CLOSING:{
				/*Завязываем с миганием*/
				ts_start_mig=0;
				if((S_OPEN)==0){
					lamps_reg|=BIT(LED_O_GREEN);
					LED_PWR_ON;
				}
				else {
					lamps_reg&=~BIT(LED_O_GREEN);
					LED_PWR_OFF;
				}
				//ЕСли во время закрытия нажали кнопку "Открыть", то делаем реверс
				if(S_KO){
					Door_T_Revers();
					Door.state=MODE_OPENING;
					return;
				}
				Door_T_Close();
			}break;
			case MODE_CLOSE:{
				/*подсветка*/
				if((S_SEKRETKA)||(S_LOCK)){
					/*Если закрыто, то откл. подсветку*/
					lamps_reg&=~BIT(LED_O_GREEN);
					LED_PWR_OFF;
				}
				else{
					/*иначе пущай горит и ярко светит, освещая путь усталому пьяному проводнику
					и не спящим пассажирам, которых понесла куда-то нелёгкая, а так же тётенькам 
					из вагона-ресторана, таскающих дорогущее  и почему-то вечно холодное пиво...*/
					lamps_reg|=BIT(LED_O_GREEN);
					LED_PWR_ON;
				}
				if(comand_reg&BIT(C_CLOSE)){
					/*Типа если стояла команда на закрытие, то убираем её ... */
					comand_reg&=~BIT(C_CLOSE);
					/*Добавляем событие успешного приёма комманды*/
					//SET_STATE_B_ACCEPTED;
				}
				if((comand_reg&BIT(C_OPEN))||(S_KO)){
					
					door_T_open_step=0;
					Door.state=MODE_OPENING;
					
				}
			}break;
			case MODE_MANUAL:{
				// Сбрасываем ошибки
				Reset_errors();
				/*Гасим  подсветку*/
				lamps_reg&=~BIT(LED_O_GREEN);
				LED_PWR_OFF;
				/*Питание с M1 и MF1 снимается */
				M1_OFF;
				CM_OFF;
				/*При нажатии на кнопку звучит непрерывистый сигнал зуммера длительностью 10 сек*/
				if((S_KO)||(S_KZ)) Set_Buzzer(10000,CONTINUOUS);
				/*Переход в автоматический режим*/
				/*Активируем режим мигания кнопок*/
				if ((flag_start_mig==0)&&((S_SEKRETKA==0)&&(S_LOCK==0)&&(S_CLOSED))){
					flag_start_mig=0x01;
					ts_start_mig=PERIOD_ZMIG;
				}
				/*Переход в автоматический режим*/
				if((S_MAN_AUTO==0)&&(rez[0]>=Door.sts_Tr1)) {
					/*Активируем режим мигания кнопок*/
					if ((S_SEKRETKA==0)&&(S_LOCK==0)&&(S_CLOSED)){
						flag_start_mig=0x00;
						ts_start_mig=PERIOD_ZMIG;
					}
					ts_sec2 = TIME_NOW;
					Door.state=MODE_FIRST_BLOOD;
				}
			}break;
			case MODE_OPEN:{
				/*Завязываем с миганием*/
				ts_start_mig=0;
				if(comand_reg&BIT(C_OPEN)){
					/*Типа если стояла команда на закрытие, то убираем её ... */
					comand_reg&=~BIT(C_OPEN);
					/*Добавляем событие успешного приёма комманды*/
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
					/*Типа если стояла команда на закрытие, то убираем её ... */
					comand_reg&=~BIT(C_OPEN);
					/*Добавляем событие успешного приёма комманды*/
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