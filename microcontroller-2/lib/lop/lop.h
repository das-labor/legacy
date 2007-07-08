/**
 * 
 * 
 * 
 */
 
#ifndef LOP_H_
#define LOP_H_

#include <stdint.h>
#include <stddef.h> //#define NULL (void*)0

typedef enum {idle,message,stream} lopstates_t;

typedef struct lop_ctx {
	lopstates_t rxstate, txstate, msgretstate;
	uint32_t msgidx;
	uint16_t msglength;
	uint8_t* msgbuffer;
	uint8_t escaped;
	void (*sendrawbyte)(uint8_t);			/* pointer to the writing function */
	void (*on_msgrx)(uint16_t, uint8_t*);	/* function called on message recieve */
	void (*on_streamrx)(uint8_t);			/* function called on recieve of a stream byte*/
	void (*on_streamstart)();				/* function called on stream initialisation */
	void (*on_streamstop)();				/* function called on stream termination */
} lop_ctx_t;

/******************************************************************************/
void lop_reset(lop_ctx_t* ctx);
void lop_sendreset(lop_ctx_t * ctx);
/******************************************************************************/
void lop_recieve_byte(lop_ctx_t* ctx, uint8_t b);
/******************************************************************************/
void lop_sendmessage(lop_ctx_t * ctx,uint16_t length, uint8_t * msg);
/******************************************************************************/
void lop_streamstart(lop_ctx_t * ctx);
void lop_streamstop(lop_ctx_t * ctx);
void lop_sendstream(lop_ctx_t * ctx, uint8_t b);

#endif /*LOP_H_*/
