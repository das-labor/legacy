/* ticketDB.h */
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


/***************************************
 *  Header file for interfacing the database
 * on external eeprom
 * 
 ***/

#include <stdint.h>
#include "types.h"


#define DB_ERROR_OK			0x00
#define DB_ERROR_IDSTRING	0x01
#define DB_ERROR_VERSION	0x02
#define DB_ERROR_HEADERSIZE	0x04
#define DB_ERROR_DBFULL		0x08
#define DB_ERROR_NOSUCHUSER	0x10

/*********************************************************************************************************/


typedef struct stats_st{
	uint16_t max_users;
	uint16_t users;
	uint16_t admins;
	uint16_t locked_users;
	uint16_t locked_admins;
} stats_t;


typedef struct userentry_st{
	uint8_t     flags;
	uint8_t	    nickname[7];
	ticketmac_t ticketmac;
	uint8_t     pinmacseed[32];
} userentry_t; /* total size is 72 bytes */

#define DB_FLAGS_OFFSET      0
#define DB_NICKNAME_OFFSET   1
#define DB_TICKETMAC_OFFSET  8
#define DB_PINMACSEED_OFFSET 40

typedef struct dbheader_st{
	uint8_t id[10]; 
	uint8_t majversion;
	uint8_t minversion;
	uint16_t headersize;
	stats_t stat;
	uint8_t reserved[8];
} dbheader_t;

#define DBSTAT_OFFSET 14

/*********************************************************************************************************/


void    ticketdb_format(dbsize_t size);
uint8_t	ticketdb_init(void);
uint8_t	ticketdb_newuser(ticketmac_t*, userid_t*, uint16_t);
uint8_t	ticketdb_deluser(userid_t);

bool    ticketdb_userexists(userid_t id);
uint8_t	ticketdb_getUserTicketMac(userid_t id, ticketmac_t*);
uint8_t	ticketdb_getUserFlags(userid_t id, userflags_t*);
uint8_t	ticketdb_setUserTicketMac(userid_t id, ticketmac_t*);
uint8_t	ticketdb_setUserFlags(userid_t id, userflags_t*);
uint8_t	ticketdb_getUserNickname(userid_t id, char* dest);
uint8_t	ticketdb_setUserNickname(userid_t id, char* dest);
uint8_t ticketdb_setUserPinMac(userid_t id, void* src);
uint8_t ticketdb_getUserPinMac(userid_t id, void* dest);
uint8_t ticketdb_addname(userid_t id, char* name);

unsigned ticketdb_getstatMaxUsers(void);
unsigned ticketdb_getstatUsers(void);
unsigned ticketdb_getstatAdmins(void);
unsigned ticketdb_getstatLockedUsers(void);
unsigned ticketdb_getstatLockedAdmins(void);

