// *****************************************************************************
// Драйвер встроенного АЦП ( AT128)
// -----------------------------------------------------------------------------
// v.1.0 (05.10.2007) AT128
// *****************************************************************************

#include "atadc.h"

#include <PLATFORM.h>
#include <macros.h>
#include <types.h>
#include "main.h"
#ifdef MCU_ATMEGA128    
 #define MCU_SET_OK
#endif

#ifndef MCU_SET_OK
 #error Драйвер не поддерживает работу с этим MCU!
#endif

// ===================================
// D A T A
// ===================================

uint16_t  atadcval = 0;
uint8_t  atadcch = 0;

// ===================================
// P R O C E D U R E S
// ===================================

// -----------------------------------
// Возвращает текущее значение АЦП
// -----------------------------------
uint16_t  
ATADC_GetValue(void)
{
  uint16_t  tmp;
  CLI();
  tmp = atadcval;  
  SEI();
  return tmp;
}


// -----------------------------------
// ADC initialize
// -----------------------------------
void 
ATADC_Init(uint8_t  channel)
{
    
  // Conversion time: 13uS
  ADCSRA = 0x00;   // disable adc
  ADMUX = channel; // select adc input 0-7
  ACSR  = 0x80;
  ADCSRA = 0xEC;
}

#pragma interrupt_handler ATADC_isr:iv_ADC
void 
ATADC_isr(void)
{
  //conversion complete, read value (int) using...
  // 0 - 1023
  atadcval = ADCL;            //Read 8 low bits first (important)
  atadcval |= (uint16_t)ADCH << 8; //read 2 high bits and shift into top byte
  

}

/*Функция остановки измерения АЦП*/
void
ATADC_Stop(void)
{
 atadcval=0x00;
 ADCSRA = 0x00;   // disable adc
// ACSR=0x00;
}

/*Функуция запуска измерения АЦП*/
void 
ATADC_Start(void)
{
   ADCSRA = 0xEC;
}