/* flmDB.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLMDB_H_
#define FLMDB_H_

#include "types.h"

typedef uint8_t hnick_t[32];

typedef struct {
	uint8_t active;
	uint8_t permanent;
	uint8_t last;
	userflags_t setflags;
	userflags_t clearflags;
	uint8_t reserved[3];
	uint64_t timestamp;
	hnick_t hnick;
} flmdb_entry_t;

#define FLMDB_ACTIVE_OFFSET 0


typedef uint16_t entryid_t;



void flmdb_loadentry(flmdb_entry_t * entry, entryid_t id);
void flmdb_setentry(flmdb_entry_t * entry, entryid_t id);
void flmdb_process(uint8_t * searchmac, userid_t uid, userflags_t * flags);
void flmdb_makeentry(uint8_t * mac, userflags_t setflags, userflags_t clearflags, uint8_t permanent);
void flmdb_format(void);
#endif /*FLMDB_H_*/
