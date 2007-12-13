/**
 * labor octet protocol
 * \file lop.c
 * \author Daniel Otte
 * \par License
 *  GPLv3
 * 
 * 
 */
 
 
#include <stdlib.h>
#include <stdint.h>
#include "lop.h"

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
/*
#define LOP_TYPE_STREAM_START	0x15
#define LOP_TYPE_STREAM_STOP	0x16
*/
static void lop_process_l1(lop_ctx_t* ctx, uint8_t b);
static void lop_process_l2(lop_ctx_t* ctx, uint8_t b);

/******************************************************************************/

static
void lop_error(uint8_t b){
	switch(b){
		case 1: /* message in message */
			break;
		case 2: /* stream start while not idle */
			break;
		case 3: /* stream start while not stream */
			break;
		case 4: /* invalid esc-sequence */
			break;
		case 5: /* message to large */
			break;			
		case 6: /* invalid recive state */
			break;
		case 7: /* stream send while not stream */
			break;
		default:
			break;
	}
	for(;;)
		;
}

/******************************************************************************/

void lop_reset(lop_ctx_t* ctx){
	ctx->rxstate = idle;
	if(ctx->msgbuffer){
		free(ctx->msgbuffer);
		ctx->msgbuffer = NULL;
	}
}

/******************************************************************************/

void lop_recieve_byte(lop_ctx_t* ctx, uint8_t b){
	lop_process_l1(ctx, b);
}

/******************************************************************************/
static
void lop_process_l1(lop_ctx_t* ctx, uint8_t b){
	if (b==LOP_RESET_CODE){
		lop_reset(ctx);
		return;
	} 
	if (b==LOP_ESC_CODE){
		ctx->escaped = 1;
		return;
	}
	if(!ctx->escaped){
		lop_process_l2(ctx, b);
		return;
	} else {
		ctx->escaped = 0;
		if(b<=0x04 && b!=0){ /* escaped data byte */
			uint8_t t[4]={LOP_RESET_CODE, LOP_ESC_CODE, LOP_XON_CODE, LOP_XOFF_CODE};
			lop_process_l2(ctx, t[b-1]);
			return;
		}
		switch(b){
			case LOP_TYPE_MSG:
				if(ctx->rxstate==message){
					/* invalid transition error */
					lop_error(1);
				}
				ctx->msgretstate = ctx->rxstate;
				ctx->rxstate=message;
				if(ctx->msgbuffer){
					free(ctx->msgbuffer);
					ctx->msgbuffer = NULL;
				}
				ctx->msgidx = 0;
				ctx->msglength = 0;
				break;
			case LOP_TYPE_STREAMSYNC:
				if(ctx->rxstate==message){
					/* invalid transition error */
					lop_error(2);
				}
				if(ctx->on_streamsync)
					ctx->on_streamsync();
				break;
			default:
				/* invalid escape-sequence */
				lop_error(4);
				break;
		}
	}
}

/******************************************************************************/
static
void lop_process_l2(lop_ctx_t* ctx, uint8_t b){
	switch(ctx->rxstate){
		case idle:
			/* stream data */
			if(ctx->on_streamrx)
				ctx->on_streamrx(b);
			break;
		case message:
			switch(ctx->msgidx){
				case 0:
					ctx->msglength = (uint16_t)b<<8;
					break;
				case 1:
					ctx->msglength += b;
					if(!(ctx->msgbuffer=malloc(ctx->msglength))){
						/* message to large error */
						lop_error(5);
					}
					break;
				default:
					ctx->msgbuffer[ctx->msgidx-2] = b;
					break;
			}
			if(ctx->msgidx==(uint32_t)ctx->msglength+1){ /* end of message */
				if(ctx->on_msgrx)
					ctx->on_msgrx(ctx->msglength, ctx->msgbuffer);
				free(ctx->msgbuffer);
				ctx->msgbuffer = NULL;
				ctx->rxstate = ctx->msgretstate;
			}
			ctx->msgidx++;
			break;
		default:
			/* invalid recive, should never happen */
			lop_error(6);
			break;
	}
}

/******************************************************************************/
static
void lop_sendbyte(lop_ctx_t * ctx,uint8_t b){
		
	if(!(ctx->sendrawbyte)){
		return;
	}
	
	switch(b){
		case LOP_ESC_CODE:
			ctx->sendrawbyte(LOP_ESC_CODE);
			ctx->sendrawbyte(LOP_ESC_ESC);
			break;
		case LOP_RESET_CODE:
			ctx->sendrawbyte(LOP_ESC_CODE);
			ctx->sendrawbyte(LOP_RESET_ESC);
			break;
		case LOP_XON_CODE:
			ctx->sendrawbyte(LOP_ESC_CODE);
			ctx->sendrawbyte(LOP_XON_ESC);
			break;
		case LOP_XOFF_CODE:
			ctx->sendrawbyte(LOP_ESC_CODE);
			ctx->sendrawbyte(LOP_XOFF_ESC);
			break;
		default:
			ctx->sendrawbyte(b);
			break;
	}
}

/******************************************************************************/

void lop_sendmessage(lop_ctx_t * ctx,uint16_t length, uint8_t * msg){
	if(!ctx->sendrawbyte)
		return;
	while(ctx->txstate==message)
		;
	ctx->txstate=message;
	ctx->sendrawbyte(LOP_ESC_CODE);
	ctx->sendrawbyte(LOP_TYPE_MSG);
	lop_sendbyte(ctx, length>>8);
	lop_sendbyte(ctx, length&0x00FF);
	while(length--)
		lop_sendbyte(ctx, *msg++);
	ctx->txstate=idle;
}

/******************************************************************************/

void lop_streamsync(lop_ctx_t * ctx){
	if(!ctx->sendrawbyte)
		return;
	while(ctx->txstate==message)
		;
	ctx->txstate=idle;
	ctx->sendrawbyte(LOP_ESC_CODE);
	ctx->sendrawbyte(LOP_TYPE_STREAMSYNC);
}

/******************************************************************************/

void lop_sendstream(lop_ctx_t * ctx, uint8_t b){
	if(!(ctx->sendrawbyte))
		return;
	if(ctx->txstate!=idle){
		/* invalid send error */
		lop_error(7);	
		return;
	}
	lop_sendbyte(ctx, b);
}

/******************************************************************************/

void lop_sendreset(lop_ctx_t * ctx){
	if(ctx->sendrawbyte)
		ctx->sendrawbyte(LOP_RESET_CODE);
}
