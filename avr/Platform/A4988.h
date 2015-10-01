/*
 * A4988.h
 *
 *  Created on: 12 ���. 2014 �.
 *      Author: �������
 */

#ifndef A4988_H_
#define A4988_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define LC_INCLUDE "lc-addrlabels.h"
#include "pt.h"
#include "pt-sem.h"

/*Driver*/
#define A4988_STEP_PORT	PORTC
#define A4988_STEP_PIN	(1<<2)

#define A4988_MS1_PORT	PORTC
#define A4988_MS1_PIN	(1<<0)

#define A4988_MS2_PORT	PORTC
#define A4988_MS2_PIN	(1<<1)

#define A4988_MS3_PORT	PORTC
#define A4988_MS3_PIN	(1<<4)

#define A4988_DIR_PORT	PORTC
#define A4988_DIR_PIN	(1<<3)

#define A4988_EN_PORT	PORTC
#define A4988_EN_PIN	(1<<5)

enum {
	A4988_SPEEDSM_OFF=0,
	A4988_SPEEDSM_PICK_UP,
	A4988_SPEEDSM_HOLDING,
	A4988_SPEEDSM_BRAKING,
	A4988_SPEEDSM_COMPLETED,
};

extern uint16_t A4988bpc;
extern uint16_t A4988startPPS;
extern uint16_t A4988marchPPS;
extern uint16_t A4988stopPPS;
extern uint16_t A4988startTime;
extern uint16_t A4988stopTime;
extern uint16_t A4988pauseBeforeShot;
extern uint16_t A4988pauseAfterShot;
extern uint16_t A4988totalSteps;
extern uint8_t A4988invdir;
extern uint8_t A4988microstep;
extern uint16_t A4988calibPPS;

extern struct pt autorotate_pt;

void A4988init(void);
void A4988SetPPS(uint16_t pps);
void A4988start(uint8_t dir,struct pt * thread, struct pt_sem * s );
void A4988SetSteps(uint16_t stps);
uint16_t  A4988GetPos(void);
void A4988SetPos(uint16_t pos);
void A4988stop(void);
void A4988StopAutoRotate(void);
void A4988StartAutoRotate(uint16_t aim);
void A4988MoveNext(void);
void A4988MovePrev(void);
uint8_t A4988GetSM(void);
void A4988MoveHome(void);
void autorotate_init(void);
PT_THREAD(autorotate_task(struct pt *pt));
#endif /* A4988_H_ */
