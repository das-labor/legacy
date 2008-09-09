/* lop.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * 
 * 
 * 
 */
 
#ifndef LOP_H_
#define LOP_H_

#include <stdint.h>
#include <stddef.h> //#define NULL (void*)0

typedef enum {idle,message} lopstates_t;

/** 
 * \typedef 
 */

/**
 * \struct lop_ctx
 * 
 * 
 */
typedef struct lop_ctx {
	volatile lopstates_t rxstate, txstate, msgretstate;
	uint32_t msgidx;
	uint16_t msglength;
	uint8_t* msgbuffer;
	uint8_t escaped;
	void (*sendrawbyte)(uint8_t);           /* pointer to the writing function */
	void (*on_msgrx)(uint16_t, uint8_t*);   /* function called on message recieve */
	void (*on_streamrx)(uint8_t);           /* function called on recieve of a stream byte*/
	void (*on_streamsync)(void);            /* function called on recieve of streamsync */
	void (*on_reset)(void);            /* function called on recieve of streamsync */
} lop_ctx_t;

/******************************************************************************/
void lop_init(lop_ctx_t* ctx);
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
