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
	   
	   Edit by skernsoft@video-studio-klettke.de*/

/*
  iecata.c V1.3
  The main source file for the IEC-ATA software, contains main()
*/

#include "iecata.h"

/* include c-files instead of linking; saves program space because
   functions may be declared inline and extern */


#define LED_PIN 2

#define LED_IS_ON ((PORTE&(1<<LED_PIN))?0:1)
#define LED_IS_OFF ((PORTE&(1<<LED_PIN))?1:0)
#define LED_OFF PORTE |= (1<<LED_PIN);
#define LED_ON PORTE &= ~(1<<LED_PIN);

#include "ata.c"
#include "iec.c"
#include "dos-file.c"
#include "dos-dir.c"
#include "dos-init.c"

#define BASIC_LINE_LENGTH    33 /* TODO: check this number */
#define CDOS_DIRENTRY_LENGTH 32

/* error numbers, given in bcd */
#define NO_ERROR         0x00
#define INIT_ERROR       0x74
#define CREATE_ERROR     0x01
#define NOT_OPEN_ERROR   0x61
#define NOT_FOUND_ERROR  0x62
#define SYNTAX_ERROR     0x30
#define VERSION_ERROR    0x73

/* protos ****************************************************************/

void init (void);
void updateLED (void);
bool_t readStatus (struct channelTableStruct *channel);
bool_t readDir (struct channelTableStruct *channel);
static void parseCommand (void);
void parseName (struct channelTableStruct *channel);
uint8_t filenamelen (char *filename);

int main (void);

/* variables *************************************************************/

volatile uint8_t command;
volatile uint8_t error;
volatile uint8_t channelNumber;
jmp_buf attention_jmp;

/* functions *************************************************************/


void init_xram (void) __attribute__ ((naked)) \
    __attribute__ ((section (".init1")));

void
init_xram (void)
{
	/* enable external SRAM */
	MCUCR = (1<<SRE);
}


void init (void) {
  MCUCR = (1<<SRE);
  /* set up I/O pins */

  DDRA = 0;
  PORTA = 0;  
  
  DDRB = 0x57;
  PORTB = 0x57;
  
  DDRC = 0;
  PORTC = 0;
 
  DDRD = 0xFB;
  PORTD = 0xFB;
  
  DDRE = 0x04;
  PORTE = 0x04;
  
  
  /* enable timer */
  TCCR0 = (1<<CS01); // clk/8

  /* setup interrupt */
  sbi (MCUCR, ISC01);
  cbi (MCUCR, ISC00);
  sbi (GICR, INT0);

  /* init variables */
  command = IDLE;
  attention = FALSE;
  attention_delay = FALSE;
  
  LED_ON;
  
  error = INIT_ERROR;
  /* init submodules */
  if (ataInit()) {
    if (dosInit()) {
      error = VERSION_ERROR;
    }
  }

  /* turn off LED */
  LED_OFF;

  /* Enable interrupts */
  sei();
}

void updateLED (void) {
	/* blink LED if error */
	if (error && (error != VERSION_ERROR)) {
		static uint16_t cnt;
      
		if (cnt > 600) {
			PORTE ^= (1<<LED_PIN);
			cnt = 0;
		}
		/* timer delay */
		TCNT0 = 0;
		while ( TCNT0 < 200);
		/* update counter */
		cnt++;
	}else if(command != IDLE){
		LED_ON;
	}else{
		LED_OFF;
	}
}

typedef struct{
	uint8_t num;
	const prog_char *string;
}error_t;

prog_char NO_ERROR_STR[] = " OK";
prog_char INIT_ERROR_STR[] = "INIT ERROR";
prog_char CREATE_ERROR_STR[] = "CREATE ERROR";
prog_char NOT_OPEN_ERROR_STR[] = "NOT OPEN";
prog_char NOT_FOUND_ERROR_STR[] = "NOT FOUND";
prog_char VERSION_ERROR_STR[] = "IEC-ATA V1.3";
prog_char SYNTAX_ERROR_STR[] = "SYNTAX ERROR";


error_t errors[] PROGMEM = {
	{NO_ERROR, NO_ERROR_STR},
	{INIT_ERROR, INIT_ERROR_STR},
	{CREATE_ERROR, CREATE_ERROR_STR},
	{NOT_OPEN_ERROR, NOT_OPEN_ERROR_STR},
	{NOT_FOUND_ERROR, NOT_FOUND_ERROR_STR},
	{VERSION_ERROR, VERSION_ERROR_STR},
	{SYNTAX_ERROR, SYNTAX_ERROR_STR}
};

