// ===================================
// ������� ���������� �������� 74HC595
// ������������� �� �������� ��� ����� ������
// ===================================
#ifndef _SHREGF_H
#define _SHREGF_H

#include "PLATFORM.h"
#include <macros.h>

// ------------------------
// ���� ���������� ��������
// ������������� ������������� �� ������
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
// ��������� ����������
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
// ��������� ���������� ��������
// -----------------------------
void Shift_reg_reset(void);

// --------------------------------------------------
// ����� ������ �� ���������� �������� � ������������
// --------------------------------------------------
void Shift_reg_store(void);

// ---------------------------------------
// ������ ������ � ��������� �������
// data - ��������� �� ������
// len - ����� � ������
// ---------------------------------------
void Shift_reg_write_data(char * data, int len);

// ---------------------------------------
// ������ 1 ����� � ��������� �������
// ---------------------------------------
void Shift_reg_write_byte(char data);

// ---------------------------------------
// ���������� ����� �������� � ������
// ---------------------------------------
void Shift_reg_write_start(void);

// ---------------------------------------
// ��������� ������? )
// ---------------------------------------
void Shift_reg_write_end(void);

#endif
