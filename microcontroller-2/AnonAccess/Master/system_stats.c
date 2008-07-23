

#include "config.h"
#include "sha256.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#ifndef FLASHSIZE
 #ifdef ATMEGA644
  #define FLASHSIZE (64L*1024)
 #endif
 #ifdef ATMEGA64
   #define FLASHSIZE (64L*1024)
 #endif
 #ifdef ATMEGA32
   #define FLASHSIZE (32*1024)
 #endif
#endif

#ifndef FLASHSIZE
 #error "FLASHSIZE not specifiyed"
#endif

void system_hash(sha256_hash_t syshash){
	sha256_ctx_t ctx;
	uint8_t buffer[SHA256_BLOCK_BYTES];
	uint16_t i;
	uint8_t j, endsize;
	sha256_init(&ctx);
	for(i=0; i<(FLASHSIZE/SHA256_BLOCK_BYTES)-1; ++i){
		for(j=0; j<SHA256_BLOCK_BYTES; ++j){
			buffer[j] = pgm_read_byte(i*SHA256_BLOCK_BYTES+j);
		}
		sha256_nextBlock(&ctx, buffer);
	}
	endsize=(FLASHSIZE%SHA256_BLOCK_BYTES);
	if(endsize==0)
		endsize=SHA256_BLOCK_BYTES;
	for(j=0; j<endsize; ++j){
		buffer[j] = pgm_read_byte(i*SHA256_BLOCK_BYTES+j);
	}
	sha256_lastBlock(&ctx, buffer, endsize*8);
	sha256_ctx2hash((sha256_hash_t*)syshash, &ctx);
}


