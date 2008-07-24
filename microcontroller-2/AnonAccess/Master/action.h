/* action.h */
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


#ifndef ACTION_H_
#define ACTION_H_

#include <stdint.h>
#include "types.h"
#include "sha256.h"

void door_init(void);
uint8_t main_open(void);
uint8_t main_close(void);
void dump_authblock(authblock_t * ab);

void add_user(char * nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags, authblock_t *dest);
uint8_t rem_user(uint8_t* id);
uint8_t lock_user(uint8_t* id);
uint8_t unlock_user(uint8_t* id);
uint8_t add_admin(uint8_t* id);
uint8_t rem_admin(uint8_t* id);
uint8_t keymigration(void);
uint8_t rem_keymigration(void);
uint8_t lock_nick(uint8_t *nick);
uint8_t open_nick(uint8_t *nick);
void self_destruct(void);

#endif /*ACTION_H_*/
