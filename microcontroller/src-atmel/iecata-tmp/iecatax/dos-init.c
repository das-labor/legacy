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
  dos-init.c V1.0
  Contains the initialization, formatting and free block management
  code for the file system
*/

#include <avr/pgmspace.h>
#include <string.h>
#include "dos.h"
#include "ata.h"

#define FS_VERSION_STRING     "adfs 1.0"
#define SUPERBLOCK            0

struct superblockStruct {
  block_t freeBlockList;
  char versionString[sizeof (FS_VERSION_STRING)];
  uint8_t reserved[BLOCKSIZE - sizeof (block_t) - sizeof (FS_VERSION_STRING)];
};

/* protos ****************************************************************/

/* variables *************************************************************/

static block_t freeBlocks[BLOCKSIZE / sizeof (block_t)];
static struct superblockStruct superblock;
static uint8_t freeBlocksIndex;
static PGM_P fs_version_string;

/* functions *************************************************************/

bool_t dosInit (void) {
  /* get pointer to version string */
  fs_version_string = PSTR (FS_VERSION_STRING);

  /* read superblock */
  ataGetBlock (SUPERBLOCK, (uint8_t *)&superblock);

  /* check if HD is formatted with correct file system */
  if (strncmp_P (superblock.versionString, fs_version_string,
                 sizeof (FS_VERSION_STRING))) {
    /* invalid disk */
    return FALSE;
  }

  /* read first block of free block list */
  ataGetBlock (superblock.freeBlockList, (uint8_t *)freeBlocks);
  freeBlocksIndex = 0;

  /* init the other dos modules */
  dosDirInit();
  dosFileInit();

  return TRUE;
}

bool_t formatDrive (void) {
  /* Set up and write superblock */
  memcpy_P (superblock.versionString, fs_version_string,
            sizeof (FS_VERSION_STRING));
  superblock.freeBlockList = 2;
  ataPutBlock (SUPERBLOCK, (uint8_t *)&superblock);

  memset (freeBlocks, '\0', BLOCKSIZE);

  /* create empty root directory */
  ataPutBlock (ROOTBLOCK, (uint8_t *)freeBlocks);

  { /* create the free block list */
    bool_t done = FALSE;
    block_t blockCount = superblock.freeBlockList + 1;

    /* one loop is one block in free block list */
    while (!done) {
      uint8_t i = 0;
      block_t currentBlock = blockCount - 1;
      
      memset (freeBlocks, '\0', BLOCKSIZE);

      /* set up the pointers in the current free block list block */
      while (!done && (i < (BLOCKSIZE / sizeof (block_t)))) {
        freeBlocks[i] = blockCount++;
        if (blockCount >= totalDiskSize) {
          done = TRUE;
        }
        i++;
      }
      ataPutBlock (currentBlock, (uint8_t *)freeBlocks);
    }
  }

  /* Init FS to newly formatted disk */
  return dosInit();
}

block_t allocateBlock (void) {
  block_t block;

  while (TRUE) {
    /* if at the end, load next block in free block list */
    if (freeBlocksIndex == ((BLOCKSIZE / sizeof (block_t)) - 1)) {
      block_t nextFreeFileBlock;
      /* if no more blocks, return 0 */
      if (!(nextFreeFileBlock = freeBlocks[freeBlocksIndex])) {
        return 0; /* FIXME: the last block don't get allocated */
      }
      /* load next block in the free block list */
      ataGetBlock (nextFreeFileBlock, (uint8_t *)freeBlocks);
      block = superblock.freeBlockList;
      superblock.freeBlockList = nextFreeFileBlock;
      freeBlocksIndex = 0;
      ataPutBlock (SUPERBLOCK, (uint8_t *)&superblock);
      /* return the previous free file block */
      return block;
    }
    /* return next block in the list if != 0 */
    if ((block = freeBlocks[freeBlocksIndex])) {
      freeBlocks[freeBlocksIndex++] = 0;
      return block;
    }
    freeBlocksIndex++;
  }
  return 0;
}

void freeBlock (block_t block) {
  bool_t done = FALSE;
  uint8_t index = 0;

  while (!done) {
    /* if block is full of ptrs to free blocks, extend 
       free block list with this one at the beginning */
    if (index == ((BLOCKSIZE / sizeof (block_t)) - 1)) {
      flushFreeBlockList();
      memset (freeBlocks, '\0', BLOCKSIZE);
      freeBlocks[(BLOCKSIZE / sizeof (block_t)) - 1] =
        superblock.freeBlockList;
      superblock.freeBlockList = block;
      ataPutBlock (SUPERBLOCK, (uint8_t *)&superblock);
      done = TRUE;
    }
    /* if empty place in free block list, put this one there */
    if (!freeBlocks[index]) {
      freeBlocks[index] = block;
      freeBlocksIndex = 0;
      done = TRUE;
    }
    index++;
  }
}

void flushFreeBlockList (void) {
  ataPutBlock (superblock.freeBlockList, (uint8_t *)freeBlocks);
}
