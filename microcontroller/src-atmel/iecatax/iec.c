
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
	while(size--){
		AvrXWaitSemaphore(&iec_rx_mutex);
		word = iec_rx_word;
		if(!(word.flags & (~FLAG_EOI))){
			buf[*index] = word.data;
			(*index)++;
		}
#ifdef DEBUG
		printf_P(PSTR("l %x"), *(uint16_t*)&word);
#endif
		if (word.flags) return word.flags;
	}
	return 0;
}




uint8_t iecTalk ( struct channelTableStruct * channel, bool_t eoi) {
	volatile bufferSize_t (*bufferPtr);
	bufferPtr = &channel->bufferPtr; 

	iec_tx_word.flags = 0;
	
	while (*bufferPtr < channel->endOfBuffer) {
		
		if (( (channel->endOfBuffer - (*bufferPtr)) == 1 ) && eoi)
			iec_tx_word.flags = FLAG_EOI;
		
		iec_tx_word.data = channel->buffer[*bufferPtr];
		/* start transmission */
		AvrXSetSemaphore(&iec_tx_mutex);
		/* wait for result */
		AvrXWaitSemaphore(&iec_rx_mutex);
	
		if( !(iec_rx_word.flags & FLAG_TXOK) ) return iec_rx_word.flags;

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
		iec_mode = 0;
	}
}

void iec_execute(uint8_t mode, uint8_t param){
	uint8_t channelNumber = param & 0x0f;
	uint8_t command = param & 0xf0;
		
	/* get pointer to channel structure */
	struct channelTableStruct *channel;
	channel = &channelTable[channelNumber];
	
	if(mode == TALK){
		printf_P(PSTR("t %d"), channelNumber);
		if (!error || (channelNumber == COMMAND_CHANNEL)) {
			iec_mode = MODE_TALK;
			fileToIec(channelNumber);
		}else{
			iec_mode = MODE_TALK_RELEASE;
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
			iec_mode = 0;	
		}
	}else{
		iec_mode = 0;	
	}
}


AVRX_GCC_TASKDEF(iec_task, 200, 1)
{
#ifdef DEBUG
	InitSerial0(BAUD(37000));
    fdevopen(put_char0, get_c0, 0);		// Set up standard I/O
#endif
	

	iec_word_t word;
	
#ifdef DEBUG
	printf_P(PSTR("<IECATA>\r"));
#endif

	while(1){
		AvrXWaitSemaphore(&iec_rx_mutex);
		word = iec_rx_word;
#ifdef DEBUG
		printf_P(PSTR("%x\r"), word);
#endif
		if(word.flags & FLAG_BREAK){
			/* do nothing */
		}else if(word.flags & FLAG_ATN){
				if (word.data == UNLISTEN || word.data == UNTALK){
					/* release bus */
					iec_mode = 0;
				}else if((word.data & 0x1f) == DEVICENUMBER){
					uint8_t mode = word.data & 0xf0;
					AvrXWaitSemaphore(&iec_rx_mutex);
					word = iec_rx_word;
#ifdef DEBUG
					printf_P(PSTR("a %x\r"), word);
#endif
					if(word.flags & FLAG_ATN){
						/* do the transfer requested */
						iec_execute(mode, word.data);
					}else{
						iec_mode = 0;	
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
