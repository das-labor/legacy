/* bcal-basic.c */
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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "blockcipher_descriptor.h"
#include "keysize_descriptor.h"

uint8_t bcal_cipher_init(const bcdesc_t* cipher_descriptor,
                         const void* key, uint16_t keysize_b, bcgen_ctx_t* ctx){
	if(!is_valid_keysize_P(cipher_descriptor->valid_keysize_desc, keysize_b)){
		return 1;
	}
	uint8_t flags;
	bc_init_fpt init_fpt;
	ctx->desc_ptr = (bcdesc_t*)cipher_descriptor;
	ctx->keysize  = keysize_b;
	flags = cipher_descriptor->flags;
	init_fpt.initvoid = (void_fpt)(cipher_descriptor->init.initvoid);
	if(init_fpt.initvoid == NULL){
		if(!(ctx->ctx = malloc((keysize_b+7)/8)))
			return 2;
		memcpy(ctx->ctx, key, (keysize_b+7)/8);
		return 0;
	}
	if(!(ctx->ctx = malloc(cipher_descriptor->ctxsize_B)))
		return 3;
	if((flags&BC_INIT_TYPE)==BC_INIT_TYPE_1){
		init_fpt.init1((void*)key, (ctx->ctx));
	}else{
		init_fpt.init2((void*)key, keysize_b, (ctx->ctx));
	}
	return 0;
}

void bcal_cipher_free(bcgen_ctx_t* ctx){
	if(!ctx)
		return;
	bc_free_fpt free_fpt;
	free_fpt = (bc_free_fpt)(ctx->desc_ptr->free);
	if(free_fpt)
		free_fpt((ctx->ctx));
	free(ctx->ctx);
}

void bcal_cipher_enc(void* block, const bcgen_ctx_t* ctx){
	bc_enc_fpt enc_fpt;
	enc_fpt.encvoid = (void_fpt)(ctx->desc_ptr->enc.encvoid);
	if(!enc_fpt.encvoid){
		/* very bad error, no enciphering function specified */
		return;
	}
	enc_fpt.enc1(block, (ctx->ctx));
	
}

void bcal_cipher_dec(void* block, const bcgen_ctx_t* ctx){
	bc_dec_fpt dec_fpt;
	dec_fpt.decvoid = (void_fpt)(ctx->desc_ptr->dec.decvoid);
	if(!dec_fpt.decvoid){
		/* very bad error, no deciphering function specified */
		return;
	}
	dec_fpt.dec1(block, (ctx->ctx));
}

uint16_t bcal_cipher_getBlocksize_b(const bcdesc_t* desc){
	return (desc->blocksize_b);
}

const void* bcal_cipher_getKeysizeDesc(const bcdesc_t* desc){
	return (desc->valid_keysize_desc);
}


