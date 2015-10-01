// ===================================
// Драйвер сдвигового регистра 74HC595
// оптимизирован по скорости дял платы вадима
// ===================================
#ifndef _SHREGF_H
#define _SHREGF_H

#include "PLATFORM.h"
#include <macros.h>

// ------------------------
// Пины сдвигового регистра
// настраиваются индивидуально по платам
// ------------------------

#define DS_REG PORTD  
#define DS_PIN 4

#define MR_REG PORTD  
#define MR_PIN 0

#define OE_REG PORTD  
#define OE_PIN 1

#define STCP_REG PORTD  
#define STCP_PIN 2

#define SHCP_REG PORTD  
#define SHCP_PIN 3

// ---------------------
// Константы управления
// ---------------------

#define SHREG_DS_SET_HIGH DS_REG |= BIT(DS_PIN)  
#define SHREG_DS_SET_LOW  DS_REG &= ~BIT(DS_PIN)

#define SHREG_MR_SET_HIGH MR_REG |= BIT(MR_PIN)  
#define SHREG_MR_SET_LOW  MR_REG &= ~BIT(MR_PIN)

#define SHREG_OE_SET_HIGH OE_REG |= BIT(OE_PIN)  
#define SHREG_OE_SET_LOW  OE_REG &= ~BIT(OE_PIN)

#define SHREG_STCP_SET_HIGH STCP_REG |= BIT(STCP_PIN)  
#define SHREG_STCP_SET_LOW  STCP_REG &= ~BIT(STCP_PIN)

#define SHREG_SHCP_SET_HIGH SHCP_REG |= BIT(SHCP_PIN)  
#define SHREG_SHCP_SET_LOW  SHCP_REG &= ~BIT(SHCP_PIN)

// -----------------------------
// Обнуление сдвигового регистра
// -----------------------------
void Shift_reg_reset(void);

// --------------------------------------------------
// Вывод данных из сдвигового регистра в параллельный
// --------------------------------------------------
void Shift_reg_store(void);

// ---------------------------------------
// Запись данных в сдвиговый регистр
// data - указатель на массив
// len - длина в байтах
// ---------------------------------------
void Shift_reg_write_data(char * data, int len);

// ---------------------------------------
// Запись 1 байта в сдвиговый регистр
// ---------------------------------------
void Shift_reg_write_byte(char data);

// ---------------------------------------
// Подготовка сдвиг регистра к записи
// ---------------------------------------
void Shift_reg_write_start(void);

// ---------------------------------------
// Окончание записи? )
// ---------------------------------------
void Shift_reg_write_end(void);

#endif
