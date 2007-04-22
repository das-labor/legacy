/* error-handling.c */


#include "error-handling.h"
#include <stdlib.h>
#include <stdarg.h>

/*
 * Aim:
 *  getting something like exceptions with low overhead
 *  for error-handling, so that we can do:
 *
 *  errorHandler_reg(MEM-ERR, myHandler);
 *
 *	...
 *	if (!(buffer=malloc(...))
 *		error(MEM-ERR);
 *	...
 *
 * error handler data structur should be single chained list
 */


//typedef enum {false=0, true=1} bool;

errorHandlerDBentry_t * DBfirst=(void*)0;

void errorHandler_reg(errorHandler_t handler,unsigned int precision, ...){
	errorHandlerDBentry_t * ptr;
	unsigned int i;
	va_list ap;

	ptr=malloc(sizeof(errorHandlerDBentry_t));
	ptr->handler=handler;
	ptr->next=(void*)0;
	ptr->desc.length=precision;
	ptr->desc.desc=malloc(precision * sizeof(errorDesc_t));
	va_start(ap, precision);
	for (i=0; i<precision; ++i){
		ptr->desc.desc[i]= (errorDesc_t)va_arg(ap, int); // hmm, this is strange, it shoud be errorDesc_t instead of int
	}
	va_end(ap);
	
	if (DBfirst){
		errorHandlerDBentry_t * p;
		p=DBfirst;
		while (p->next){
			p=p->next;
		}
		p->next=ptr;
	} else
	{
		DBfirst=ptr;
	}	
}


bool error_desc_match(errorFullDesc_t *sample,errorFullDesc_t *pattern){
	unsigned int i;
	if (pattern->length > sample->length)
		return false;
	for (i=0; i<pattern->length; ++i){
		if (sample->desc[i] != pattern->desc[i])
			return false;
	}
	return true;
}

void errorHandler_unreg_full(errorFullDesc_t desc){
	errorHandlerDBentry_t *lastmatch,*p;
	p=lastmatch=DBfirst;
	do{/* todo: unchain */
		if(error_desc_match(&desc, &(p->desc)))
			lastmatch=p;
	}while((p=p->next));
	
}

void errorHandler_unreg(unsigned int precision, ...){

}


void* error_full (errorDataPtr_t data, errorFullDesc_t error){
	errorHandlerDBentry_t *lastmatch,*p;
	p=lastmatch=DBfirst;
	do{
		if(error_desc_match(&error, &(p->desc)))
			lastmatch=p;
	}while((p=p->next));
	
	return (lastmatch->handler)(data);
}

void* error (errorDataPtr_t data, unsigned int precision, ...){
	errorFullDesc_t error;
	unsigned int i;
	va_list ap;
	
	error.length=precision;
	error.desc=malloc(precision * sizeof(errorDesc_t));
	va_start(ap, precision);
	for(i=0;i<precision;++i){
		error.desc[i] = (errorDesc_t)va_arg(ap, int); // hmm, this is strange, it shoud be errorDesc_t instead of int
	}
	va_end(ap);
	return error_full(data, error);
}

