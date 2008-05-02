/**
 * multi labor octet protocol
 * \file lop.c
 * \author Daniel Otte
 * \par License
 *  GPLv3
 * 
 * 
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "multilop.h"
#include "lop.h"

static
uint8_t isbroadcast(mlop_addr_t addr){
	uint8_t i;
	for(i=0; i<MLOP_ADDR_SIZEB; ++i)
		if(addr[i]!=0xFF)
			return 0;
	return 1;
}

static
uint8_t issync(mlop_addr_t addr){
	uint8_t i;
	for(i=0; i<MLOP_ADDR_SIZEB; ++i)
		if(addr[i]!=0x00)
			return 0;
	return 1;
}

static
uint8_t isself(mlop_ctx_t* ctx, mlop_addr_t addr){
	return !(memcmp(ctx->addr, addr, MLOP_ADDR_SIZEB));
}


uint8_t mlop_autobrc(mlop_ctx_t* ctx, uint16_t length, const uint8_t* msg);

/******************************************************************************/
void mlop_reset(mlop_ctx_t* ctx);
void mlop_sendreset(mlop_ctx_t * ctx);
/******************************************************************************/

/******************************************************************************/

void mlop_sendmessage_raw(mlop_ctx_t* ctx, const mlop_addr_t dest, uint16_t length, uint8_t type, const uint8_t * msg){
	uint8_t buffer[length+2*MLOP_ADDR_SIZEB+1];
	memcpy(buffer, dest, MLOP_ADDR_SIZEB);
	memcpy(buffer+MLOP_ADDR_SIZEB, ctx->addr, MLOP_ADDR_SIZEB);
	buffer[2*MLOP_ADDR_SIZEB+1] = type;
	if(msg)
		memcpy(buffer+2*MLOP_ADDR_SIZEB+1, msg, length);
	lop_sendmessage(ctx->lop, length+2*MLOP_ADDR_SIZE+1, buffer);
}

/******************************************************************************/

void mlop_recieve_msg(mlop_ctx_t* ctx, uint16_t length, uint8_t *msg){
	if(length<2*MLOP_ADDR_SIZEB+1)
		return; /* DROP */
	if(isbroadcast(msg)){
		if(ctx->automatic)
			if(!mlop_autobrc(ctx, length, msg))
				return;
		ctx->on_brcmsgrx(length-MLOP_ADDR_SIZEB, msg-MLOP_ADDR_SIZEB);
		return;
	}
	if(memcmp(msg, ctx->addr, MLOP_ADDR_SIZEB)==0 && msg[2*MLOP_ADDR_SIZEB+1]==MLOP_MSG){
		ctx->on_msgrx(msg+MLOP_ADDR_SIZEB, length-2*MLOP_ADDR_SIZEB-1, msg+2*MLOP_ADDR_SIZEB+1);
		return;
	}
	if(memcmp(msg, ctx->addr, MLOP_ADDR_SIZEB)==0 && msg[2*MLOP_ADDR_SIZEB+1]==MLOP_PING){
		ctx->on_msgrx(msg+MLOP_ADDR_SIZEB, length-2*MLOP_ADDR_SIZEB-1, msg+2*MLOP_ADDR_SIZEB+1);
		mlop_sendmessage_raw(ctx, msg+MLOP_ADDR_SIZEB,0, MLOP_PONG, NULL);
		return;
	}
}

/******************************************************************************/

void mlop_recieve_stream(mlop_ctx_t* ctx, uint8_t b){
	if(ctx->headbufidx==2*MLOP_ADDR_SIZEB){
		if(ctx->streamrxact){
			ctx->on_streamrx(b);
		}
	}else{
		ctx->headbuffer[ctx->headbufidx]=b;
		ctx->headbufidx++;
		if(ctx->headbufidx==MLOP_ADDR_SIZEB){
			if(issync(ctx->headbuffer)){
				ctx->headbufidx=2*MLOP_ADDR_SIZEB;
				ctx->on_streamsync(ctx->headbuffer);
			}
		}
		if(ctx->headbufidx==2*MLOP_ADDR_SIZEB){
			if(isself(ctx, ctx->headbuffer)){
				ctx->streamrxact=1;
				ctx->on_streamsync(ctx->headbuffer+MLOP_ADDR_SIZEB);
			}else{
				ctx->streamrxact=0;
			}
		}
	}
}

