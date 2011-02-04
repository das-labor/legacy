/* bcal-nessie.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

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

#include "nessie_bc_test.h"
#include "blockcipher_descriptor.h"
#include "keysize_descriptor.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void(*bcal_nessie_dummy_init_fpt)(const void* key, void* ctx)=NULL;

void bcal_nessie_dummy_init(const void* key, uint16_t keysize, void* ctx){
	if(bcal_nessie_dummy_init_fpt){
		bcal_nessie_dummy_init_fpt(key, ctx);
	}else{
		memcpy(ctx, key, (keysize+7)/8);
	}
}

void bcal_nessie(const bcdesc_t* bcd){
	if(bcd->type!=BCDESC_TYPE_BLOCKCIPHER)
		return;
	nessie_bc_init();

	nessie_bc_ctx.blocksize_B = (bcd->blocksize_b+7)/8;
	nessie_bc_ctx.name        = bcd->name;
	nessie_bc_ctx.ctx_size_B  = bcd->ctxsize_B;
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)(bcd->enc.encvoid);
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)(bcd->dec.decvoid);
	nessie_bc_ctx.cipher_free = (nessie_bc_free_fpt)(bcd->free);
	if(((bcd->flags)&BC_INIT_TYPE)==BC_INIT_TYPE_2){
		nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)(bcd->init.initvoid);
	}else{
		bcal_nessie_dummy_init_fpt = (void(*)(const void*,void*))(bcd->init.initvoid);
		nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)bcal_nessie_dummy_init;
	}

	uint16_t *keysize_list=NULL;
	uint16_t items,i;
	items = get_keysizes(bcd->valid_keysize_desc, &keysize_list);
	if(items){
		for(i=0; i<items; ++i){
			nessie_bc_ctx.keysize_b   = keysize_list[i];
			nessie_bc_run();
		}
		free(keysize_list);
	}

}

void bcal_nessie_multiple(const bcdesc_t** bcd_list){
	const bcdesc_t* bcd;
	for(;;){
		bcd = *bcd_list++;
		if(!bcd)
			return;
		bcal_nessie(bcd);
	}
}

