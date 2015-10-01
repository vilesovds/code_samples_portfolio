//ICC-AVR application builder : 25.05.2009 12:16:57
// Target : M128
// Crystal: 16.000Mhz

#include <PLATFORM.h>
#include <timer0.h>
#include <types.h>
#include <macros.h>
#include <mcp2510.h>
#include <SPI.h>
#include <MSP_DAC.h>
#include <MSP430_MUX.h>
#include <USART.h>
#include <eeprom.h>
#include <watchdog.h>
#include "ledgraph.h"
#include "main.h"
#include "algoritm.h"
#include "elsiel.c"
#include "test.h"
uint16_t ts=0,ts_CAN=0,ts_dac=0,ts_debcan=0;
uint16_t main_ts_1sec;
USER_CAN_MESSAGE /**/Out_msg,In_msg,deb_msg;
extern unsigned char DAC_Data[COUNT_CHANNELS];
extern uint16_t err_count;


extern struct DAC_STATUS;
extern uint8_t Pbit;
extern unsigned int msp_caldata[BUFLEN*2];
extern struct {
	
	uint8_t work_mode;
	
	uint16_t tpause_package;
	
	uint8_t current_channel;
	
	uint8_t current_bit;
}DAC_STATUS;

uint8_t flag=0;
extern uint16_t cal_time_counter;
extern char rxbyte[8];// ????? ?????? ?????? ?? MSP
 uint16_t m_ts=0;
uint8_t num_pos=0;
extern TMSP MSP[MSP_COUNT];
extern unsigned int msp_data[BUFLEN];
	

extern 	struct {
		char current_MSP; //  номер(id) MSP, с которой происходит обмен данными
		char current_chanel;
		char state;// состояние уарта: READY, BUSY, NO_ANSWER (смотри дефайнесы в разделе "Для обмена с MSP")
	}UART;
	
uint8_t flag_cal=ENABLE_CAL;

/*для 6 минутного таймера*/
uint16_t Time6minuts=0;

uint32_t moto_timeA=0;
uint32_t moto_timeM=0;
uint8_t flag6min=0;

/*========================================================================
		Антидребезг
*=========================================================================*/
uint8_t debounce[N_SIGNAL_BUF_COUNT]; // состояние вводов после антидребезговой обработки
uint8_t bufval[N_SIGNAL_BUF_COUNT];   // счётчики антидребезга 
/* ===================================================================================
		FROM PD_v2_7
======================================================================================*/
//{
unsigned int RXID=0;

signed int  T_USTAVKI=0;


extern unsigned char NLoh; 
extern signed int  T_PRIT;
extern unsigned char TimerOUT;
extern unsigned char ErrorDamper;

extern unsigned char WORK_MODE;
extern unsigned char ALARM_REG;
extern unsigned char STEP_COUNT;
extern unsigned char STEP_PRIT;

extern uint16_t CountVVK1;
extern uint16_t CountVVK2;

extern unsigned int Tdamp[6];
extern char Flag_SpeedDTV;
extern char workmin;
extern signed int SPEEDDTV;
// Список регистров
unsigned char STATUS_REG_ADC=0;        // Регистр статуса АЦП (дополнительный)
unsigned char ENABLE_ADC_CH_REG=0;     // Регистр исправности канала АЦП
unsigned char END_C_ADC_CH_REG_0=0;    // Регистр окончания калибровки канала АЦП Т=0
unsigned char END_C_ADC_CH_REG_25=0;   // Регистр окончания калибровки канала АЦП Т=25
unsigned char END_C_ADC_CH_REG_130=0;  // Регистр окончания калибровки канала АЦП Т=130
unsigned char FIN_ADC_OPERATION_REG=0; // Регистр 
unsigned char EVENT_REG=0;             // Регистр событий
extern unsigned char ErrorReg=0;       // Регистр ошибок температур
unsigned int  AllErrorReg=0;           // Регистр ошибок
unsigned char EnaStandByInfo=0;        // Регистр состояния
unsigned char EnaStandByBAU=0;


unsigned char TimeOutExtMaster=3;

//==========================

// Список переменных для АЦП


//Для раздельных вентагрегатов
   unsigned char QuantityADC=7; // Количество каналов АЦП
   
   signed int MT_DataADC[8];   // Усредненные данные от АЦП
   signed int T_DataADC[8];   // Значения температур
   signed int SIM_T_DATA[8];   // Значения температур симулированные с ПК


   unsigned int DataPU[16];   // Данные от ПУ
   unsigned char NumberADC=0;               // Номер канала АЦП

   
   unsigned int TimerButP=0;
   unsigned int Timer1sec=0;
   unsigned int Timer01sec=0;
   unsigned int Timer5min=0;
   unsigned int CountTesting=0;
    
   unsigned char Timer3sec=0;
   unsigned char StepErr=0;
   unsigned char QuantityError=0;
   unsigned char MassivScreenError[16];


   // Параметры первого вентагрегата
signed int T_out1=0; // Измеренная температура наружнего воздуха
signed int T_rec1=0; // Измеренная температура рециркуляционного воздуха
signed int T_prit1=0; // Измеренная температура приточного воздуха
unsigned int D_out1=0; // Измеренное положение заслонки наружнего воздуха
unsigned int D_rec1=0; // Измеренное положение заслонки рециркуляционного воздуха
unsigned int D_pot1=0; // Измеренное положение заслонки потока воздуха
// Параметры второго вентагрегата
signed int T_out2=0; // Измеренная температура наружнего воздуха
signed int T_rec2=0; // Измеренная температура рециркуляционного воздуха
signed int T_prit2=0; // Измеренная температура приточного воздуха
unsigned int D_out2=0; // Измеренное положение заслонки наружнего воздуха
unsigned int D_rec2=0; // Измеренное положение заслонки рециркуляционного воздуха
unsigned int D_pot2=0; // Измеренное положение заслонки потока воздуха
// Общие параметры вагона
extern signed int  T_INSIDE; //Текущее значение температуры в вагоне
signed int T_in=0;   // Пересчет значения T_INSIDE 
signed int T_in1=0;   // Измеренная температура воздуха внутри вагона
signed int T_in2=0;   // Измеренная температура воздуха внутри вагона
unsigned int LT_in=160; // Заданная температура воздуха внутри вагона


unsigned char Line2[8];     // Массив второй строки
unsigned int Nscreen=21;     // Порядковый номер экрана

unsigned char CountBut_plus=0;   // Цифровой фильтр кнопки +
unsigned char CountBut_minus=0;  // Цифровой фильтр кнопки -
unsigned char CountBut_prog=0;   // Цифровой фильтр кнопки P

unsigned char ButtonReg=0;          // Регистр кнопок
unsigned char DataIN=0;             // Дискретные входы
unsigned char DataOUT=0;            // Дискретные выходы
unsigned char DataLED=0;            // Светодиодные индикаторы (единичные)


unsigned char TimerMsgCAN=0;         // Счетчик 25 мс для передачи CAN сообщения
unsigned char Timer20ms=0;           // Запись дисплея

unsigned int TimerButtonClickOff=0;
unsigned int Timer1hour=0;

unsigned char CounterID=0;           // Счетчик ID для CAN 
unsigned char pageID_100=0;          // Счетчик страниц ID=100 для CAN
unsigned char pageID_101=0;          // Счетчик страниц ID=101 для CAN
unsigned char pageID_102=0;          // Счетчик страниц ID=102 для CAN
unsigned char pageID_103=0;          // Счетчик страниц ID=103 для CAN
unsigned char NumCH=0;


unsigned char DamperDataIN[6];
unsigned char DamperDataOUT[6];


unsigned char DamperAdres=0;

unsigned char ReadRecBufCAN=0;


extern unsigned int FlagWorkRegister;
extern unsigned char Timer_smeny_loh;
extern unsigned int Timer_may_up_loh;
extern char mayup;
extern char maydown;
extern unsigned char Timer_smeny_loh_up;
extern unsigned char Timer_smeny_loh_down;
extern unsigned int Timer_close_damper;


