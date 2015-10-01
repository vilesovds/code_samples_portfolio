/*
 * timer2.h
 *
 *  Created on: 24.12.2014
 *      Author: Vilesov.DS
 */

#ifndef TIMER2_H_
#define TIMER2_H_
#include <avr/io.h>
void timer2_init(void);


uint16_t timer2_getCounter(void);
uint16_t time_interval(uint16_t timestamp);

#endif /* TIMER2_H_ */
