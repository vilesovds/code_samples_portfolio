/*
 * buttons.h
 *
 *  Created on: 09 ���. 2014 �.
 *      Author: �������
 */

#ifndef _BUTTONS_H_
#define _BUTTONS_H_


#define  F_BUTOK   		0x01
#define  F_BUTUP   		0x02
#define  F_BUTDOWN   	0x04
#define  F_BUTCANCEL   	0x08
#define F_ALL_BUTTONS ( F_BUTOK|F_BUTUP | F_BUTDOWN | F_BUTCANCEL)

extern volatile uint8_t buttons_flags;

int Buttons_Init(void);
int8_t get_buttons(void);
void check_pins(void);
#endif /* BUTTONS_H_ */