//}
extern uint8_t nChannel;
extern 	TMSP MSP[MSP_COUNT];
/*DEBUG*/
uint8_t 
deb_msg_send(void)
{
	if(time_interval(ts_debcan)<300) return 1;
	ts_debcan=timer0_getCounter();
	deb_msg.ID=0x200;
	deb_msg.DLEN=8;
	
	deb_msg.data[0]=MSP[0].count_attempt_reinit;
	deb_msg.data[1]=MSP[1].count_attempt_reinit;
	deb_msg.data[2]=MSP[2].count_attempt_reinit;
	deb_msg.data[3]=UART.current_MSP+1;
	deb_msg.data[4]=UART.current_chanel+1;
	deb_msg.data[5]=MSP[0].status;
	deb_msg.data[6]=MSP[1].status;
	deb_msg.data[7]=MSP[2].status;
	CAN_Msg_Send(CAN_PARAMS,&deb_msg);
	return 0;
}
/*================================================================================
*	Запись в еепром моточасов  работы
*=================================================================================*/
void
Save_motoA(void)
{
	EEPROM_WriteChar(ADDR_MOTOA,(moto_timeA>>24));
	EEPROM_WriteChar(ADDR_MOTOA+1,(moto_timeA>>16));
	EEPROM_WriteChar(ADDR_MOTOA+2,(moto_timeA>>8));
	EEPROM_WriteChar(ADDR_MOTOA+3,((uint8_t)moto_timeA));
	
}
/*=======================================================================================
*	Запись в еепром моточасов  работы блока в ручном режиме
*========================================================================================*/
void
Save_motoM(void)
{
	EEPROM_WriteChar(ADDR_MOTOM,(moto_timeM>>24));
	EEPROM_WriteChar(ADDR_MOTOM+1,(moto_timeM>>16));
	EEPROM_WriteChar(ADDR_MOTOM+2,(moto_timeM>>8));
	EEPROM_WriteChar(ADDR_MOTOM+3,((uint8_t)moto_timeM));
}
/*======================================================================================
*	Увеличение счётчика 6 минут для подсчёта мотточасов работы блока
*	INPUT: 
*	 operation - код операции: 	0 - обнуление
*						1 - инкремент
*=======================================================================================*/
void
Set_motoa_time(uint8_t operation)
{
	switch (operation){
		case 0:{
			moto_timeA=0x0000;
			/*записываем в еепром*/
			Save_motoA();
		}
		break;
		case 1:{
			if (moto_timeA<655350){
				moto_timeA++;
			}
			else {
				moto_timeA=0x000;
			}
			/*записываем в еепром*/
			Save_motoA();
		}
		break;
	}
}

/*================================================================================
*	Увеличение счётчика 6 минут для подсчёта моточасов в ручном режиме
*	INPUT: 
*	 operation - код операции: 	0 - обнуление
*						1 - инкремент
*=================================================================================*/
void
Set_motom_time(uint8_t operation)
{
	switch (operation){
		case 0:{
			moto_timeM=0x0000;
			/*записываем в еепром*/
			Save_motoM();
		}
		break;
		case 1:{
			if ((moto_timeM<655350)&&(WORK_MODE&BIT(MANUAL_MODE))){
				moto_timeM++;
			}
			else {
				moto_timeM=0x000;
			}
			/*записываем в еепром*/
			Save_motoM();
		}
		break;
	}
}

/*==================================================================
// расчёт антидребезга
// debounce_time - время задержки антидребезга  (мс)
*===================================================================*/
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

/* ========================================================
* опрос и антидребезг для всех входов 
* вызывается по таймеру (1 ms)
* =========================================================*/
void 
check_pins(void)
{
  checkbuf(N_SIGNAL_J1,SIGNAL_J1,DEBOUNCE_TIME);
  checkbuf(N_SIGNAL_J2,SIGNAL_J2,DEBOUNCE_TIME);
  checkbuf(N_SIGNAL_J3,SIGNAL_J3,DEBOUNCE_TIME);
  checkbuf(N_SIGNAL_J4,SIGNAL_J4,DEBOUNCE_TIME);
  checkbuf(N_SIGNAL_J5,SIGNAL_J5,DEBOUNCE_TIME);
  checkbuf(N_SIGNAL_BUT1,SIGNAL_BUT1,50);
  checkbuf(N_SIGNAL_BUT2,SIGNAL_BUT2,50);
  checkbuf(N_SIGNAL_JUMPER,SIGNAL_JUMPER,100);
}

/*============================================
Пользовательская функция, 
вызывается по прерыванию от таймера каждую мс
==============================================*/
void
Timer0_1ms_int(void)
{
	check_pins();
	on_timer();
}

/*============================================
Пользовательская функция, 
вызывается по прерыванию от таймера каждую секунду
==============================================*/
void
Timer0_1s_int(void)
{
	/*6 минутный таймер*/
	if (Time6minuts<356)Time6minuts++;
	else {
		flag6min=0x01;
		Time6minuts=0;
	}
	/*Следующие действия в зависимости от состояния калибровки 
	создают нам соответсвующую индикацию лампочек*/
	switch (flag_cal){
		case START_CAL:{
			LED_SET_ON;
			Nscreen=0x00;
			LEDGRAPH_Print_str(0,0, "Т_вр=\0");
			LoadDataInLine(160);
			//LoadDataInLine(err_count*10);
			LEDGRAPH_Print_str(0,1, &Line2[0]); 
		}
		break;
		case END_CAL:{
			LED_SET_SWITCH;
		}
		break;
		case OUT_CAL:{
			LED_SET_SWITCH;
			LED_ERR_ON;
		}
		break;
		case	ENABLE_CAL:{
			LED_SET_OFF;
		}
		break;
	}
}

/*================================================================
*	Установка 1го бита для регистора
*=================================================================*/
void 
SetBitForDataOut(uint16_t data)
{
 if (data) PORTA|=BIT(DATA_OUT);
 else      PORTA&=~BIT(DATA_OUT);
}

/*====================================================================
*	индикация лампочек на морде посредством сдвигового регистра
*=====================================================================*/
void 
OutLed(uint8_t Led)
{
unsigned int i=0;
unsigned int buffer=0;
buffer=Led;
PORTA&=~BIT(LOKCH_OUT);
PORTA&=~BIT(CS_DATA_OUT);
PORTA|=BIT(RESET_DATA_OUT);
for(i=0;i<8;i++){
 PORTA|=BIT(CLOCK_DATA_OUT);
 SetBitForDataOut(buffer&0x0001);
 buffer=buffer>>1;
 PORTA&=~BIT(CLOCK_DATA_OUT);
}
PORTA|=BIT(LOKCH_OUT);
NOP();
PORTA&=~BIT(LOKCH_OUT);
}


/*=============================================================================
*		ну тут и ежу понятно чё происходит ))
*=============================================================================*/
void port_init(void)
{
PORTA = 0xFF;
 DDRA  = 0xF8;
 PORTB = 0x00;
 DDRB  = 0x16;
 PORTC = 0x00; //m103 output only
 DDRC  = 0x00;
/*
 PORTD = 0x00;
 DDRD  = 0x7F;
*/
  PORTD = 0xDF;
 DDRD  = 0xFF;
 
 PORTE = 0x00;
 DDRE  = 0x0E;
 //PORTF = 0x80;
 PORTF = 0x00;
 DDRF  = 0xB0;
 PORTG = 0x00;
 DDRG  = 0x1F;
}


/*===================================================================================
*	Функция возвращает истинное напряжение на выходе определённого канала МСП.
*===================================================================================*/
uint16_t
MSP_DAC_Get_Realval(uint8_t channel)
{	uint8_t zt=0x00;
	static char lookup_table[]={1,0,3,2,4,5};
	
	main_ts_1sec=timer0_getCounter();
	while (time_interval(main_ts_1sec)<1000){
			MSP_Refresh();
			Read_DamperIN();
	}
    zt=DamperDataIN[lookup_table[nChannel]];
	return zt;
	
}


