/*
     IEC-ATA, a hard drive controller for the CBM IEC bus 
     Copyright (C) 2002  Asbjørn Djupdal
     
     This program is free software; you can redistribute it and/or
     modify it under the terms of the GNU General Public License
     as published by the Free Software Foundation; either version 2
     of the License, or (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

     ----

     The author of IEC-ATA may be reached by electronic mail:
     
       djupdal@idi.ntnu.no

     or if the email address no longer is valid, by paper mail:

       Asbjørn Djupdal
       Grønngjelet 50
       4640 SØGNE
       NORWAY
	   
	   Edit by tixiv@das-labor.org
       and by www.dienstagtreff.de
*/

/*
  iec.c V1.3
  Contains the IEC serial bus protocol code
*/

#include "iecata.h"

/*
old config

#define JUMPER  PB0

#define IEC_CLK_OUT  PD0
#define IEC_DATA_OUT PD1
#define IEC_ATN_IN   PD2
#define IEC_CLK_IN   PD3
#define IEC_DATA_IN  PD4

*/

#define IEC_CLK_OUT  PB2
#define IEC_DATA_OUT PB4
#define IEC_ATN_IN   PD2
#define IEC_CLK_IN   PB3
#define IEC_DATA_IN  PB5

#define LISTEN    0x20
#define TALK      0x40
#define UNLISTEN  0x3f
#define UNTALK    0x5f

#define OPEN      0xf0
#define CLOSE     0xe0
#define DATA      0x60


#define CLKOUT_SET PORTB |= (1<<IEC_CLK_OUT)
#define CLKOUT_CLEAR PORTB &= ~(1<<IEC_CLK_OUT)

#define DATAOUT_SET PORTB |= (1<<IEC_DATA_OUT)
#define DATAOUT_CLEAR PORTB &= ~(1<<IEC_DATA_OUT)

#define ATTENTION_OFF() attention_delay = 1;
#define ATTENTION_ON() \
	attention_delay = 0;\
	if(attention){\
		attention = 0;\
		iecAttention();\
	}
/* protos ****************************************************************/

static bool_t iecGetByte (uint8_t *data);
static void iecPutByte (uint8_t data, bool_t eoi);
static void iecDelay (void);
static void iecDelay80 (void);

/* variables *************************************************************/

volatile bool_t attention;
volatile bool_t attention_delay;
volatile uint8_t speed = 60;
volatile uint8_t DEVICENUMBER = 9;

/* functions *************************************************************/

iecListen_r iecListen (uint8_t *data, bufferSize_t maxBytes) {
	bool_t eoi = FALSE;

	/* init bytes received */
	bufferSize_t bytesReceived = 0;

	while (!eoi && (bytesReceived < maxBytes)) {
		/* wait for talker ready to send */
		loop_until_bit_is_set (PINB, IEC_CLK_IN);
		/* fetch next byte */
		eoi = iecGetByte (data + bytesReceived++);
	}
	
	/* Release line after a suitable (?) pause */
	/* TODO: find the correct delay.
	   This seems to cause errors sometimes */
	if (eoi) {
		iecDelay80();
		DATAOUT_SET;
	}    

	return (iecListen_r){bytesReceived, eoi};
}


inline extern void iecTalk ( struct channelTableStruct * channel, bool_t eoi) {
	volatile bufferSize_t (*bufferPtr);
	bufferPtr = &channel->bufferPtr; 

	while (*bufferPtr < channel->endOfBuffer) {
		
		if ( (channel->endOfBuffer - (*bufferPtr)) == 1 ) {
			/* send last byte */
			iecPutByte (channel->buffer[*bufferPtr], eoi);
		} else {
			/* send byte */
			iecPutByte (channel->buffer[*bufferPtr], FALSE);
		}
		/* increment byte counter */
		(*bufferPtr)++;
	}
	/* Release line after a suitable (?) pause */
	/* TODO: find the correct delay.
	   This seems to cause errors sometimes */
	if (eoi) {
		iecDelay();
		CLKOUT_SET;
	}    
}


void iecUnAttention(){
	TCNT0 = 0;
	while(!bit_is_set (PIND, IEC_ATN_IN)){
		if(TCNT0>60){
			/* release IEC DATA */
			DATAOUT_SET;
		}
	}
	DATAOUT_SET;
	sei();
}

