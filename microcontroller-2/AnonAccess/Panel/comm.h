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
#include "../common/comm_const.h"

extern const char str_class_char_P[]; // PROGMEM = {'#','i','w','E','X','d'};

extern volatile uint8_t msg_wait;
extern volatile uint16_t msg_length;
extern volatile void* msg_data;

void session_init(void);
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
