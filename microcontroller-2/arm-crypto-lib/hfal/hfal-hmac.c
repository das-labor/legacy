/* hfal-hmac.c */
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
#include "hfal-hmac.h"
#include <stdlib.h>
#include <string.h>

#define IPAD 0x36
#define OPAD 0x5C

uint8_t hfal_hmac_init(const hfdesc_t* hash_descriptor, 
                       hfhmacgen_ctx_t* ctx, 
					   const void* key, uint16_t keylength_b){
	uint16_t  bs = hfal_hash_getBlocksize(hash_descriptor);
	uint8_t buffer[bs/8];
	uint8_t i;
	hf_init_fpt init;
	hf_nextBlock_fpt nextBlock;
	memset(buffer, 0, bs/8);
	ctx->desc   = hash_descriptor;
	ctx->ctx    = malloc(hash_descriptor->ctxsize_B);
	ctx->finctx = malloc(hash_descriptor->ctxsize_B);
	if(ctx->ctx==NULL && ctx->finctx==NULL)
		return 3;
	if(ctx->finctx==NULL){
		free(ctx->ctx);
		return 2;
	}
	if(ctx->ctx==NULL){
		free(ctx->finctx);
		return 1;
	}		
	if(keylength_b>bs){
		hfal_hash_mem(hash_descriptor, buffer, key, keylength_b);
	} else {
		memcpy(buffer, key, (keylength_b+7)/8);
	}
	for(i=0; i<bs/8; ++i){
		buffer[i] ^= IPAD;
	}
	init = hash_descriptor->init;
	nextBlock = hash_descriptor->nextBlock;
	init(ctx->ctx);
	init(ctx->finctx);
	nextBlock(ctx->ctx, buffer);
	for(i=0; i<bs/8; ++i){
		buffer[i] ^= IPAD^OPAD;
	}
	nextBlock(ctx->finctx, buffer);
	memset(buffer, 0, bs/8);
	return 0;
}

int hfal_hmac_ctxcopy(hfhmacgen_ctx_t* dest, hfhmacgen_ctx_t* src){
	dest->desc = src->desc;
	dest->ctx = malloc(dest->desc->ctxsize_B);
	if(dest->ctx == NULL){
		return -1;
	}
	memcpy(dest->ctx, src->ctx, dest->desc->ctxsize_B);
	dest->finctx = malloc(dest->desc->ctxsize_B);
	if(dest->finctx == NULL){
		return -1;
	}
	memcpy(dest->finctx, src->finctx, dest->desc->ctxsize_B);
	return 0;
}

void hfal_hmac_nextBlock(hfhmacgen_ctx_t* ctx, const void* block){
	ctx->desc->nextBlock(ctx->ctx, block);
}

void hfal_hmac_lastBlock(hfhmacgen_ctx_t* ctx, const void* block, uint16_t length_b){
	hf_lastBlock_fpt lastBlock;
	hf_ctx2hash_fpt  ctx2hash;
	uint16_t hs = ctx->desc->hashsize_b;
	uint8_t buffer[(hs+7)/8];
	lastBlock = ctx->desc->lastBlock;
	ctx2hash = ctx->desc->ctx2hash;
	lastBlock(ctx->ctx, block, length_b);
	ctx2hash(buffer, ctx->ctx);
	lastBlock(ctx->finctx, buffer, hs);
}

void hfal_hmac_ctx2mac(void* dest, hfhmacgen_ctx_t* ctx){
	hf_ctx2hash_fpt  ctx2hash;
	ctx2hash = ctx->desc->ctx2hash;
	ctx2hash(dest, ctx->finctx);
}

void hfal_hmac_free(hfhmacgen_ctx_t* ctx){
	hf_free_fpt free_fpt;
	free_fpt = ctx->desc->free;
	if(free_fpt){
		free_fpt(ctx->ctx);
		free_fpt(ctx->finctx);
	}
	free(ctx->ctx);
	free(ctx->finctx);
}

void hfal_hmac_mem(const hfdesc_t* hash_descriptor, const void* key, uint16_t keylength_b, void* dest, const void* msg, uint32_t length_b){
	hfhmacgen_ctx_t ctx;
	uint16_t  bs = hfal_hash_getBlocksize(hash_descriptor);
	hfal_hmac_init(hash_descriptor, &ctx, key, keylength_b);
	while(length_b>bs){
		hfal_hmac_nextBlock(&ctx, msg);
		msg = (uint8_t*)msg + bs/8;
		length_b-=bs;
	}
	hfal_hmac_lastBlock(&ctx, msg, length_b);
	hfal_hmac_ctx2mac(dest, &ctx);
	hfal_hmac_free(&ctx);
}

uint16_t hfal_hmac_getBlocksize(const hfdesc_t* hash_descriptor){
	return hfal_hash_getBlocksize(hash_descriptor);
}

uint16_t hfal_hmac_getMACsize(const hfdesc_t* hash_descriptor){
	return hfal_hash_getHashsize(hash_descriptor);
}


