/*
 * A4988.c
 *
 *  Created on: 12 ���. 2014 �.
 *      Author: �������
 */
#include "A4988.h"
#include <stdio.h>
#include "timer2.h"
#include "common.h"


static uint16_t timer1_TCNT = 0;

static uint16_t timer1_TCCRB = 0;

static uint8_t half_step = 0;
static uint16_t steps = 0;


static volatile uint16_t t0;
static uint8_t auto_dir = 0;

// settings
uint16_t A4988bpc;
uint16_t A4988startPPS;
uint16_t A4988marchPPS;
uint16_t A4988stopPPS;
uint16_t A4988startTime;
uint16_t A4988stopTime;
uint16_t A4988pauseBeforeShot;
uint16_t A4988pauseAfterShot;
uint16_t A4988totalSteps;
uint8_t A4988invdir;
uint8_t A4988microstep;
uint16_t A4988calibPPS;

struct pt autorotate_pt;
struct pt_sem mutex;
struct pt_sem sema;
struct pt_sem * sem = NULL;
struct pt * sem_thread = NULL;

static volatile uint8_t workmode = 0;

enum {
	A4988_WORK_MODE_OFF=0,
	A4988_WORK_MODE_MANUAL,
	A4988_WORK_MODE_AUTO,
};

static uint8_t SpeedSM=0;

static uint16_t last_pps = 0;
static volatile uint16_t position=0;
static uint8_t _dir=0;


static uint16_t K1M,K1N,K2,K3N,K3M;
static uint16_t S,S1,S2,S3;
static uint16_t Vm;

static uint8_t auto_cmd=0;

/*************************PROTOTYPES**********************************/
static int8_t PPSToTCNT(uint16_t pps, uint16_t *tcnt, uint16_t *tccrb);
static void timer1_init(void);

static void init_MS(uint8_t ms);
/***************************IMPL**************************************/

static int8_t PPSToTCNT(uint16_t pps, uint16_t *tcnt, uint16_t *tccrb) {
#define MAX_TIMER_TCNC (0x0000FFFFL-100)
	uint32_t ft[] = { F_CPU / 2, F_CPU / 16, F_CPU / 128, F_CPU / 512, F_CPU
			/ 2048 };
	uint8_t i = 0;
	uint32_t val;
	do {
		val = ft[i++] / pps;
	} while ((val > MAX_TIMER_TCNC) && (i < 6));
	if (i >= 6)
		return 0;
	*tcnt = (0xFFFF - (uint16_t) val + 1);
	*tccrb = i;
	return 1;
}

//TIMER1 initialize - prescale:1
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 400Hz
// actual value: 400,000Hz (0,0%)
static void timer1_init(void) {
	TCCR1B = 0x00; //stop
// TCNT1H = 0xB1; //reload counter high value
// TCNT1L = 0xE0; //reload counter low value
	TCNT1 = timer1_TCNT;
	OCR1AH = 0x9C;
	OCR1AL = 0x40;
	OCR1BH = 0x9C;
	OCR1BL = 0x40;
	ICR1H = 0x9C;
	ICR1L = 0x40;
	TCCR1A = 0x00;
	TCCR1B = 0; //stop Timer
}

static void init_MS(uint8_t ms)
{
	switch(ms){
	case 0: // full step
		A4988_MS1_PORT &=~A4988_MS1_PIN;
		A4988_MS2_PORT &=~A4988_MS2_PIN;
		A4988_MS3_PORT &=~A4988_MS3_PIN;
		break;
	case 1: // 1/2
		A4988_MS1_PORT |=A4988_MS1_PIN;
		A4988_MS2_PORT &=~A4988_MS2_PIN;
		A4988_MS3_PORT &=~A4988_MS3_PIN;
		break;
	case 2: // 1/4
		A4988_MS1_PORT &=~A4988_MS1_PIN;
		A4988_MS2_PORT |=A4988_MS2_PIN;
		A4988_MS3_PORT &=~A4988_MS3_PIN;
		break;
	case 3:// 1/8
		A4988_MS1_PORT |=A4988_MS1_PIN;
		A4988_MS2_PORT |=A4988_MS2_PIN;
		A4988_MS3_PORT &=~A4988_MS3_PIN;
		break;
	case 4: // 1/16
		A4988_MS1_PORT |=A4988_MS1_PIN;
		A4988_MS2_PORT |=A4988_MS2_PIN;
		A4988_MS3_PORT |=A4988_MS3_PIN;
		break;
	}
}

