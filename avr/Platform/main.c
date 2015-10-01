/*
 * main.c
 *
 *  Created on: 25 дек. 2014 г.
 *      Author: Дмитрий
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "settings.h"
#include "gui.h"
static FILE mystdout = FDEV_SETUP_STREAM(uart1_puthcar, // ������� ������ �������
		NULL,           // ������� ����� �������, ��� ������ �� �����
		_FDEV_SETUP_WRITE // ����� ������ - ������ �����
		);



static void port_init(void) {
	PORTB = 0;
	DDRB = 0b00111111;
	PORTD = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	DDRD = 0b11000000;
	PORTC = 0;
	DDRC = 0b00111111;
}


int main(void)
{
	//cli();

	port_init();
	A4988init();
	display_init();
	display_clear();
	text_set_font(FONT_NORMAL);
	text_print(1,0,PSTR("Started\r\n"),0);


	uart1_init();
	stdout = &mystdout;

	if(SettingsCheck()==0){
		SettingsLoadDefaults();
		SettingsSaveAll();
	}
	else {
		SettingsLoadAll();
	}

	timer2_init();
	TIMSK2 = 0x01; //timer 2 interrupt sources

	sei();
	printf_P(PSTR("start\r\n"));
	gui_init();

	while(1){
		uart_task(&uart_pt);
		gui_task(&gui_pt);
		autorotate_task(&autorotate_pt);
	}
	return 0;
}
