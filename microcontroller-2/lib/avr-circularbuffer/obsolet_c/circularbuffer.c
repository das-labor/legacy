/* circularbuffer.c */
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
 * \file     circularbuffer.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-07-24
 * \license  GPLv3 or later
 * \ingroup  circularbuffer
 * \brief    implementation for circular buffer
 */
 #include <stdint.h>
 #include <stdlib.h>
 #include <string.h>
 #include "circularbuffer.h"

uint8_t circularbuffer_init(uint8_t elementsize_B, uint8_t buffersize, circularbuffer_t* buffer){
	buffer->element_size_B = elementsize_B;
	buffer->buffer_size = buffersize;
	buffer->abs_size = buffersize*elementsize_B;
	buffer->buffer = malloc(buffer->abs_size);
	buffer->head = buffer->tail = buffer->buffer;
	buffer->top = buffer->buffer+buffer->abs_size;
	buffer->fillcount = 0;
	if(buffer->buffer)
		return 1; /* success */
	return 0; /* malloc failed */	
}

void* circularbuffer_get_lifo(circularbuffer_t* cb){
	void* ret;
	if(cb->fillcount==0)
		return NULL;
	--cb->fillcount;
	ret=cb->tail;
	cb->tail = (uint8_t*)(cb->tail) + cb->element_size_B;
	if(cb->tail>=cb->top)	
		cb->tail = (uint8_t*)(cb->tail) - cb->abs_size;
	return ret;	
}

void* circularbuffer_get_fifo(circularbuffer_t* cb){
	void* ret;
	if(cb->fillcount==0)
		return NULL;
	--cb->fillcount;
	ret=cb->head;
	cb->head = (uint8_t*)(cb->head) - cb->element_size_B;
	if(cb->head<cb->buffer)	
		cb->head = (uint8_t*)(cb->head) + cb->abs_size;
	return ret;
}

uint8_t circularbuffer_append(void* elem, circularbuffer_t* cb){
	if(cb->fillcount==cb->buffer_size)
		return 1;
	cb->fillcount++;	
	cb->tail = cb->tail - cb->element_size_B;
	if(cb->tail<cb->buffer)	
		cb->tail = (uint8_t*)(cb->tail) + cb->abs_size;
	if(cb->fillcount==1)
		cb->head = cb->tail;
	memcpy(cb->tail, elem, cb->element_size_B);
	return 0;
}

uint8_t circularbuffer_push(void* elem, circularbuffer_t* cb){
	if(cb->fillcount==cb->buffer_size)
		return 1;
	cb->fillcount++;	
	cb->head = cb->head + cb->element_size_B;
	if(cb->head>=cb->top)	
		cb->head = (uint8_t*)(cb->head) - cb->abs_size;
	if(cb->fillcount==1)
		cb->tail = cb->head;
	memcpy(cb->head, elem, cb->element_size_B);
	return 0;
}

uint8_t circularbuffer_cnt(circularbuffer_t* cb){
	return (cb->fillcount);
}

uint8_t circularbuffer_free(circularbuffer_t* cb){
	free(cb->buffer);
}