static void deEnable(void)
{
	A4988_EN_PORT |= A4988_EN_PIN;
	printf_P(PSTR("deEnabled\r\n"));
}

void A4988init(void) {

	timer1_init();

	init_MS(A4988microstep);

	TIMSK1 = 0x01; //timer 1 interrupt sources
}

void A4988stop(void) {
	TCCR1B = 0;
	A4988_STEP_PORT &= ~A4988_STEP_PIN;
	if((sem_thread)&&(sem)){
		PT_SEM_SIGNAL(sem_thread,sem);
		sem_thread = NULL;
		sem = NULL;
	}
/*
	if (xA4988Timer != NULL){
		xTimerReset(xA4988Timer, 0);
	}
*/
	//workmode  = A4988_WORK_MODE_OFF;
	//printf("stopped\r\n");
	{

		//uint8_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
		//printf_P(PSTR("IDLE: %u\r\n"), uxHighWaterMark);
	}
}
/**/
void A4988start(uint8_t dir,struct pt * thread, struct pt_sem * s ) {
//TODO improve
	A4988init();
	if(dir^A4988invdir) A4988_DIR_PORT |= A4988_DIR_PIN;
	else 				A4988_DIR_PORT &= ~A4988_DIR_PIN;
	A4988_EN_PORT &= ~A4988_EN_PIN;
	_dir = dir;
	if((_dir)&&(0 ==position)){
		return;
	}
	if((!_dir)&&(A4988totalSteps/*0xFFFF*/==position)){
		return;
	}
	TCCR1B = timer1_TCCRB;
	//printf("started\r\n");
	if((thread)&&(s)){
		sem_thread = thread;
		sem = s;
	}
	//xSemaphoreTake(xSemaphore, wait);
}

void A4988SetSteps(uint16_t stps) {
	if (stps) {
#if 0
		if((_dir)&&(0 ==position)){
			return;
		}
#endif
		steps = stps-1;
		//printf_P(PSTR("steps : %u\r\n"), steps);
	}
}

/**/
void A4988SetPPS(uint16_t pps) {
	uint16_t tcnt = 0;
	uint16_t tccrb = 0;
	if((last_pps!=pps)&&(pps)){
		int8_t res = PPSToTCNT(pps, &tcnt, &tccrb);
		if (res) {
			timer1_TCNT = tcnt;
			timer1_TCCRB = tccrb;
			// if timer already started - change prescaler
			//if(TCCR1B&0x07)	TCCR1B = timer1_TCCRB;
#ifdef DEBUG
			{
				static uint8_t oldTCCRB = 0;

				if(oldTCCRB!=timer1_TCCRB){
					//printf_P(PSTR("TCCRB = %u\r\n"),timer1_TCCRB);
					PORTB|=(1<<0);
					oldTCCRB = timer1_TCCRB;
					//
				}else {
					PORTB&=~(1<<0);
				}
			}
#endif
			last_pps = pps;
		}
		//printf("\r\ntcnt = %u; txxrb= %u\r\n",tcnt, tccrb);
	}


}


uint16_t A4988GetPos(void)
{
	uint16_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		t = position;
	}
	return t;
}

void A4988SetPos(uint16_t pos)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		position = pos;
	}
}

/**
 *
 */
void A4988StopAutoRotate(void)
{
	auto_cmd =1;
	A4988stop();
}

/**
 *
 */
void A4988MoveHome(void)
{
	if (position){
		S1 = ((uint32_t)A4988startPPS + A4988marchPPS)*A4988startTime/2000;
		S3 = ((uint32_t)A4988stopPPS + A4988marchPPS)*A4988stopTime/2000;
		if(position > (A4988totalSteps/2)){
			S = A4988totalSteps - position;
			auto_dir = 0; // forward
		}else{
			S = position;
			auto_dir = 1; // backward
		}

		if(S>(S1+S3)){
			S2 = S - S1 - S3;
			Vm = A4988marchPPS;
		}else{
			Vm = (2000*(uint32_t)S -(uint32_t)A4988startPPS*A4988startTime - (uint32_t)A4988stopPPS*A4988stopTime)/(A4988startTime + A4988stopTime);
			if (Vm<A4988startPPS) Vm = A4988startPPS;

			S1 = ((uint32_t)A4988startPPS + Vm)*A4988startTime/2000;

			S3 = ((uint32_t)A4988stopPPS + Vm)*A4988stopTime/2000;
			S2 = 0;
		}
#ifdef DEBUG
		printf("Vm %u, S %u, S1 %u, S2 %u, S3 %u\r\n",Vm, S,S1,S2,S3);
#endif

		K1M = (Vm - A4988startPPS);
		K1N = A4988startTime;

		K2 = 0;
		K3M = Vm - A4988stopPPS ;
		K3N = A4988stopTime;

		//set work mode
		SpeedSM  = A4988_SPEEDSM_PICK_UP;
		workmode = A4988_WORK_MODE_AUTO;
		auto_cmd = 0;

		// sem
		PT_SEM_SIGNAL(&autorotate_pt, &mutex);
	}
}

