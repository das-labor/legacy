

/* 1. Stream-Cipher used for data: 
 * encipher:
 *     +-----+               +-----+
 *     |  A  |               |  B  |
 *     +-----+               +-----+
 *        |                     |
 *        V       +----+        V
 * P ---> X ----->| S  |------> + ------> C
 *                +----+
 * decipher:
 *     +-----+               +-----+
 *     |  B  |               |  A  |
 *     +-----+               +-----+
 *        |                     |
 *        V       +----+        V
 * C ---> - ----->| S' |------> X ------> P
 *                +----+
 * 
 * where:
 * 	A: stream-generator/cipher A
 *  B: stream-generator/cipher B
 *  S: s(ubstition)-Box 8bit -> 8bit
 *  S': inverse substitution to S so: S'(S(x))=x
 *  P: plaintext
 *  C: ciphertext
 *  X: xor 
 *  +: addition modulo 256
 *  -: substraction modulo 256
 */

/* selected parameters:
 *  since we don't have good stream ciphers at the moment we test with:
 *  A: XTEA in OFB-mode
 *  B: Skipjack in OFB-mode
 * both have a blocksize of 64bit. XTEA has a key of 128bit Skipjack uses only 80bits as key.
 */

#include <stdint.h>
#include <string.h> /* for memcmp() */
#include <avr/eeprom.h> 
#include "config.h"
#include "qcrypt.h"
#include "qstructure.h"
#include "error-handling.h"

#include "skipjack.h"
#include "xtea.h"
#include "qsbox.h"

/* BEGIN global vars */
uint8_t		keyA_rx[KEYA_SIZE];
uint8_t		keyA_tx[KEYA_SIZE];
uint8_t		keyB_rx[KEYB_SIZE];
uint8_t		keyB_tx[KEYB_SIZE];

#ifdef QP_MSG_CHECK_HMAC
uint8_t		keyS_tx[KEYS_SIZE];	/* siganture keys */
uint8_t		keyS_rx[KEYS_SIZE];
#endif

uint8_t EEMEM q_ltk_1_eeprom[128/8]={0xde,0xad,0xbe,0xef};
uint8_t EEMEM q_ltk_2_eeprom[ 80/8]={0x0B, 0xAD, 0xC0, 0xDE, 0x0B, 0xAD, 0xC0, 0xDE};
uint8_t EEMEM q_ltk_3_eeprom[256/8]={0x01};
uint8_t EEMEM q_ltk_S_eeprom[256/8]={0xAA, 0x55};

uint8_t qe_a_block[8];	/* data block for A in encrypt-direction (OFB) */
uint8_t qe_b_block[8]; 	/* data block for B in encrypt-direction (OFB) */
uint8_t qd_a_block[8];	/* data block for A in decrypt-direction (OFB) */
uint8_t qd_b_block[8];	/* data block for B in decrypt-direction (OFB) */

/* END global vars */

/******************************************************************************/
/**
 * xors source in destination until dlen or slen, whichever is larger.
 * 
 */
void memxor(void* dest, void* src, uint16_t dlen, uint16_t slen){
	uint16_t i=0, max, didx=0, sidx=0;
	
	max = (dlen < slen)?slen:dlen;
	
	while(i<max){
		((uint8_t*)dest)[didx] ^= ((uint8_t*)src)[sidx];
		++i; ++didx; ++sidx;
		if(didx==dlen)
			didx=0;
		if(sidx==slen)
			sidx=0;
	}
}

/******************************************************************************/

void q_stream_crypt_init(void){
	/*
	memcpy(qe_a_block, &(keyA_tx[16]), 8);
	memcpy(qe_b_block, &(keyB_tx[10]), 8);
	memcpy(qd_a_block, &(keyA_rx[16]), 8);
	memcpy(qd_b_block, &(keyB_rx[10]), 8);
	*/
	memset(qe_a_block, 0, 8);
	memset(qe_b_block, 0, 8);
	memset(qd_a_block, 0, 8);
	memset(qd_b_block, 0, 8);
}

uint8_t q_encipher(uint8_t d){
	xtea_enc((uint32_t*)qe_a_block, (uint32_t*)qe_a_block, (uint32_t*)keyA_tx);
	skipjack_enc(qe_b_block, keyB_tx);
	d ^= qe_a_block[0];
	d = pgm_read_byte_near(&(qsbox[d]));
	d += qe_b_block[0];
	return d;
}

