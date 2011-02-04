/* bcal_des.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * \file     bcal_des.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-01-09
 * \license  GPLv3 or later
 * 
 */

#include <stdlib.h>
#include "blockcipher_descriptor.h"
#include "des.h"
#include "keysize_descriptor.h"

const char des_str[]  = "DES";

const uint8_t des_keysize_desc[] = { KS_TYPE_LIST, 1, KS_INT(64),
                                     KS_TYPE_TERMINATOR    };
static
void des_dummy_enc(void* block, void* key){
	des_enc(block, block, key);
}

static
void des_dummy_dec(void* block, void* key){
	des_dec(block, block, key);
}

const bcdesc_t des_desc = {
	BCDESC_TYPE_BLOCKCIPHER,
	BC_INIT_TYPE_1,
	des_str,
	8,
	128,
	{(void_fpt)NULL},
	{(void_fpt)des_dummy_enc},
	{(void_fpt)des_dummy_dec},
	(bc_free_fpt)NULL,
	des_keysize_desc
};