#define PW(a) pgm_read_word(&(a))
#define PB(a) pgm_read_byte(&(a))


bool_t readStatus (struct channelTableStruct *channel) {
	uint8_t myerror = error;
	
	uint8_t *buffer = channel->buffer;
	uint8_t x;

	/* error number */
	*(buffer++) = (myerror >> 4) | '0';
	*(buffer++) = (myerror & 0x0f) | '0';
		
	/* space */
	*(buffer++) = ',';

	/* error message */

	for(x=0;x < (sizeof(errors)/sizeof(error_t)) ;x++){
		if(myerror == PB(errors[x].num) ){
			buffer += strlcpy_P (buffer, (char*)PW(errors[x].string), 20);
		}
	}
    
	/* clear error */
	error = NO_ERROR;

	/* track and sector (not used; always 0,0) */
	memcpy_P (buffer, PSTR (",00,00\x0d"), 7);
	buffer += 7;

	/* record buffer length */
	channel->endOfBuffer = buffer - channel->buffer;

	return TRUE;
}

bool_t readDir (struct channelTableStruct *channel) {
  bool_t eof = FALSE;
  uint8_t *buffer = channel->buffer;
  static entryIndex_t entryIndex;
  static uint8_t writtenEntries;

  if (channel->readDirState == READ_DIR_BEGIN) {
    if (channelNumber == 0) {
      memcpy_P (buffer, PSTR ("\x01\x04\x01\x01\x00\x00"
                              "\x12\"IEC-ATA V1.3    \" AD 2A\x00"), 32);
      buffer += 32;
    } else {
      memset (buffer, 255, 142);
      memcpy_P (buffer, PSTR ("\x41\x00"), 2);
      buffer += 142;
      memset (buffer, 0, 112);
      memcpy_P (buffer, PSTR ("IEC-ATA V1.3\xa0\xa0\xa0\xa0\xa0\xa0"
                              "AD\xa0""2A\xa0\xa0\xa0\xa0"), 27);
      buffer += 112;
    }

    /* not begin anymore */
    channel->readDirState = READ_DIR_PROGRESS;
    /* start at dir index 0 */
    entryIndex = 0;
    writtenEntries = 0;
  }

  /* put directory */
  while ((channel->readDirState == READ_DIR_PROGRESS) &&
         (buffer < (channel->buffer + BLOCKSIZE - (BASIC_LINE_LENGTH + 20)))) {

    struct dirEntryStruct *entry;

    /* get entry */
    if ((entry = getEntry (entryIndex))&&(entryIndex < 255)) {
      /* only process non-deleted files */
      if (entry->startBlock) {
        /* only show files that match pattern */
        if (filenameMatch (entry->fileName, channel->dirEntry.fileName) ||
            !(channel->dirEntry.fileName)) {
          fileSize_t fileSize = entry->fileSize;
          { /* convert fileSize to number of 254 byte blocks (like 1541) */
            fileSize *= 2;
            uint16_t extraBytes = entry->bytesInLastBlock + (fileSize * 2);
            if (fileSize) {
              fileSize--;
            }
            while (extraBytes >= 254) {
              fileSize++;
              extraBytes -= 254;
            }
          }
          if (channelNumber == 0) {
            /* pointer to next line */
            *(buffer++) = 1;
            *(buffer++) = 1;
            /* linenumber */
            *(buffer++) = (uint8_t)fileSize;
            *(buffer++) = (uint8_t)(fileSize >> 8);
            /* clear line */
            memset (buffer, ' ', BASIC_LINE_LENGTH - 5);
            /* space in beginning of line */
            if (fileSize < 1000) buffer++;
            if (fileSize < 100) buffer++;
            if (fileSize < 10) buffer++;
            { /* filename */
              uint8_t fileNameSize = filenamelen (entry->fileName);
              /* quotes */
              *(buffer++) = '"';
              /* name */
              memcpy (buffer, entry->fileName, fileNameSize);
              buffer += fileNameSize;
              /* quotes */
              *(buffer++) = '"';
              /* spaces */
              buffer += FILE_NAME_SIZE - fileNameSize;
            }
            /* splat */
            if (entry->splat) *buffer = '*';
            buffer++;
            /* filetype */
            switch (entry->fileType) {
              case PRG:
                memcpy_P (buffer, PSTR ("PRG"), 3);
                break;
              case SEQ:
                memcpy_P (buffer, PSTR ("SEQ"), 3);
                break;
              case DIR:
                memcpy_P (buffer, PSTR ("DIR"), 3);
                break;
            }
			buffer +=5;
            if (fileSize >= 1000) buffer++;
            if (fileSize >= 100) buffer++;
            if (fileSize >= 10) buffer++;
            /* TODO: locked */
            /* end of line */
            *(buffer++) = 0;
          } else {
            /* clear direntry */
            memset (buffer, 0, CDOS_DIRENTRY_LENGTH);
            /* file type */
            *buffer = entry->fileType;
			
            if (!entry->splat) *buffer |= 0x80;
            /* todo: locked */
            buffer += 3;
            /* name */
            memset (buffer, 160, FILE_NAME_SIZE);
            memcpy (buffer, entry->fileName, filenamelen (entry->fileName));
            buffer += (FILE_NAME_SIZE + 9);
            /* file size */
            *(buffer++) = (uint8_t)fileSize;
            *(buffer++) = (uint8_t)(fileSize >> 8);
            /* write to bytes of 0 if not at end of 1541 block */
            writtenEntries++;
            if (writtenEntries == 8) {
              writtenEntries = 0;
            } else {
              buffer += 2;
            }
          }
        }
      }
      entryIndex++;
	  
    } else {
      if (channelNumber == 0) {
        /* put blocks free */		
        memcpy_P (buffer,
                  PSTR ("\x01\x01\xff\xf9""BLOCKS FREE.             \x00\x00\x00"),
                  32);
        buffer += 32;
      }
      /* read dir finished */
      channel->readDirState = READ_DIR_FINISHED;
    }
  }

  if (channel->readDirState == READ_DIR_FINISHED) {
    eof = TRUE;
  }

  /* number of bytes to save */
  channel->endOfBuffer = buffer - channel->buffer;

  return eof;
}

