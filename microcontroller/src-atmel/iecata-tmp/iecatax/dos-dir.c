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
  dos-dir.c V1.0
  Contains the directory handling code for the file system
*/

#include <avr/pgmspace.h>
#include <string.h>
#include "dos.h"
#include "ata.h"

#define DIR_ENTRIES_IN_BLOCK ((BLOCKSIZE - sizeof (block_t)) / \
                             sizeof (struct dirEntryStruct))

struct dirBlockStruct {
  block_t nextBlock;
  struct dirEntryStruct dirEntry[DIR_ENTRIES_IN_BLOCK];
  uint8_t reserved[BLOCKSIZE - sizeof (block_t) -
                  (DIR_ENTRIES_IN_BLOCK * sizeof (struct dirEntryStruct))];
};

/* protos ****************************************************************/

struct dirEntryStruct *getUsedEntryInCurrentBlock (void);

/* variables *************************************************************/

static struct dirBlockStruct dirBuffer;
static block_t dirBufferBlock;
static entryIndex_t dirBufferBlockNumber;
bool_t dirBufferChanged;
block_t currentDir;

/* functions *************************************************************/

void dosDirInit (void) {
  /* Remove contents from directory buffer */
  dirBufferChanged = FALSE;
  dirBufferBlock = 0; 
  dirBufferBlockNumber = ~0;

  /* start with root dir */
  currentDir = ROOTBLOCK;
}

bool_t setCurrentDir (char *path) {
  struct dirEntryStruct *entry;
  block_t nextCurrentDir = 0;

  if (*path == '/') {
    path++;
    nextCurrentDir = ROOTBLOCK;
    getBlock (ROOTBLOCK);
  } else {
    getBlock (currentDir);
  }

  while (TRUE) {
    if (!(*path)) {
      currentDir = nextCurrentDir;
      dirBufferBlockNumber = ~0;
      return TRUE;
    }
    if (!(entry = getEntryByName (path))) {
      /* can't find dir */
      return FALSE;
    }
    if (entry->fileType != DIR) {
      /* not a dir */
      return FALSE;
    }
    nextCurrentDir = entry->startBlock;
    if (!(path = strchr (path, '/'))) {
      /* no more elements in path */
      currentDir = nextCurrentDir;
      dirBufferBlockNumber = ~0;
      return TRUE;
    }
    path++; /* skip '/' */
    getBlock (entry->startBlock);
  }
}

