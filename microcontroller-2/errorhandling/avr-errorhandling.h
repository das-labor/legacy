/**
 * \file avr-errorhandling.h
 * \author Daniel Otte
 * \date 13.10.2006
 * \par License GPL
 * 
 * 
 * 
 */

#ifndef AVRERRORHANDLING_H_
#define AVRERRORHANDLING_H_

#include <stdint.h>
#include <stdlib.h> /* malloc & Co. */
#include <string.h> /* memcmp & Co. */
#include <stdarg.h> /* va_list, va_start, va_end */
#include <avr/pgmspace.h>

#define ERROR_PANIC_ACTION asm("jmp 0") /* oh yeah, this is evil and will reset the device */

/********************************************************************************************************/

typedef void* void_func_ptr_t(void);
typedef void* errorDataPtr_t;
typedef void* errorHandler_t(errorDataPtr_t);

/********************************************************************************************************/

typedef struct{
	uint8_t	length;
	uint8_t *data;
} error_id_t;

/********************************************************************************************************/
/*
typedef struct{
	errorHandler_t* handler;
	uint8_t precision;
	uint8_t* descriptor;
} error_ramitem_t;
*/
typedef PROGMEM uint8_t PROGMEM errorBlockId_t[];

typedef struct{
	prog_uint8_t* PROGMEM id;
	errorHandler_t*  PROGMEM handler;
} errorblockentry_t;

typedef struct {
	const uint8_t n;
	const errorblockentry_t entry[];
} PROGMEM errorblock_t;

/********************************************************************************************************/
void error_init(void);
/********************************************************************************************************/
void error_register_handler(errorHandler_t* h, uint8_t precision, uint8_t* descriptor);
/********************************************************************************************************/
uint8_t error_unregister_handler_bydesc(uint8_t precision, uint8_t* descriptor);
/********************************************************************************************************/
void error_register_handlerblock(void* block);
/********************************************************************************************************/
uint8_t error_unregister_handlerblock(void* block);
/********************************************************************************************************/
void* error_full (errorDataPtr_t data, error_id_t error);
/********************************************************************************************************/
void* error (errorDataPtr_t data, unsigned int precision, ...);
/********************************************************************************************************/

#endif /*AVRERRORHANDLING_H_*/