typedef struct{
	void(*fkt)(uint8_t* arg1, uint8_t* arg2);
	const prog_char * name;
}command_t;

char cd_name[] PROGMEM="CD";

void cd_fkt(uint8_t* dir, uint8_t* dummy){
	if (!setCurrentDir (dir)) {
		error = NOT_FOUND_ERROR;
	}	
}


char cdr_name[] PROGMEM="CD/";

void cdr_fkt(uint8_t* dummy1, uint8_t* dummy2){
	setCurrentDir ("/");
}


char cdpp_name[] PROGMEM="CD_";

void cdpp_fkt(uint8_t* dummy1, uint8_t* dummy2){
	if (!setCurrentDir ("..")) {
		error = NOT_FOUND_ERROR;
	}
}


char uj_name[] PROGMEM="UJ";

void uj_fkt(uint8_t* dummy1, uint8_t* dummy2){
	error = VERSION_ERROR;
}

char md_name[] PROGMEM="MD";

void md_fkt(uint8_t* dir, uint8_t* dummy2){
	/* create directory */
	if (!createDir (dir)) {
		error = CREATE_ERROR;
	}
}

char rd_name[] PROGMEM="RD";

void rd_fkt(uint8_t* dir, uint8_t* dummy2){
	/* delete directory */
	deleteDir (dir);
}

char r_name[] PROGMEM="R";

void r_fkt(uint8_t* new, uint8_t* old){
	/* rename entry */
	if (!renameEntry (new, old)) {
		error = NOT_FOUND_ERROR;
	}
}


char s_name[] PROGMEM="S";

void s_fkt(uint8_t* file, uint8_t* dummy){
	/* delete file */
	deleteFile (file);
}

#if 0
char n_name[] PROGMEM="N";

void n_fkt(uint8_t* dummy1, uint8_t* dummy2){
	/* format drive */
	if (!formatDrive()) {
		error = INIT_ERROR;
	}
}
#endif


command_t commands[] PROGMEM={
		{&cd_fkt, cd_name},
		{&cdr_fkt, cdr_name},
		{&cdpp_fkt, cdpp_name},
		{&uj_fkt, uj_name},
		{&md_fkt, md_name},
		{&rd_fkt, rd_name},
		{&r_fkt, r_name},
		{&s_fkt, s_name},
		
};

