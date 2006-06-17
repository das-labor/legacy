#ifndef DOS_H
#define DOS_H

#include "types.h"


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
bool_t writeFile (struct channelTableStruct*);
void deleteFile (char *pattern);

#endif //DOS_H
