#include "timer2.h"

//ICC-AVR application builder : 24.12.2014 16:31:44
// Target : M328p
// Crystal: 16.000Mhz

#include <avr/interrupt.h>
#include "common.h"
#include "buttons.h"

volatile uint16_t timer2_counter = 0;

//TIMER2 initialize - prescale:64
// WGM: Normal
// desired value: 1mSec
// actual value:  1,000mSec (0,0%)
void timer2_init(void) {
	TCNT2 = 0x00;
	TCCR2A = 0x00;
	TCCR2B = (1 << CS22);
	TCNT2 = 0x06;
}

ISR(TIMER2_OVF_vect, ISR_BLOCK) {
	TCNT2 = 0x06; //reload counter value
	timer2_counter++;
	check_pins();
}

/**
 *
 */
uint16_t timer2_getCounter(void) {
	uint16_t time_now;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){	// Disable Interrupts
		time_now = timer2_counter;
	}
	return time_now;
}

/**
 *
 */
uint16_t time_interval(uint16_t timestamp) {
	uint16_t time_now;

	time_now = timer2_getCounter();

	if (time_now >= timestamp)
		return (time_now - timestamp);
	else
		return (0xffff - timestamp + time_now + 1);
}
