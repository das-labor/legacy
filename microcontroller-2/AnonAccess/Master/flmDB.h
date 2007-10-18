#ifndef FLMDB_H_
#define FLMDB_H_

#include "types.h"

typedef uint8_t hnick_t[32];

typedef struct {
	uint8_t active;
	userflags_t setflags;
	userflags_t clearflags;
	uint8_t reserved[5];
	hnick_t hnick;
} flmdb_entry_t;

#define FLMDB_ACTIVE_OFFSET 0


typedef uint16_t entryid_t;



void flmdb_loadentry(flmdb_entry_t * entry, entryid_t id);
void flmdb_setentry(flmdb_entry_t * entry, entryid_t id);
void flmdb_process(uint8_t * searchmac, userid_t uid, userflags_t * flags);
void flmdb_makeentry(uint8_t * mac, userflags_t setflags, userflags_t clearflags);

#endif /*FLMDB_H_*/