uint8_t q_decipher(uint8_t d){
	xtea_enc((uint32_t*)qd_a_block, (uint32_t*)qd_a_block, (uint32_t*)keyA_rx);
	skipjack_enc(qd_b_block, keyB_rx);
	d -= qd_b_block[0];
	d = pgm_read_byte_near(&(qsbox_inv[d]));
	d ^= qd_a_block[0];
	return d;
}

/*****************************************************************************/
/* keys are multiple encrypted.
 * encrytion alogos in order of encrytion:
 *  o camellia
 *  o skipjack
 *  o rc6
 * (decrytion is done in reverse order of course)
 * we use CBC-mode without extra IV since first block has 
 */
#include "camellia.h"
#include "skipjack.h"
#include "rc6.h"


/*#define KEY ((void*)0)*/
#define RC6_INIT_ERROR	((void*)0), 1, 0xC6

void q_decipher_keyblock_camellia(uint8_t* block);
void q_decipher_keyblock_skipjack(uint8_t* block);
void q_decipher_keyblock_rc6(uint8_t* block);
void q_encipher_keyblock_camellia(uint8_t* block);
void q_encipher_keyblock_skipjack(uint8_t* block);
void q_encipher_keyblock_rc6(uint8_t* block);

 #include <avr/eeprom.h>

void q_decipher_keyblock(void* block, uint8_t n){
	switch (n){
		case 0:	q_decipher_keyblock_rc6(block); break;
		case 1:	q_decipher_keyblock_skipjack(block); break;
		case 2:	q_decipher_keyblock_camellia(block); break;
		default: error(STUPID_BUT_EVIL_PROGRAMMER_ERROR);
	}
}


#include <util/delay.h> /* delays for debugging */
#include "config.h"
#include "debug.h"

#define RC6_BLOCKSIZE		16 /* 128 bit */
#define SKIPJACK_BLOCKSIZE	 8 /*  64 bit */
#define CAMELLIA_BLOCKSIZE	16 /* 128 bit */

void q_decipher_keyblock_rc6(uint8_t* block){
	uint8_t i,j;
	rc6_ctx_t s;
	{	
		uint8_t key[32];
		eeprom_read_block(key, q_ltk_3_eeprom, 32);
		if(rc6_init(&s,key,256)){
			error(RC6_INIT_ERROR);
		}
	}
	
	i = SIZEOF_KEYPACKET - RC6_BLOCKSIZE;
	for(;i>=RC6_BLOCKSIZE; i-=RC6_BLOCKSIZE){
		rc6_dec(&s,&(block[i]));
		for(j=0; j<RC6_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-RC6_BLOCKSIZE]; /* xor block with preblock-cipher */
		}
	}	
	rc6_dec(&s,&(block[i])); /* decrypt first block */		
	/* xor with IV skiped 'cause IV=0 */
	rc6_free(&s);
}

void q_decipher_keyblock_skipjack(uint8_t* block){
	uint8_t i,j;
	uint8_t key[10];
	eeprom_read_block(key, q_ltk_2_eeprom, 10);
	i = SIZEOF_KEYPACKET - SKIPJACK_BLOCKSIZE;
	for(;i>=SKIPJACK_BLOCKSIZE; i-=SKIPJACK_BLOCKSIZE){
		skipjack_dec(&(block[i]), key);
		for(j=0; j<SKIPJACK_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-SKIPJACK_BLOCKSIZE]; /* xor block with preblock-cipher */
		}
	}	
	skipjack_dec(&(block[i]), key); /* decrypt first block */		
	/* xor with IV skiped 'cause IV=0 */
}

void q_decipher_keyblock_camellia(uint8_t* block){
	uint8_t i,j;
	camellia128_ctx_t s;
	{
		uint8_t key[16];
		eeprom_read_block(key, q_ltk_1_eeprom, 16);
		camellia128_init(&s, key);
	}
	i = SIZEOF_KEYPACKET - CAMELLIA_BLOCKSIZE;
	for(;i>=CAMELLIA_BLOCKSIZE; i-=CAMELLIA_BLOCKSIZE){
		camellia128_dec(&s,&(block[i]));
		for(j=0; j<CAMELLIA_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-CAMELLIA_BLOCKSIZE]; /* xor block with preblock-cipher */
		}
	}	
	camellia128_dec(&s,&(block[i])); /* decrypt first block */		
	/* xor with IV skiped 'cause IV=0 */
}

/*****************************************************************************/

void q_encipher_keyblock(void* block, uint8_t n){
	switch (n){
		case 0:	q_encipher_keyblock_camellia(block); break;
		case 1:	q_encipher_keyblock_skipjack(block); break;
		case 2:	q_encipher_keyblock_rc6(block); break;
		default: error(STUPID_BUT_EVIL_PROGRAMMER_ERROR);
	}
}

