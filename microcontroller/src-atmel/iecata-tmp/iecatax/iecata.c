
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "dos.h"
#include "iecata.h"
#include "iec.h"
#include "config.h"


#define BASIC_LINE_LENGTH    33 /* TODO: check this number */
#define CDOS_DIRENTRY_LENGTH 32


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


uint8_t filenamelen (char *filename) {
	uint8_t len = 0;
  
	while (*filename && (len < 16)) {
		filename++;
		len++;
	}

	return len;
}


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

bool_t readDir (uint8_t channelNumber) {
  struct channelTableStruct *channel = &channelTable[channelNumber];
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


char n_name[] PROGMEM="N";

void n_fkt(uint8_t* dummy1, uint8_t* dummy2){
	/* format drive */
	if (!formatDrive()) {
		error = INIT_ERROR;
	}
}


command_t commands[] PROGMEM={
		{&cd_fkt, cd_name},
		{&cdr_fkt, cdr_name},
		{&cdpp_fkt, cdpp_name},
		{&uj_fkt, uj_name},
		{&md_fkt, md_name},
		{&rd_fkt, rd_name},
		{&r_fkt, r_name},
		{&s_fkt, s_name},
		{&n_fkt, n_name},
		
};

void parseCommand (void) {
	static uint8_t command[255];
	uint8_t x;
	uint8_t *cmdArg1;
	uint8_t *cmdArg2;

  { /* get message */
	bufferSize_t bytesReceived = 0;
    
    iecListen (command, &bytesReceived, 254);

	if(!bytesReceived) return;	
	
    /* make message a proper string */
    command[bytesReceived] = '\0';
#ifdef DEBUG
    printf_P(PSTR("command: %s"), command);
#endif
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
    if(! strcmp_P(command,(PGM_P)PW(commands[x].name)) ){
		void(*fkt)(uint8_t* arg1, uint8_t* arg2);
		fkt = (void *)PW(commands[x].fkt);
		fkt(cmdArg1, cmdArg2);
		return;
	}  
  }
  
  error = SYNTAX_ERROR;
}

void parseName (uint8_t channelNumber) {
	struct channelTableStruct *channel = &channelTable[channelNumber];
	static uint8_t commandBuffer[255];
	uint8_t *bufferPtr = commandBuffer;

	bool_t overwrite = FALSE;
	char *filename;
	uint8_t filetype;
	bool_t read;
	
	{ /* get string */
		bufferSize_t bytesReceived = 0;
    
    	iecListen (commandBuffer, &bytesReceived, 254);

		/* make buffer a proper string */
		commandBuffer[bytesReceived] = '\0';
#ifdef DEBUG
	    printf_P(PSTR("name: %s"), commandBuffer);
#endif
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
