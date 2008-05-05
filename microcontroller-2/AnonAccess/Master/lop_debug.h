#ifndef LOP_DEBUG_H_
#define LOP_DEBUG_H_


#include <stdint.h>
#include <avr/pgmspace.h>
#include "lop.h"


void lop_dbg_str(lop_ctx_t * ctx, char * str);
void lop_dbg_str_P(lop_ctx_t * ctx, PGM_P str);
void lop_dbg_hexdump(lop_ctx_t * ctx, void * buf, uint16_t length);
void terminal_print(lop_ctx_t * ctx, uint8_t class, char* str);
void terminal_print_P(lop_ctx_t * ctx, uint8_t class, PGM_P str);

#endif /*LOP_DEBUG_H_*/