/****************************************
	инициализация таймера 1
*****************************************/
void 
timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xF0; //setup
 TCNT1L = 0x60;
 OCR1AH = 0x0F;
 OCR1AL = 0xA0;
 OCR1BH = 0x0F;
 OCR1BL = 0xA0;
 OCR1CH = 0x0F;
 OCR1CL = 0xA0;
 ICR1H  = 0x0F;
 ICR1L  = 0xA0;
 TCCR1A = 0x00;
 TCCR1B = 0x01; //start Timer
 TIMSK+=0x04;
}


/*********************************************************************************************
	-Вась, тебе девушка звонит...
	 - Девушка!?Какая?
	 -Да уже никакая! 
	В прерывании от таймера 1 обновляем экран
*****************************************************************/
#pragma interrupt_handler timer1_ovf_isr:15
void 
timer1_ovf_isr(void)
{

 
 /**/
 LEDGRAPH_Refresh();  // Запись следующего столбца индикации

 //TIMER1 has overflowed
 TCNT1H = 0xF0; //reload counter high value
 TCNT1L = 0x60; //reload counter low value
}
/*=====================================================================================
*	Основной обработчик прерываний от таймера
*======================================================================================*/
void 
on_timer(void)
{
			
	if (Timer20ms<5){
		Timer20ms++;
	}
	else{
		EVENT_REG|=BIT(EnaDispley);
		Timer20ms=0;
	}

	if (Timer01sec<100){
		Timer01sec++;
		// FlagWorkRegister&=~BIT(Flag100msec); 
		
	}
	else{
		Timer01sec=0;
		FlagWorkRegister|=BIT(Flag100msec); 
		if (TimerButtonClickOff) TimerButtonClickOff--;
 		//===========================================================// 
		// Установка события удержания 5 сек кнопки P
		//{===========================================================//
        if (CountBut_prog>0xF0)	{
			if (TimerButP<50){
				TimerButP++;
				ButtonReg&=~BIT(FlagButP);
			}
			else{
			   ButtonReg|=BIT(FlagButP);
			}
		}
		else{
		   TimerButP=0; 
		   ButtonReg&=~BIT(FlagButP);
		}
		//}===========================================================// 
		//EVENT_REG|=BIT(ENA_DATA_ADC);     // Разрешить измерение АЦП
	}
	
	//{***************************************************************************
	if (Timer1sec<500){
		Timer1sec++;
	}
	else{
		if (TimerButtonClickOff==0) {
			if (Timer3sec<3) Timer3sec++;
			else {
				if (AllErrorReg){
					if (StepErr<QuantityError) {
						Nscreen=MassivScreenError[StepErr];
						StepErr++;
					}
					else StepErr=0;
				}
				Timer3sec=0; 
			}
		}
		Timer1sec=0;
		EVENT_REG|=BIT(Event_Led_SET); // Разрешить переключение светодиода калибровка
		//EVENT_REG|=BIT(ENA_DATA_ADC);  // Разрешить измерение АЦП
		
		if (Timer5min<400) Timer5min++;
		else{
			Timer5min=0;
			EVENT_REG|=BIT(Heating_End);
		}
		
		if (TimeOutExtMaster<2) TimeOutExtMaster++;
		else{
			EnaStandByInfo=0;
		}
	}
	//}*************************************************************************
	//============================================================//
	// Установка события нажатия на кнопку +
	//{===========================================================//
    if (PINE&BIT(But_plus)) {
		if (CountBut_plus>4) CountBut_plus=CountBut_plus-5;
	}
	else{
		if (CountBut_plus<0xFE) CountBut_plus=CountBut_plus+1;
	}
 
	if (ButtonReg&BIT(EnaBut_plus)){
		if (CountBut_plus>0x80){
			EVENT_REG|=BIT(But_plus); 
			ButtonReg&=~BIT(EnaBut_plus);
		}
	}
	else{
		if (CountBut_plus<0x40) {
			ButtonReg|=BIT(EnaBut_plus);
		}
	}
	//}===============================================================//
	//===========================================================// 
	// Установка события нажатия на кнопку -
	//{===========================================================//
	if (PINE&BIT(But_minus)) {
		if (CountBut_minus>4) CountBut_minus=CountBut_minus-5;
	}
	else{
		if (CountBut_minus<0xFE) CountBut_minus=CountBut_minus+1;
	}
 
	if (ButtonReg&BIT(EnaBut_minus)){
		if (CountBut_minus>0x80) {
			EVENT_REG|=BIT(But_minus); 
			ButtonReg&=~BIT(EnaBut_minus);
		}
	}
	else{
		if (CountBut_minus<0x40) {
			ButtonReg|=BIT(EnaBut_minus);
		}
	}
	//}====================================================================
	//===========================================================// 
	// Установка события нажатия на кнопку P
	//{===========================================================//
    if (PINE&BIT(But_prog)) {
		if (CountBut_prog>4) CountBut_prog=CountBut_prog-5;
	}
	else{
		if (CountBut_prog<0xFE) CountBut_prog=CountBut_prog+1;
	}
 
	if (ButtonReg&BIT(EnaBut_prog)){
		if (CountBut_prog>0x80) {
			EVENT_REG|=BIT(But_prog); 
			ButtonReg&=~BIT(EnaBut_prog);
		}
	}
	else{
		if (CountBut_prog<0x40) {
			ButtonReg|=BIT(EnaBut_prog);
		}
	}
	//}===========================================================// 
 
	//===========================================================// 
	// Установка разрешения посылки сообщения в сеть CAN (25 ms)
	//{===========================================================//
	if (TimerMsgCAN<25) TimerMsgCAN++;
	else{
		TimerMsgCAN=0;
		EVENT_REG|=BIT(SendMSG); // Разрешить посылку сообщения CAN
	}
	//}===========================================================//
}
/*===========================================================================
*	Пустая линия 
*============================================================================*/
void 
NopLine(void)
{
    Line2[0]=' ';
	Line2[1]=' ';
	Line2[2]=' '; 
    Line2[3]=0xB0;
	Line2[4]=0x43;
	Line2[5]=0x5C;
	Line2[6]=0x30;
}

