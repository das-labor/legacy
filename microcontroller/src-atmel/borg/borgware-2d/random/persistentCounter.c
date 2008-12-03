/**
 * \file	persistentCounter.c
 * \author	Daniel Otte 
 * \brief	a persistent 24-bit counter in EEPROM for ATmega µC
 * 
 */

#include <stdint.h>
#include <avr/interrupt.h> /* cli() & sei() */
#include <avr/eeprom.h>
#include "../config.h"

#ifdef ERROR_HANDLING
	#include "error-handling.h"
	#define PERSISTENT_COUNTER_OVERFLOW		(void*)0, 2,4,1
	#define PERSISTENT_COUNTER_WRITER_ERROR	(void*)0, 2,4,2
#endif

#define RING_SIZE 168

uint8_t ring_idx = 0xff;

uint16_t  EEMEM B08_23;
uint8_t EEMEM B0_7[RING_SIZE];

#ifdef INIT_EEPROM
void init_buffer(void){
	uint8_t i;
	eeprom_busy_wait();
	eeprom_write_word(&B08_23, 0x0000);
	for(i=0; i<RING_SIZE; ++i){
		eeprom_busy_wait();
		eeprom_write_byte(&(B0_7[i]), 0x00);
	}
}
#endif

void percnt_init(void){
	uint8_t i;
	uint8_t maxidx=0;
	uint8_t t,max=eeprom_read_byte(&(B0_7[0]));
	#ifdef INIT_EEPROM
		if (eeprom_read_word(&B08_23)==0xFFFF){	/* test if the 2 MSB == 0xFFFF*/
			if (eeprom_read_word((uint16_t*)&(B0_7[0]))==0xFFFF) /* test the first to bytes of ringbuffer*/
				init_buffer();
		}
	#endif
	for(i=0; i<RING_SIZE; ++i){ /* this might be speed up, but such optimisation could lead to timing attacks */
		eeprom_busy_wait();
		t=eeprom_read_byte(&(B0_7[i]));
		if(t==max+1){
			max=t;
			maxidx=i;
		}
	}
	ring_idx = (maxidx==RING_SIZE)?0:maxidx;
}

uint32_t percnt_get(void){
	uint32_t ret=0;

	if(ring_idx==0xff)
		percnt_init();
	cli();
	eeprom_busy_wait();
	ret=eeprom_read_word(&B08_23)<<8;
	eeprom_busy_wait();
	ret |= eeprom_read_byte(&(B0_7[ring_idx]));
	sei();
	return ret;
}

void percnt_inc(void){
	/* we must make this resistant agaist power off while this is running ... */
	uint32_t u;
		
	if(ring_idx==0xff)
		percnt_init();

	u = percnt_get();
	cli();
	/* it's important to write msb first! */
	if((u&0x000000ff) == 0xff){
		if((u&0x0000ffff) == 0xffff){
			if((u&0x00ffffff) == 0xffffff){
				/* we can turn the lights off now. it's time to die */
			#ifdef ERROR_HANDLING	
				error(PERSISTENT_COUNTER_OVERFLOW);
			#endif
			}
			eeprom_busy_wait();
			eeprom_write_byte(&(((uint8_t*)&B08_23)[1]),((u+1)>>16)&0xff);
		}
		eeprom_busy_wait();
		eeprom_write_byte(&(((uint8_t*)&B08_23)[0]),((u+1)>>8)&0xff);
	}
	/* set least significant byte (in ringbuffer) */
	ring_idx = (ring_idx+1)%RING_SIZE;
	eeprom_busy_wait();
	eeprom_write_byte(&(B0_7[ring_idx]),(u+1)&0xff);
	eeprom_busy_wait();
	
	if(u+1 != percnt_get()){
	#ifdef ERROR_HANDLING	
		error(PERSISTENT_COUNTER_WRITER_ERROR);
	#endif 
	}
	
	sei();
}

