/**
 * \file avr-errorhandling.c
 * \author Daniel Otte
 * \date 13.10.2006
 * \par License GPL
 * 
 * 
 * 
 */


#include "config.h"

#undef DEBUG

#ifdef DEBUG
 #include "uart.h"
 #include "debug.h"
#endif

#include <stdint.h>
#include <stdlib.h> /* malloc & Co. */
#include <string.h> /* memcmp & Co. */
#include <stdarg.h> /* va_list, va_start, va_end */
#include <avr/pgmspace.h>




/********************************************************************************************************/

#define ERROR_PANIC_ACTION asm("jmp 0") /* oh yeah, this is evil and will reset the device */

/********************************************************************************************************/

typedef void* void_func_ptr_t();
typedef void* errorDataPtr_t;
typedef void* errorHandler_t(errorDataPtr_t);

/********************************************************************************************************/

typedef struct{
	enum{flash, ram} media;
	void* ptr;
} ptr_t;

/********************************************************************************************************/

typedef struct {
	enum{flashblock, ramhandler}	type;
	void*	dataptr;
	void*	next;
} listitem_t;

/********************************************************************************************************/

typedef struct{
	uint8_t	length;
	uint8_t *data;
} error_id_t;

/********************************************************************************************************/

typedef struct{
	errorHandler_t* handler;
	uint8_t precision;
	uint8_t* descriptor;
} error_ramitem_t;

/********************************************************************************************************/

/* GLOBAL VARS */
listitem_t* anchor;

/********************************************************************************************************/

void error_init(){
	anchor = (void*)0;
}

/********************************************************************************************************/

listitem_t* error_search_handler_bydesc(uint8_t precision, uint8_t* descriptor);


/********************************************************************************************************/

/**
 * \brief registers an error-handler
 * This allows simple error-handler registration, by simply supplying the error-handler (pointer to a 
 * function) and the error-description which specifys when to call the error-handler
 * @param h points to the function which acts as error-handler
 * @param precision 
 * @param descriptor
 */ 
void error_register_handler(errorHandler_t* h, uint8_t precision, uint8_t* descriptor){
	listitem_t* lp;
	lp = malloc(sizeof(listitem_t));
	/* xxx error handling */
	lp->type = ramhandler;
	lp->next = anchor;
	anchor = lp;
	lp->dataptr = malloc(sizeof(error_ramitem_t));
	/* xxx error handling */
	((error_ramitem_t*)(lp->dataptr))->handler = h;
	((error_ramitem_t*)(lp->dataptr))->precision = precision;
	((error_ramitem_t*)(lp->dataptr))->descriptor = descriptor;
}

/********************************************************************************************************/

uint8_t error_unregister_handler_bydesc(uint8_t precision, uint8_t* descriptor){
	listitem_t* lp;
	lp = anchor;
	if(!lp)
		return 0;
	while( (lp->next) &&
		(	(((listitem_t*)(lp->next))->type != ramhandler) ||	/* doen't it look like some kind of crazy lisp? */
			(((error_ramitem_t*)(((listitem_t*)(lp->next))->dataptr))->precision > precision) ||
			memcmp(((error_ramitem_t*)(((listitem_t*)(lp->next))->dataptr))->descriptor, descriptor, 
				((error_ramitem_t*)(((listitem_t*)(lp->next))->dataptr))->precision) )){
		lp = lp->next;
	}
	if(!lp->next)
		return 0;
	/* the unchaining step */	
	{
	listitem_t* tp;
	tp = lp->next;
	lp->next = ((listitem_t*)(lp->next))->next;
	free(tp->dataptr);
	free(tp);
	}
	return 1;
}


/********************************************************************************************************/

void error_register_handlerblock(void* block){
	listitem_t* lp;
	lp = malloc(sizeof(listitem_t));
	/* xxx error handling */
	lp->type = flashblock;
	lp->dataptr = block;
	lp->next = anchor;
	anchor = lp;
 #ifdef DEBUG
	/* DEBUG DUMPING */
	{
	uint8_t items,item,b,i;
	uint16_t d;
	
	DEBUG_S("\r\nregistering following handlerblock:\r\n Addr:");
	uart_hexdump(block, 2);
	
	DEBUG_S("\r\nsize: 0x"); 
	items = pgm_read_byte_near(block);
	DEBUG_B(items);
	for(item=0; item<items; ++item){
		DEBUG_S("\r\nID"); DEBUG_B(item); DEBUG_S("-ptr: ");
		d = pgm_read_word_near(block+1+item*4);
		uart_hexdump(&d, 2);
		
		DEBUG_S("\r\n\tlength: ");
		b = pgm_read_byte_near(d);
		DEBUG_B(b);
		
		DEBUG_S("\r\n\tid: ");
		for(i=0;i<b;++i){
			DEBUG_B(pgm_read_byte_near(++d));
		}
		
		DEBUG_S("\r\nHandler"); DEBUG_B(item); DEBUG_S("-ptr: ");
		d = pgm_read_word_near(block+3+item*4);
		uart_hexdump(&d, 2);
	}
	}
 #endif /* debug */	
}

/********************************************************************************************************/

