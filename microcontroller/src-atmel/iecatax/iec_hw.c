
#include <avrx-io.h>
#include <avrx.h>
#include "fifo.h"

#include "config.h"
#define IEC_HW_C
#include "iec_hw.h"


#include <avr/pgmspace.h>
#include <stdio.h>


//#define MONITOR_MODE

#define IEC_CLK_OUT  PB2
#define IEC_DATA_OUT PB4
#define PIN_ATN_IN PIND
#define BIT_ATN_IN   PD2
#define IEC_CLK_IN   PB3
#define IEC_DATA_IN  PB5


/* Makros for setting and clearing bus lines */
#define CLKOUT_SET PORTB |= (1<<IEC_CLK_OUT)
#define CLKOUT_CLEAR PORTB &= ~(1<<IEC_CLK_OUT)

#define DATAOUT_SET PORTB |= (1<<IEC_DATA_OUT)
#define DATAOUT_CLEAR PORTB &= ~(1<<IEC_DATA_OUT)

AVRX_DECL_FIFO(iec_rx_fifo, 2);
AVRX_DECL_FIFO(iec_tx_fifo, 2);


void iecDelay80 (void) {
	TCNT2 = 0;
	while (TCNT2 < 100);
}

iec_word_t iecGetByte() {
	iec_word_t word;
	word.data = 0;
	word.flags= 0;

	/* wait for Talker */
	loop_until_bit_is_set (PINB, IEC_CLK_IN);
	{/* check ATN */
		if(bit_is_clear (PIN_ATN_IN, BIT_ATN_IN)){
			word.flags |= FLAG_ATN;
		}
	}
	UCSR0B &= ~(1<<UDRIE0);
	TIMSK &= ~_BV(TOIE0);//Timer int off
	/* signal ready to listen */
	DATAOUT_SET;
    
	/* wait for all other devices */
	loop_until_bit_is_set (PINB, IEC_DATA_IN);

	/* wait for talker, maximum 230us */
	TCNT2 = 0;
	while (! (word.flags & FLAG_EOI)) {
		if (TCNT2 > 230) {
			word.flags |= FLAG_EOI;
			/* acknowledge if eoi */
#			ifndef MONITOR_MODE
			DATAOUT_CLEAR;
			iecDelay80();
			DATAOUT_SET;
#			else
			iecDelay80();
#			endif
			/* wait for talker */
			loop_until_bit_is_clear (PINB, IEC_CLK_IN);
		}
		if (bit_is_clear (PINB, IEC_CLK_IN)) {
			break;
		}
	}
	
	{ /* fetch byte */
		uint8_t bitnumber;
		for (bitnumber = 0; bitnumber < 8; bitnumber++) {
			word.data >>= 1;

			/* wait for next bit */
			loop_until_bit_is_set (PINB, IEC_CLK_IN);
			/* fetch bit */
			if (bit_is_set (PINB, IEC_DATA_IN)) {
				word.data |= 0x80;
			}
			/* wait for IEC CLK */
			loop_until_bit_is_clear (PINB, IEC_CLK_IN);
		}
		/* acknowledge byte received */
#ifndef MONITOR_MODE
		DATAOUT_CLEAR;
#endif
#ifdef DEBUG
		UCSR0B |= (1<<UDRIE0);
#endif
		//TIMSK |= _BV(TOIE0);//Timer int back on
	}
	return word;
}

void iecTurnAround(){
	/* do turn-around sequence */
	loop_until_bit_is_set (PIN_ATN_IN, BIT_ATN_IN);
	loop_until_bit_is_set (PINB, IEC_CLK_IN);
	DATAOUT_SET;
	CLKOUT_CLEAR;
	iecDelay80();	
}

