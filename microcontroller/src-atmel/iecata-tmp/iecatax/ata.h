#ifndef ATA_H
#define ATA_H
/* ata.c */

#include "types.h"

extern block_t totalDiskSize;
bool_t ataInit (void);
void ataGetBlock (block_t blockNumber, uint8_t *block);
void ataPutBlock (block_t blockNumber, uint8_t *block);

#endif
