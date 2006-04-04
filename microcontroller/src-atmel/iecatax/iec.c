
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
#include "iec_hw.h"
#include "config.h"
#include "types.h"
#include "AvrXSerialIo.h"
#include "iecata.h"
#include "dos.h"
#include "iec.h"

volatile uint8_t error;

// get bytes to buffer from iec bus starting at *index
// *index is updated
// Returns 0 when Buffer is full or
// iec flags, if terminated by bus
uint8_t iecListen(uint8_t *buf, uint16_t *index, uint16_t size){
	iec_word_t word;
#ifdef DEBUG
		printf_P(PSTR("listen:\r"));
#endif

	while(size--){
		//Ask iec_hw task to receive a byte
		AvrXPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_LISTEN});
		//wait for our byte
		word = AvrXWaitPullFifo(iec_rx_fifo);
		//The byte can be buffered, if no flag but EOI is set.
		if(!(word.flags & (~FLAG_EOI))){
			buf[*index] = word.data;
			(*index)++;
		}
#ifdef DEBUG
		printf_P(PSTR(" %x"), *(uint16_t*)&word);
		if ((*index)%16 == 0)
			printf_P(PSTR("\r"));		
#endif
		if (word.flags & FLAG_EOI){
			//EOI received, so we can release the bus.
			AvrXPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
		}
		
		if (word.flags){
			//we've got flags, so we end the transmission
			return word.flags;	
		}
	}
	return 0;
}




uint8_t iecTalk ( struct channelTableStruct * channel, bool_t eoi) {
	volatile bufferSize_t (*bufferPtr);
	bufferPtr = &channel->bufferPtr; 

	iec_word_t tx_word, rx_word;
	
	tx_word.flags = 0;
	
	while (*bufferPtr < channel->endOfBuffer) {
		
		if (( (channel->endOfBuffer - (*bufferPtr)) == 1 ) && eoi)
			tx_word.flags = FLAG_EOI;
		
		tx_word.data = channel->buffer[*bufferPtr];
		
#ifdef DEBUG		
		printf_P(PSTR(" %x"), tx_word.data);
#endif

		// start transmission
		AvrXWaitPutFifo(iec_tx_fifo, tx_word);
		/* wait for result */
		rx_word = AvrXWaitPullFifo(iec_rx_fifo);
			
		if(tx_word.flags & FLAG_EOI){
			//release bus if this was the last byte.
			AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
		}
		
		if( !(rx_word.flags & FLAG_TXOK) ){
#ifdef DEBUG		
			printf_P(PSTR("!"));			
#endif
			return rx_word.flags;
		}
		
#ifdef DEBUG		
		printf_P(PSTR("."));
		if (!(*bufferPtr % 16))
			printf_P(PSTR("\r"));		
#endif

		/* increment byte counter */
		(*bufferPtr)++;
	}
	return 0;    
}

void fileToIec(uint8_t channelNumber){
	struct channelTableStruct *channel = &channelTable[channelNumber];
	bool_t eof = 0;
	while (!eof) {
		
		/* get new block of data */
		if (channel->bufferPtr == channel->endOfBuffer) {
			/* start new block at beginning */
			channel->bufferPtr = 0;
			eof = FALSE;
	
			if (channelNumber == COMMAND_CHANNEL) {
				eof = readStatus (channel);
			} else if (channel->readDirState) {
				eof = readDir (channelNumber);
			} else {
				readFile (channelNumber, &eof);
				if (eof) {
					channel->endOfBuffer = channel->dirEntry.bytesInLastBlock;
				} else {
					channel->endOfBuffer = BLOCKSIZE;
				}
			}
		}

		/* send data block */
		if (iecTalk (channel, eof)){
			return;
		}
	}
}

	
void iecToFile(struct channelTableStruct *channel){
	if (channel->fileState == WRITE_FILE) {
		uint8_t flags;
		do {
			//bufPtr shows where we are in the buffer right now
			bufferSize_t *bufPtr = &(channel->bufferPtr);
       	 
			/* receive bytes, *bufPtr is updated. */
			flags = iecListen (channel->buffer, bufPtr, BLOCKSIZE - *bufPtr);

			/* If the buffer is full, write it to disk and begin new buffer */
			if (*bufPtr == BLOCKSIZE) { 
				writeFile (channel);
				*bufPtr = 0;
			}
		} while (!flags);
	} else {
		error = NOT_OPEN_ERROR;
		/* release bus */
		AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
	}
}