inline extern void iecAttention (void) {
	uint8_t data[2];
	uint8_t idx=0, release=0;
		
	cli();
	
	/* default command */
	command = IDLE;
	
	while(1){
		if (bit_is_set (PIND, IEC_ATN_IN)){
			/* quit interrupt when IEC ATN is released */
			if(release){
				DATAOUT_SET;
			}
			sei();
			return;
		}
		/* fetch new byte if IEC CLK is released */
		if (bit_is_set (PINB, IEC_CLK_IN)) {
			iecGetByte (&data[idx]);
			if(idx == 0){
				if ((data[0] & 0x1f) == DEVICENUMBER){
					
				}else if ((data[0] == UNLISTEN) || (data[0] == UNTALK)){
					release = 1;
				}else{
					iecUnAttention();
					return;
				}	
			}else if(idx == 1){
				channelNumber = data[1] & 0x0f;
				if ((data[0] == UNLISTEN) || (data[0] == UNTALK)){
				
				}else if ((data[0] & 0xe0) == LISTEN) {
					if ((data[1] & 0xf0) == OPEN) {
						command = LISTEN_OPEN;
						error = 0;
					} else if ((data[1] & 0xf0) == CLOSE) {
						if (!error) {
							command = LISTEN_CLOSE;
						}
						/* release IEC DATA */
						release = 1;
					} else if ((data[1] & 0xf0) == DATA) {
						if (error) {
							/* release IEC DATA */
							release = 1;
						} else {
							command = LISTEN_DATA;
						}
					}
				} else if ((data[0] & 0xe0) == TALK) {
					/* do turn-around sequence */
					loop_until_bit_is_set (PINB, IEC_ATN_IN);
					/* wait for IEC CLK */
					loop_until_bit_is_set (PINB, IEC_CLK_IN);
					/* release IEC DATA */
					DATAOUT_SET;
					/* delay */
					//iecDelay();
					/* take IEC CLK */
					CLKOUT_CLEAR;
					/* delay */
					iecDelay80();
					if (!error || (channelNumber == COMMAND_CHANNEL)) {
						command = TALK_DATA;
					} else {
						/* release IEC CLK */
						CLKOUT_SET;
					}
				} else {
					release = 1;
				}
			}
			idx++;
		}
	}
}


bool_t iecGetByte (uint8_t *data) {
	bool_t eoi = FALSE;

	/* signal ready to listen */
	DATAOUT_SET;
    
	/* wait for all other devices */
	loop_until_bit_is_set (PINB, IEC_DATA_IN);

	/* wait for talker, maximum 230us */
	TCNT0 = 0;
	while (!eoi) {
		if (TCNT0 > 230) {
			eoi = TRUE;
		}
		if (bit_is_clear (PINB, IEC_CLK_IN)) {
			break;
		}
	}
  
	/* acknowledge if eoi */
	if (eoi) {
		/* set IEC DATA */
		DATAOUT_CLEAR;
		/* delay */
		iecDelay80();
		/* clear IEC DATA */
		DATAOUT_SET;;
		/* wait for talker */
		loop_until_bit_is_clear (PINB, IEC_CLK_IN);
	}

	{ /* fetch byte */
		uint8_t bitnumber;
    
		for (bitnumber = 0; bitnumber < 8; bitnumber++) {
			/* shift result right */
			*data >>= 1;

			/* wait for next bit */
			loop_until_bit_is_set (PINB, IEC_CLK_IN);
			/* fetch bit */
			if (bit_is_set (PINB, IEC_DATA_IN)) {
				*data |= 0x80;
			}

			/* wait for IEC CLK */
			loop_until_bit_is_clear (PINB, IEC_CLK_IN);
		}

		/* acknowledge byte received */
		DATAOUT_CLEAR;
	}
	
	return eoi;
}

void iecPutByte (uint8_t data, bool_t eoi) {

	/*Talker ready to send */
	CLKOUT_SET;
	/* wait for listener */
	
	loop_until_bit_is_set (PINB, IEC_DATA_IN);
	
	if (eoi) {
	/* wait for listener EOI Timeout Handshake */
	loop_until_bit_is_clear (PINB, IEC_DATA_IN);
	/* wait for Listener Ready-for-Data */
	loop_until_bit_is_set (PINB, IEC_DATA_IN);
	}
	
	/* Talker sending */
	CLKOUT_CLEAR;
	loop_until_bit_is_set (PINB, IEC_DATA_IN);
	{ /* send byte */
		
	uint8_t bitnumber;
		
		for (bitnumber = 0; bitnumber < 8; bitnumber++) {
			/*if(bit_is_set (PINB, IEC_DATA_IN)){break}*/
			
			iecDelay80();
			/* put data on line */
			if (data & 0x01) {
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
			data >>= 1;
			
		}
		
	/* wait for listener data accepted */
	loop_until_bit_is_clear (PINB, IEC_DATA_IN);
	/* TODO: timeout */
	CLKOUT_CLEAR;
	}

}

static void iecDelay (void) {
	TCNT0 = 0;
	/* slow (C64) timing: > 60us */
	while (TCNT0 < speed);
}



static void iecDelay80 (void) {
	TCNT0 = 0;
	while (TCNT0 < 80);
}

/*
static void iecDelay20 (void) {
	TCNT0 = 0;
	while (TCNT0 < 80);
}
*/

extern jmp_buf attention_jmp;
SIGNAL(SIG_INTERRUPT0) {

	/* wait until host is ready */
	TCNT0 = 0;
	while (TCNT0 < 5);
	loop_until_bit_is_clear (PINB, IEC_CLK_IN);
	/* release IEC CLK */
	CLKOUT_SET;

	/* set IEC DATA */
	DATAOUT_CLEAR;
	/*if we may not handle Attention immediatly*/
	if(attention_delay){
		/* set attention flag */
		attention = TRUE;
	}else{
		/* forget what we were doing, and handle Attention now */
		longjmp(attention_jmp,1);
	}
}
