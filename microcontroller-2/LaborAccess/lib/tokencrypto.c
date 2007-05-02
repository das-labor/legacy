

#include <stdint.h>
#include <string.h>
#include <avr/eeprom.h>
#include "prng.h"
#include "hmac-sha256.h"
#include "tokencrypto.h"

uint8_t dbkey[DBKEY_SIZE] EEMEM = {DB_KEY};

void getDBkey(uint8_t *keybuffer){
	eeprom_read_block(keybuffer, dbkey, DBKEY_SIZE); 
}

uint8_t validtoken(uint8_t* dbtoken, uint8_t* cardtoken){
	uint8_t key[DBKEY_SIZE];
	uint8_t hmac[HMAC_SHA256_SIZE];
	getDBkey(key);
	hmac_sha256(hmac,key,256,DBKEY_SIZE*8,TOKEN_SIZE*8);
	if (memcmp(hmac,cardtoken,HMAC_SHA256_SIZE){
		return 0;
	} else {
		return 1;
	}
}
	
void nexttoken (uint8_t* dbtoken, uint8_t* cardtoken){
	uint8_t key[DBKEY_SIZE];
	getDBkey(key);
	fillBlockRandom(dbtoken,TOKEN_SIZE);
	hmac_sha256(cardtoken,key,256,DBKEY_SIZE*8,TOKEN_SIZE*8);
}
