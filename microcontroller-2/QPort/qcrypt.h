#ifndef QCRYPT_H_
#define QCRYPT_H_

#include <stdint.h>
#include "config.h"
#include "sha256.h"
#include "hmac-sha256.h"

#define HASH_SIZE (SHA256_HASH_BITS/8)

#define HMAC_SIZE HMAC_SHA256_SIZE
#define SIG_SIZE HMAC_SIZE
#define SIGNATURE_SIZE SIG_SIZE /* sed might be the better solution */

#define KEYA_SIZE 16 /* 256 bits */
#define KEYB_SIZE 16 /* 256 bits */
#define KEYS_SIZE 32 /* 256 bits */

#ifdef QP_MSG_CHECK_HMAC
	#define SIZEOF_KEYPACKET (12+4+(KEYA_SIZE+KEYB_SIZE+KEYS_SIZE)*2+SIG_SIZE)
#else
	#define SIZEOF_KEYPACKET (12+4+(KEYA_SIZE+KEYB_SIZE)*2+SIG_SIZE)
#endif

extern uint8_t		keyA_rx[KEYA_SIZE];
extern uint8_t		keyA_tx[KEYA_SIZE];
extern uint8_t		keyB_rx[KEYB_SIZE];
extern uint8_t		keyB_tx[KEYB_SIZE];
#ifdef QP_MSG_CHECK_HMAC
	extern uint8_t		keyS_rx[KEYS_SIZE];
	extern uint8_t		keyS_tx[KEYS_SIZE];
#endif

typedef enum{failed=0, ok=1} verify_t;

void memxor(void* dest, void* src, uint16_t dlen, uint16_t slen);

void q_stream_crypt_init(void);
uint8_t q_encipher(uint8_t d);
uint8_t q_decipher(uint8_t d);
void q_decipher_keyblock(void* block, uint8_t n);

void q_decipher_keyblock_rc6(uint8_t* block);
void q_decipher_keyblock_skipjack(uint8_t* block);
void q_decipher_keyblock_camellia(uint8_t* block);

void q_encipher_keyblock(void* block, uint8_t n);

void q_encipher_keyblock_camellia(uint8_t* block);
void q_encipher_keyblock_skipjack(uint8_t* block);
void q_encipher_keyblock_rc6(uint8_t* block);

verify_t q_verify_std(void* block, uint16_t size, void* reference);
verify_t q_verify_ltk(void* block);

/*#define MSG_CHECK=HASH
*/

#ifdef QP_MSG_CHECK_HMAC
//	#warning MAC on
	#define q_sign_std(dest, block, size) hmac_sha256((dest), keyS_tx, KEYS_SIZE, (block), (uint64_t)(size))
#else /* MSG_CHECK == HASH */
//	#warning HASH on
	#define q_sign_std(dest, block, size) sha256((sha256_hash_t*)(dest), (void*)(block), (uint32_t)(size))
#endif

void q_sign_ltk(void* block);



#endif /*QCRYPT_H_*/
