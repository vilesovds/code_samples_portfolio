/************************************************************************/
/*                                                                      */
/*        Access Dallas 1-Wire Device with ATMEL AVRs                   */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */

/* modified by VilDi 5/2009       */
/************************************************************************/

#include <types.h>
#include <macros.h>
#include <PLATFORM.h>
#include "onewire.h"
//#include <timer1.h>

#define OW_GET_IN()   ( OW_IN & (1<<OW_PIN))
#define OW_OUT_LOW()  ( OW_OUT &= (~(1 << OW_PIN)) )
#define OW_OUT_HIGH() ( OW_OUT |= (1 << OW_PIN) )
#define OW_DIR_IN()   ( OW_DDR &= (~(1 << OW_PIN )) )
#define OW_DIR_OUT()  ( OW_DDR |= (1 << OW_PIN) )



void	usecsleep(uint8_t, uint8_t);

/**/
uint8_t 
ow_input_pin_state(void)
{ 
	return OW_GET_IN();
}

/**/
void 
ow_parasite_enable(void)
{ 
	OW_OUT_HIGH();
	OW_DIR_OUT();
}

/**/
void 
ow_parasite_disable(void)
{ 
	OW_OUT_LOW();
	OW_DIR_IN();
}
 
/*********************************
*RESET
-*********************************/
uint8_t 
ow_reset(void)
{	uint8_t err;
	uint8_t sreg;

	OW_OUT_LOW(); // disable internal pull-up (maybe on from parasite)
	OW_DIR_OUT(); // pull OW-Pin low for 480us
	
	//   delay_us(480);
	usecsleep(2,240);
	//timer1_Delay(480);
	sreg=SREG;
	/*TRY it*///
	CLI();
	/* set Pin as input - wait for clients to pull low*/
	OW_DIR_IN(); /* input*/

	//   delay_us(66);
	usecsleep(2,33);
	//timer1_Delay(480);
	err = OW_GET_IN();		/* no presence detect*/
	/* nobody pulled to low, still high*/
	/*TRY it*///SREG=sreg; /* 
	SEI();

	/* after a delay the clients should release the line
	and input-pin gets back to high due to pull-up-resistor*/
  
	// delay_us(480-66);
	usecsleep(2,207); 
	//timer1_Delay(480-66);
	if( OW_GET_IN() == 0 )/* short circuit*/ err = 1;
	return err;
}

/******************************************************************
Timing issue when using runtime-bus-selection (!OW_ONE_BUS):
   The master should sample at the end of the 15-slot after initiating
   the read-time-slot. The variable bus-settings need more
   cycles than the constant ones so the delays had to be shortened 
   to achive a 15uS overall delay 
   Setting/clearing a bit in I/O Register needs 1 cyle in OW_ONE_BUS
 but around 14 cyles in configureable bus (us-Delay is 4 cyles per uS) 
*********************************************************************/
uint8_t 
ow_bit_io( uint8_t b )
{	uint8_t sreg;

	sreg=SREG;
	/*TRY it*///
	CLI();
	OW_DIR_OUT(); /* drive bus low*/

	// delay_us(1); /* Recovery-Time wuffwuff was 1*/
	usecsleep(1,1); 
	//timer1_Delay(1);
	if ( b ) OW_DIR_IN(); /* if bit is 1 set bus high (by ext. pull-up)*/
	/* wuffwuff delay was 15uS-1 see comment above*/
	// delay_us(15-1-OW_CONF_DELAYOFFSET);
	usecsleep(1,15-1-OW_CONF_DELAYOFFSET); 
	//timer1_Delay(15-1-OW_CONF_DELAYOFFSET);
        
	if( OW_GET_IN() == 0 ) b = 0;  /* sample at end of read-timeslot*/
	//   delay_us(60-15);
	usecsleep(1,60-15);
	//timer1_Delay(60-15);
	OW_DIR_IN();
	/*TRY it*///SREG=sreg; /* sei();*/
	SEI();
	return b;
}

/******************************************
*******************************************/
uint8_t 
ow_byte_wr( uint8_t b )
{ 
	uint8_t i = 8, j;
	do { 
		j = ow_bit_io( b & 1 );
		b >>= 1;
		if( j ) b |= 0x80;
    } 
	while( --i );
	return b;
}

/*****************************************
*****************************************/
uint8_t 
ow_byte_rd( void )
{ /* read by sending 0xff (a dontcare?)*/
   return ow_byte_wr( 0xFF ); 
}

/***************************************
*
*
-***************************************/
uint8_t 
ow_rom_search( uint8_t diff, uint8_t *id )
{	uint8_t i, j, next_diff;
	uint8_t b;

	if( ow_reset() ) return OW_PRESENCE_ERR;/* error, no device found*/
   
	ow_byte_wr( OW_SEARCH_ROM ); /*ROM search command*/
	next_diff = OW_LAST_DEVICE; /* unchanged on last device*/
	i = OW_ROMCODE_SIZE * 8;    /* 8 bytes*/
	do {
		j = 8;                  /* 8 bits*/
		do{
			b = ow_bit_io( 1 );	/* read bit*/
			if( ow_bit_io( 1 ) ) {
				/* read complement bit*/
				if( b )/* 11*/ return OW_DATA_ERR;  /* data error*/
			}
			else {
				if( !b ) { /* 00 = 2 devices*/
					if( diff > i || ((*id & 1) && diff != i) ) { 
						b = 1;          /* now 1*/
						next_diff = i;  /* next pass 0*/
					}
				}
			}
			ow_bit_io( b ); /* write bit*/
			*id >>= 1;
			if( b ) *id |= 0x80;/* store bit*/
			i--;
		} 
		while( --j );
		id++; /* next byte*/
    } 
	while( i ); /* to continue search*/
	return next_diff;   
}

/*************************************************************
Send command to specific device
**************************************************************/
void 
ow_command( uint8_t command, uint8_t *id )
{	uint8_t i;
	ow_reset();
	if( id ) {
		/* to a single device*/
		ow_byte_wr( OW_MATCH_ROM );
		i = OW_ROMCODE_SIZE;
		do { 
			ow_byte_wr( *id );
			id++;
		} while( --i );
    } 
	else /* to all devices*/ ow_byte_wr( OW_SKIP_ROM );
	ow_byte_wr( command );
}