/******************************************************************************/

void mlop_recieve_streamsync(mlop_ctx_t* ctx){
	ctx->headbufidx=0;
}

/******************************************************************************/

void mlop_sendmessage(mlop_ctx_t* ctx, const mlop_addr_t dest, uint16_t length, const uint8_t * msg){
	mlop_sendmessage_raw(ctx, dest, length, MLOP_MSG, msg);
}

/******************************************************************************/

void mlop_streamsync(mlop_ctx_t * ctx, mlop_addr_t dest){
	uint8_t i;
	if(memcmp(dest, ctx->lastdest, MLOP_ADDR_SIZEB)){
		/* resync needed */
		lop_streamsync(ctx->lop);
		for(i=0; i<MLOP_ADDR_SIZEB; ++i)
			lop_sendstream(ctx->lop, dest[i]);
		for(i=0; i<MLOP_ADDR_SIZEB; ++i)
			lop_sendstream(ctx->lop, ctx->addr[i]);
	}
	lop_streamsync(ctx->lop);
	for(i=0; i<MLOP_ADDR_SIZEB; ++i)
		lop_sendstream(ctx->lop, 0);	
}

/******************************************************************************/

void mlop_sendstream(mlop_ctx_t* ctx, mlop_addr_t dest, uint8_t b){
	if(memcmp(dest, ctx->lastdest, MLOP_ADDR_SIZEB)){
		/* resync needed */
		lop_streamsync(ctx->lop);
		uint8_t i;
		for(i=0; i<MLOP_ADDR_SIZEB; ++i)
			lop_sendstream(ctx->lop, dest[i]);
		for(i=0; i<MLOP_ADDR_SIZEB; ++i)
			lop_sendstream(ctx->lop, ctx->addr[i]);
	}
	lop_sendstream(ctx->lop, b);
}

/******************************************************************************/

void mlop_init(mlop_ctx_t* ctx){
	ctx->lop->optctx = ctx;
	ctx->lop->on_msgrx = (void(*)(uint16_t,uint8_t*))mlop_recieve_msg;
	ctx->lop->on_streamrx = (void(*)(uint8_t))mlop_recieve_stream;
	ctx->lop->on_streamsync = (void(*)(void))mlop_recieve_streamsync;
	ctx->automatic=1;
	ctx->addrnack=1;
	ctx->headbufidx=2*MLOP_ADDR_SIZEB;
	memset(ctx->lastdest, 0, MLOP_ADDR_SIZEB);
	memset(ctx->lastsrc, 0, MLOP_ADDR_SIZEB);
	ctx->streamrxact = 0;
}

/******************************************************************************/

uint8_t mlop_autobrc(mlop_ctx_t* ctx, uint16_t length, const uint8_t* msg){
	if(length==2*MLOP_ADDR_SIZEB+1){
		if(memcmp(msg+MLOP_ADDR_SIZE, ctx->tempaddr, MLOP_ADDR_SIZEB)==0 &&
		   msg[2*MLOP_ADDR_SIZEB+1]==MLOP_PONG){
			ctx->addrnack=1;
			return 0;
		}
	}
	return 1;
}

/******************************************************************************/

void mlop_reqaddr(mlop_ctx_t* ctx){	
	uint16_t i;
	
	for(i=0; i<MLOP_ADDR_SIZEB; ++i){
		ctx->addr[i]=0xFF;
	}
	while(ctx->addrnack){
		/* gen new addr */
		do{
			for(i=0; i<MLOP_ADDR_SIZEB; ++i){
				ctx->tempaddr[i]=rand()&0xff;
			}
		}while(isbroadcast(ctx->tempaddr) || issync(ctx->tempaddr));
		ctx->addrnack=0;
		mlop_sendmessage_raw(ctx, ctx->tempaddr, 0, MLOP_PING, NULL);
		i=MLOP_PING_TIMEOUT;
		while(i-- && ctx->addrnack==0)
			_delay_ms(1);
	}
	memcpy(ctx->addr, ctx->tempaddr, MLOP_ADDR_SIZEB);
	for(i=0; i<MLOP_ADDR_SIZEB; ++i){
		ctx->tempaddr[i]=0x00;
	}
}

/******************************************************************************/
