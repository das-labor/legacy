/* bcal_serpent.c */
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
 * \file     bcal_serpent.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-01-09
 * \license  GPLv3 or later
 * 
 */

#include <stdlib.h>
#include "blockcipher_descriptor.h"
#include "serpent.h"
#include "keysize_descriptor.h"

const char serpent_str[]  = "serpent";

const uint8_t serpent_keysize_desc[] = { KS_TYPE_RANGE, KS_INT(1), KS_INT(256),
                                         KS_TYPE_TERMINATOR    };

const bcdesc_t serpent_desc = {
	BCDESC_TYPE_BLOCKCIPHER,
	BC_INIT_TYPE_2,
	serpent_str,
	sizeof(serpent_ctx_t),
	128,
	{(void_fpt)serpent_init},
	{(void_fpt)serpent_enc},
	{(void_fpt)serpent_dec},
	(bc_free_fpt)NULL,
	serpent_keysize_desc
};


