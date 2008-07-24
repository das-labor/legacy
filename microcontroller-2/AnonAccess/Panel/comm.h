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
#include <stdint.h>

#define MASTERUNIT_ID 0
#define TERMINALUNIT_ID 1

#define MSGID_SESSION_INIT  0
#define MSGID_ADD_AB        1
#define MSGID_AB_REPLY      2
#define MSGID_AB_PINREQ     8
#define MSGID_AB_PINREPLY   9
#define MSGID_AB_ERROR      3
#define MSGID_ACTION        4
#define MSGID_ACTION_REPLY  5
#define MSGID_PRINT         6
#define MSGID_ADD_BOOTSTRAP 7

#define AB_ERROR_WONTTELL   0
#define AB_ERROR_HMAC       1
#define AB_ERROR_EXPIRED    2
#define AB_ERROR_TICKET     3
#define AB_ERROR_LOCKED     4

#define ACTION_MAINOPEN     0x00
#define ACTION_MAINCLOSE    0x01
#define ACTION_ADDUSER      0x10
#define ACTION_REMUSER      0x11
#define ACTION_LOCKUSER     0x12
#define ACTION_UNLOCKUSER   0x13
#define ACTION_ADDADMIN     0x14
#define ACTION_REMADMIN     0x15
#define ACTION_KEYMIGRATION 0x16
#define ACTION_LOCKNICK     0x17
#define ACTION_OPENNICK     0x18
#define ACTION_GETINFO      0x19
#define ACTION_SEARCH       0x1A
#define ACTION_SEARCHCONT   0x1B
#define ACTION_GETSTATS     0x1C
#define ACTION_MIN_N        0x10
#define ACTION_MAX_N        0x1C

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
extern const char str_class_char_P[]; // PROGMEM = {'#','i','w','E','X','d'};


#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0

extern volatile uint8_t msg_wait;
extern volatile uint16_t msg_length;
extern volatile void* msg_data;

void init_session(void);
void submit_ab(authblock_t * ab, uint8_t admin);
void submit_pin(char* pin, uint16_t length_B);
void send_mainopen(void);
void send_mainclose(void);
void send_getstat(void);
void send_getinfo(void);
void req_bootab(char* name, char* pin, uint16_t pinlen_B, uint8_t anon, uint8_t pinflags);

void freemsg(void);
uint8_t waitformessage(uint16_t timeout);
uint8_t getmsgid(volatile const void* msg);

#endif /*COMM_H_*/
