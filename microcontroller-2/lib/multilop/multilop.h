#ifndef MULTILOP_H_
#define MULTILOP_H_

#include <stdint.h>
#include "lop.h"

#define MLOP_ADDR_SIZE   16
#define MLOP_ADDR_SIZEB ((MLOP_ADDR_SIZE+7)/8)
#define MLOP_HWADDR_SIZE   128
#define MLOP_HWADDR_SIZEB ((MLOP_HWADDR_SIZE+7)/8)

typedef uint8_t mlop_addr_t[MLOP_ADDR_SIZEB];
typedef uint8_t mlop_hwaddr_t[MLOP_HWADDR_SIZEB];

#define MLOP_PING 0x01
#define MLOP_PONG 0x02
#define MLOP_MSG  0x03
#define MLOP_PING_TIMEOUT 1000


typedef struct mlop_ctx {
	mlop_addr_t   addr, tempaddr;
	mlop_hwaddr_t hwaddr;
	mlop_addr_t   lastdest, lastsrc;
	uint8_t streamrxact;
	uint8_t automatic;
	uint8_t addrnack;
	uint8_t headbuffer[MLOP_ADDR_SIZEB*2];
	uint8_t headbufidx;
	lop_ctx_t* lop;
	void (*on_msgrx)(mlop_addr_t src, uint16_t, uint8_t*);   /* function called on message recieve */
	void (*on_brcmsgrx)(uint16_t, uint8_t*);   /* function called on message recieve */
	void (*on_streamrx)(uint8_t);           /* function called on recieve of a stream byte*/
	void (*on_streamsync)(mlop_addr_t);            /* function called on recieve of streamsync */
} mlop_ctx_t;

/******************************************************************************/
void mlop_init(mlop_ctx_t* ctx);
void mlop_reqaddr(mlop_ctx_t* ctx);
/******************************************************************************/
void mlop_reset(mlop_ctx_t* ctx);
void mlop_sendreset(mlop_ctx_t * ctx);
/******************************************************************************/
void mlop_sendmessage(mlop_ctx_t* ctx, const mlop_addr_t dest, uint16_t length, const uint8_t * msg);

/******************************************************************************/
void mlop_streamsync(mlop_ctx_t * ctx, mlop_addr_t dest);
void mlop_sendstream(mlop_ctx_t* ctx, mlop_addr_t dest, uint8_t b);


#endif /*MULTILOP_H_*/