/**
 *
 */
void A4988MovePrev(void){
	uint8_t cur;
	uint16_t step;
	uint16_t next;

	step = A4988totalSteps/A4988bpc;
	cur  = position/step;
#ifdef DEBUG
	printf_P(PSTR("cur %u of %u\r\n"),cur,A4988bpc);
#endif
	// if not first
	if((cur==0)&&(position==0)) return;
	else if(cur>0){
		cur--;
	}
	next = ((cur)*(uint32_t)A4988totalSteps)/A4988bpc;
#ifdef DEBUG
	printf_P(PSTR("next %u \r\n"),next);
#endif

	A4988StartAutoRotate(next);

}

/**
 *
 */
void A4988MoveNext(void){
	uint8_t cur;
	uint16_t step;
	uint16_t next;

	step = A4988totalSteps/A4988bpc;
	cur  = position/step;
#ifdef DEBUG
	printf_P(PSTR("cur %u of %u\r\n"),cur,A4988bpc);
#endif
	// if not last
	if(cur<A4988bpc){
		next = ((cur+1)*(uint32_t)A4988totalSteps)/A4988bpc;
#ifdef DEBUG
	printf_P(PSTR("next %u \r\n"),next);
#endif

		A4988StartAutoRotate(next);
	}

}

/**
 *
 */
void A4988StartAutoRotate(uint16_t aim)
{


	S1 = ((uint32_t)A4988startPPS + A4988marchPPS)*A4988startTime/2000;

	S3 = ((uint32_t)A4988stopPPS + A4988marchPPS)*A4988stopTime/2000;

	if(position < aim){
		S = aim - position;
		auto_dir = 0; // forward
	}else{
		S = position - aim;
		auto_dir = 1; // backward
	}

	if(S>(S1+S3)){
		S2 = S - S1 - S3;
		Vm = A4988marchPPS;
	}else{
		Vm = (2000*(uint32_t)S -(uint32_t)A4988startPPS*A4988startTime - (uint32_t)A4988stopPPS*A4988stopTime)/(A4988startTime + A4988stopTime);
		if (Vm<A4988startPPS) Vm = A4988startPPS;

		S1 = ((uint32_t)A4988startPPS + Vm)*A4988startTime/2000;

		S3 = ((uint32_t)A4988stopPPS + Vm)*A4988stopTime/2000;
		S2 = 0;
	}
#ifdef DEBUG
	printf("Vm %u, S %u, S1 %u, S2 %u, S3 %u\r\n",Vm, S,S1,S2,S3);
#endif
	K1M = (Vm - A4988startPPS);
	K1N = A4988startTime;

	K2 = 0;
	K3M = Vm - A4988stopPPS ;
	K3N = A4988stopTime;
	//printf("K1M %u, K3M %u\r\n",K1M,K3M);
	//set work mode
	SpeedSM  = A4988_SPEEDSM_PICK_UP;
	workmode = A4988_WORK_MODE_AUTO;
	auto_cmd = 0;
	//A4988start(0,0);
	// sem
	PT_SEM_SIGNAL(&autorotate_pt, &mutex);
}

void halhStep(void)
{
	uint16_t dv;
	switch(SpeedSM){
	case A4988_SPEEDSM_PICK_UP:
			dv =(uint32_t)K1M*time_interval(t0)/K1N;
			if((A4988startPPS + dv)<Vm)
				A4988SetPPS(A4988startPPS + dv);
			else A4988SetPPS(Vm);
		break;
	case A4988_SPEEDSM_BRAKING:
			dv = (uint32_t)K3M*time_interval(t0)/K3N;
			if(Vm>(A4988stopPPS+dv))
				A4988SetPPS(Vm - dv);
			else A4988SetPPS(A4988stopPPS);
		break;
	}
}

