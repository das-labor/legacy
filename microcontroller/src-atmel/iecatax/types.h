#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>

typedef uint32_t block_t;
typedef uint8_t bool_t;
typedef uint16_t bufferSize_t;

#define FALSE  0
#define TRUE   (~FALSE)

typedef uint8_t entryIndex_t;/*dir counter 8 bit max 255 entrys*/
typedef uint16_t fileSize_t;


#define ROOTBLOCK              1
#define BLOCKSIZE            512
#define FILE_NAME_SIZE        16
#define MAX_OPEN_FILES        16

#define COMMAND_CHANNEL     0x0f


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

enum filetypes {
  ANY, SEQ = 1, PRG = 2, DIR = 6
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



#endif //TYPES_H
