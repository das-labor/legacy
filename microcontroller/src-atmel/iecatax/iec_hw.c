
#include <avrx-io.h>
#include <avrx.h>
#include "fifo.h"

#define IEC_HW_C
#include "iec_hw.h"

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

AVRX_DECL_FIFO(iec_rx_fifo, 5);
AVRX_DECL_FIFO(iec_tx_fifo, 5);


static void iecDelay80 (void) {
	TCNT2 = 0;
	while (TCNT2 < 80);
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
			//DATAOUT_CLEAR;
			iecDelay80();
			//DATAOUT_SET;
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
		//DATAOUT_CLEAR;
	}
	return word;
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

AVRX_GCC_TASKDEF(iecComTask, 200, 3){
	uint16_t x;
	LED_PORT ^= (1<<LED_BIT);
	iec_word_t word;
	iec_mode = MODE_LISTEN;
	while(1){
		if(iec_mode == MODE_LISTEN){
			word = iecGetByte();
			AvrXWaitPutFifo(iec_rx_fifo, *(uint16_t*)&word);
		}else if(iec_mode == MODE_TALK){
			uint16_t x;
			x = AvrXWaitPullFifo(iec_tx_fifo);
			iecPutByte(*(iec_word_t*)&x);
		}else{
			CLKOUT_SET;
			DATAOUT_SET;			
		}	
	}
}

Mutex atnMutex;

AVRX_SIGINT(SIG_INTERRUPT0){
	IntProlog();
	
	TCNT2 = 0;
	while (TCNT2 < 5);
	loop_until_bit_is_clear (PINB, IEC_CLK_IN);
	/* set IEC DATA */
	DATAOUT_CLEAR;
	/* release IEC CLK */
	CLKOUT_SET;
	/* wait for talker holdoff */
	loop_until_bit_is_clear (PINB, IEC_CLK_IN);

	AvrXIntSetSemaphore(&atnMutex);
	Epilog();                // Return to tasks
}

AVRX_GCC_TASKDEF(iecAtnTask, 100, 2){
	
	while(1){
		AvrXWaitSemaphore(&atnMutex);
		AvrXTerminate(PID(iecComTask));
		AvrXWaitPutFifo(iec_rx_fifo, *(uint16_t*)&(iec_word_t){0,FLAG_BREAK});
		AvrXRunTask(TCB(iecComTask));
	}	
}

void iec_hw_init(){
	AvrXResetSemaphore(&atnMutex);
	AVRX_INIT_FIFO(iec_tx_fifo);
	AVRX_INIT_FIFO(iec_rx_fifo);
	
}
