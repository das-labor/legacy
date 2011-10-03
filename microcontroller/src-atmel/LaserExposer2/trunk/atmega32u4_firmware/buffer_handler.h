#ifndef buffer_handler_h__
#define buffer_handler_h__

//buffer_handler.h

#include "config.h"

void resetbufferpointer(exposer_buffer_t *buf);

void append_n_one_bits(exposer_buffer_t *buf, uint16_t num);

void append_n_zero_bits(exposer_buffer_t *buf, uint16_t num);

void append_byte(exposer_buffer_t *buf,uint8_t data);

void set_byte(exposer_buffer_t *buf,uint16_t index, uint8_t data);

#endif
