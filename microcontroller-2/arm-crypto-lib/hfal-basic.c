/* hfal-basic.c */
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

#include "hashfunction_descriptor.h"
#include "hfal-basic.h"
#include <stdlib.h>

uint8_t hfal_hash_init(const hfdesc_t* hash_descriptor, hfgen_ctx_t* ctx){
	ctx->desc_ptr = (hfdesc_t*)hash_descriptor;
	if(!(ctx->ctx=malloc(hash_descriptor->ctxsize_B)))
		return 3;
	hash_descriptor->init(ctx->ctx);
	return 0;
}

void hfal_hash_nextBlock(hfgen_ctx_t* ctx, const void* block){
	ctx->desc_ptr->nextBlock(ctx->ctx, block);
}

void hfal_hash_lastBlock(hfgen_ctx_t* ctx, const void* block, uint16_t length_b){
	ctx->desc_ptr->lastBlock(ctx->ctx, block, length_b);
}

void hfal_hash_ctx2hash(void* dest, hfgen_ctx_t* ctx){
	ctx->desc_ptr->ctx2hash(dest, ctx->ctx);
}

void hfal_hash_free(hfgen_ctx_t* ctx){
	hf_free_fpt f;
	f = ctx->desc_ptr->free;
	if(f)
		f(ctx->ctx);
	free(ctx->ctx);
}

void hfal_hash_mem(const hfdesc_t* hash_descriptor, void* dest, const void* msg, uint32_t length_b){
	void_fpt f;
	f = (void_fpt)(hash_descriptor->mem);
	if(f){
		((hf_mem_fpt)f)(dest, msg, length_b);
	}else{
		uint16_t bs,bsb;
		uint8_t ctx[hash_descriptor->ctxsize_B];
		hash_descriptor->init(ctx);
		bs = hash_descriptor->blocksize_b;
		bsb=bs/8;
		f=(void_fpt)(hash_descriptor->nextBlock);
		while(length_b>bs){
			((hf_nextBlock_fpt)f)(ctx, msg);
			length_b -= bs;
			msg = (uint8_t*)msg + bsb;
		}
		hash_descriptor->lastBlock(ctx, msg, length_b);
		hash_descriptor->ctx2hash(dest, ctx);
	}
}

uint16_t hfal_hash_getBlocksize(const hfdesc_t* hash_descriptor){
	return hash_descriptor->blocksize_b;
}

uint16_t hfal_hash_getHashsize(const hfdesc_t* hash_descriptor){
	return hash_descriptor->hashsize_b;
}