void q_encipher_keyblock_camellia(uint8_t* block){
	uint8_t i,j;
	camellia128_ctx_t s;
	{
		uint8_t key[16];
		eeprom_read_block(key, q_ltk_1_eeprom, 16);
		camellia128_init(&s, key);
	}
	/* ignore xor with IV (IV=0) */
	camellia128_enc(&s, block);
	for(i=CAMELLIA_BLOCKSIZE; i<SIZEOF_KEYPACKET; i+=CAMELLIA_BLOCKSIZE){
		/* xor with previous block */
		for(j=0; j<CAMELLIA_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-CAMELLIA_BLOCKSIZE];
		}
		camellia128_enc(&s,&(block[i]));
	}
}

void q_encipher_keyblock_skipjack(uint8_t* block){
	uint8_t i,j;
	uint8_t key[10];
	eeprom_read_block(key, q_ltk_2_eeprom, 10);
	/* ignore xor with IV (IV=0) */
	skipjack_enc(block, key);
	for(i=SKIPJACK_BLOCKSIZE; i<SIZEOF_KEYPACKET; i+=SKIPJACK_BLOCKSIZE){
		/* xor with previous block */
		for(j=0; j<SKIPJACK_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-SKIPJACK_BLOCKSIZE];
		}
		skipjack_enc(&(block[i]), key);
	}
}

void q_encipher_keyblock_rc6(uint8_t* block){
	uint8_t i,j;
	rc6_ctx_t s;
	{	
		uint8_t key[32];
		eeprom_read_block(key, q_ltk_3_eeprom, 32);
		if(rc6_init(&s,key,256))
			error(RC6_INIT_ERROR);
	}
	/* ignore xor with IV (IV=0) */
	rc6_enc(&s, block);
	for(i=RC6_BLOCKSIZE; i<SIZEOF_KEYPACKET; i+=RC6_BLOCKSIZE){
		/* xor with previous block */
		for(j=0; j<RC6_BLOCKSIZE; ++j){
			block[i+j] ^= block[i+j-RC6_BLOCKSIZE];
		}
		rc6_enc(&s,&(block[i]));
	}
	rc6_free(&s);
}

/*****************************************************************************/
/* Message Verification
 * 
 * 
 * 
 */


#ifdef QP_MSG_CHECK_HMAC
verify_t q_verify_std(void* block, uint16_t size, void* reference){
	uint8_t hmac[HMAC_SIZE];
	/* hash the stuff */
	hmac_sha256(hmac, keyS_rx, KEYS_SIZE, block, size);
	/* memcmp */
	return memcmp(hmac, reference, HMAC_SIZE)?failed:ok;
}
#else
#ifdef QP_MSG_CHECK_HASH
verify_t q_verify_std(void* block, uint16_t size, void* reference){
	uint8_t hash[HASH_SIZE];
	/* hash the stuff */
	sha256((sha256_hash_t*)hash, (void*)block, (uint32_t)size);
	/* memcmp */
	return memcmp(hash, reference, HASH_SIZE)?failed:ok;
}
#endif
#endif

/* block ends wiht HMAC */
verify_t q_verify_ltk(void* block){
	uint8_t hmac[HMAC_SIZE];
	uint8_t key[KEYS_SIZE];
	eeprom_read_block(key, q_ltk_S_eeprom, KEYS_SIZE);
	hmac_sha256(hmac, key, KEYS_SIZE*8, block, KP_SIGN_OFFSET*8);
	block += KP_SIGN_OFFSET;	
	return (memcmp(hmac, block, HMAC_SIZE)?failed:ok);	
}
/*
#if QP_MSG_CHECK == HMAC
	#define q_sign_std(dest, block, size) hmac_sha256((dest), keyS_tx, KEYS_SIZE, (block), (size))
#else
#if QP_MSG_CHECK == HASH
	#define q_sign_std(dest, block, size) sha256((dest), (block), (size))
#endif
#endif
*/

void q_sign_ltk(void* block){
	uint8_t hmac[HMAC_SIZE];
	uint8_t key[KEYS_SIZE];

	eeprom_read_block(key, q_ltk_S_eeprom, KEYS_SIZE);
	hmac_sha256(hmac, key, KEYS_SIZE*8, block, KP_SIGN_OFFSET*8);
	memcpy((uint8_t*)block+KP_SIGN_OFFSET, hmac, HMAC_SIZE);	
}










