/**
 * 
 * 
 * 
 */
 
#ifndef LOP_H_
#define LOP_H_

#include "config.h"
#include "uart.h"
#include <stdint.h>
#include <stddef.h> //#define NULL (void*)0

#define LOP_ESC_CODE		0x05
#define LOP_RESET_CODE		0x06
#define LOP_XON_CODE		0x11
#define LOP_XOFF_CODE		0x13

#define LOP_RESET_ESC		0x01
#define LOP_ESC_ESC			0x02
#define LOP_XON_ESC			0x03
#define LOP_XOFF_ESC		0x04

#define LOP_TYPE_MSG			0x14
#define LOP_TYPE_STREAMSYNC		0x15

typedef enum {idle,message} lopstates_t;

typedef struct lop_ctx {
	lopstates_t rxstate, txstate, msgretstate;
	uint32_t msgidx;
	uint16_t msglength;
	uint8_t* msgbuffer;
	uint8_t escaped;
	void (*sendrawbyte)(uint8_t);           /* pointer to the writing function */
	void (*on_msgrx)(uint16_t, uint8_t*);   /* function called on message recieve */
	void (*on_streamrx)(uint8_t);           /* function called on recieve of a stream byte*/
	void (*on_streamsync)(void);            /* function called on recieve of streamsync */
} lop_ctx_t;

/******************************************************************************/
void lop_reset(lop_ctx_t* ctx);
void lop_sendreset(lop_ctx_t * ctx);
/******************************************************************************/
void lop_recieve_byte(lop_ctx_t* ctx, uint8_t b);
/******************************************************************************/
void lop_sendmessage(lop_ctx_t * ctx,uint16_t length, uint8_t * msg);
/******************************************************************************/
void lop_streamsync(lop_ctx_t * ctx);
void lop_sendstream(lop_ctx_t * ctx, uint8_t b);

#endif /*LOP_H_*/