void parseCommand (void) {
	static uint8_t command[255];
	uint8_t x;
	uint8_t *cmdArg1;
	uint8_t *cmdArg2;

  { /* get message */
	bufferSize_t bytesReceived;
    
    bytesReceived = iecListen (command, 255).bytesReceived;
		
		ATTENTION_OFF();
	
    /* make message a proper string */
    command[bytesReceived] = '\0';
  }

  /* get arg1 */
  if ((cmdArg1 = strchr (command, ':'))) {
    *(cmdArg1++) = '\0';
  }

  /* get arg2 */
  if ((cmdArg2 = strchr (cmdArg1, '='))) {
    *(cmdArg2++) = '\0';
  } else {
    cmdArg2 = cmdArg1;
  }
  
  for(x=0;x<(sizeof(commands)/sizeof(command_t));x++){
    if(! strcmp_P(command,PW(commands[x].name)) ){
		void(*fkt)(uint8_t* arg1, uint8_t* arg2);
		fkt = PW(commands[x].fkt);
		fkt(cmdArg1, cmdArg2);
		return;
	}  
  }
  
  error = SYNTAX_ERROR;
  

#if 0
  { /* erase possible CR at end of arg2 */
    uint8_t *cr;

    if ((cr = strchr (cmdArg2, 0x0d))) {
      *cr = '\0';
    }
  }
  

  { /* interpret and execute command */
	LED_ON;
 	char c1 = *command;
	char c2 = *(command + 1);
	char c3 = *(command + 2);
	char c4 = *(command + 3);
	
	switch(c1){
		case (char)'C':
				if (c2 == 'D') {
	  				char *dir;
					switch (c3){
						case '_':
							dir = "..";
							break;
						case '/':
							dir = "/";
							break;
						default:
							dir = cmdArg1;
							break;
					}
					/* change directory */
					if (!setCurrentDir (dir)) {
						error = NOT_FOUND_ERROR;
					}
				}
				break;
		case (char)'U':
				if (c2 == 'J') {
					/* reset */
					error = VERSION_ERROR;
				} else if ((c2 == '0') && (c3 == '>')) {
					/* change device */
					DEVICENUMBER = c4;
				}
				break;
		case (char)'M':
				if (c2 == 'D') {
      		/* create directory */
      		if (!createDir (cmdArg1)) {
						error = CREATE_ERROR;
					}
				}
				break;
		case (char)'R':
				if (c2 == 'D') {
					/* delete directory */
					deleteDir (cmdArg1);
				}else{
					/* rename entry */
					if (!renameEntry (cmdArg1, cmdArg2)) {
						error = NOT_FOUND_ERROR;
					}
				}
				break;
		case (char)'S':
				/* delete file */
				deleteFile (cmdArg1);
				break;
		case (char)'N':
				/* format drive */
				if (!formatDrive()) {
					error = INIT_ERROR;
				}
				break;
		case (char)'I':
				/* initialize */
				break;
		default:
				/* not a valid command */
				error = SYNTAX_ERROR;
				break;
	}
	}
#endif
}

void parseName (struct channelTableStruct *channel) {
	static uint8_t commandBuffer[255];
	uint8_t *bufferPtr = commandBuffer;

	bool_t overwrite = FALSE;
	char *filename;
	uint8_t filetype;
	bool_t read;

	{ /* get string */
		bufferSize_t bytesReceived;

    	bytesReceived = iecListen (commandBuffer, 255).bytesReceived;
		
		/*delay Attention handling until next begin of main loop*/
		ATTENTION_OFF();

		/* make buffer a proper string */
		commandBuffer[bytesReceived] = '\0';
	}

	channel->readDirState = NOT_READ_DIR;

	switch (*bufferPtr) {
		case '@':
			/* overwrite */
			overwrite = TRUE;
			bufferPtr++;
			break;
		case '$':
			/* directory */
			channel->readDirState = READ_DIR_BEGIN;
			bufferPtr++;
			break;
	}

	{ /* skip drive specifier if present */
		char *ptr;

		if ((ptr = strchr (bufferPtr, ':'))) {
			bufferPtr = ptr + 1;
		}
	}

	filename = bufferPtr;

	/* file type and direction */
	switch (channelNumber) {
		case 0:
			/* read PRG */
			filetype = PRG;
			read = TRUE;
			break;
		case 1:
			/* write PRG */
			filetype = PRG;
			read = FALSE;
			break;
		default:
			filetype = ANY; /* read anything, write SEQ */
			read = TRUE;
			break;
	}

  { /* override file type and direction with data extracted from file name */
    char *ptr = NULL;

    do {
      if ((ptr = strchr (bufferPtr, ','))) {
        *ptr = '\0'; /* to make sure filename is properly ended */
        bufferPtr = ptr + 1;

        switch (*bufferPtr) {
          case 'S':
            filetype = SEQ;
            break;
          case 'P':
            filetype = PRG;
            break;
          case 'W':
            read = FALSE;
            if (filetype == ANY) {
              filetype = SEQ;
            }
            break;
          case 'R':
            read = TRUE;
            break;
        }
      }
    } while (ptr);
  }

  if (channel->readDirState) {
    /* directory */
    if (read) {
      channel->fileState = READ_FILE;
      /* load "$" or load "$0" ==> filename = "*" */
      if (((*filename == 0) && (*(filename - 1) != ':')) ||
          ((*filename == '0') && (*(filename + 1) == 0))) {
        *filename = '*';
        *(filename + 1) = 0;
      }
      /* copy filename (to be used by pattern matching) */
      memcpy (channel->dirEntry.fileName, filename, FILE_NAME_SIZE);
    } else {
      error = CREATE_ERROR;
    }
  } else {
    /* normal file */
    if (read) {
		//hierhin kommt er
      /* open file */
      if (!openRead (filename, filetype, channelNumber)) {
        error = NOT_FOUND_ERROR;
      } else {
        channel->fileState = READ_FILE;
      }
    } else {
      /* delete old file */
      if (overwrite) {
        deleteFile (filename);
      }
      /* open file */
      if (!openWrite (filename, filetype, channelNumber)) {
        error = CREATE_ERROR;
      } else {
        channel->fileState = WRITE_FILE;
      }
    }
  }
}

