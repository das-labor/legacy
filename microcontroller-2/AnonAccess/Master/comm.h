/* comm.h */
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


#ifndef COMM_H_
#define COMM_H_

#include "types.h"
#include "lop.h"
#include "qport.h"
#include <stdint.h>

#define MASTERUNIT_ID 0
#define TERMINALUNIT_ID 1

#define MSGID_SESSION_INIT  0
#define MSGID_ADD_AB        1
#define MSGID_AB_REPLY      2
#define MSGID_AB_ERROR      3
#define MSGID_ACTION        4
#define MSGID_ACTION_REPLY  5
#define MSGID_PRINT         6
#define MSGID_ADD_BOOTSTRAP 7
#define MSGID_AB_PINREQ     8
#define MSGID_AB_PINREPLY   9

#define MSGID_MAX  9
#define MSGID_CNT (MSGID_MAX+1)

#define AB_ERROR_WONTTELL   0
#define AB_ERROR_HMAC       1
#define AB_ERROR_EXPIRED    2
#define AB_ERROR_TICKET     3
#define AB_ERROR_LOCKED     4
#define AB_ERROR_ALLREADY_PARTICIPATING 5
#define AB_ERROR_TOO_MANY_PARTICIPANTS  6
#define AB_ERROR_TIMEOUT    7
#define AB_ERROR_AUTHBLOCK  8
#define AB_ERROR_AUTHBLOCK_TIMEOUT 11
#define AB_ERROR_PINTIMEOUT 9
#define AB_ERROR_PIN       10


#define ACTION_MAINOPEN    0x00
#define ACTION_MAINCLOSE   0x01
#define ACTION_ADDUSER     0x10
#define ACTION_REMUSER     0x11
#define ACTION_LOCKUSER    0x12
#define ACTION_UNLOCKUSER  0x13
#define ACTION_ADDADMIN    0x14
#define ACTION_REMADMIN    0x15
#define ACTION_KEYMIGRATION 0x16

#define STR_CLASS_NO    0x00
#define STR_CLASS_INFO  0x01
#define STR_CLASS_WARN  0x02
#define STR_CLASS_ERROR 0x03
#define STR_CLASS_SEC   0x04
#define STR_CLASS_DBG   0x04
#define STR_CLASS_MAX   0x05
/* the following are the same as above but define that the string is stored in PROGMEM */
#define STR_CLASS_NO_P    0x10
#define STR_CLASS_INFO_P  0x11
#define STR_CLASS_WARN_P  0x12
#define STR_CLASS_ERROR_P 0x13
#define STR_CLASS_SEC_P   0x14
#define STR_CLASS_DBG_P   0x15
#define STR_CLASS_MAX_P   0x15

#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0

/* these are defined in main.c */
extern lop_ctx_t lop0;
extern qport_ctx_t qp0;
extern lop_ctx_t lop1;



void messagerx(uint16_t length, void* msg);
void send_str(uint8_t terminal_id, char* str, uint8_t str_class);

void freemsg(void);
uint8_t waitformessage(uint32_t timeout);
uint8_t getmsgid(const void* msg);

#endif /*COMM_H_*/
