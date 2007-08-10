/**
 * 
 * 
 * 
 * 
 * 
 */

#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <avr/eeprom.h> 
#include "qport.h"
#include "lop.h"
#include "prng.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "xtea.h"

#include <avr/io.h>

#include "uart.h"

/******************************************************************************/

#ifndef QPORT_STREAMAUTH
 #define QPORT_STREAMAUTH 5
#endif

/******************************************************************************/

uint8_t xtea_cfb_enc(uint8_t p, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = p ^ cb[0];
	return p ^ cb[0];
}

/******************************************************************************/

uint8_t xtea_cfb_dec(uint8_t c, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = c;
	return c ^ cb[0];
}
/******************************************************************************/

uint8_t xtea_ofb(uint8_t p, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = cb[0];
	return p ^ cb[0];
}
/******************************************************************************/
static
uint8_t stream_enc(qport_ctx_t * ctx, uint8_t b){
	b = xtea_cfb_enc(b,ctx->streamkey_txa, ctx->streamstate_txa);
	b = xtea_ofb(b,ctx->streamkey_txb, ctx->streamstate_txb);
	return b;
}

/******************************************************************************/
static
uint8_t stream_dec(qport_ctx_t * ctx, uint8_t b){
	b = xtea_ofb(b,ctx->streamkey_rxb, ctx->streamstate_rxb);
	b = xtea_cfb_dec(b,ctx->streamkey_rxa, ctx->streamstate_rxa);
	return b;
}

/******************************************************************************/
static inline
void qport_sendbyte(qport_ctx_t * ctx, uint8_t b){
	lop_sendstream(ctx->lop, b);
}

/******************************************************************************/

void qport_streamsend(qport_ctx_t * ctx, uint8_t p){
	uint8_t i;
	for(i=0; i<QPORT_STREAMAUTH; ++i){
		qport_sendbyte(ctx, stream_enc(ctx,p));				
	}
}

/******************************************************************************/

void qport_recieve_byte(qport_ctx_t * ctx, uint8_t b){
	b = stream_dec(ctx, b);
	if(ctx->streamrx_index==0){
		ctx->streamrx_value = b;
	} else {
		if(ctx->streamrx_value != b){
			/* evil error, maybe someone changed some packets */
			/* we should do a resync */
			return;
		}
	}
	ctx->streamrx_index++;
	if(ctx->streamrx_index==QPORT_STREAMAUTH){
		if(ctx->on_byterx)
			ctx->on_byterx(b);
		ctx->streamrx_index=0;
	}
		
}

/******************************************************************************/

#define MARKER_KP         0x01
static
void genkeypacket(qport_keypacket_t * kp){
	fillBlockRandom(kp, sizeof(qport_keypacket_t));
	kp->marker = MARKER_KP;
}

/******************************************************************************/

void memxor(void * dest, const void * src, uint16_t n){
	while(n--){
		((uint8_t*)dest)[n] ^= ((uint8_t*)src)[n];
	}
}

/******************************************************************************/

#define RX 0
#define TX 1

void qport_setupstream(qport_ctx_t * ctx, uint8_t rxtx, uint8_t * a, uint8_t * b){
	sha256_hash_t hmac;
	uint8_t hmackey[32];
	uint8_t buffer[64];

	eeprom_read_block(hmackey, ctx->master_mac_key, 32);
	memcpy(buffer +  0, a, 32);	
	memcpy(buffer + 32, b, 32);
	hmac_sha256(hmac, hmackey, 32*8, buffer, 64*8);	
	if(rxtx==RX){
		eeprom_read_block(ctx->streamkey_rxa, ctx->master_enc_key_rxa, 16);
		eeprom_read_block(ctx->streamkey_rxb, ctx->master_enc_key_rxb, 16);
		memcpy(ctx->streamstate_rxa, hmac +  0, 8);
		memcpy(ctx->streamstate_rxb, hmac +  8, 8);
		memxor(ctx->streamkey_rxa, hmac + 16, 8);
		memxor(ctx->streamkey_rxb, hmac + 24, 8);
	} else {
		eeprom_read_block(ctx->streamkey_txa, ctx->master_enc_key_txa, 16);
		eeprom_read_block(ctx->streamkey_txb, ctx->master_enc_key_txb, 16);
		memcpy(ctx->streamstate_txa, hmac +  0, 8);
		memcpy(ctx->streamstate_txb, hmac +  8, 8);
		memxor(ctx->streamkey_txa, hmac + 16, 8);
		memxor(ctx->streamkey_txb, hmac + 24, 8);
	}
} 

/******************************************************************************/

void qport_onkp(qport_ctx_t * ctx, qport_keypacket_t *kp){
	PORTC = 0x02;
	if(ctx->keyingdata && (ctx->keyingdata->id == kp->id)){
		/* we recived a response to our kp */
		qport_setupstream(ctx, TX, ctx->keyingdata->seed_a, kp->seed_a);
		qport_setupstream(ctx, RX, ctx->keyingdata->seed_b, kp->seed_b);
		free(ctx->keyingdata);
		ctx->keyingdata = 0;
	} else {
		/* we should respond to the incomming packet */
		qport_keypacket_t kpresponse;
		genkeypacket(&kpresponse);
		kpresponse.id = kp->id;
		lop_sendmessage(ctx->lop, sizeof(qport_keypacket_t), (uint8_t*)&kpresponse);
		qport_setupstream(ctx,TX, kp->seed_a, kpresponse.seed_a);
		qport_setupstream(ctx,RX, kp->seed_b, kpresponse.seed_b);
		{
			uint8_t help[16];
			/* swap rx and tx keys */
			memcpy(help, ctx->streamkey_rxa, 16);
			memcpy(ctx->streamkey_rxa, ctx->streamkey_txa, 16);
			memcpy(ctx->streamkey_txa, help, 16);
			memcpy(help, ctx->streamkey_rxb, 16);
			memcpy(ctx->streamkey_rxb, ctx->streamkey_txb, 16);
			memcpy(ctx->streamkey_txb, help, 16);
			/* swap rx and tx states */
			memcpy(help, ctx->streamstate_rxa, 8);
			memcpy(ctx->streamstate_rxa, ctx->streamstate_txa, 8);
			memcpy(ctx->streamstate_txa, help, 8);
			memcpy(help, ctx->streamstate_rxb, 8);
			memcpy(ctx->streamstate_rxb, ctx->streamstate_txb, 8);
			memcpy(ctx->streamstate_txb, help, 8);
		}
	}
}
			
/******************************************************************************/

void qport_rekey(qport_ctx_t * ctx){
	ctx->keyingdata = malloc(sizeof(qport_keypacket_t));
	genkeypacket(ctx->keyingdata);
	lop_sendmessage(ctx->lop, sizeof(qport_keypacket_t), (uint8_t*)(ctx->keyingdata));
}

/******************************************************************************/
 /* server part */
/******************************************************************************/
void qport_incomming_msg(qport_ctx_t * ctx, uint16_t length, uint8_t * buffer){	
	if(length==0)
	 return;
		
	switch(buffer[0]){
		case MARKER_KP:
			if(length==sizeof(qport_keypacket_t))
				qport_onkp(ctx, (qport_keypacket_t*)buffer);
			break;
		default:
			/* drop? */
			break;
	}
}

/******************************************************************************/














