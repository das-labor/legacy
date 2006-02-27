
/*
#define LISTEN    0x20
#define TALK      0x40
#define UNLISTEN  0x3f
#define UNTALK    0x5f

#define OPEN      0xf0
#define CLOSE     0xe0
#define DATA      0x60
*/

#define DEVICENUMBER 9

#include <avrx-io.h>
#include <avrx.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "AvrXSerialIo.h"
#include "iec_hw.h"

void iec_talk(uint8_t param){
	uint8_t channel = param & 0x0f;
	uint8_t command = param & 0xf0;
	
	
	iec_tx_word.flags = FLAG_EOI;
	iec_tx_word.data = 65;
	AvrXSetSemaphore(&iec_tx_mutex);
	AvrXWaitSemaphore(&iec_rx_mutex);
	iec_mode = 0;
	
}

void iec_listen(uint8_t param){
	uint8_t channel = param & 0x0f;
	uint8_t command = param & 0xf0;
	iec_word_t w;
	do{
		AvrXWaitSemaphore(&iec_rx_mutex);
		w = iec_rx_word;
		//printf_P(PSTR("d %x\r"), *(uint16_t*)&w);
	}while(w.flags == 0);
	iec_mode = 0;
}


AVRX_GCC_TASKDEF(iec_task, 200, 2)
{
	TimerControlBlock timer;

	InitSerial0(BAUD(38400));
    fdevopen(put_char0, get_c0, 0);		// Set up standard I/O

	while(1)
	{
		iec_word_t word;
		
		printf_P(PSTR("<IECATA>\r"));
		
		while(1){
			AvrXWaitSemaphore(&iec_rx_mutex);
			word = iec_rx_word;
			//printf_P(PSTR("%x\r"), word);
			if(word.flags & FLAG_BREAK){
				/* do nothing */
			}else if(word.flags & FLAG_ATN){
					if (word.data == UNLISTEN || word.data == UNTALK){
						/* release bus */
						iec_mode = 0;
					}else if((word.data & 0x0f) == DEVICENUMBER){
						uint8_t mode = word.data & 0xf0;
						AvrXWaitSemaphore(&iec_rx_mutex);
						word = iec_rx_word;
						//printf_P(PSTR("%x\r"), word);
						if(word.flags & FLAG_ATN){
							iec_mode = mode;
							if(mode == TALK){
								iec_talk(word.data);	
							}else if(mode == LISTEN){
								iec_listen(word.data);	
							}
						}
					}else{
						/* release bus */
						iec_mode = 0;
					}
				
			}else{
				/* Middle of Data Transfer? */
				iec_mode = 0;	
			}
			
			
		}
		
	}
}
