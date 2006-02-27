
#include <avrx-io.h>
#include <avrx.h>
#include "fifo.h"

#define IEC_HW_C
#include "iec_hw.h"

//#define MONITOR_MODE

#define IEC_CLK_OUT  PB2
#define IEC_DATA_OUT PB4
#define PIN_ATN_IN PIND
#define BIT_ATN_IN   PD2
#define IEC_CLK_IN   PB3
#define IEC_DATA_IN  PB5


#define LED_BIT 2
#define LED_PORT PORTE


/* Makros for setting and clearing bus lines */
#define CLKOUT_SET PORTB |= (1<<IEC_CLK_OUT)
#define CLKOUT_CLEAR PORTB &= ~(1<<IEC_CLK_OUT)

#define DATAOUT_SET PORTB |= (1<<IEC_DATA_OUT)
#define DATAOUT_CLEAR PORTB &= ~(1<<IEC_DATA_OUT)


Mutex iec_tx_mutex, iec_rx_mutex;

static void iecDelay80 (void) {
	TCNT2 = 0;
	while (TCNT2 < 100);
}

iec_word_t iecGetByte() {
	iec_word_t word;
	word.data = 0;
	word.flags= 0;

	/* wait for Talker */
	loop_until_bit_is_set (PINB, IEC_CLK_IN);
	
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
  
	{/* check ATN */
		if(bit_is_clear (PIN_ATN_IN, BIT_ATN_IN)){
			word.flags |= FLAG_ATN;
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
#		ifndef MONITOR_MODE
		DATAOUT_CLEAR;
#		endif
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
	//loop_until_bit_is_set (PINB, IEC_DATA_IN);
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
	}
}

volatile uint8_t iec_mode;
volatile iec_word_t iec_tx_word, iec_rx_word;

AVRX_GCC_TASKDEF(iecComTask, 200, 3){
	LED_PORT ^= (1<<LED_BIT);
	iec_mode = LISTEN;
	while(iec_mode == LISTEN){
			iec_rx_word = iecGetByte();
			AvrXSetSemaphore(&iec_rx_mutex);
	};
	if(iec_mode == TALK){
		iecTurnAround();
	}
	while(iec_mode == TALK){
		AvrXWaitSemaphore(&iec_tx_mutex);
		iecPutByte(iec_tx_word);
		iec_rx_word.flags = FLAG_TXOK;
		AvrXSetSemaphore(&iec_rx_mutex);
	}
	CLKOUT_SET;
	DATAOUT_SET;			
	while(1);	
}

Mutex atnMutex;

AVRX_SIGINT(SIG_INTERRUPT0){
	/* set IEC DATA */
	DATAOUT_CLEAR;
	/* release IEC CLK */
	CLKOUT_SET;
	
	IntProlog();
	
	//TCNT2 = 0;
	//while (TCNT2 < 5);
	//loop_until_bit_is_clear (PINB, IEC_CLK_IN);
	
	/* wait for talker holdoff */
	loop_until_bit_is_clear (PINB, IEC_CLK_IN);

	AvrXIntSetSemaphore(&atnMutex);
	Epilog();                // Return to tasks
}

AVRX_GCC_TASKDEF(iecAtnTask, 100, 1){
	
	while(1){
		AvrXWaitSemaphore(&atnMutex);
		AvrXTerminate(PID(iecComTask));
		
		iec_rx_word.flags = FLAG_BREAK;
		iec_rx_word.data = 0;
		AvrXSetSemaphore(&iec_rx_mutex);
		
		AvrXRunTask(TCB(iecComTask));
	}	
}

void iec_hw_init(){
	AvrXResetSemaphore(&atnMutex);
	AvrXResetSemaphore(&iec_rx_mutex);
	AvrXResetSemaphore(&iec_tx_mutex);
}