/*============================================================================
*	Модуль signed int
*=============================================================================*/
uint16_t 
Mod16(int16_t idata)
{
	
	if (idata&0x8000){
		return ((uint16_t)((~(idata))+1));
	}
	else return ((uint16_t)idata);
}
/*=============================================================================
*	 С помощью этой функции индицируется обрыв провода
*==============================================================================*/
void 
ErrorLine(void)
{
    Line2[0]='-';
	Line2[1]='-';
	Line2[2]='-'; 
    Line2[3]=0x25;
	Line2[4]=0x20;
	Line2[5]=0x5C;
	Line2[6]=0x30;
}
/*======================================================================
*	Переводим число в символы для отображения на второй строке
*=======================================================================*/
void 
LoadDataInLine(signed int Zdata)
{
  unsigned int i=0;
  unsigned int data=0;
  unsigned char doli=0;
  unsigned char ed=0;
  unsigned char dec=0;
  unsigned char sot=0;
  unsigned char tis=0;
 if (Nscreen<17){
 //data=0x7fff&Zdata;

  data=Mod16(Zdata);
	if (Nscreen<10) {
		/*   C */
		Line2[3]=0xB0;
		Line2[4]=0x43;
		Line2[5]=0x5C;
		Line2[6]=0x30;
	}else if ((Nscreen>9)&&(Nscreen<16)){
		/* % */
		Line2[3]=0x25;
		Line2[4]=0x20;
		Line2[5]=0x5C;
		Line2[6]=0x30;
	}else if (Nscreen==16){
		/*  */
		Line2[3]=0xB0;
		Line2[4]=0x43;
		Line2[5]=0x5C;
		Line2[6]=0x30;
	}
	
	sot=data/1000;
	data=data-(sot*1000);
	dec=data/100;
	data=data-(dec*100);
	ed=data/10;
	doli=data-(ed*10);
	
	//data=0x7fff&Zdata;
	data=Mod16(Zdata);
  // Индикация положительных значений	
  //if (Zdata<0x800A)
	if ((Zdata&0x8000)==0){	
		if(data>999) {
			Line2[0]=sot|0x30; 
			Line2[1]=dec|0x30; 
			Line2[2]=ed|0x30;
		}else if ((data<1000) && (data>99)){ 
			Line2[0]=' '; 
			Line2[1]=dec|0x30; 
			Line2[2]=ed|0x30;
		}else if ((data<100) && (data>9)){ 
			Line2[0]=' '; 
			Line2[1]=' '; 
			Line2[2]=ed|0x30;
		}else if (((data>>8)==0)||((data>>8)==0x800))  { 
			Line2[0]=' '; 
			Line2[1]=' '; 
			Line2[2]=ed|0x30;
		}
	}	
	// Индикация отрицательных значений
	//else if (Zdata&0x8000){
	else{
  		if ((data<611) && (data>99)){ 
			Line2[0]='-'; 
			Line2[1]=dec|0x30; 
			Line2[2]=ed|0x30;
		}
		else if ((data<100)&& (data>9))  { 
			Line2[0]=' '; 
			Line2[1]='-'; 
			Line2[2]=ed|0x30;
		}
		else if ((data<10))  { 
			Line2[0]=' '; 
			Line2[1]=' '; 
			Line2[2]=ed|0x30;
		}
		else if(data>610){     // Отрицательное значение ниже допустимого
			Line2[0]='-'; 
			Line2[1]='-'; 
			Line2[2]='-';
		}
	}
}	
else 
  /*индикация кол-ва срабатываний контакторов и моточасов*/
  if ((Nscreen>16)&&(Nscreen<21))
  {
    data=Zdata;
    
    tis=data/10000;
	data=data-(tis*10000);
    sot=data/1000;
	data=data-(sot*1000);
	dec=data/100;
	data=data-(dec*100);
	ed=data/10;
	doli=data-(ed*10);
	
	Line2[5]=0x5C;
	Line2[6]=0x30;
	data=Zdata;
	if (data>9999)
	{
	    Line2[0]=tis|0x30;
		Line2[1]=sot|0x30;
		Line2[2]=dec|0x30;
		Line2[3]=ed|0x30;
		Line2[4]=doli|0x30;
	}
	else if ((data>999)&&(data<10000))
	{
	    Line2[0]=0x20;
		Line2[1]=sot|0x30;
		Line2[2]=dec|0x30;
		Line2[3]=ed|0x30;
		Line2[4]=doli|0x30;
	}
	else if ((data>99)&&(data<1000))
	{
	    Line2[0]=0x20;
		Line2[1]=0x20;
		Line2[2]=dec|0x30;
		Line2[3]=ed|0x30;
		Line2[4]=doli|0x30;
	}
	else if ((data>9)&&(data<100))
	{
	    Line2[0]=0x20;
		Line2[1]=0x20;
		Line2[2]=0x20;
		Line2[3]=ed|0x30;
		Line2[4]=doli|0x30;
	}
	else if (data<10)
	{
	    Line2[0]=0x20;
		Line2[1]=0x20;
		Line2[2]=0x20;
		Line2[3]=0x20;
		Line2[4]=doli|0x30;
	}
	else
	{
	    Line2[0]=0x20;
		Line2[1]='-';
		Line2[2]='-';
		Line2[3]='-';
		Line2[4]=0x20;
	}
  }
}

