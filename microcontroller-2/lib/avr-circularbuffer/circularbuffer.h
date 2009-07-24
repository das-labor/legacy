/* circularbuffer.h */
/*
    This file is part of the AVR-circularbuffer.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * \file     circularbuffer.h
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-07-24
 * \license  GPLv3 or later
 * \ingroup  circularbuffer
 * \brief    declaration for circular buffer
 */
 #include <stdint.h>
 #include <stdlib.h>
 
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_
 
 typedef struct {
	 uint8_t element_size_B;
	 uint8_t buffer_size;
	 uint8_t fillcount;
	 uint16_t abs_size;
	 uint8_t* buffer;
	 uint8_t* head;
	 uint8_t* tail;
	 uint8_t* top;
} circularbuffer_t;

uint8_t circularbuffer_init(uint8_t elementsize_B, uint8_t buffersize, circularbuffer_t* buffer);
void* circularbuffer_get_lifo(circularbuffer_t* cb);
void* circularbuffer_get_fifo(circularbuffer_t* cb);
uint8_t circularbuffer_append(void* elem, circularbuffer_t* cb);
uint8_t circularbuffer_push(void* elem, circularbuffer_t* cb);
uint8_t circularbuffer_cnt(circularbuffer_t* cb);
uint8_t circularbuffer_free(circularbuffer_t* cb);

#endif /* CIRCULARBUFFER_H_ */
