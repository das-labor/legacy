/* bcal_xtea.c */
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
 * \file     bcal_xtea.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-01-09
 * \license  GPLv3 or later
 * 
 */

#include <stdlib.h>
#include "blockcipher_descriptor.h"
#include "xtea.h"
#include "keysize_descriptor.h"

const char xtea_str[] = "XTEA";

const uint8_t xtea_keysize_desc[] = { KS_TYPE_LIST, 1, KS_INT(128),
                                      KS_TYPE_TERMINATOR    };

static
void xtea_dummy_enc(void* block, void* key){
	xtea_enc(block, block, key);
}

static
void xtea_dummy_dec(void* block, void* key){
	xtea_dec(block, block, key);
}

const bcdesc_t xtea_desc = {
	BCDESC_TYPE_BLOCKCIPHER,
	BC_INIT_TYPE_2,
	xtea_str,
	16,
	64,
	{(void_fpt)NULL},
	{(void_fpt)xtea_dummy_enc},
	{(void_fpt)xtea_dummy_dec},
	(bc_free_fpt)NULL,
	xtea_keysize_desc
};


