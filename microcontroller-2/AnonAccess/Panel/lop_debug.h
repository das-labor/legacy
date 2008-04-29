#ifndef LOP_DEBUG_H_
#define LOP_DEBUG_H_


#include <stdint.h>
#include <avr/pgmspace.h>
#include "lop.h"


void lop_dbg_str(lop_ctx_t * ctx, const char * str);
void lop_dbg_str_P(lop_ctx_t * ctx, PGM_P str);
void lop_dbg_hexdump(lop_ctx_t * ctx, const void * buf, uint16_t length);


#endif /*LOP_DEBUG_H_*/
