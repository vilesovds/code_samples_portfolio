#ifndef MAIN_H
#define MAIN_H
//************************  INCLUDES*******************************
//*
//*******************************************************************
# include <iom162v.h>
//hardware
#include <PLATFORM.h>
#include <mcp2510.h>
#include <timer.h>
#include <SPI.h>
#include <macros.h>
// PORTS VS Signals============================================
#define SIGNAL_DFK1 (PINA&BIT(0))
#define SIGNAL_DFK2 (PINA&BIT(1))
#define SIGNAL_DFK3 (PINA&BIT(2))
#define SIGNAL_DFK4 (PINA&BIT(3))
#define SIGNAL_DFK5 (PINA&BIT(4))
#define SIGNAL_UZD1OUT (PINA&BIT(5))
#define SIGNAL_UZD2OUT (PINA&BIT(7))
#define SIGNAL_B1OUT (PIND&BIT(4))
#define SIGNAL_B2OUT (PIND&BIT(2))
#define SIGNAL_IKOUT (PIND&BIT(5))

//===========================================================
#define MAX_timeout 5000
#define ERR_off_time 100
#define ERR_on_time 20

// макросы
#define B2LED_OFF (PORTD|=BIT(1))
#define B2LED_ON (PORTD&=~BIT(1))

#define B1LED_OFF (PORTD|=BIT(3))
#define B1LED_ON (PORTD&=~BIT(3))
// номера для антидребезговой буферезации входов
enum {
  N_SIGNAL_DFK1, 
  N_SIGNAL_DFK2, 
  N_SIGNAL_DFK3, 
  N_SIGNAL_DFK4, 
  N_SIGNAL_DFK5, 
  N_SIGNAL_UZD1OUT,
  N_SIGNAL_UZD2OUT,
  N_SIGNAL_B1OUT,
  N_SIGNAL_B2OUT,
  N_SIGNAL_IKOUT,
  N_SIGNAL_BUF_COUNT
};
#endif