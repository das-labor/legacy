/* circularbytebuffer.c */
/*
    This file is part of the ARM-circularbytebuffer.
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
 * \file     circularbytebuffer.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-07-24
 * \license  GPLv3 or later
 * \ingroup  circularbytebuffer
 * \brief    implementation for circular byte buffer
 */
 #include <stdint.h>
 #include <stdlib.h>
 #include <string.h>
 #include "circularbytebuffer.h"
 

#ifndef CIRCULARBYTEBUFFER_NO_MALLOC
#  define CIRCULARBYTEBUFFER_NO_MALLOC 0
#endif

#ifndef CIRCULARBYTEBUFFER_NO_INIT2
#  define CIRCULARBYTEBUFFER_NO_INIT2 0
#endif

#if CIRCULARBYTEBUFFER_NO_MALLOC==0
uint8_t circularbytebuffer_init(uint32_t buffersize, circularbytebuffer_t* cb){
	cb->buffer_size = buffersize;
	cb->buffer = malloc(buffersize);
	cb->head = cb->tail = cb->buffer;
	cb->top = cb->buffer + cb->buffer_size;
	cb->fillcount = 0;
	if(cb->buffer)
		return 1; /* success */
	return 0; /* malloc failed */	
}
#endif

#if CIRCULARBYTEBUFFER_NO_INIT2==0
void circularbytebuffer_init2(uint32_t buffersize, circularbytebuffer_t* cb, void* buffer){
	cb->buffer_size = buffersize;
	cb->buffer = buffer;
	cb->head = cb->tail = cb->buffer;
	cb->top = cb->buffer + cb->buffer_size;
	cb->fillcount = 0;
}
#endif

uint16_t circularbytebuffer_get_lifo(circularbytebuffer_t* cb){
	uint8_t ret;
	if(cb->fillcount==0)
		return 0xffff;
	--cb->fillcount;
	ret=*(cb->tail);
	cb->tail = (uint8_t*)(cb->tail) + 1;
	if(cb->tail>=cb->top)	
		cb->tail = (uint8_t*)(cb->tail) - cb->buffer_size;
	return ret;	
}

uint16_t circularbytebuffer_get_fifo(circularbytebuffer_t* cb){
	uint8_t ret;
	if(cb->fillcount==0)
		return 0xffff;
	--cb->fillcount;
	ret=*(cb->head);
	cb->head = (uint8_t*)(cb->head) - 1;
	if(cb->head<cb->buffer)	
		cb->head = (uint8_t*)(cb->head) + cb->buffer_size;
	return ret;
}

uint8_t circularbytebuffer_append(uint8_t elem, circularbytebuffer_t* cb){
	if(cb->fillcount==cb->buffer_size)
		return 1;
	cb->fillcount++;	
	cb->tail = cb->tail - 1;
	if(cb->tail<cb->buffer)	
		cb->tail = (uint8_t*)(cb->tail) + cb->buffer_size;
	if(cb->fillcount==1)
		cb->head = cb->tail;
	*(cb->tail) = elem;
	return 0;
}

uint8_t circularbytebuffer_push(uint8_t elem, circularbytebuffer_t* cb){
	if(cb->fillcount==cb->buffer_size)
		return 1;
	cb->fillcount++;	
	cb->head = cb->head + 1;
	if(cb->head>=cb->top)	
		cb->head = (uint8_t*)(cb->head) - cb->buffer_size;
	if(cb->fillcount==1)
		cb->tail = cb->head;
	*(cb->head) = elem;
	return 0;
}

uint32_t circularbytebuffer_cnt(circularbytebuffer_t* cb){
	return (cb->fillcount);
}

#if CIRCULARBYTEBUFFER_NO_MALLOC==0
void circularbytebuffer_free(circularbytebuffer_t* cb){
	free(cb->buffer);
}
#endif
