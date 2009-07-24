/* circularbuffer-asm.S */
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
 * \file     circularbuffer-asm.S
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-07-24
 * \license  GPLv3 or later
 * \ingroup  circularbuffer
 * \brief    implementation for circular buffer
 */
/*
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
*/
#define ELEMENT_SIZE_OFFSET  0
#define BUFFER_SIZE_OFFSET   1
#define FILLCOUNT_OFFSET     2
#define ABS_SIZE_OFFSET      3
#define BUFFER_OFFSET        5
#define HEAD_OFFSET          7
#define TAIL_OFFSET          9
#define TOP_OFFSET          11

/******************************************************************************/
/* 
 *	uint8_t circularbuffer_init(uint8_t elementsize_B, uint8_t buffersize, circularbuffer_t* buffer){
 *		buffer->element_size_B = elementsize_B;
 *		buffer->buffer_size = buffersize;
 *		buffer->abs_size = buffersize*elementsize_B;
 *		buffer->buffer = malloc(buffer->abs_size);
 *		buffer->head = buffer->tail = buffer->buffer;
 *		buffer->top = buffer->buffer+buffer->abs_size;
 *		buffer->fillcount = 0;
 *		if(buffer->buffer)
 *			return 1; / * success * /
 *		return 0; / * malloc failed * /	
 *	}
 *
 * param elementsize_B: r24
 * param buffersize:    r22
 * param buffer:        r20:r21
 */
.global circularbuffer_init 
circularbuffer_init:
	push r28
	push r29
	movw r28, r20
	st Y+, r24
	st Y+, r22
	st Y+, r1
	mul r22, r24
	std Y+0, r0
	std Y+1, r1
	movw r24, r0
	clr r1
	call malloc
	adiw r24, 0
	brne 1f
	ldi r24, 1
	rjmp 99f
1:
	std Y+2, r24
	std Y+3, r25
	std Y+4, r24
	std Y+5, r25
	std Y+6, r24
	std Y+7, r25
	ldd r22, Y+0
	ldd r23, Y+1
	add r24, r22
	adc r25, r23
	std Y+8, r24
	std Y+9, r25
	clr r24
99:	
	clr r25
	pop r29
	pop r28
	ret
	
/******************************************************************************/
/*
 *	uint8_t circularbuffer_cnt(circularbuffer_t* cb){
 *		return (cb->fillcount);
 *	}
 *
 * param cb: r24:r25
 */
.global circularbuffer_cnt
circularbuffer_cnt:
	movw r30, r24
	ldd r24, Z+2
 	clr r25
	ret
 
/******************************************************************************/
/*
 *	uint8_t circularbuffer_free(circularbuffer_t* cb){
 *		free(cb->buffer);
 *	}
 *
 * param cb:  r24:r25
 */
.global circularbuffer_free 
circularbuffer_free:
	movw r30, r24
	ldd r24, Z+5
	ldd r25, Z+6
	rjmp free

/******************************************************************************/
/*
 *	void* circularbuffer_get_lifo(circularbuffer_t* cb){
 *		void* ret;
 *		if(cb->fillcount==0)
 *			return NULL;
 *		--cb->fillcount;
 *		ret=cb->tail;
 *		cb->tail = (uint8_t*)(cb->tail) + cb->element_size_B;
 *		if(cb->tail>=cb->top)	
 *			cb->tail = (uint8_t*)(cb->tail) - cb->abs_size;
 *		return ret;	
 *	}
 *
 * param cb:  r24:r25
 */
.global circularbuffer_get_lifo 
circularbuffer_get_lifo:
	movw r30, r24
	ldd r24, Z+FILLCOUNT_OFFSET
	tst r24
	brne 30f
	clr r25
	ret
30: dec r24
	std Z+2, r24
	ldd r24, Z+TAIL_OFFSET
	ldd r25, Z+TAIL_OFFSET+1
	movw r26, r24
	ldd r23, Z+ELEMENT_SIZE_OFFSET
	add r26, r23
	adc r27, r1
	ldd r22, Z+TOP_OFFSET
	ldd r23, Z+TOP_OFFSET+1
	cp  r26, r22
	cpc r27, r23
	brlo 90f
	ldd r22, Z+ABS_SIZE_OFFSET
	ldd r23, Z+ABS_SIZE_OFFSET+1
	sub r26, r22
	sbc r27, r23
90:
	std Z+TAIL_OFFSET, r26
	std Z+TAIL_OFFSET+1, r27
	ret	

/******************************************************************************/
/*
 *	void* circularbuffer_get_fifo(circularbuffer_t* cb){
 *		void* ret;
 *		if(cb->fillcount==0)
 *			return NULL;
 *		--cb->fillcount;
 *		ret=cb->head;
 *		cb->head = (uint8_t*)(cb->head) - cb->element_size_B;
 *		if(cb->head<cb->buffer)	
 *			cb->head = (uint8_t*)(cb->head) + cb->abs_size;
 *		return ret;
 *	}
 *
 * param cb:  r24:r25
 */