void iec_execute(uint8_t mode, uint8_t param){
	uint8_t channelNumber = param & 0x0f;
	uint8_t command = param & 0xf0;
		
	/* get pointer to channel structure */
	struct channelTableStruct *channel;
	channel = &channelTable[channelNumber];
	
	if(mode == TALK){
#ifdef DEBUG
		printf_P(PSTR("talk c %d\r"), channelNumber);
#endif
		if (!error || (channelNumber == COMMAND_CHANNEL)) {
			fileToIec(channelNumber);
		}else{
			AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_TALK_RELEASE});
		}
	}else if(mode == LISTEN){
		if(command == OPEN){
			/* reset variables */
			error = 0;
			channel->bufferPtr = 0;
			channel->endOfBuffer = 0;
			if (channelNumber == COMMAND_CHANNEL) {
				/* get command and execute it */
				parseCommand();
			} else {
				/* normal data channel, get file name and open file */
				parseName (channelNumber);
			}
		}else if(command == CLOSE){
			if (channelNumber == COMMAND_CHANNEL) {
				/* close all files */
				uint8_t i;
				for (i = 0; i < 15; i++) {
					closeFile (i);
				}
			} else {
				/* close requested file */
				closeFile (channelNumber);
			}
		}else if(command == DATA){
			if (channelNumber == COMMAND_CHANNEL) {
				/* status channel must be reset before each command */
				channel->bufferPtr = 0;
				channel->endOfBuffer = 0;
				parseCommand();
			}else{
				iecToFile(channel);	
			}
		}else{
			AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
		}
	}else{
		AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
	}
}

//This task runs all the command parsing and Data transfer logic.
//It communicates with the iec_hw_task using two fifos:
// iec_rx_fifo and iec_tx_fifo.
//The routines for the ATA device are just called from this function.
//The iec_hw routine always blocks on iec_tx_fifo waiting for new instructions from
//this task. 
AVRX_GCC_TASKDEF(iec_task, 200, 1)
{
#ifdef DEBUG
	InitSerial0(BAUD(37000));
    fdevopen(put_char0, 0, 0);		// Set up standard I/O
#endif
	

	iec_word_t word;
#ifdef DEBUG
	printf_P(PSTR("<IECATA>\r"));
#endif

	while(1){
		//wait for first byte received, this is supposed to be either
		//FLAG_BREAK or the first byte under ATN
		word = AvrXWaitPullFifo(iec_rx_fifo);
		
#ifdef DEBUG
		printf_P(PSTR("\ra1: %x\r"), word);
#endif
		if(word.flags & FLAG_BREAK){
			// Break received, so we just wait for the next byte
		}else if(word.flags & FLAG_ATN){
				if (word.data == UNLISTEN || word.data == UNTALK){
					/* release bus */
					AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
				}else if((word.data & 0x1f) == DEVICENUMBER){
					//o.k. - so the master addresses us
					//the mode the master requests is in the first byte
					uint8_t mode = word.data & 0xf0;
					
					//get second byte (command and channel) from master
					AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_LISTEN});
					word = AvrXWaitPullFifo(iec_rx_fifo);
					
#ifdef DEBUG
					printf_P(PSTR("a2: %x\r"), word);
#endif
					if(word.flags & FLAG_ATN){
						//Now we've got our command, and can execute it.
						iec_execute(mode, word.data);
					}else{
						//No ATN? Something is wrong. Release Bus.
						AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});	
					}
				}else{
					//The Command is not for us. Release Bus
					AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});
				}
			
		}else{
			/* Middle of Data Transfer? We should never get here. */
			AvrXWaitPutFifo(iec_tx_fifo, (iec_word_t){0, FLAG_MODE|MODE_RELEASE});	
		}
		
	}
}

TimerControlBlock led_timer;

AVRX_GCC_TASKDEF(led_task, 20, 10)
{
	while(1){
		AvrXDelay(&led_timer, 1000);

		}
}
