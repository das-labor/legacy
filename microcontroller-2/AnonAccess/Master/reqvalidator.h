/* reqvalidator.h */
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


#ifndef REQVALIDATOR_H_
#define REQVALIDATOR_H_

bool check_permissions(uint8_t users, uint8_t admins, action_t action);
bool final_action(action_t action);
bool check_pin(authblock_t * ab, sha256_hash_t pinhash);
void change_pin(authblock_t * ab, sha256_hash_t pinhash);

authcredvalid_state_t check_authblock(authblock_t * ab);
void new_account(authblock_t * ab, char* nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags);

void modify_account(char * nickname, userflags_t setflags, userflags_t clearflags);
void modify_account_byuid(uid_t uid, userflags_t setflags, userflags_t clearflags);

uint8_t pin_required(uid_t uid, uint8_t admin);
bool final_action(action_t action);

#endif /*REQVALIDATOR_H_*/
