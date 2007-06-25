/**
 * labor octet protocol
 * 
 * 
 * 
 * 
 * 
 */
 
 
#include <stdlib.h>
#include <stdint.h>
#include "lop.h"

#include "uart.h"

#define LOP_ESC_CODE		0x23
#define LOP_RESET_CODE		0x42
#define LOP_XON_CODE		0x11
#define LOP_XOFF_CODE		0x13

#define LOP_RESET_ESC		0x01
#define LOP_ESC_ESC			0x02
#define LOP_XON_ESC			0x03
#define LOP_XOFF_ESC		0x04

#define LOP_TYPE_MSG			0x14
#define LOP_TYPE_STREAM_START	0x15
#define LOP_TYPE_STREAM_STOP	0x16

static void lop_process_l1(lop_ctx_t* ctx, uint8_t b);
static void lop_process_l2(lop_ctx_t* ctx, uint8_t b);

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
				if(!ctx->rxstate==idle){
					/* invalid transition error */
				}
				ctx->rxstate=message;
				if(ctx->msgbuffer){
					free(ctx->msgbuffer);
					ctx->msgbuffer = NULL;
				}
				ctx->msgidx = 0;
				ctx->msglength = 0;
				break;
			case LOP_TYPE_STREAM_START:
				if(!ctx->rxstate==idle){
					/* invalid transition error */
				}
				ctx->rxstate=stream;
				if(ctx->on_streamstart)
					ctx->on_streamstart();
				break;
			case LOP_TYPE_STREAM_STOP:
				if(!ctx->rxstate==stream){
					/* invalid transition error */
				}
				ctx->rxstate=idle;
				if(ctx->on_streamstop)
					ctx->on_streamstop();
				break;
			default:
				/* invalid escape-sequence */
				break;
		}
	}
}

/******************************************************************************/
static
void lop_process_l2(lop_ctx_t* ctx, uint8_t b){
	switch(ctx->rxstate){
		case stream:
			if(ctx->on_streamrx)
				ctx->on_streamrx(b);
			break;
		case idle:
			/* drop, may happen on asynchonous access */
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
			}
			ctx->msgidx++;
			break;
		default:
			/* invalid recive, should never happen */
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
	while(ctx->txstate!=idle)
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

void lop_streamstart(lop_ctx_t * ctx){
	if(!ctx->sendrawbyte)
		return;
	while(ctx->txstate!=idle)
		;
	ctx->txstate=stream;
	ctx->sendrawbyte(LOP_ESC_CODE);
	ctx->sendrawbyte(LOP_TYPE_STREAM_START);
}

/******************************************************************************/

void lop_streamstop(lop_ctx_t * ctx){
	if(!ctx->sendrawbyte)
		return;
	if(ctx->txstate!=stream)
		return;
	ctx->sendrawbyte(LOP_ESC_CODE);
	ctx->sendrawbyte(LOP_TYPE_STREAM_STOP);
	ctx->txstate=idle;
}

/******************************************************************************/

void lop_sendstream(lop_ctx_t * ctx, uint8_t b){
	if(!(ctx->sendrawbyte))
		return;
	if(ctx->txstate!=stream){
		/* invalid send error */	
		return;
	}
	lop_sendbyte(ctx, b);
}

/******************************************************************************/
void lop_sendreset(lop_ctx_t * ctx){
	if(ctx->sendrawbyte)
		ctx->sendrawbyte(LOP_RESET_CODE);
}

