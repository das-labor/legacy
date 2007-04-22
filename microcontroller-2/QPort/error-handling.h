#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

typedef void* void_func_ptr_t();
typedef void* errorDataPtr_t;
typedef void* errorHandler_t(errorDataPtr_t);

typedef char errorDesc_t;


/*
 * Maintain errorHnadler DB as simpled chained list
 * [ handlerFullDesc_t | handler* | next* ]
 * 
 * 
 * 
 */

typedef struct{
	unsigned int length;
	errorDesc_t* desc;
} errorFullDesc_t;

/*typedef  errorHandlerDBentry_t;*/

typedef  struct errorHandlerDBentry_t {
	errorHandler_t* handler;
	errorFullDesc_t desc;
	struct errorHandlerDBentry_t* next;
} errorHandlerDBentry_t;


typedef enum {false=0, true=1} bool;


void errorHandler_reg(errorHandler_t handler,unsigned int precision, ...);
bool error_desc_match(errorFullDesc_t *sample,errorFullDesc_t *pattern);
void errorHandler_unreg_full(errorFullDesc_t desc);
void errorHandler_unreg(unsigned int precision, ...);
void* error_full (errorDataPtr_t data, errorFullDesc_t error);
void* error (errorDataPtr_t data, unsigned int precision, ...);

#endif /*ERRORHANDLING_H_*/


