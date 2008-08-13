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
	lopstates_t rxstate, txstate, msgretstate;
	uint32_t msgidx;
	uint16_t msglength;
	uint8_t* msgbuffer;
	uint8_t escaped;
	void* aux; /* auxiliary data, not used by lop */
	    /* pointer to the writing function */
	void (*sendrawbyte)(uint8_t, void*);          
	    /* function called on message recieve */
	void (*on_msgrx)(uint16_t, uint8_t*, void*);  
	    /* function called on recieve of a stream byte*/ 
	void (*on_streamrx)(uint8_t, void*);          
	    /* function called on recieve of streamsync */ 
	void (*on_streamsync)(void*);       
	    /* function called on recieve of streamsync */     
	void (*on_reset)(void*);            
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
