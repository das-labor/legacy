/**
 * File:		rng.c
 * Author:	Daniel Otte
 * Date:		17.05.2006
 * License:	GPL
 * Description:	This file contains an implementaition of a pseudo-random-number generator.
 * 
 **/

/*
 * 
 *                      ############################################################################
 *                      #																	       #
 * 					    #		  +-------------------+										 	   #
 * 					    #	  	  |				      |											   #
 * 					    #		  V				      |				  							   #
 *                      #      (concat)	    		      |											   #
 *  +---------------+   #    o---------o          +---------+      o---------o       o---------o   #    +--------------+
 *  | entropy Block | -----> | sha-256 | --->(xor)| rndCore | ---> | sha-256 | --+-> | sha-256 | -----> | random Block |
 *  +---------------+   #    o---------o          +---------+      o---------o   |   o---------o   #    +--------------+
 * 						#					         (xor)						 |				   #
 * 						#						       ^						     |				   #
 * 						#						       |						     |				   #
 * 						#						       +-------------------------+				   #
 * 						#																	       #
 *						############################################################################
 * 
 */

#include <stdint.h>
#include <string.h>
#include "sha256.c"




uint32_t rndCore[8]; /* secret */

/*
 * idea is: hash the message and add it via xor to rndCore
 *
 * length in bits 
 * 
 * we do not use real concatenation. we just use rndCore and pad it with 0x42 to 
 * the blocksize of the hash-function and hash the block, then entropy block gets
 * hashed.
 */
void addEntropy(unsigned length, void* data){
	sha256_ctx_t s;
	uint8_t block[SHA256_BLOCK_BITS/8];
	
	memset(block, 'B', SHA256_BLOCK_BITS/8);
	sha256_starts(&s);
	memcpy(block, rndCore, SHA256_HASH_BITS/8);
	sha256_nextBlock(&s, block);
#if defined SECURE_WIPE_BUFFER	
	memset(block, 'B', SHA256_BLOCK_BITS/8); /* just to leave no secret shit on stack */
#endif
	while (length>=512){
		sha256_nextBlock(&s, data);
		data += 512/8;
		length -= 512;	
	}
	sha256_lastBlock(&s, data, length);
	uint8_t i;
	for (i=0; i<8; ++i){
		rndCore[i] ^= s.h[i]
	}
}
 
void getRandomBlock(uint32_t *b){
	sha256_ctx_t s;
	sha256_start(&s);
	sha256_lastBlock(&s, rndCore, 256); /* remeber the byte order! */
	uint8_t i;
	for (i=0; i<8; ++i){
		rndCore[i] ^= s.h[i]
	}
	memcpy(b, s.h, 32); /* back up first hash in b */
	sha256_start(&s);
	sha256_lastBlock(&s, b, 256);
	memcpy(b, s.h, 32);
}
 
uint8_t getRandomByte(){
	static uint8_t block[32];
	static uint8_t i=32;
	
	if (i==32){
		getRandomBlock(block);
		i=0;
	}	
	return block[i++];
}
 
 
 
 