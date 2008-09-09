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
#include "../common/comm_const.h"

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