.global circularbuffer_get_fifo 
circularbuffer_get_fifo:
	movw r30, r24
	ldd r24, Z+FILLCOUNT_OFFSET
	tst r24
	brne 30f
	clr r25
	ret
30: dec r24
	std Z+2, r24
	ldd r24, Z+HEAD_OFFSET
	ldd r25, Z+HEAD_OFFSET+1
	movw r26, r24
	ldd r23, Z+ELEMENT_SIZE_OFFSET
	sub r26, r23
	sbc r27, r1
	ldd r22, Z+BUFFER_OFFSET
	ldd r23, Z+BUFFER_OFFSET+1
	cp  r26, r22
	cpc r27, r23
	brge 90f
	ldd r22, Z+ABS_SIZE_OFFSET
	ldd r23, Z+ABS_SIZE_OFFSET+1
	add r26, r22
	adc r27, r23
90:
	std Z+HEAD_OFFSET, r26
	std Z+HEAD_OFFSET+1, r27
	ret	

/******************************************************************************/
/*
 *	uint8_t circularbuffer_append(void* elem, circularbuffer_t* cb){
 *		if(cb->fillcount==cb->buffer_size)
 *			return 1;
 *		cb->fillcount++;	
 *		cb->tail = cb->tail - cb->element_size_B;
 *		if(cb->tail < cb->buffer)	
 *			cb->tail = (uint8_t*)(cb->tail) + cb->abs_size;
 *		if(cb->fillcount==1)
 *			cb->head = cb->tail;
 *		memcpy(cb->tail, elem, cb->element_size_B);
 *		return 0;
 *	}
 *
 * param elem:  r24:r25
 * param cb:    r22:r23
 */
.global circularbuffer_append 
circularbuffer_append:
	movw r30, r22
	movw r26, r24
	clr r25
	clt
	ldd r20, Z+FILLCOUNT_OFFSET
	ldd r21, Z+BUFFER_SIZE_OFFSET
	cp r20, r21
	brne 10f
	ldi r24, 1
	ret
10:
	tst r20
	brne 11f
	set
11:
	inc r20
	std Z+FILLCOUNT_OFFSET, r20
	ldd r22, Z+TAIL_OFFSET
	ldd r23, Z+TAIL_OFFSET+1
	ldd r21, Z+ELEMENT_SIZE_OFFSET
	sub r22, r21
	sbc r23, r1
	ldd r20, Z+BUFFER_OFFSET
	ldd r21, Z+BUFFER_OFFSET+1
	cp  r22, r20
	cpc r23, r21
	brge 30f
	ldd r20, Z+ABS_SIZE_OFFSET
	ldd r21, Z+ABS_SIZE_OFFSET+1
	add r22, r20
	adc r23, r21
30:	
	std Z+TAIL_OFFSET, r22
	std Z+TAIL_OFFSET+1, r23
	brtc 40f
	std Z+HEAD_OFFSET, r22
	std Z+HEAD_OFFSET+1, r23
40:
	ldd r20, Z+ELEMENT_SIZE_OFFSET
	movw r30, r22
41:
	ld r21, X+
	st Z+, r21
	dec r20
	brne 41b
	clr r24
	ret
	
/******************************************************************************/
/*	
 *	uint8_t circularbuffer_push(void* elem, circularbuffer_t* cb){
 *		if(cb->fillcount==cb->buffer_size)
 *			return 1;
 *		cb->fillcount++;	
 *		cb->head = cb->head + cb->element_size_B;
 *		if(cb->head>=cb->top)	
 *			cb->head = (uint8_t*)(cb->head) - cb->abs_size;
 *		if(cb->fillcount==1)
 *			cb->tail = cb->head;
 *		memcpy(cb->head, elem, cb->element_size_B);
 *		return 0;
 *	}	
 *	
 * param elem:  r24:r25
 * param cb:    r22:r23
 */
 .global circularbuffer_push 
circularbuffer_push:
	movw r30, r22
	movw r26, r24
	clr r25
	clt
	ldd r20, Z+FILLCOUNT_OFFSET
	ldd r21, Z+BUFFER_SIZE_OFFSET
	cp r20, r21
	brne 10f
	ldi r24, 1
	ret
10:
	tst r20
	brne 11f
	set
11:
	inc r20
	std Z+FILLCOUNT_OFFSET, r20
	ldd r22, Z+HEAD_OFFSET
	ldd r23, Z+HEAD_OFFSET+1
	ldd r21, Z+ELEMENT_SIZE_OFFSET
	add r22, r21
	adc r23, r1
	ldd r20, Z+TOP_OFFSET
	ldd r21, Z+TOP_OFFSET+1
	cp  r22, r20
	cpc r23, r21
	brlo 30f
	ldd r20, Z+ABS_SIZE_OFFSET
	ldd r21, Z+ABS_SIZE_OFFSET+1
	sub r22, r20
	sbc r23, r21
30:	
	std Z+HEAD_OFFSET, r22
	std Z+HEAD_OFFSET+1, r23
	brtc 40b
	std Z+TAIL_OFFSET, r22
	std Z+TAIL_OFFSET+1, r23
	rjmp 40b
	