bool_t createDir (char *name) {
  struct dirEntryStruct *entry;

  getBlock (currentDir);

  if (nameValid (name)) {
    if ((entry = getUnusedEntry())) {
      if (!getEntryByName (name)) {
        if ((entry->startBlock = allocateBlock())) {

          dirBufferChanged = TRUE;

          /* set entry */
          memcpy (entry->fileName, name, FILE_NAME_SIZE);
          entry->fileSize = 0;
          entry->fileType = DIR;
          entry->readOnly = FALSE;
          entry->splat = FALSE;

          { /* init new directory block */
            getBlock (entry->startBlock);
            memset (&dirBuffer, '\0', BLOCKSIZE);
            dirBuffer.dirEntry[0].startBlock = currentDir;
            memcpy_P (dirBuffer.dirEntry[0].fileName, PSTR (".."), 3);
            dirBuffer.dirEntry[0].fileSize = 0;
            dirBuffer.dirEntry[0].fileType = DIR;
            dirBuffer.dirEntry[0].readOnly = TRUE;
            dirBuffer.dirEntry[0].splat = FALSE;
            dirBufferChanged = TRUE;
          }

          flushDirBuffer();
          flushFreeBlockList();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

void deleteDir (char *pattern) {
  struct dirEntryStruct *entry;
  entryIndex_t entryIndex = 0;

  while ((entry = getEntry (entryIndex++))) {
    /* only process non-deleted dirs */
    if (entry->startBlock) {
      /* only delete dirs that match pattern */
      if (filenameMatch (entry->fileName, pattern)) {
        block_t blockWithEntry = dirBufferBlock;

        /* only delete dirs */
        if (entry->fileType == DIR) {
          getBlock (entry->startBlock);
          /* only delete empty dirs */
          if (!getUsedEntryInCurrentBlock()) {
            /* delete directory */
            getBlock (blockWithEntry);
            freeBlock (entry->startBlock);
            deleteEntry (entry);
            flushDirBuffer();
            flushFreeBlockList();
            /* begin counting at 0 because direntries may have been moved */
            entryIndex = 0;
          }
        }
      }
    }
  }
}

bool_t renameEntry (char *newName, char *oldNamePattern) {
  struct dirEntryStruct *entry;

  getBlock (currentDir);

  if ((entry = getEntryByName (oldNamePattern))) {
    block_t blockWithEntry = dirBufferBlock;
    if (!getEntryByName (newName)) {
      getBlock (blockWithEntry);
      memcpy (entry->fileName, newName, FILE_NAME_SIZE);
      dirBufferChanged = TRUE;
      flushDirBuffer();
      return TRUE;
    }
  }
  return FALSE;
}

struct dirEntryStruct *getEntry (entryIndex_t entryIndex) {
  entryIndex_t blockNumber = entryIndex / DIR_ENTRIES_IN_BLOCK;

  if (dirBufferBlockNumber != blockNumber) {
    entryIndex_t i;

    /* find correct directory block */
    getBlock (currentDir);
    for (i = 0; i < blockNumber; i++) {
      if (!dirBuffer.nextBlock) {
        return NULL;
      }
      getBlock (dirBuffer.nextBlock);
    }
    dirBufferBlockNumber = blockNumber;
  }

  return &dirBuffer.dirEntry[entryIndex % DIR_ENTRIES_IN_BLOCK];
}

struct dirEntryStruct *getUsedEntryInCurrentBlock (void) {
  entryIndex_t i;

  for (i = 0; i < DIR_ENTRIES_IN_BLOCK; i++) {
    if (dirBuffer.dirEntry[i].startBlock &&
        !(dirBuffer.dirEntry[i].readOnly)) {
      return &dirBuffer.dirEntry[i];
    }
  }
  return NULL;
}

struct dirEntryStruct *getUnusedEntry (void) {
  while (TRUE) {
    entryIndex_t i;

    /* search through current block for an empty entry */
    for (i = 0; i < DIR_ENTRIES_IN_BLOCK; i++) {
      if (!dirBuffer.dirEntry[i].startBlock) {
        return &dirBuffer.dirEntry[i];
      }
    }
    /* if no more blocks in directory chain, make a new one */
    if (!dirBuffer.nextBlock) {
      if (!(dirBuffer.nextBlock = allocateBlock())) {
        return NULL;
      }
      dirBufferChanged = TRUE;
      flushDirBuffer();
      dirBufferBlock = dirBuffer.nextBlock;
      dirBufferBlockNumber = ~0;
      memset (&dirBuffer, '\0', BLOCKSIZE);
      dirBufferChanged = TRUE;
    } else { /* load the next dir block in chain */
      getBlock (dirBuffer.nextBlock);
    }
  }
  return NULL;
}

struct dirEntryStruct *getEntryByName (char *name) {
  while (TRUE) {
    entryIndex_t i;

    /* search for entry in current block */
    for (i = 0; i < DIR_ENTRIES_IN_BLOCK; i++) {
      if (dirBuffer.dirEntry[i].startBlock) { /* not deleted */
        if (filenameMatch (dirBuffer.dirEntry[i].fileName, name)) {
          return &dirBuffer.dirEntry[i];
        }
      }
    }
    /* if no more blocks in chain, return NULL */
    if (!dirBuffer.nextBlock) {
      return NULL;
    }
    /* get next block */
    getBlock (dirBuffer.nextBlock);
  }
  return NULL;
}

void getBlock (block_t block) {
  /* only read block if not allready in memory */
  if (block != dirBufferBlock) {
    flushDirBuffer();
    dirBufferBlock = block;
    dirBufferBlockNumber = ~0;
    dirBufferChanged = FALSE;
    ataGetBlock (block, (uint8_t *)&dirBuffer);
  }
}

void flushDirBuffer (void) {
  /* only flush if buffer has changed */
  if (dirBufferChanged) {
    ataPutBlock (dirBufferBlock, (uint8_t *)&dirBuffer);
  }
}    

void deleteEntry (struct dirEntryStruct *entry) {
  block_t oldBlock = dirBufferBlock;

  /* if directory has more blocks in chain, move an entry from
     the last block in chain to this block */
  if (dirBuffer.nextBlock) {
    struct dirEntryStruct *entryToMove;
    struct dirEntryStruct copyOfEntry;
    block_t prevBlock = oldBlock;

    /* find the last block in dir chain */
    while (dirBuffer.nextBlock) {
      prevBlock = dirBufferBlock;
      getBlock (dirBuffer.nextBlock);
    }
    /* if no entries in last dir block, remove the last dir block */
    if (!(entryToMove = getUsedEntryInCurrentBlock())) {
      getBlock (prevBlock);
      freeBlock (dirBuffer.nextBlock);
      flushFreeBlockList();
      dirBuffer.nextBlock = 0;
      dirBufferChanged = TRUE;
      /* get an entry to move */
      if (!(entryToMove = getUsedEntryInCurrentBlock())) {
        /* TODO: error recovery */
      }
    }
    /* overwrite the entry to be deleted with the one just found */
    if (dirBufferBlock != oldBlock) {
      memcpy (&copyOfEntry, entryToMove, sizeof (struct dirEntryStruct));
      entryToMove->startBlock = 0;
      dirBufferChanged = TRUE;
      getBlock (oldBlock);
      memcpy (entry, &copyOfEntry, sizeof (struct dirEntryStruct));
      dirBufferChanged = TRUE;
    } else { /* delete entry */
      entry->startBlock = 0;
      dirBufferChanged = TRUE;
    }
  } else { /* delete entry */
    entry->startBlock = 0;
    dirBufferChanged = TRUE;
  }
}

bool_t nameValid (char *name) {
  uint8_t bytes = 16;

  /* don't accept empty file names */
  if (!(*name)) {
    return FALSE;
  }

  /* check up to 16 bytes for illegal characters */
  while (*name && (bytes-- != 0)) {
    if ((*name == '?') || (*name == '*') || (*name == '/') ||
        (*name == ':') || (*name == ',')) {
      return FALSE;
    }
    name++;
  }
  return TRUE;
}

bool_t filenameMatch (char *name, char *pattern) {
  bool_t done = FALSE;
  uint8_t bytes = 16;

  while (!done) {
    if ((bytes-- == 0) || (*pattern == '/') || (*pattern == '*') ||
        ((*name == '\0') && (*pattern == '\0'))) {
      return TRUE;
    }
    if ((*pattern == '?') || (*name == *pattern)) {
      name++;
      pattern++;
    } else {
      done = TRUE;
    }
  }
  return FALSE;
}
