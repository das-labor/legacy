/* hfal-nessie.c */
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
 * \file    hfal-nessie.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2009-05-10
 * \license GPLv3 or later
 * 
 */

#include "nessie_hash_test.h"
#include "hashfunction_descriptor.h"
#include <stdint.h>

void hfal_nessie(const hfdesc_t* hd){
	if(hd->type!=HFDESC_TYPE_HASHFUNCTION)
		return;
	
	nessie_hash_ctx.hashsize_b  = hd->hashsize_b;
	nessie_hash_ctx.name        = hd->name;
	nessie_hash_ctx.blocksize_B = hd->blocksize_b/8;
	nessie_hash_ctx.ctx_size_B  = hd->ctxsize_B;
	nessie_hash_ctx.hash_init = (nessie_hash_init_fpt)(hd->init);
	nessie_hash_ctx.hash_next = (nessie_hash_next_fpt)(hd->nextBlock);
	nessie_hash_ctx.hash_last = (nessie_hash_last_fpt)(hd->lastBlock);
	nessie_hash_ctx.hash_conv = (nessie_hash_conv_fpt)(hd->ctx2hash);
	
	nessie_hash_run();
}

void hfal_nessie_multiple(const hfdesc_t** hd_list){
	const hfdesc_t* hd;
	for(;;){
		hd = *hd_list;
		if(!hd)
			return;
		hfal_nessie(hd);
		hd_list += 1;
	}
}

