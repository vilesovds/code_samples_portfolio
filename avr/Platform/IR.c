/*
 * IR.c
 *
 *  Created on: 11.12.2014
 *      Author: Vilesov.DS
 */

#include "IR.h"

#include <util/atomic.h>

void IR_shot(uint8_t puls) {
	uint8_t pulscount = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	// setup timer1
	TIMSK1 = 0x00; //timer 1 interrupt sources
	TCCR1B = 0x00; //stop
	TCNT1 = 0;
	TCCR1A = _BV(COM1A1)/* | _BV(COM1B1)*/;
//phase and frequency correct mode.NON-inverted mode
	TCCR1B = _BV(WGM13) | _BV(CS10); // �������� N = 1 - (_BV(CS10)); 8 - (_BV(CS11))
	OCR1A = 0; // ���������� 102 - ������� ������, 0 - ������ �����������
	ICR1 = 210; // ������� 16���/2*N*ICR1 = 16000���/(2*1*210)=38095��
	PORTB&=~0x01; // PB0 = 0
	while (pulscount < puls) { // ���� ������ ���������

		OCR1A = 102; // �������� ���������
		_delay_us(2432); // ��������� ����� �����������
		OCR1A = 0; // ��������� ���������
		_delay_us(576); // ��������� �������� �����������
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(640);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(576);
		OCR1A = 102; // �������� ���������
		_delay_us(1216);
		OCR1A = 0;
		_delay_us(11136);

		pulscount++;
	}

	}
}