void iecPutByte (iec_word_t word) {
	UCSR0B &= ~(1<<UDRIE0);
	TIMSK &= ~_BV(TOIE0);//Timer int off
	
	/*Talker ready to send */
	CLKOUT_SET;
	
	/* wait for listener */
	loop_until_bit_is_set (PINB, IEC_DATA_IN);
	
	if (word.flags & FLAG_EOI) {
		/* wait for listener EOI Timeout Handshake */
		loop_until_bit_is_clear (PINB, IEC_DATA_IN);
		/* wait for Listener Ready-for-Data */
		loop_until_bit_is_set (PINB, IEC_DATA_IN);
	}
	
	/* Talker sending */
	CLKOUT_CLEAR;
	{ /* send byte */
		
		uint8_t bitnumber;
		for (bitnumber = 0; bitnumber < 8; bitnumber++) {
			iecDelay80();
			/* put data on line */
			if (word.data & 0x01) {
				DATAOUT_SET;
			} else {
				DATAOUT_CLEAR;
			}
			/* signal data ready */
			CLKOUT_SET;
			/* delay */
			iecDelay80();
			/* set IEC CLK */
			CLKOUT_CLEAR;
			DATAOUT_SET;
			/* shift data right */
			word.data >>= 1;
		}	
		/* wait for listener data accepted */
		loop_until_bit_is_clear (PINB, IEC_DATA_IN);
		/* TODO: timeout */
		CLKOUT_CLEAR;
		if (word.flags & FLAG_EOI) {
			/* wait for listener line release */
			loop_until_bit_is_set (PINB, IEC_DATA_IN);
		}
	}
#ifdef DEBUG
	UCSR0B |= (1<<UDRIE0);
#endif
	//TIMSK |= _BV(TOIE0);//Timer int on
}


volatile iec_word_t iec_tx_word, iec_rx_word;

AVRX_GCC_TASKDEF(iecComTask, 200, 3){
	iec_word_t word;
	//mode we are in right now
	uint8_t mode = MODE_LISTEN;
	
	//The first byte is returned without request. It should
	//be the first byte of an atn sequence.
	//PORT_LED ^= (1<<BIT_LED);
	word = iecGetByte();
	AvrXWaitPutFifo(iec_rx_fifo, word);
	
	while(1){
		//get our next command
		word = AvrXWaitPullFifo(iec_tx_fifo);	
		if (word.flags == (FLAG_MODE | MODE_LISTEN)){
			//listen for a byte requested
			word = iecGetByte();
			AvrXWaitPutFifo(iec_rx_fifo, word);
		}else if(word.flags == (FLAG_MODE | MODE_RELEASE)){
			//system line release requested
			CLKOUT_SET;
			DATAOUT_SET;			
			while(1);
		}else if(word.flags == (FLAG_MODE | MODE_TALK_RELEASE)){
			//do turnaround sequence and then release
			//this is used, when there is an error on opening a file for reading
			iecTurnAround();
			CLKOUT_SET;
			DATAOUT_SET;
			while(1);
		}else{
			//default: talk mode - send the word
			if(mode == MODE_LISTEN){
				//do turnaround if we were listening
				iecTurnAround();
				mode = MODE_TALK;
			}
			iecPutByte(word);
			AvrXWaitPutFifo(iec_rx_fifo, (iec_word_t){0,FLAG_TXOK});			
		}
	}	
}

Mutex atnMutex;

AVRX_SIGINT(SIG_INTERRUPT0){
	/* signal holdoff */
	DATAOUT_CLEAR;
	/* release line for talker */
	CLKOUT_SET;
	
	IntProlog();
	
	/* wait for talker holdoff */
	loop_until_bit_is_clear (PINB, IEC_CLK_IN);
	
	AvrXIntSetSemaphore(&atnMutex);
	Epilog();                // Return to tasks
}

AVRX_GCC_TASKDEF(iecAtnTask, 100, 2){
	
	while(1){
		AvrXWaitSemaphore(&atnMutex);
		AvrXTerminate(PID(iecComTask));
		//Turn Timer int on again, in case it was left 
		//off by iecComTask
		//TIMSK |= _BV(TOIE0);
#ifdef DEBUG
		//same for UART int
		UCSR0B |= (1<<UDRIE0);
#endif		
		AvrXFlushFifo(iec_tx_fifo);
		
		iec_word_t word;
		word.flags = FLAG_BREAK;
		word.data = 0;
		AvrXWaitPutFifo(iec_rx_fifo, word);
		
		AvrXRunTask(TCB(iecComTask));
	}	
}

void iec_hw_init(){
	AvrXResetSemaphore(&atnMutex);
	AVRX_INIT_FIFO(iec_tx_fifo);
	AVRX_INIT_FIFO(iec_rx_fifo);
}