uint8_t error_unregister_handlerblock(void* block){
	listitem_t* lp;
	/* lets find our block */
	lp=anchor;
	if(!lp)
		return 0;
	while ((lp->next) && (
		(((listitem_t*)(lp->next))->type != flashblock) ||
		(((listitem_t*)(lp->next))->dataptr != block) )){
			 /* we must be sure that the left expression is evaluated first! */
		lp=lp->next;
	}
	if(!(lp->next))
		return 0;
	/* the unchaining step */
	{	
	listitem_t* tp;
	tp = lp->next;
	lp->next = ((listitem_t*)(lp->next))->next;
	free(tp);
	}
	return 1;
}

/********************************************************************************************************/

int memcmp_P(const void* s1, PGM_VOID_P s2, size_t n){ /* why the fuck isn't this implemented in avr-libc??? */
 #ifdef DEBUG
	DEBUG_S("\r\ncomparig:\r\na: ");
	uart_hexdump((void*)s1, n);
	DEBUG_S("\r\nb: ");
	{int i;
	 PGM_VOID_P p=s2;
	 for(i=0;i<n;++i){
	 	uint8_t t;
	 	t=pgm_read_byte_near(p++);
	 	uart_hexdump(&t, 1);
	 }
	}
 #endif	
	while ((*((uint8_t*)(s1)) == (uint8_t)pgm_read_byte_near(s2)) && n--){
		++s1;
		++s2; /* ignore those warnings */
	}
	return n;
}

/********************************************************************************************************/

void* error_full (errorDataPtr_t data, error_id_t error){
	listitem_t *lp;
	/* search right handler */
	lp=anchor;
	if(!lp){
 #ifdef DEBUG	
		int x;
		DEBUG_S("\r\n!!! P A N I C !!!\r\n");
		for(x=1;x;++x);
 #endif
	}	
	while (lp){		
		if(lp->type == ramhandler){
		 #ifdef DEBUG	
			DEBUG_S("\r\nlp = "); uart_hexdump(lp, sizeof(listitem_t));
		 #endif
			if((((error_ramitem_t*)lp->dataptr)->precision <= error.length) 
				&& !memcmp(((error_ramitem_t*)lp->dataptr)->descriptor,error.data,
						((error_ramitem_t*)lp->dataptr)->precision) ){
				/* we found it */
				return ((error_ramitem_t*)lp->dataptr)->handler(data);
			}
		} else { /* then type == flashblock*/
			/* we won't use a C-stuct here, but the structure is fairly simple: 
			 * first we have a byte telling us the number of entrys
			 * then the entrys are following, each consisting of a 16-bit pointer to ID and a 16-bit
			 * pointer to code (both, code and ID, are located in flash).
			 * ID itself consists of a byte specifying the "precision", and as much bytes as specifyed by
			 * "precision".
			 * 
			 *                   +-----+
			 *                   | size|
			 * +-----+-----+-----+-----+
			 * | ID_PTR    | HANDLER_P |
			 * +-----+-----+-----+-----+
			 *       .............
			 * +-----+-----+-----+-----+
			 * | ID_PTR    | HANDLER_P |
			 * +-----+-----+-----+-----+
			 * 
			 */
			uint8_t i,n;
			n=pgm_read_byte_near(lp->dataptr);
			for(i=0; i<n; ++i){
				uint16_t idptr;
				uint8_t idlength;
				idptr=pgm_read_word(lp->dataptr+1+i*4); /* calculate ID addr */
				idlength=pgm_read_byte_near(idptr);
				if(idlength<=error.length){
				 #ifdef DEBUG
					DEBUG_S("\r\nTADA");
				 #endif
					if(!memcmp_P(error.data, (PGM_VOID_P)(idptr+1), idlength)){
						/* we got it */
						errorHandler_t* handler;
						handler=(errorHandler_t*)pgm_read_word(lp->dataptr+1+i*4+2);
						return handler(data);
					}
				}
			} /* END flashblock inner iteration loop */
		}
		lp=lp->next;
	}/* END of while loop iterating the list */
	/* oh shit we didn't find a handler what should we do???*/
	/* */
   #ifdef DEBUG	
	DEBUG_S("\r\nNO HANDLER FOUND");
   #endif
 	{uint16_t x;for(x=0x8000;x;++x);} /* insert some delay to avoid to fast resetting */
	ERROR_PANIC_ACTION;
	return (void*)0; /* just to satisfy the compiler */
}

/********************************************************************************************************/

void* error (errorDataPtr_t data, unsigned int precision, ...){
	error_id_t error;
	unsigned int i;
	va_list ap;
   #ifdef DEBUG
	DEBUG_S("\r\n entering error ...");{int x;for(x=0x100;x;++x);}
   #endif
	error.length=precision;
	error.data = malloc(precision * sizeof(uint8_t));
	va_start(ap, precision);
	for(i=0;i<precision;++i){
		error.data[i] = (uint8_t)va_arg(ap, int); 
		/* hmm, this is strange, it shoud be uint8_t instead of int */
	}
	va_end(ap);
   #ifdef DEBUG
	DEBUG_S("\r\n calling error_full ...");{int x;for(x=0x100;x;++x);}
   #endif
	return error_full(data, error);
}

/********************************************************************************************************/