/*==========================================================================
*	В зависимости от № экрана загружаем в видеопамять необходимые данные
*==========================================================================*/
void SelectScreen(void)
{
CLI();
	switch (Nscreen){
		case 0:
		//T_in=T_DataADC[0];
			LoadDataInLine(T_in);
			// Запись в видео память Т_вн
			LEDGRAPH_Print_str(0,0, "Т_вр=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]); 
		break;
		case 1:
			T_out1=T_DataADC[Tout1_ch];
			LoadDataInLine(T_out1);
			// Запись в видео память Т_н1
			LEDGRAPH_Print_str(0,0, "Т_н1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 2:
			T_out2=T_DataADC[Tout2_ch];
			LoadDataInLine(T_out2);
			// Запись в видео память Т_н2
			LEDGRAPH_Print_str(0,0, "Т_н2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 3:
			T_prit1=T_DataADC[Tprit1_ch];
			LoadDataInLine(T_prit1);
			// Запись в видео память Т_п1
			LEDGRAPH_Print_str(0,0, "Т_п1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 4:
			T_prit2=T_DataADC[Tprit2_ch];
			LoadDataInLine(T_prit2);
			// Запись в видео память Т_п2
			LEDGRAPH_Print_str(0,0, "Т_п2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 5:
			T_rec1=T_DataADC[Trec1_ch];
			LoadDataInLine(T_rec1);
			// Запись в видео память Т_р1
			LEDGRAPH_Print_str(0,0, "Т_р1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 6:
			T_rec2=T_DataADC[Trec2_ch];
			LoadDataInLine(T_rec2);
			// Запись в видео память Т_р2
			LEDGRAPH_Print_str(0,0, "Т_р2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 7:
			T_in1=T_DataADC[Tin1_ch];
			LoadDataInLine(T_in1);
			// Запись в видео память Твн
			LEDGRAPH_Print_str(0,0, "Т_с1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
			break;
		case 8:
			T_in2=T_DataADC[Tin2_ch];
			LoadDataInLine(T_in2);
			// Запись в видео память Твн
			LEDGRAPH_Print_str(0,0, "Т_с2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 9:
			LoadDataInLine(LT_in);
			// Запись в видео память Туст
			LEDGRAPH_Print_str(0,0, "Т_уст\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 10:
			if (ErrorDamper&BIT(D_OUT1)) 
				ErrorLine();
			else                   
				LoadDataInLine(D_out1);
			// Запись в видео память З_н1
			LEDGRAPH_Print_str(0,0, "З_н1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 11:
			if (ErrorDamper&BIT(D_OUT2)) 
				ErrorLine();
			else 
				LoadDataInLine(D_out2);
			// Запись в видео память З_н2
			LEDGRAPH_Print_str(0,0, "З_н2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 12:
			if (ErrorDamper&BIT(D_POT1)) 
				ErrorLine();
			else 
				LoadDataInLine(D_pot1);
			// Запись в видео память З_п1
			LEDGRAPH_Print_str(0,0, "З_п1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 13:
			if (ErrorDamper&BIT(D_POT2)) 
				ErrorLine();
			else 
				LoadDataInLine(D_pot2);
			// Запись в видео память З_п2
			LEDGRAPH_Print_str(0,0, "З_п2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 14:
			if (ErrorDamper&BIT(D_REC1)) 
				ErrorLine();
			else 
				LoadDataInLine(D_rec1);
			// Запись в видео память З_р1
			LEDGRAPH_Print_str(0,0, "З_р1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 15:
			if (ErrorDamper&BIT(D_REC2)) 
				ErrorLine();
			else 
				LoadDataInLine(D_rec2);
			// Запись в видео память З_р2
			LEDGRAPH_Print_str(0,0, "З_р2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 16:
			if (Timer1sec>250) 
				LoadDataInLine(LT_in);
			else               
				NopLine();
			// Запись в видео память Туст
			LEDGRAPH_Print_str(0,0, "Т_уст\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 17:
			LoadDataInLine(CountVVK1);
			LEDGRAPH_Print_str(0,0, "ввк1=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
			break;
		case 18:
			LoadDataInLine(CountVVK2);
			LEDGRAPH_Print_str(0,0, "ввк2=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 19:
			LoadDataInLine((uint16_t)(moto_timeA/10));
			LEDGRAPH_Print_str(0,0, "Траб=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 20:
			LoadDataInLine((uint16_t)(moto_timeM/10));
			LEDGRAPH_Print_str(0,0, "Труч=\0");
			LEDGRAPH_Print_str(0,1, &Line2[0]);
		break;
		case 21:
			if (WORK_MODE&BIT(BLASTING)) {
			// Запись в видео память экрана ТЕСТ
				LEDGRAPH_Put_image( &LG_PIC_2[0][0] ); 
			}
			else Nscreen=0;
		break;
	}
SEI();	
} 

/*==========================================================================
*	Событие нажатия на кнопку
*===========================================================================*/
void 
OnClickButton(unsigned char button)
{
	TimerButtonClickOff=150;
	if (button==But_plus){
		if (Nscreen<15){
			Nscreen++;
		}
		else if (Nscreen==15){ 
			Nscreen=0;
		}
		else if (Nscreen==16){
			if (TEST_JAMP) {
				if (LT_in<400){
					LT_in = LT_in +10; 
					//T_USTAVKI=LT_in;
				}
			}
			else {
				if (LT_in<240){
					LT_in = LT_in +10; 
					//T_USTAVKI=LT_in;
				}
			} 	
		}
		else if ((Nscreen>=17)&&(Nscreen<=19)){
			Nscreen++;
		}
		else if (Nscreen==20) {
			Nscreen=0;
		}  
	}
	else if (button==But_minus){
		if ((Nscreen>0)&&(Nscreen<16)){
			Nscreen--;
		}
		else if (Nscreen==0){
			Nscreen=15;
		}
		else if (Nscreen==16){
			if (TEST_JAMP){
				if (LT_in>50) {
					LT_in = LT_in -10;
					//T_USTAVKI=LT_in;
				}
			}
			else{
				if (LT_in>140){
					LT_in = LT_in -10;
					//T_USTAVKI=LT_in;
				}
			}	 
		}
		else if (Nscreen==17) {
			Nscreen=0;
		}
		else if ((Nscreen>=18)&&(Nscreen<=20)){
			Nscreen--;
		}
	}
	else if (button==But_prog){
		if (Nscreen==9){
			Nscreen=16;
		}
		else if (Nscreen==16){
			Nscreen=9;
			// FRAM_WriteChar(CS_FRAM, 0x5, LT_in);
			EEPROM_WriteChar(ADDR_TUST,LT_in);
			T_USTAVKI=LT_in;
		} 
	}
	EVENT_REG&=~BIT(button);
}

/*============================================================================
*	В зависимости от ошибок выбираем номер экрана
*=============================================================================*/
void 
LoadScreenError(void)
{
	unsigned char i=0;
	QuantityError=0;
	for (i=0;i<16;i++) {
		if (AllErrorReg&BIT(i)){
			switch (i){
				case Tout1_ch:
					MassivScreenError[QuantityError]=1;
				break;
				case Tout2_ch:
					MassivScreenError[QuantityError]=2;
				break;
				case Tprit1_ch:
					MassivScreenError[QuantityError]=3;
				break;
				case Tprit2_ch:
					MassivScreenError[QuantityError]=4;
				break;
				case Trec1_ch:
					MassivScreenError[QuantityError]=5;
				break;
				case Trec2_ch:
					MassivScreenError[QuantityError]=6;
				break;
				case Tin1_ch:
					MassivScreenError[QuantityError]=7;
				break;
				case Tin2_ch:
					MassivScreenError[QuantityError]=8;
				break;
				case (D_OUT1+8):
					MassivScreenError[QuantityError]=10;
				break;
				case (D_OUT2+8):
					MassivScreenError[QuantityError]=11;//12;//11;
				break;
				case (D_POT1+8):
					MassivScreenError[QuantityError]=12;//14;//12;
				break;
				case (D_POT2+8):
					MassivScreenError[QuantityError]=13;//11;//13;
				break;
				case (D_REC1+8):
					MassivScreenError[QuantityError]=14;//13;//14;
				break;
				case (D_REC2+8):
					MassivScreenError[QuantityError]=15;
				break;
			}	
			QuantityError++;
		} 
	}
}
/*=============================================================================
*	Обработка продолжительного нажатия на кнопку "P"
*===============================================================================*/
void 
LochButP (void)
{
	if (Nscreen==0){
		if (ButtonReg&BIT(FlagButP)){
			Nscreen=17;
			TimerButP=0;
			ButtonReg&=~BIT(FlagButP);
		}
	} 
	else if (Nscreen==17){
		if (ButtonReg&BIT(FlagButP)){
			CountVVK1=0;
			EEPROM_WriteChar(ADDR_VVK1,(CountVVK1>>8));
			EEPROM_WriteChar(ADDR_VVK1+1,((uint8_t)CountVVK1));
			TimerButP=0;
			ButtonReg&=~BIT(FlagButP);
		}
	}
	else if (Nscreen==18){
		if (ButtonReg&BIT(FlagButP)){
			CountVVK2=0;
			EEPROM_WriteChar(ADDR_VVK2,(CountVVK2>>8));
			EEPROM_WriteChar(ADDR_VVK2+1,((uint8_t)CountVVK2));
			ButtonReg&=~BIT(FlagButP);
		}
	}
	else if (Nscreen==19){
		if (ButtonReg&BIT(FlagButP)){
			moto_timeA=0;
			Set_motoa_time(0);
			ButtonReg&=~BIT(FlagButP);
		}
	}
	else if (Nscreen==20){
		if (ButtonReg&BIT(FlagButP)){
			moto_timeM=0;
			Set_motom_time(0);
			ButtonReg&=~BIT(FlagButP);
		}
	}
	else ButtonReg&=~BIT(FlagButP);
}
/*===============================================================================
*	Чтение температурных показаний
*================================================================================*/
void 
ReadDataADC(void)
{
	unsigned char i=0;

		for(i=0;i<4;i++){
			if (EnaStandByInfo&BIT(SIMULATION)){
				T_DataADC[i]=SIM_T_DATA[i];
				T_DataADC[i+4]=SIM_T_DATA[i+4];
			}
			else{
				T_DataADC[i]=MSP_Recalc(0,i);
				T_DataADC[i+4]=MSP_Recalc(1,i);
			}
		}

}
/*================================================================================
*	Проверка валидности намерянных температур
*=================================================================================*/
void 
CheckDataADC(void)
{
	uint8_t i=0;
	for (i=0;i<8;i++){
       if ((T_DataADC[i]<-610)||(T_DataADC[i]>1200)) ErrorReg|=BIT(i);
	   else                           ErrorReg&=~BIT(i);
	}
}

/*Отправляем КАН сообщения*/
void 
SendCANPacketMessage(void)
{
	if (EVENT_REG&BIT(SendMSG)){
	    if (CounterID==0){
			/*похерил напроч*/
			CounterID=CounterID+1;
			EVENT_REG&=~BIT(SendMSG);
		}
		else if (CounterID==1){
			Out_msg.ID=0x101;     
			Out_msg.DLEN=8;
			Out_msg.data[0]=pageID_101;	
			switch (pageID_101){
				case 0:
					Out_msg.data[1]=msp_data[Tout1_ch]>>8;
					Out_msg.data[2]=msp_data[Tout1_ch];
					Out_msg.data[3]=T_DataADC[Tout1_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tout1_ch];
					Out_msg.data[5]=MT_DataADC[Tout1_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tout1_ch];
					Out_msg.data[7]=NLoh;//
					pageID_101++;
				break; 
				case 1:
					Out_msg.data[1]=msp_data[Tprit1_ch]>>8;
					Out_msg.data[2]=msp_data[Tprit1_ch];
					Out_msg.data[3]=T_DataADC[Tprit1_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tprit1_ch];
					Out_msg.data[5]=MT_DataADC[Tprit1_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tprit1_ch];
					pageID_101++;
				break;
				case 2:
					Out_msg.data[1]=msp_data[Trec1_ch]>>8;
					Out_msg.data[2]=msp_data[Trec1_ch];
					Out_msg.data[3]=T_DataADC[Trec1_ch]>>8;
					Out_msg.data[4]=T_DataADC[Trec1_ch];
					Out_msg.data[5]=MT_DataADC[Trec1_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Trec1_ch];
					pageID_101++;
				break;
				case 3:
					Out_msg.data[1]=msp_data[Tin1_ch]>>8;
					Out_msg.data[2]=msp_data[Tin1_ch];
					Out_msg.data[3]=T_DataADC[Tin1_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tin1_ch];
					Out_msg.data[5]=MT_DataADC[Tin1_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tin1_ch];
					pageID_101=0;
				break;
			}  
			CAN_Msg_Send(CAN_PARAMS,&Out_msg);
			CounterID=CounterID+1;
			EVENT_REG&=~BIT(SendMSG);
		}
		else if (CounterID==2){
			Out_msg.ID=0x0102;     
			Out_msg.DLEN=7;
			Out_msg.data[0]=pageID_102;	
			switch (pageID_102){
				case 0:
					Out_msg.data[1]=msp_data[Tout2_ch]>>8;
					Out_msg.data[2]=msp_data[Tout2_ch];
					Out_msg.data[3]=T_DataADC[Tout2_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tout2_ch];
					Out_msg.data[5]=MT_DataADC[Tout2_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tout2_ch];
					pageID_102++;
				break; 
				case 1:
					Out_msg.data[1]=msp_data[Tprit2_ch]>>8;
					Out_msg.data[2]=msp_data[Tprit2_ch];
					Out_msg.data[3]=T_DataADC[Tprit2_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tprit2_ch];
					Out_msg.data[5]=MT_DataADC[Tprit2_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tprit2_ch];
					pageID_102++;
				break;
				case 2:
					Out_msg.data[1]=msp_data[Trec2_ch]>>8;
					Out_msg.data[2]=msp_data[Trec2_ch];
					Out_msg.data[3]=T_DataADC[Trec2_ch]>>8;
					Out_msg.data[4]=T_DataADC[Trec2_ch];
					Out_msg.data[5]=MT_DataADC[Trec2_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Trec2_ch];
					pageID_102++;
				break;
				case 3:
					Out_msg.data[1]=msp_data[Tin2_ch]>>8;
					Out_msg.data[2]=msp_data[Tin2_ch];
					Out_msg.data[3]=T_DataADC[Tin2_ch]>>8;
					Out_msg.data[4]=T_DataADC[Tin2_ch];
					Out_msg.data[5]=MT_DataADC[Tin2_ch]>>8;
					Out_msg.data[6]=MT_DataADC[Tin2_ch];
					pageID_102=0;
				break;
			}  
			CAN_Msg_Send(CAN_PARAMS,&Out_msg);
			CounterID=CounterID+1;
			EVENT_REG&=~BIT(SendMSG);
		}	
		else if (CounterID==3){
			Out_msg.ID=0x103;     
			Out_msg.DLEN=8;
			Out_msg.data[0]=pageID_103;	
			DataPU[0]=(int)DamperDataOUT[D_OUT1];
			DataPU[1]=(int)DamperDataOUT[D_OUT2];
			DataPU[2]=(int)DamperDataOUT[D_POT1];
			DataPU[3]=(int)DamperDataOUT[D_POT2];
			DataPU[4]=(int)DamperDataOUT[D_REC1];
			DataPU[5]=(int)DamperDataOUT[D_REC2];
			   
			DataPU[6]=(int)DamperDataIN[D_OUT1];
			DataPU[7]=(int)DamperDataIN[D_OUT2];
			DataPU[8]=(int)DamperDataIN[D_POT1];
			DataPU[9]=(int)DamperDataIN[D_POT2];
			DataPU[10]=(int)DamperDataIN[D_REC1];
			   //DataPU[11]=(int)DamperDataIN[D_REC1];
			DataPU[11]=(int)DamperDataIN[D_REC2];
			switch (pageID_103){
				case 0:
					Out_msg.data[1]=DataPU[0]>>8;
					Out_msg.data[2]=DataPU[0];
					Out_msg.data[3]=DataPU[1]>>8;
					Out_msg.data[4]=DataPU[1];
					Out_msg.data[5]=DataPU[2]>>8;
					Out_msg.data[6]=DataPU[2];
					pageID_103++;
				break; 
				case 1:
					Out_msg.data[1]=DataPU[0]>>8;
					Out_msg.data[2]=DataPU[0];
					Out_msg.data[3]=DataPU[2]>>8;
					Out_msg.data[4]=DataPU[2];
					Out_msg.data[5]=DataPU[4]>>8;
					Out_msg.data[6]=DataPU[4];
					pageID_103++;
				break;
				case 2:
					Out_msg.data[1]=DataPU[6]>>8;
					Out_msg.data[2]=DataPU[6];
					Out_msg.data[3]=DataPU[8]>>8;
					Out_msg.data[4]=DataPU[8];
					Out_msg.data[5]=DataPU[10]>>8;
					Out_msg.data[6]=DataPU[10];
					pageID_103++;
				break;
				case 3:
					Out_msg.data[1]=DataPU[1]>>8;
					Out_msg.data[2]=DataPU[1];
					Out_msg.data[3]=DataPU[3]>>8;
					Out_msg.data[4]=DataPU[3];
					Out_msg.data[5]=DataPU[5]>>8;
					Out_msg.data[6]=DataPU[5];
					pageID_103++;
				break;
				case 4:
					Out_msg.data[1]=DataPU[7]>>8;
					Out_msg.data[2]=DataPU[7];
					Out_msg.data[3]=DataPU[9]>>8;
					Out_msg.data[4]=DataPU[9];
					Out_msg.data[5]=DataPU[11]>>8;
					Out_msg.data[6]=DataPU[11];
					Out_msg.data[7]=ALARM_REG;
					pageID_103++;
				break;
				case 5:
					Out_msg.data[1]=LT_in;//FRAM_ReadChar(CS_FRAM, 0x5);//LT_in;
					Out_msg.data[2]=DataIN;
					Out_msg.data[3]=DataLED;
					Out_msg.data[4]=DataOUT;
					Out_msg.data[5]=WORK_MODE;
					Out_msg.data[6]=STEP_COUNT;
					Out_msg.data[7]=STEP_PRIT;
					pageID_103++;
				break;
				case 6:
					Out_msg.data[1]=T_in>>8;
					Out_msg.data[2]=T_in;
					Out_msg.data[3]=Timer_smeny_loh_up;//Flag_SpeedDTV;
					Out_msg.data[4]=Timer_smeny_loh_down;//workmin;
					Out_msg.data[5]=SPEEDDTV>>8;
					Out_msg.data[6]=SPEEDDTV;
					Out_msg.data[7]=mayup+ (maydown<<4);
					pageID_103=0;
				break;
			}  
			CAN_Msg_Send(CAN_PARAMS,&Out_msg);
			CounterID=0;
			EVENT_REG&=~BIT(SendMSG);
		} 
    }
}
/*=====================================================================
*	Принимаем и разбираем КАН сообщения
*======================================================================*/
void 
ReciveCAN (void)
{   
	LED_RUNN_OFF;
	// Сброс счетчика внешнего мастера
	TimeOutExtMaster=0; 
		if (In_msg.ID==0x400){
		             
			EnaStandByInfo=In_msg.data[0];

		}else if (In_msg.ID==0x401){
			SIM_T_DATA[Tout1_ch]=In_msg.data[0];
			SIM_T_DATA[Tout1_ch]=SIM_T_DATA[Tout1_ch]<<8;
			SIM_T_DATA[Tout1_ch]+=In_msg.data[1];
			SIM_T_DATA[Tprit1_ch]=In_msg.data[2];
			SIM_T_DATA[Tprit1_ch]=SIM_T_DATA[Tprit1_ch]<<8;
			SIM_T_DATA[Tprit1_ch]+=In_msg.data[3];
			SIM_T_DATA[Trec1_ch]=In_msg.data[4];
			SIM_T_DATA[Trec1_ch]=SIM_T_DATA[Trec1_ch]<<8;
			SIM_T_DATA[Trec1_ch]+=In_msg.data[5];
			SIM_T_DATA[Tout2_ch]=In_msg.data[6];
			SIM_T_DATA[Tout2_ch]=SIM_T_DATA[Tout2_ch]<<8;
			SIM_T_DATA[Tout2_ch]+=In_msg.data[7];
		}	
		else if (In_msg.ID==0x402){
			SIM_T_DATA[Tprit2_ch]=In_msg.data[0];
			SIM_T_DATA[Tprit2_ch]=SIM_T_DATA[Tprit2_ch]<<8;
			SIM_T_DATA[Tprit2_ch]+=In_msg.data[1];
			SIM_T_DATA[Trec2_ch]=In_msg.data[2];
			SIM_T_DATA[Trec2_ch]=SIM_T_DATA[Trec2_ch]<<8;
			SIM_T_DATA[Trec2_ch]+=In_msg.data[3];
			SIM_T_DATA[Tin1_ch]=In_msg.data[4];
			SIM_T_DATA[Tin1_ch]=SIM_T_DATA[Tin1_ch]<<8;
			SIM_T_DATA[Tin1_ch]+=In_msg.data[5];
			SIM_T_DATA[Tin2_ch]=In_msg.data[6];
			SIM_T_DATA[Tin2_ch]=SIM_T_DATA[Tin2_ch]<<8;
			SIM_T_DATA[Tin2_ch]+=In_msg.data[7];	   
		}
	LED_RUNN_ON;
}

/*=======================================================================
*  Считываем кнопки 
*========================================================================*/
void
ReadDataDIN(void)
{
	/*Кнопки*/
	if (DBUT1) 	DataIN|=BIT(0);
	else		DataIN&=~BIT(0) ;
	
	if (DBUT2) 	DataIN|=BIT(1);
	else		DataIN&=~BIT(1) ;
}

/*============================================================================
*	Чтение чего там намерялось по  измерительным каналам заслонок
*=============================================================================*/
void
Read_DamperIN(void)
{
	uint8_t ch=0;
	
	for (ch=0x00;ch<6;ch++)
		DamperDataIN[ch]=(uint8_t)MSP_Recalc(2,ch);
	
}
/*==============================================================================
*	Выставляет управляющие сигналы для заслонок.
*===============================================================================*/
void
Write_DamperOUT(void)
{	static uint8_t look_up_tab[]={1,0,3,2,4,5};
	uint8_t ch=0;
	for (ch=0x00;ch<6;ch++)
		//DAC_Data[ch]=MSP_DAC_Recalc(DamperDataOUT[ch],ch);
		DAC_Data[look_up_tab[ch]]=MSP_DAC_Recalc(DamperDataOUT[ch],look_up_tab[ch]);
}


/*==============================================================================
*	Функция реализации пилы на выходе ЦАПов
*================================================================================*/
void 
Saw(void)
{
	uint8_t nChannel;
	/* временная переменная, для хранения текущего значения для ЦАПа */
	uint8_t DAC_curval;
	/*необходимая начальная инициализация - сброс всех счётчиков
	/*	отправлять с нулевого (то биж первого) бита	*/
	DAC_STATUS.current_bit=0;
	/*	текущий канал  - нулевой	*/
	DAC_STATUS.current_channel=0;
	Pbit=0;
	DAC_curval=0;
	while(1){
		/*	для каждого канала МСП...	*/
		for (nChannel=0;nChannel<COUNT_CHANNELS;nChannel++){
			/*послать!*/
			//MSP_DAC_Send_One_Package(nChannel,DAC_curval);
			DAC_Data[nChannel]=DAC_curval;
				DAC_STATUS.current_channel=nChannel;
				while (DAC_STATUS.current_channel==nChannel){
					WDR();
					MSP_DAC_WORK();
				}
		}

		DAC_curval+=10;
		if (DAC_curval>0xC0) DAC_curval=0x00;
		//timer0_Delay(1);
	}
}

/*=====================================================================================
*	функция стирания калибровочных данных
*=======================================================================================*/
void 
Clear_Calibrate(void)
{
	uint16_t i;
	uint16_t adr;
	/*стираем данные для первой калибровочной точки*/
	adr=STARTING_ADDRES_P1;
	for (i=0;i<BUFLEN;i++){
		EEPROM_WriteChar(adr++,0xFF);
		EEPROM_WriteChar(adr++,0xFF);
	}
	/*стираем чекпоинт для первой калибровочной точки*/
	EEPROM_WriteChar(adr++,0xFF);
	EEPROM_WriteChar(adr,0xFF);
	/*стираем данные для второй калибровочной точки*/
	adr=STARTING_ADDRES_P2;
	for (i=0;i<BUFLEN;i++){
		EEPROM_WriteChar(adr++,0xFF);
		EEPROM_WriteChar(adr++,0xFF);
	}
	
	/*стираем чекпоинт для второй калибровочной точки*/
	EEPROM_WriteChar(adr++,0xFF);
	EEPROM_WriteChar(adr,0xFF);
	
	/* стираем калибровочные данные для ЦАПов*/
	adr=STARTING_ADDRES_DP;
	for(i=0x00;i<COUNT_CHANNELS;i++){
		EEPROM_WriteChar(adr++,0xFF);
	}
	/*  clear старший байт check point */
	EEPROM_WriteChar(STARTING_ADDRES_DP+COUNT_CHANNELS,0xFF);
	/* clear  младший байт check point */
	EEPROM_WriteChar(STARTING_ADDRES_DP+COUNT_CHANNELS+1,0xFF);
}

/*=====================================================================================
*	Инициализация CAN
*=====================================================================================*/
void 
CAN_SetUp(void)
{
	CAN_Init(CAN_PARAMS, CAN_BAUD_500);  
	/*Принимаем всё*/
	CAN_setAcceptFilter(CAN_PARAMS, 1, 0x7FF);
	CAN_setAcceptMask(CAN_PARAMS,0,0x0000);
}

/*=====================================================================================
*	Инициализация периферии
*=====================================================================================*/
void
Init_Periphery(void)
{
	port_init(); 
	timer0_init();
	timer1_init();
	/*Предстартовая задержка 3 сек*/ 
	timer0_Delay(3000);
	MSP_DAC_Init(); 
	timer0_Delay(500);
	SPI_masterInit(); 
	CAN_SetUp();
	USART_Init(0, 2400, 8, USART_PARITY_NONE, 1); 

	LED_ERR_ON;
	MSP_Init();
	LED_ERR_OFF; 
	LEDGRAPH_Init(5, 2);
	WDT_Set(WDT_2000ms);
}


/* =====================================================================================
*	Работа с калибровкой
*=====================================================================================*/
void 
Do_Load_Or_Clear_Calibrate(void)
{
	if (J4){
		Clear_Calibrate();
	}	
	/*Калибруемся по первой точке*/
	if (J1){
		flag_cal=START_CAL;
		if (MSP_Calibrate(STARTING_ADDRES_P1,POINT_T1)==0 ){
			flag_cal=END_CAL;
			while(1) WDR();
		}
		else {
			flag_cal=OUT_CAL;
			while(1) WDR();
		}
	}
	/*Калибруемся по второй точке*/
	if (J2){
		flag_cal=START_CAL;
		if (MSP_Calibrate(STARTING_ADDRES_P2,POINT_T2)==0 ){
			flag_cal=END_CAL;
			while(1)WDR();
		}
		else {
			flag_cal=OUT_CAL;
			while(1)WDR();
		}
	}
	/*Загрузка калибровочных данных для  МСП*/
	if (MSP_Read_CAlibrate(STARTING_ADDRES_P1,POINT_T1)==TRUE){
		
	}
	else{
		flag_cal=OUT_CAL;
		while(1)WDR();
	}
	if(MSP_Read_CAlibrate(STARTING_ADDRES_P2,POINT_T2)==TRUE){
	
	}
	else {
		flag_cal=OUT_CAL;
		while(1)WDR();
	}
  
	/*калибровка ЦАПов*/  
	if (J3){
		flag_cal=START_CAL;
		m_ts=timer0_getCounter();
		while(time_interval(m_ts)<=10000){
			MSP_Refresh();
			MSP_DAC_WORK();
			WDR();
		}
		if (MSP_DAC_Calibration(STARTING_ADDRES_DP)==0 ){
			flag_cal=END_CAL;
			while(1)WDR();
		}
		else {
			flag_cal=OUT_CAL;
			while(1)WDR();
		}
	}
	/*Загрузка калибровки ЦАПов*/
	if(MSP_DAC_Load_Calibration(STARTING_ADDRES_DP)==TRUE){
		flag_cal=ENABLE_CAL;
	}
	else {
		flag_cal=OUT_CAL;
		while(1)WDR();
	}
}

/*=====================================================================================
*	Загрузка значений срабатываний контакт.
*=====================================================================================*/
void
Load_Count_VVK(void)
{
	CountVVK1=EEPROM_ReadChar(ADDR_VVK1);
	CountVVK1=CountVVK1<<8;
	CountVVK1=CountVVK1|EEPROM_ReadChar(ADDR_VVK1+1);
	/*если было пусто*/
	if (CountVVK1==0xFFFF) {
		/*обнуляем и сохраняем*/
		CountVVK1=0x0000;
		EEPROM_WriteChar(ADDR_VVK1,(CountVVK1>>8));
		EEPROM_WriteChar(ADDR_VVK1+1,((uint8_t)CountVVK1));
	}
	CountVVK2=EEPROM_ReadChar(ADDR_VVK2);
	CountVVK2=CountVVK2<<8;
	CountVVK2=CountVVK2|EEPROM_ReadChar(ADDR_VVK2+1);
	/*если было пусто*/
	if (CountVVK2==0xFFFF) {
		/*обнуляем и сохраняем*/
		CountVVK2=0x0000;
		EEPROM_WriteChar(ADDR_VVK2,(CountVVK2>>8));
		EEPROM_WriteChar(ADDR_VVK2+1,((uint8_t)CountVVK2));
	}

}

/*=====================================================================================
* Загрузка моточасов из памяти
*=====================================================================================*/
void
LoadMotoTime(void)
{
	/*	Загружаем общее кол-во моточасов*/
	moto_timeA=	(EEPROM_ReadChar(ADDR_MOTOA)<<24)
				|(EEPROM_ReadChar(ADDR_MOTOA+1)<<16)
				|(EEPROM_ReadChar(ADDR_MOTOA+2)<<8)
				|EEPROM_ReadChar(ADDR_MOTOA+3);
	
	/*если в епромине небыло нихрена - обнуляем*/
	if (moto_timeA==0xFFFFFFFF){
		moto_timeA=0x00;
		Set_motoa_time(0);
	}

	/*	Загружаем кол-во моточасов работы в ручном режиме*/
	moto_timeM=	(EEPROM_ReadChar(ADDR_MOTOM)<<24)
				|(EEPROM_ReadChar(ADDR_MOTOM+1)<<16)
				|(EEPROM_ReadChar(ADDR_MOTOM+2)<<8)
				|EEPROM_ReadChar(ADDR_MOTOM+3);
	
	/*если в епромине небыло нихрена - обнуляем*/
	if (moto_timeM==0xFFFFFFFF){
		moto_timeM=0x00;
		Set_motom_time(0);
	}

}

void 
flash_err(uint8_t num_flash)
{	uint16_t er_ts;
	uint8_t i=0;
	while(1){
		for(i=0;i<num_flash;i++){
			LED_ERR_ON;
			timer0_Delay(100);
			LED_ERR_OFF;
			timer0_Delay(100);
		}
		timer0_Delay(1000);
	}
}
/*******************************************************************************************************
*
*
*			main
*
**********************************************************************************************************/
void 
main(void)
{	uint8_t i=0;
	uint8_t TEMP=0;
	uint8_t fe=0;
	uint8_t j=0;
	/*Инициализация всякой приблуды типа кана, уарта, МСПшек и пр.*/
	Init_Periphery();
	/* посмотрим, что там с калибровкой*/
	Do_Load_Or_Clear_Calibrate();
	LED_RUNN_ON; 
	/*Пила*/
	if (J5){
		Saw();
	}
 
/*********************************************************************/
	  // Загружаем или считываем значение уставки
	LT_in=EEPROM_ReadChar(ADDR_TUST);//160;//FRAM_ReadChar(CS_FRAM, 0x5);
	TEMP=(LT_in/10);
	LT_in=TEMP*10;
	if ((LT_in<140)||(LT_in>240)) {
		LT_in=160;  // 16,0 градусов
		EEPROM_WriteChar(ADDR_TUST,LT_in);
	}
	T_USTAVKI=LT_in;
/**********************************************************************/
	/*Загружаем значение срабатываний контакторами*/
	Load_Count_VVK();
	/*Загружаем моточасы*/
	LoadMotoTime();
 /*forever loop*/
	while(1){
		WDR();
		MSP_DAC_WORK();
		fe=MSP_Refresh();
		/*смотрим ошибки*/
 		if (fe>0) {
			/*если есть, начинаем разбирать*/
			if (MSP[fe-1].type==TEMPERATURE_SENSOR){
				for(j=0;j<MSP[fe-1].count_chanels;j++){
					MSP[fe-1].data[j]=0xFFFF;
				}
			}
			else {
				for(j=0;j<MSP[fe-1].count_chanels;j++){
					MSP[fe-1].data[j]=MSP[fe-1].cal_data_p1[j];
				}
			}
		}
		
		/*DEBUG*/
		//while(deb_msg_send()) WDR();
        /******************/
		AllErrorReg=ErrorDamper&0x3F;
		AllErrorReg=AllErrorReg<<8;
		AllErrorReg=AllErrorReg+(ErrorReg&0xFF);
		// Присвоение значений положений заслонок
		D_out1=Tdamp[D_OUT1]*10;   //  Умножение на 10 необходимо т.к.
		D_out2=Tdamp[D_OUT2]*10;   //  функция преобразования числовых данных  
		D_pot1=Tdamp[D_POT1]*10;   //  для дисплея имеет 4 десятичных разряда, 
		D_pot2=Tdamp[D_POT2]*10;   //  где младший первый знак после запятой,
		D_rec1=Tdamp[D_REC1]*10;   //  а DamperDataIN является целым числом в %
		D_rec2=Tdamp[D_REC2]*10;   //
		T_in=T_INSIDE;
		if (EVENT_REG&BIT(But_plus))  OnClickButton(But_plus);   // Обработка события нажатия кнопки
		if (EVENT_REG&BIT(But_minus)) OnClickButton(But_minus);  // Обработка события нажатия кнопки
		if (EVENT_REG&BIT(But_prog))  OnClickButton(But_prog);   // Обработка события нажатия кнопки
		ReadDataDIN();
		LochButP();
		Read_DamperIN();
		Write_DamperOUT();
		ReadDataADC();	                        // Цикличное чтение данных АЦП
		CheckDataADC();                          // Проверка АЦП на достоверность
		LoadScreenError();                       // Формирование массива ошибок
		OutLed(DataLED);
		SelectScreen();                          // Обновление данных для вывода на дисплей
		/*Моточасы*/
		if(flag6min){	
			flag6min=0x00;
			/*inc moto_timeA*/
			Set_motoa_time(1);
			/*try inc moto_timeM*/
			Set_motom_time(1);
		}
		if (CAN_msg_Read(CAN_PARAMS, &In_msg)){
			ReciveCAN ();
		}
		SendCANPacketMessage();                 // отправка сообщения в сеть CAN
		/* Работаем по алгоритму*/
		WORK_DUBLE();
	}
}