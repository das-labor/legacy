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
*/

/*
  iecata.h V1.0
  The main header file for the IEC-ATA software. 
*/

#ifndef IECATA_H
#define IECATA_H

#include <string.h>
#include <ctype.h>

#ifdef UNIX
/* compile for unix.
   dos-init.c, dos-file.c and dos-dir.c may be compiled for unix,
   making the file system code available for unix programs */ 
#include <stdio.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#define strncpy_P strncpy
#define memcpy_P memcpy
#define strncmp_P strncmp
#define PSTR(x) x
#define PGM_P const char *

#else
/* compile for AVR */
//#include <inttypes.h>
//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <setjmp.h>

#define outp(b,p) p=(b)
#define inp(p) p

#define sbi(p,b) p|=(1<<b)
#define cbi(p,b) p&=~(1<<b) 

#endif

typedef uint8_t bool_t;
typedef uint32_t block_t;
typedef uint8_t entryIndex_t;/*dir counter 8 bit max 255 entrys*/
typedef uint16_t fileSize_t;
typedef uint16_t bufferSize_t;

#define FALSE  0
#define TRUE   (~FALSE)

enum commands {
  IDLE, LISTEN_OPEN, LISTEN_CLOSE, LISTEN_DATA, TALK_DATA
};

#define ROOTBLOCK              1
#define BLOCKSIZE            512
#define FILE_NAME_SIZE        16
#define MAX_OPEN_FILES        16
#define LED                  PD5
#define COMMAND_CHANNEL     0x0f

enum filetypes {
  ANY, SEQ = 1, PRG = 2, DIR = 6
};

struct dirEntryStruct {
  char fileName[FILE_NAME_SIZE];
  block_t startBlock;
  fileSize_t fileSize; /* number of blocks in file */
  bufferSize_t bytesInLastBlock; /* number of bytes in the last block */
  uint8_t fileType;
  bool_t readOnly; /* TODO: this works like dm-dos TYPE_SYSTEM */
  bool_t splat; /* file not closed properly */
  uint8_t pad; /* dummy byte to get a structure of 28 bytes */
};

enum fileStates {
  NO_FILE, READ_FILE, WRITE_FILE
};

enum readDirStates {
  NOT_READ_DIR, READ_DIR_BEGIN, READ_DIR_PROGRESS, READ_DIR_FINISHED
};

struct channelTableStruct {
  struct dirEntryStruct dirEntry;
  uint8_t fileState;
  uint8_t readDirState;
  bufferSize_t bufferPtr;
  bufferSize_t endOfBuffer;
  block_t dirBlock;
  block_t inodeBlock;
  bufferSize_t inodePtr;
  uint8_t buffer[BLOCKSIZE];
  block_t inode[BLOCKSIZE / sizeof (block_t)];
};

typedef struct{
	bufferSize_t bytesReceived;
	bool_t eoi;
	uint8_t dummy;
}iecListen_r;

/* protos & externs ******************************************************/

/* iecata.c */
extern volatile uint8_t command;
extern volatile uint8_t error;
extern volatile uint8_t channelNumber;

/* iec.c */
iecListen_r iecListen (uint8_t *data, bufferSize_t maxBytes);
void iecTalk (struct channelTableStruct * channel, bool_t eoi);
void iecAttention (void);

/* ata.c */
extern block_t totalDiskSize;
bool_t ataInit (void);
void ataGetBlock (block_t blockNumber, uint8_t *block);
void ataPutBlock (block_t blockNumber, uint8_t *block);

/* dos-init.c */
bool_t dosInit (void);
bool_t formatDrive (void);
block_t allocateBlock (void);
void freeBlock (block_t block);
void flushFreeBlockList (void);

/* dos-dir.c */
extern block_t currentDir;
extern bool_t dirBufferChanged;
void dosDirInit (void);
bool_t setCurrentDir (char *path);
bool_t createDir (char *name);
void deleteDir (char *pattern);
bool_t renameEntry (char *newName, char *oldNamePattern);
struct dirEntryStruct *getEntry (entryIndex_t entryIndex);
struct dirEntryStruct *getUnusedEntry (void);
struct dirEntryStruct *getEntryByName (char *name);
void deleteEntry (struct dirEntryStruct *entry);
void getBlock (block_t block);
void flushDirBuffer (void);
bool_t nameValid (char *name);
bool_t filenameMatch (char *name, char *pattern);

/* dos-file.c */
extern struct channelTableStruct channelTable[MAX_OPEN_FILES];
void dosFileInit (void);
bool_t openRead (char *pattern, uint8_t fileType, uint8_t channel);
bool_t openWrite (char *name, uint8_t fileType, uint8_t channel);
void closeFile (uint8_t channel);
bool_t readFile (uint8_t channel, bool_t *eof);
bool_t writeFile (uint8_t channel);
void deleteFile (char *pattern);

#endif
