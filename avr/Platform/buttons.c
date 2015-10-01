/*
 * buttons.c
 *
 *  Created on: 09 ���. 2014 �.
 *      Author: �������
 */
#include <avr/io.h>
#include <stdio.h>
#include "buttons.h"

#ifndef BIT
#define BIT(x)	(1<<x)
#endif

#define SIGNAL_BUTOK 		((PIND&BIT(2))^BIT(2))
#define SIGNAL_BUTUP 		((PIND&BIT(5))^BIT(5))
#define SIGNAL_BUTDOWN 		((PIND&BIT(4))^BIT(4))
#define SIGNAL_BUTCANCEL 	((PIND&BIT(3))^BIT(3))
// ������ ��� ��������������� ����������� ������
enum {
	N_SIGNAL_BUTOK,
	N_SIGNAL_BUTUP,
	N_SIGNAL_BUTDOWN,
	N_SIGNAL_BUTCANCEL,
	N_SIGNAL_BUF_COUNT
};

#define MIN_DEBOUNCE_TIME  5

// Speeds
#define S1	50
#define S2	500
#define S3	150
#define S4	50

// Times
#define T1	S1
#define T2  (S2+S3)
#define T3	1400
#define T4 2800

uint8_t debounce[N_SIGNAL_BUF_COUNT] = { S1 }; // ��������� ������ ����� ��������������� ���������
uint8_t bufval[N_SIGNAL_BUF_COUNT] = { 0 };   // �������� ������������

uint16_t waitcnt[N_SIGNAL_BUF_COUNT] = { 0 };

volatile uint8_t buttons_flags=0;
static void
checkbuf(uint8_t sig_num, uint8_t sig_val, const uint8_t mask);


int Buttons_Init(void) {
	buttons_flags = 0;
	return 1;
}


int8_t get_buttons(void) {
	int8_t res = buttons_flags;
	buttons_flags = 0;
	return res;
}

/*==================================================================
 // ������ ������������
 // debounce_time - ����� �������� ������������  (��)
 *===================================================================*/
static void checkbuf(uint8_t sig_num, uint8_t sig_val, const uint8_t mask) {
	if (sig_val) {
		bufval[sig_num]++;
		waitcnt[sig_num]++;
		if ((waitcnt[sig_num] > T1) && (waitcnt[sig_num] <= T2)) {
			debounce[sig_num] = S2;
		} else if ((waitcnt[sig_num] > T2) && ((waitcnt[sig_num] <= T3))) {
			debounce[sig_num] = S3;
#if 0
		} else if ((waitcnt[sig_num] > T3) && ((waitcnt[sig_num] <= T4))) {
			debounce[sig_num] = S4;
		} else if (waitcnt[sig_num] > T4) {
			waitcnt[sig_num] = T4 + 1;
		}
#endif
		} else if (waitcnt[sig_num] > T3) {
				waitcnt[sig_num] = T3 + 1;
			}
	} else {
		bufval[sig_num] = 0;
		debounce[sig_num] = S1;
		waitcnt[sig_num] = 0;
	}

	if (bufval[sig_num] >= debounce[sig_num]) {
		// set flag
		buttons_flags |= mask;
		bufval[sig_num] = 0;
	}

}


void check_pins(void) {
	checkbuf(N_SIGNAL_BUTOK, SIGNAL_BUTOK, F_BUTOK);
	checkbuf(N_SIGNAL_BUTUP, SIGNAL_BUTUP, F_BUTUP);
	checkbuf(N_SIGNAL_BUTDOWN, SIGNAL_BUTDOWN, F_BUTDOWN);
	checkbuf(N_SIGNAL_BUTCANCEL, SIGNAL_BUTCANCEL, F_BUTCANCEL);
}
