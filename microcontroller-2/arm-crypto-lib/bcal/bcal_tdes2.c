/* bcal_tdes2.c */
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
 * \file     bcal_tdes.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2010-02-02
 * \license  GPLv3 or later
 * 
 */

#include <stdlib.h>
#include "blockcipher_descriptor.h"
#include "des.h"
#include "keysize_descriptor.h"

#include <string.h>

const char tdes2_str[]   = "TDES-2";

const uint8_t tdes2_keysize_desc[] = { KS_TYPE_LIST, 1, KS_INT(128),
                                       KS_TYPE_TERMINATOR    };

void tdes2_dummy_enc(void* block, const void* key){
	tdes_enc(block, block, key);
}

void tdes2_dummy_dec(void* block, const void* key){
	tdes_dec(block, block, key);
}

void tdes2_init(void* key, void* ctx){
	memcpy(ctx, key, 16);
	memcpy((uint8_t*)ctx+16, key, 8);
}



const bcdesc_t tdes2_desc = {
	BCDESC_TYPE_BLOCKCIPHER,
	BC_INIT_TYPE_1,
	tdes2_str,
	24,
	64,
	{(void_fpt)tdes2_init},
	{(void_fpt)tdes2_dummy_enc},
	{(void_fpt)tdes2_dummy_dec},
	(bc_free_fpt)NULL,
	tdes2_keysize_desc
};


