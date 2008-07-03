/**
 * \file qport.c
 * \author Daniel Otte
 * \date 2007-08-10
 * \par license 
 *  GPLv3
 * 
 */

#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "config.h"
#include "qport.h"
#include "lop.h"
#include "entropium.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "xtea.h"


#ifdef LED_DEBUG
 #include <avr/io.h>
#endif

/******************************************************************************/

#ifndef QPTINY_STREAMAUTH
 #define QPTINY_STREAMAUTH 5
#endif

/******************************************************************************/
/**
 * \brief encrypt a byte with xtea in cipher feedback mode
 * \param[in]     p      plaintext byte
 * \param[in]     key    encryption key
 * \param[in,out] state  state of the cfb mode
 * \return ciphertext byte
 */
uint8_t xtea_cfb_enc(uint8_t p, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = p ^ cb[0];
	return p ^ cb[0];
}

/******************************************************************************/
/**
 * \brief decrypt a byte with xtea in cipher feedback mode
 * \param[in]     c      ciphertext byte
 * \param[in]     key    decryption key
 * \param[in,out] state  state of the cfb mode
 * \return plaintext byte
 */
uint8_t xtea_cfb_dec(uint8_t c, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = c;
	return c ^ cb[0];
}
/******************************************************************************/
/**
 * \brief decrypt/encrypt a byte with xtea in output feedback mode
 * \param[in]     c      ciphertext/plaintext byte
 * \param[in]     key    decryption/encryption key
 * \param[in,out] state  state of the cfb mode
 * \return plaintext/ciphertext byte
 */
uint8_t xtea_ofb(uint8_t p, uint8_t * key, uint8_t * state){
	uint8_t cb[XTEA_BLOCKSIZEB];
	xtea_enc((uint32_t*)cb,(uint32_t*)state,(uint32_t*)key);
	memmove(state, state+1,XTEA_BLOCKSIZEB-1);
	state[XTEA_BLOCKSIZEB-1] = cb[0];
	return p ^ cb[0];
}
/******************************************************************************/
/**
 * \brief encrypt a single byte for stream transmission 
 * \param[in,out] ctx    qport context for keys, state & co.
 * \param[in]     b      plaintext byte
 * \return plaintext/ciphertext byte
 */
static
uint8_t stream_enc(qport_ctx_t * ctx, uint8_t b){
	b = xtea_cfb_enc(b,ctx->streamkey_txa, ctx->streamstate_txa);
	b = xtea_ofb(b,ctx->streamkey_txb, ctx->streamstate_txb);
	return b;
}

/******************************************************************************/
/**
 * \brief decrypt a single byte for stream transmission 
 * \param[in,out] ctx    qport context for keys, state & co.
 * \param[in]     b      ciphertext byte
 * \return plaintext/ciphertext byte
 */
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
	if(ctx->keystate==unkeyed){
	 #ifdef QPTINY_AUTOKEYING
		qport_rekey(ctx);	
		while(ctx->keystate==unkeyed)
			;
	 #else
	  	return;
	 #endif
	
	}
	for(i=0; i<QPTINY_STREAMAUTH; ++i){
	#ifndef QPTINY_CRYPTO_OFF	
		qport_sendbyte(ctx, stream_enc(ctx,p));				
	#else
		qport_sendbyte(ctx, p);				
	#endif
	}
}

/******************************************************************************/

void qport_recieve_byte(qport_ctx_t * ctx, uint8_t b){
	#ifndef QPTINY_CRYPTO_OFF
	b = stream_dec(ctx, b);
	#endif
	if(ctx->keystate == unkeyed){
		#ifdef LED_DEBUG
	//	PORTC |= 0x40;
		#endif
		return;
	}
	if(ctx->streamrx_index==0){
		ctx->streamrx_value = b;
	} else {
		if(ctx->streamrx_value != b){
			/* evil error, maybe someone changed some packets */
			/* we should do a resync */
			ctx->keystate = unkeyed;
			ctx->streamrx_index = 0;
		    #ifdef LED_DEBUG
			PORTC |= 0x80;
			#endif
			return;
		}
	}
	ctx->streamrx_index++;
	if(ctx->streamrx_index==QPTINY_STREAMAUTH){
		if(ctx->on_byterx)
			ctx->on_byterx(b);
		ctx->streamrx_index=0;
	}
		
}

/******************************************************************************/

#define MARKER_KP         0x01
static
void genkeypacket(qport_keypacket_t * kp){
	entropium_fillBlockRandom(kp, sizeof(qport_keypacket_t));
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
	#ifdef LED_DEBUG
	PORTC ^= 0x10;
	#endif
	while(ctx->keystate==makeingkey){
		#ifdef LED_DEBUG
		PORTC ^= 0x80;
		#endif
		return;		
	}
	if(ctx->keyingdata && (ctx->keyingdata->id == kp->id)){
		/* we recived a response to our kp */
		#ifdef LED_DEBUG
		PORTC |= 0x40;
		#endif
		qport_setupstream(ctx, TX, ctx->keyingdata->seed_a, kp->seed_a);
		qport_setupstream(ctx, RX, ctx->keyingdata->seed_b, kp->seed_b);
		free(ctx->keyingdata);
		ctx->keyingdata = 0;
		ctx->keystate = keyed;
	} else {
		if((!ctx->keyingdata)||(ctx->keyingdata && ((ctx->keyingdata->id) > (kp->id)))){ /* lower ID wins */
		/* we should respond to the incomming packet */
			#ifdef LED_DEBUG
			PORTC ^= 0x20;
			#endif
			if (ctx->keyingdata){
				free(ctx->keyingdata);
				ctx->keyingdata = 0;
			}
			qport_keypacket_t kpresponse;
			genkeypacket(&kpresponse);
			kpresponse.id = kp->id;
			lop_sendmessage(ctx->lop, sizeof(qport_keypacket_t), (uint8_t*)&kpresponse);
			qport_setupstream(ctx,TX, kp->seed_a, kpresponse.seed_a);
			qport_setupstream(ctx,RX, kp->seed_b, kpresponse.seed_b);
			{
			#ifdef QPORT_NOOPTIMISE
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
			#else
				memxor(ctx->streamkey_txa, ctx->streamkey_rxa, 48);
				memxor(ctx->streamkey_rxa, ctx->streamkey_txa, 48);
				memxor(ctx->streamkey_txa, ctx->streamkey_rxa, 48);
			#endif
			
			}
			ctx->keystate = keyed;
		}else{
			/* the other party should respond to our packet and we might simply their */		
		}
	}
}
			
/******************************************************************************/

void qport_rekey(qport_ctx_t * ctx){
	qport_keypacket_t* temp;
	
	temp = malloc(sizeof(qport_keypacket_t));
	genkeypacket(temp);
	
	if(ctx->keystate == unkeyed){
		ctx->keystate = makeingkey;
		cli();
		if(ctx->keyingdata)
			free(ctx->keyingdata);
		ctx->keyingdata = temp;
		lop_sendmessage(ctx->lop, sizeof(qport_keypacket_t), (uint8_t*)(ctx->keyingdata));
		ctx->keystate = unkeyed;
		sei();
	}else{
		free(temp);
	}
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














