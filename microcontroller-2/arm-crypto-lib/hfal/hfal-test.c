/* hfal-test.c */
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
/*
 * \file    hfal-test.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2009-05-10
 * \license GPLv3 or later
 * 
 */

#include "hfal-basic.h"
#include "hashfunction_descriptor.h"
#include "cli.h"
#include <stdint.h>

void hfal_test(const hfdesc_t* hd, const void* msg, uint32_t length_b){
	if(hd->type!=HFDESC_TYPE_HASHFUNCTION)
		return;
	uint16_t dlen = (hd->hashsize_b+7)/8;
	uint8_t digest[dlen];
	cli_putstr("\r\n=== ");
	cli_putstr(hd->name);
	cli_putstr(" ===\r\n message:");
	cli_hexdump_block(msg, (length_b+7)/8, 4, 16);
	hfal_hash_mem(hd, digest, msg, length_b);
	cli_putstr(" \r\n digest:");
	cli_hexdump_block(digest, dlen, 4, 16);
	cli_putstr("\r\n");
}