int main (void) {
	/* I/O register setup, etc. */
	init();
  
	/* service commanding device */
	if(setjmp(attention_jmp)){
		iecAttention();
	}
	
	/* main loop */
	while (TRUE) {
		struct channelTableStruct *channel;

		/* Handle delayed Attention requests now */
		ATTENTION_ON();
		
		/* blink LED if error condition */
		updateLED();
		
		/* get pointer to channel structure */
		channel = &channelTable[channelNumber];

		/* execute command */
		switch (command) {

			case LISTEN_OPEN: {
				/* reset variables */
				channel->bufferPtr = 0;
				channel->endOfBuffer = 0;

				if (channelNumber == COMMAND_CHANNEL) {
					/* get command and execute it */
					parseCommand();
				} else {
					/* normal data channel, get file name and open file */
					parseName (channel);
				}
				break;
			}

			case LISTEN_CLOSE:
				ATTENTION_OFF();
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
				break;

			case LISTEN_DATA: {
				if (channelNumber == COMMAND_CHANNEL) {
					/* status channel must be reset before each command */
					channel->bufferPtr = 0;
					channel->endOfBuffer = 0;
					parseCommand();
				} else {
					if (channel->fileState == WRITE_FILE) {
						iecListen_r iecret;

						//ATTENTION_OFF();
						
						do {
							
							//bufferSize_t bytesReceived;
							bufferSize_t *bufPtr = &(channel->bufferPtr);
        
							/* receive bytes */
							iecret = iecListen (channel->buffer + *bufPtr, BLOCKSIZE - *bufPtr);

							/* update bufferPtr */
							*bufPtr += iecret.bytesReceived;

							/* save to disk */
							if (*bufPtr == BLOCKSIZE) { 
								writeFile (channelNumber);
								*bufPtr = 0;
							}
						} while (!iecret.eoi);

					} else {
						error = NOT_OPEN_ERROR;
					}
				}
				break;
			} 

			case TALK_DATA: {
				if ((channel->fileState == READ_FILE) || (channelNumber == COMMAND_CHANNEL)) {
					bool_t done = FALSE;

					while (!done) {
						static bool_t eof;

						/* get new block of data */
						if (channel->bufferPtr == channel->endOfBuffer) {
							ATTENTION_OFF();
							/* start new block at beginning */
							channel->bufferPtr = 0;
							eof = FALSE;

							if (channelNumber == COMMAND_CHANNEL) {
								eof = readStatus (channel);
							} else if (channel->readDirState) {
								//LED_ON;
								eof = readDir (channel);
							} else {
								//LED_ON;
								readFile (channelNumber, &eof);
								if (eof) {
									channel->endOfBuffer = channel->dirEntry.bytesInLastBlock;
								} else {
									channel->endOfBuffer = BLOCKSIZE;
								}
							}
						}

						ATTENTION_ON();

						/* send data block */
						iecTalk (channel, eof);

						done = eof;

					}
				} else {
					error = NOT_OPEN_ERROR;
				}
				break;
			}

		}
		
		command = IDLE;
	}
	return 0;
}

uint8_t filenamelen (char *filename) {
	uint8_t len = 0;
  
	while (*filename && (len < 16)) {
		filename++;
		len++;
	}

	return len;
}
