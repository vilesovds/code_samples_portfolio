
 
#include "pwm.h"
#include <types.h>
#include <PLATFORM.h>

#include <macros.h>

#include "atadc.h"
#ifdef ONE_PULSE_MEASUREMENT
	#include "main.h"
#endif
// MCU Atmega 162
#ifdef MCU_ATMEGA162
	#define MCU_SET_OK
#endif
// MCU Atmega 128
#ifdef MCU_ATMEGA128
	#define MCU_SET_OK
#endif

#ifndef MCU_SET_OK
 #error Заданный MCU не поддерживается этой версией драйвера! 
#endif

// ===============================================================================
// CONSTANT
// ===============================================================================

// desired value: 100 Hz
// actual value:  100.000 Hz (0.0%)


//#define CNT_FREQ 0xB1E0

// 6 ms
//#define CNT_FREQ 0xD120
// 4ms
//#define CNT_FREQ 0xE0C0
//2 ms
//#define CNT_FREQ 0xF060
//1 ms
#define CNT_FREQ 0xF830

// ===============================================================================
// DATA
// ===============================================================================

unsigned char pwm_port_addr, pwm_pin; 

/*Разрешение для измерения тока*/
//extern en_Imez;

unsigned int div=0;
uint16_t adc_buf=0;
uint16_t tmp_buf=0;
uint8_t i=0;
// ===============================================================================
// FUNCTIONS
// ===============================================================================

// ----------------------
// Запуск ШИМ модулятора
// ----------------------
// high/low divider ( % соотношение длительности высокого уровня к длительности низкого)
// в reg:pin выдаются импульсы
void PWM_Start(char reg, char pin, unsigned char hldiv)
{ 
  
  // преобразуем и запоминаем буквенные регистры в адреса для скорости
  pwm_port_addr = get_port_addr(reg);
  pwm_pin = BIT(pin);
  pwm_port_addr &= ~pwm_pin;
 
// TIMER3 initialize - 
// WGM: 0) Normal, TOP=0xFFFF

 TCCR3B= 0x00;   // stop timer

 ETIMSK |= (BIT(TOIE3) | BIT(OCIE3A)); // прерывания от T3 ovf и T3 counter A
 
 TCNT3 = CNT_FREQ; // Freq setup (стартовое значение счетчика, растет до FFFF, потом OVRFL INT) 
 OCR3A = ((((long)0xFFFF - (long)CNT_FREQ) * (long)hldiv)) / 100  + CNT_FREQ;
 div=OCR3A;
/* OCR3B = 0;
#ifdef MCU_ATMEGA128    
 OCR3C = 0;
 ICR3  = 0;
#endif
 TCCR3A= 0x00;   
 TCCR3B= 0x02;   // start Timer (prescale:8)
*/
 
 
 OCR3B = 0x00;

 OCR3C = 0x00;
 ICR3  = 0x0;
 
 TCCR3A = 0x00;
 TCCR3B = 0x02; //start Timer(prescale:8)

 
 
//TIMER3 initialize - prescale:1
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 10KHz
// actual value: 10,000KHz (0,0%)
/*
 
 TCNT3H = 0xF9; //setup
 TCNT3L = 0xC0;
 
 OCR3AH = 0x06;
 OCR3AL = 0x40;
 
 OCR3BH = 0x06;
 OCR3BL = 0x40;
 OCR3CH = 0x06;
 OCR3CL = 0x40;
 ICR3H  = 0x06;
 ICR3L  = 0x40;
 TCCR3A = 0x00;
 TCCR3B = 0x01; //start Timer
 */
}





/**
*   Функция динамического изменения  high/low divider 
*   ( % соотношение длительности высокого уровня к длительности низкого)
*/
void 
PWM_Set_Div(char reg, char pin,char hldiv)
{
	if(pwm_pin!=BIT(pin)){
		// На всяк случай выключаем старый пин
		pwm_port_addr &= ~pwm_pin;
		pwm_pin = BIT(pin);
	}
	// преобразуем и запоминаем буквенные регистры в адреса для скорости
	if(pwm_port_addr!=reg)  pwm_port_addr = get_port_addr(reg);
	div= ((((long)0xFFFF - (long)CNT_FREQ) * (long)hldiv)) / 100  + CNT_FREQ;
	//и если шим был остановлен по каким-либо причинам, то восстанавливаем таймер
	TCCR3B= 0x02;   // start Timer (prescale:8)
}


// ----------------------
// Останов ШИМ модулятора
// ----------------------
void PWM_Stop(void)
{
*(volatile unsigned char *) pwm_port_addr &= ~pwm_pin;
 TCCR3B= 0x00;   // stop timer
}

#ifdef ONE_PULSE_MEASUREMENT
// Функция усреднения значения тока
void
buf_i(void)
{
	i++;
	if (i>=5){
		i=0;
		adc_buf=tmp_buf;
		tmp_buf=0;
	}
	else	tmp_buf+=(ATADC_GetValue()>>2);

}
#endif

#pragma interrupt_handler timer3_compa_isr:iv_TIMER3_COMPA
void timer3_compa_isr(void)
{
	if (OCR3A!=0xFFFF){
		*(volatile unsigned char *)pwm_port_addr &= ~pwm_pin;
		#ifdef ONE_PULSE_MEASUREMENT
			buf_i();
		#endif
	}
	else 
		*(volatile unsigned char *)pwm_port_addr |= pwm_pin;
	if(OCR3A!=div) OCR3A=div;
}

#pragma interrupt_handler timer3_ovf_isr:iv_TIMER3_OVF
void timer3_ovf_isr(void)
{
 //TIMER3 has overflowed
	TCNT3 = CNT_FREQ; // Freq setup (стартовое значение счетчика
	if (OCR3A!=CNT_FREQ){  
		*(volatile unsigned char *)pwm_port_addr |= pwm_pin;
	}
	else{ 
		*(volatile unsigned char *)pwm_port_addr &= ~pwm_pin;
		#ifdef ONE_PULSE_MEASUREMENT
			buf_i();
		#endif
	}
	if(OCR3A!=div) OCR3A=div;
}