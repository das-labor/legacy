#ifndef QPORT_H_
#define QPORT_H_

#include <stdint.h>
#include "lop.h"
#include "sha256.h"
#include "hmac-sha256.h"

/******************************************************************************/

typedef struct{
	uint8_t marker;
} qport_keyreq_t;

/******************************************************************************/

typedef struct {
	uint8_t marker;
//	uint8_t marker2;
	uint64_t id;
	uint8_t seed_a[32];
	uint8_t seed_b[32];
} qport_keypacket_t;

/******************************************************************************/

typedef struct qport_ctx{
	uint8_t *master_enc_key_rxa;
	uint8_t *master_enc_key_rxb;
	uint8_t *master_enc_key_txa;
	uint8_t *master_enc_key_txb;
	uint8_t	*master_mac_key;
	uint8_t streamkey_txa[16];
	uint8_t streamkey_txb[16];
	uint8_t streamstate_txa[8];
	uint8_t streamstate_txb[8];
	uint8_t streamkey_rxa[16];
	uint8_t streamkey_rxb[16];
	uint8_t streamstate_rxa[8];
	uint8_t streamstate_rxb[8];
	lop_ctx_t * lop;
	uint8_t streamrx_index;
	uint8_t streamrx_value;
	void (*on_byterx)(uint8_t);
	volatile enum {unkeyed, keyed, makeingkey} keystate;
	qport_keypacket_t * keyingdata;
} qport_ctx_t;

/******************************************************************************/
uint8_t xtea_cfb_enc(uint8_t p, uint8_t * key, uint8_t * state);
uint8_t xtea_cfb_dec(uint8_t c, uint8_t * key, uint8_t * state);
uint8_t xtea_ofb(uint8_t p, uint8_t * key, uint8_t * state);
void qport_streamsend(qport_ctx_t * ctx, uint8_t p);
void qport_recieve_byte(qport_ctx_t * ctx, uint8_t b);
void qport_setupstream(qport_ctx_t * ctx, uint8_t rxtx, uint8_t * a, uint8_t * b);
void qport_onkp(qport_ctx_t * ctx, qport_keypacket_t *kp);
void qport_rekey(qport_ctx_t * ctx);
void qport_incomming_msg(qport_ctx_t * ctx, uint16_t length, uint8_t * buffer);
/******************************************************************************/









#endif /*QPORT_H_*/
