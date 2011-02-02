/* bcal_cast6.c */
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
 * \file     bcal_cast6.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-04-03
 * \license  GPLv3 or later
 * 
 */

#include <stdlib.h>
#include "blockcipher_descriptor.h"
#include "cast6.h"
#include "keysize_descriptor.h"

const char cast6_str[] = "CAST-256";

const uint8_t cast6_keysize_desc[] = { KS_TYPE_RANGE, KS_INT(0), KS_INT(256),
                                       KS_TYPE_TERMINATOR    };

const bcdesc_t cast6_desc = {
	BCDESC_TYPE_BLOCKCIPHER,
	BC_INIT_TYPE_2,
	cast6_str,
	sizeof(cast6_ctx_t),
	128,
	{(void_fpt)cast6_init},
	{(void_fpt)cast6_enc},
	{(void_fpt)cast6_dec},
	(bc_free_fpt)NULL,
	cast6_keysize_desc
};


