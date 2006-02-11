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
  dos-file.c V1.0
  Contains the file system code that handles file access
*/

#include "iecata.h"

/* protos ****************************************************************/

/* variables *************************************************************/

struct channelTableStruct channelTable[MAX_OPEN_FILES];
static uint8_t tmpBuffer[BLOCKSIZE];

/* functions *************************************************************/

inline extern void dosFileInit (void) {
  uint8_t i;
  struct channelTableStruct *channelStruct;
  
  for (i = 0; i < MAX_OPEN_FILES; i++) {
    channelStruct = &channelTable[i];
    channelStruct->fileState = NO_FILE;
    channelStruct->bufferPtr = 0;
    channelStruct->endOfBuffer = 0;
  }
}

inline extern bool_t openRead (char *pattern, uint8_t fileType,
                               uint8_t channel) {
  struct dirEntryStruct *entry;
  struct channelTableStruct *channelStruct = &channelTable[channel];

  if (channelStruct->fileState == NO_FILE) {
    getBlock (currentDir);
    if ((entry = getEntryByName (pattern))) {
      if ((fileType == ANY) || (entry->fileType == fileType)) {
        /* TODO: should perhaps have some kind of file lock here */

        /* set channel struct */
        memcpy (&channelStruct->dirEntry, entry,
                sizeof (struct dirEntryStruct));
        channelStruct->inodePtr = 0;
        channelStruct->dirBlock = currentDir;
        /* read first inode block into mem */
        ataGetBlock (entry->startBlock,
                     (uint8_t *)(channelStruct->inode));
  
        return TRUE;
      }
    }
  }
  return FALSE;
}

inline extern bool_t openWrite (char *name, uint8_t fileType,
                                uint8_t channel) {
  struct dirEntryStruct *entry;
  struct channelTableStruct *channelStruct = &channelTable[channel];

  if (nameValid (name)) {
    if (channelStruct->fileState == NO_FILE) {
      getBlock (currentDir);
      if (!getEntryByName (name)) {
        if ((entry = getUnusedEntry())) {
          /* create first inode */
          memset (&(channelStruct->inode), '\0', BLOCKSIZE);

          if ((channelStruct->inodeBlock = allocateBlock())) {
            /* set entry */
            entry->startBlock = channelStruct->inodeBlock;
            memcpy (entry->fileName, name, FILE_NAME_SIZE);
            entry->fileSize = 0;
            entry->fileType = fileType;
            entry->splat = TRUE;
            /* set channel struct */
            memcpy (&channelStruct->dirEntry, entry,
                    sizeof (struct dirEntryStruct));
            channelStruct->dirBlock = currentDir;
            channelStruct->inodePtr = 0;
            /* save entry to disk */
            dirBufferChanged = TRUE;
            flushDirBuffer();

            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

void closeFile (uint8_t channel) {
  struct channelTableStruct *channelStruct = &channelTable[channel];
  
  if ((channelStruct->readDirState == NOT_READ_DIR) &&
      (channelStruct->fileState == WRITE_FILE)) {
    struct dirEntryStruct *entry;

    /* save last block */
    writeFile (channel);
    channelStruct->dirEntry.bytesInLastBlock = channelStruct->bufferPtr;

    getBlock (channelStruct->dirBlock);
    if (!(entry = getEntryByName (channelStruct->dirEntry.fileName))) {
      return; /* FIXME, cleanup */
    }
    /* write inode */
    ataPutBlock (channelStruct->inodeBlock,
                 (uint8_t *)(channelStruct->inode));
    /* copy back direntry */
    memcpy (entry, &channelStruct->dirEntry, sizeof (struct dirEntryStruct));
    entry->splat = FALSE;
    dirBufferChanged = TRUE;
    flushDirBuffer();
    /* flush free block list */
    flushFreeBlockList();

  }
  channelStruct->fileState = NO_FILE;
}

inline extern bool_t readFile (uint8_t channel, bool_t *eof) {
  struct channelTableStruct *channelStruct = &channelTable[channel];

  if (!channelStruct->dirEntry.fileSize) {
    /* don't read anything if file has no more bytes */
    *eof = TRUE;
  } else {

    /* if at end of inode, get next inode */
    if (channelStruct->inodePtr == (BLOCKSIZE / sizeof (block_t)) - 1) {
      ataGetBlock (channelStruct->inode[channelStruct->inodePtr],
                   (uint8_t *)(channelStruct->inode));
      channelStruct->inodePtr = 0;
    }

    /* get data block */
    ataGetBlock (channelStruct->inode[channelStruct->inodePtr++],
                 channelStruct->buffer);

    channelStruct->dirEntry.fileSize--;

    if (!channelStruct->dirEntry.fileSize) {
      *eof = TRUE;
    }
  }
  
  return TRUE;
}

bool_t writeFile (uint8_t channel) {
  struct channelTableStruct *channelStruct = &channelTable[channel];

  /* if at end of inode, make new inode */
  if (channelStruct->inodePtr ==
      (BLOCKSIZE / sizeof (block_t)) - 1) {
    if (!(channelStruct->inode[channelStruct->inodePtr] = allocateBlock())) {
      return FALSE; /* FIXME, cleanup */
    }
    ataPutBlock (channelStruct->inodeBlock, (uint8_t *)(channelStruct->inode));
    channelStruct->inodeBlock = channelStruct->inode[channelStruct->inodePtr];
    memset (channelStruct->inode, '\0', BLOCKSIZE);
    channelStruct->inodePtr = 0;
  }
  /* allocate new block */
  if (!(channelStruct->inode[channelStruct->inodePtr] = allocateBlock())) {
    return FALSE; /* FIXME, cleanup */
  }
  /* write block to disk */
  ataPutBlock (channelStruct->inode[channelStruct->inodePtr++],
               channelStruct->buffer);
  channelStruct->dirEntry.fileSize++;

  return TRUE;
}
 
void deleteFile (char *pattern) {
  struct dirEntryStruct *entry;
  entryIndex_t entryIndex = 0;

  while ((entry = getEntry (entryIndex++))) {
    /* only process non-deleted files */
    if (entry->startBlock) {
      /* only delete files that match pattern */
      if (filenameMatch (entry->fileName, pattern)) {
        /* only delete non-locked files */
        if ((entry->fileType != DIR) && !(entry->readOnly)) {
          block_t tmpBufferBlock;
          bufferSize_t tmpBufferPtr;

          /* get inode of file to delete */
          tmpBufferBlock = entry->startBlock;
          ataGetBlock (tmpBufferBlock, tmpBuffer);
          tmpBufferPtr = 0;

          /* free all blocks belonging to file */
          while (entry->fileSize) {
            /* if at the end of inode, get next inode */
            if (tmpBufferPtr == (BLOCKSIZE / sizeof (block_t)) - 1) {
              freeBlock (tmpBufferBlock);
              tmpBufferBlock = ((block_t *)tmpBuffer)[tmpBufferPtr];
              ataGetBlock (tmpBufferBlock, tmpBuffer);
              tmpBufferPtr = 0;
            }
            /* free block */
            freeBlock (((block_t *)tmpBuffer)[tmpBufferPtr++]);
            entry->fileSize--;
          }
          /* free last inode block */
          freeBlock (tmpBufferBlock);

          /* remove entry from directory */
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
