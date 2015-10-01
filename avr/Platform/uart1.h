/*
 * uart1.h
 *
 *  Created on: 09.12.2014
 *      Author: Vilesov.DS
 */

#ifndef _UART1_H_
#define _UART1_H_

#include "common.h"
#include <stdio.h>

extern struct pt uart_pt;
void uart1_init(void);
int uart1_getchar(char * c, uint16_t wait);
int uart1_puthcar(char c, FILE *stream);

PT_THREAD(uart_task(struct pt *pt));

#endif /* UART1_H_ */
