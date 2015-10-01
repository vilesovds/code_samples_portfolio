/*
 * uart1.c
 *
 *  Created on: 09.12.2014
 *      Author: Vilesov.DS
 */

#include "common.h"

struct pt uart_pt;

static volatile uint8_t new_data = 0;
//UART0 initialize
// desired baud rate: 115200
// actual: baud rate:117647 (2,1%)
// char size: 8 bit
// parity: Disabled
static void uart0_init(void) {
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x02;
	UCSR0C = 0x06;
	UBRR0L = 0x10; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}

void uart1_init(void) {
	uart0_init();
	PT_INIT(&uart_pt);
}

// функция вывода символа
int uart1_puthcar(char c, FILE *stream) {
// Wait for empty transmit buffer
	while (!(UCSR0A & (_BV(UDRE0))))
		;

	UDR0 = c;

	return 0;
}

PT_THREAD(uart_task(struct pt *pt)) {
	static uint16_t uart_ts;
#define _BSIZE 20
	static int len = 0;
	static char buf[_BSIZE];
	static char smsg;

	PT_BEGIN(pt);
		while (1) {
			PT_WAIT_UNTIL(pt, (new_data));
			uart_ts = timer2_getCounter();
			smsg = new_data;
			new_data = 0;
			if ((smsg == '\0') || (smsg == '\r') || (smsg == '\n')) {
				uart1_puthcar(smsg, stdout);
				buf[len] = '\0';
				if (0 == strncmp_P(buf, PSTR("UPGRADE"), 7)) {
					// goto boot
					display_clear();
					text_print(2, 0, PSTR("Переходим в"), 0);
					text_print(3, 10, PSTR("загрузчик"), 0);
					PT_WAIT_UNTIL(pt, (time_interval(uart_ts) >= 1000));
					wdt_reset();
					WDTCSR |= (1 << WDCE) | (1 << WDE);/* 30-Oct-2006 Umesh*/
					WDTCSR = 0x08; //WATCHDOG ENABLED - dont forget to issue WDRs
					while (1)
						;
				} else {
					if (strncmp_P(buf, PSTR("pps"), 3) == 0) {
						uint16_t pps;
						if (sscanf_P(buf, PSTR("pps %u"), &pps)) {
							//A4988SetPPS(pps);
						}
					}
				}
				len = 0;
			} else {
				buf[len++] = (char) smsg;
				uart1_puthcar(smsg, stdout);
				if (len == _BSIZE) {
					len = 0;
				}
			}
			PORTB ^= (1 << 0);
		}
	PT_END(pt);
}

ISR( USART_RX_vect) {
//char cIn;
	/* Obtain a byte from the buffer. */
	new_data = UDR0;
//new_data = 0x01;
}