void autorotate_init(void)
{
	PT_SEM_INIT(&mutex, 1);
	PT_SEM_INIT(&sema, 1);
	PT_INIT(&autorotate_pt);
}

/**
 *
 */
uint8_t A4988GetSM(void)
{
	return SpeedSM;
}

/**
 *
 */
PT_THREAD(autorotate_task(struct pt *pt))
{
//	static uint8_t cmd;
PT_BEGIN(pt);
	while(1){

		//xSemaphoreTake(xRSemaphore, portMAX_DELAY);
		PT_SEM_WAIT(pt, &mutex);
		if(A4988_WORK_MODE_AUTO == workmode){
			while(SpeedSM != A4988_SPEEDSM_COMPLETED){
//				PORTC|=(1<<0);
				if(auto_cmd){
					switch (auto_cmd) {
						case 1:
							if(SpeedSM!=A4988_SPEEDSM_BRAKING){
								Vm = last_pps;
								S3 = ((uint32_t)A4988stopPPS + Vm)*A4988stopTime/2000;

								K3M = Vm - A4988stopPPS ;
								SpeedSM = A4988_SPEEDSM_BRAKING;
							}
							break;
						default:
							break;
					}
					auto_cmd = 0;
				}
				//calculate PPS
				switch(SpeedSM){
				case A4988_SPEEDSM_PICK_UP:
						// set started PPS
						A4988SetPPS(A4988startPPS);
						A4988SetSteps(S1);
						t0 = timer2_getCounter();
						A4988start(auto_dir,pt,&sema);
						PT_SEM_WAIT(pt,&sema);
#ifdef DEBUG
						printf_P(PSTR("time:%u\r\n"),time_interval(t0));
						printf_P(PSTR("pps:%u\r\n"),last_pps);
#endif
						if(S2>0)
							SpeedSM = A4988_SPEEDSM_HOLDING;
						else
							SpeedSM = A4988_SPEEDSM_BRAKING;
					break;
				case A4988_SPEEDSM_HOLDING:
						A4988SetPPS(Vm);
						A4988SetSteps(S2);
						t0 = timer2_getCounter();
						A4988start(auto_dir,pt, &sema);
						PT_SEM_WAIT(pt,&sema);
#ifdef DEBUG
						printf_P(PSTR("time:%u\r\n"),time_interval(t0));
						printf_P(PSTR("pps:%u\r\n"),last_pps);
#endif
						SpeedSM = A4988_SPEEDSM_BRAKING;
					break;
				case A4988_SPEEDSM_BRAKING:
						A4988SetPPS(Vm);
						A4988SetSteps(S3);
						t0 = timer2_getCounter();
						A4988start(auto_dir,pt,&sema);
						PT_SEM_WAIT(pt,&sema);
#ifdef DEBUG
						printf_P(PSTR("time:%u\r\n"),time_interval(t0));
						printf_P(PSTR("pps:%u\r\n"),last_pps);
#endif
						if(position == A4988totalSteps){
							position = 0;
						}
						SpeedSM = A4988_SPEEDSM_COMPLETED;
					break;
				}
//				PORTC&=~(1<<0);
			}
		}
	}
PT_END(pt);
}

//TIMER1 has overflowed
ISR(TIMER1_OVF_vect)
{
//TCNT1H = 0xB1; //reload counter high value
//TCNT1L = 0xE0; //reload counter low value
	TCNT1 = timer1_TCNT;
	TCCR1B = timer1_TCCRB;

//PORTB|=(1<<0);
	switch (half_step) {
	case 0:
		A4988_STEP_PORT |= A4988_STEP_PIN;
		half_step++;
		//TODO Prepare next PPS
		halhStep();
		/* Unblock the task by releasing the semaphore. */
		break;

	case 1:
		A4988_STEP_PORT &= ~A4988_STEP_PIN;
		if((_dir)&&(position)){
			position --;
			if(0==position) steps = 0;
		}
		else if((!_dir)&&(position<A4988totalSteps/*0xFFFF*/)) {
			position++;
			if(/*0xFFFF*/A4988totalSteps==position) steps = 0;
		}
		if (steps){
			steps--;
		}
		else {
			A4988stop();
			/* Unblock the task by releasing the semaphore. */

		}
		half_step = 0;
		break;
	}
	/*
	if(xA4988Timer!=NULL){
		xTimerResetFromISR(xA4988Timer,NULL);
	}
	*/

//PORTB&=~(1<<0);
}
