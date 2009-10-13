/* avr-huffman-decode-asm.S */
/*
    This file is part of the AVR-Huffman.
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

#include "avr-asm-macros.S"

#define PREFIX_SIZE_B 32

/*
typedef struct {
	void* tree;
	uint8_t  rbuffer;
	uint8_t  rbuffer_index;
#if HUFFMAN_USE_ADDR_16
	uint16_t(*read_byte)(uint16_t addr);
	uint16_t addr;
#else
	uint16_t(*read_byte)(uint32_t addr);
	uint32_t addr;
#endif
} huffman_dec_ctx_t;
*/

/*
 * param ctx: r24:r25
 */
.global read_bit
read_bit:
	movw r26, r24
	adiw r26, 3		/* X points at rbuffer_index */
	ld r25, X		/* load rbuffer_index to r25 */
	cpi r25, 8
	brne 5f			/* there is data in the buffer */
	st X, r1
	push r26
	push r27
	adiw r26, 1
	ld r30, X+		/* load function pointer */
	ld r31, X+
	ld r24, X+		/* load addr */
	ld r25, X+
	movw r22, r24
	adiw r24, 1
	st -X, r25		/* store incremented addr */
	st -X, r24
	movw r24, r22
	icall
	pop r27
	pop r26
	tst r25
	breq 2f
	/* we got EOF */
	ser r24
	ser r25
	ret
2:
	mov r22, r24
	sbiw r26, 1
	rjmp 6f
5: /* there is data in the buffer */

	ld r22, -X
6:
	lsl r22
	st X+, r22
	clr r24
	rol r24
	inc r25
	st X, r25
	clr r25
	ret

V_EOF_HIGH = 0x02
V_EOF_LOW  = 0xff
V_NODE_HIGH = 0x01
V_NODE_LOW  = 0xff

/******************************************************************************/
/* uint16_t huffman_dec_byte(huffman_dec_ctx_t* ctx)
 * param ctx: r24:r25
*/
CTX_0 = 16
CTX_1 = 17
.global huffman_dec_byte
huffman_dec_byte:
;	push r24
;	push r25
;	ldi r24, 'X'
;	clr r25
;	call cli_putc
;	pop r25
;	pop r24

	push r28
	push r29
	movw r30, r24
	ld  r28, Z
	ldd r29, Z+1
	adiw r28, 0
	brne 5f
	push r30
	push r31
	rcall build_tree
;	/* DBG */
;	push_range 24, 31
;	ldi r28, '0'
;	add r24, r28
;	clr r25
;	call cli_putc
;	pop_range 24, 31
;	/* END DBG*/
	pop r31
	pop r30
	tst r24
	breq 2f
	ser r25
	ser r24
	pop r29
	pop r28
	ret
2: /* tree has been build successfuly */
	ld  r28, Z
	ldd r29, Z+1
5: /* tree is there and pointed to by Y */
	push CTX_0
	push CTX_1
	movw CTX_0, r30
6:
	ldd r23, Y+1
	tst r23
	breq 8f /* value found */
	cpi r23, V_EOF_HIGH
	breq 9f /* eof found */
	/* we are at a node */
	cpi r23, V_NODE_HIGH
	brne 9f /* Tree is broken */
	movw r24, CTX_0
	rcall read_bit
	tst r25
	brne 9f /* eof occured */
	cpse r24, r1 /* take the left way if r24==0 */
	/* take the right way */
	adiw r28, 2
7:
	adiw r28, 2
	ld r24, Y+
	ld r25, Y
	movw r28, r24
	rjmp 6b

8:  ld r24, Y
	clr r25
	pop CTX_1
	pop CTX_0
	pop r29
	pop r28
	ret

9:
	movw r30, CTX_0
	ld r24, Z+
	ld r25, Z+
	rcall free
;	movw r30, CTX_0 ; xxx
;	st Z+, r1       ; xxx
;	st Z+, r1       ; xxx
	ser r25
	ser r24
	pop	CTX_1
	pop CTX_0
	pop r29
	pop r28
	ret

/******************************************************************************/
/* void huffman_dec_init(huffman_dec_ctx_t* ctx, uint16_t(*rb_func)(uint16_t))
 *  param ctx:     r24:r25
 *  param rb_func: r22:r23
 */
.global huffman_dec_init
huffman_dec_init:
	movw r30, r24
	st Z+, r1 /* set tree to NULL */
	st Z+, r1
	st Z+, r1 /* set rbuffer to 0 */
	ldi r24, 8
	st Z+, r24 /* set rbuffer_index to 8 */
	st Z+, r22 /* set pointer to rb_func */
	st Z+, r23
	st Z+, r1  /* set addr to 0 */
	st Z+, r1
	ret

/******************************************************************************/
/* uint8_t build_tree(huffman_dec_ctx_t* ctx)
 *  param ctx: r24:r25
 */
CTX_0            =  2
CTX_1            =  3
LEAF_END_PTR_0   =  4
LEAF_END_PTR_1   =  5
T_0              =  6
T_1              =  7
PART_SIZE_0      =  8
PART_SIZE_1      =  9
NEXT_PART_SIZE_0 = 10
NEXT_PART_SIZE_1 = 11
LEAF_PTR_0       = 28
LEAF_PTR_1       = 29
FREE_PTR_0       = 16
FREE_PTR_1       = 17
.global build_tree
build_tree:
	push_range 2, 17
	push_range 28, 29
	movw CTX_0, r24
	rcall read_byte
	cpi r24, 0xC0
	breq 2f
	ldi r24, 2
	rjmp 90f
2:	rcall read_byte
	lsr r24
	cpi r24, (0xde>>1)
	breq 3f
	ldi r24, 3
	rjmp 90f
3:
	clr T_0
	rol T_0
	rcall read_byte
	mov r25, T_0 /* r24:r25 contains the amount of leafs */
	lsl r24        /* we need space for leafs and nodes */
	rol r25        /* leaf_count*2 + (leaf_count-1)*6 = 8*leaf_count-6*/
	movw T_0, r24
	lsl r24
	rol r25
	lsl r24
	rol r25
	sbiw r24, 6
	movw LEAF_END_PTR_0, r24
	clr r23
	ldi r22, 1
	rcall calloc
	movw r30, CTX_0
	st Z+, r24
	st Z+, r25
	adiw r24, 0
	brne 4f
	/* calloc returned NULL */
	ldi r24, 4
	rjmp 90f
4:	/* now we have allocated memory for the tree */
	add LEAF_END_PTR_0, r24
	adc LEAF_END_PTR_1, r25
	movw LEAF_PTR_0, LEAF_END_PTR_0
	sub LEAF_PTR_0, T_0
	sbc LEAF_PTR_1, T_1
	movw r30, r24
	adiw r24, 6
	movw FREE_PTR_0, r24
	clr PART_SIZE_1
	clr PART_SIZE_0
	inc PART_SIZE_0
5: /* major loop (loading new T) */
	push r30
	push r31
	clr T_1
	rcall read_byte
	mov T_0, r24
	cpi r24, 0xFF
	brne 501f
	rcall read_byte
	add T_0, r24
	adc T_1, r1
501:
	pop r31
	pop r30
	clr r26
	clr r27
	clr NEXT_PART_SIZE_0
	clr NEXT_PART_SIZE_1
	ldi r20, 6
	movw r24, PART_SIZE_0
51: /* minor loop */
	ldi r22, V_NODE_LOW
	st Z+, r22
	ldi r22, V_NODE_HIGH
	st Z+, r22
	cp  r26, T_0
	cpc r27, T_1
	brsh 53f
	st Z+, LEAF_PTR_0
	st Z+, LEAF_PTR_1
	adiw LEAF_PTR_0, 2
	adiw r26, 1
	rjmp 531f
53:
	st Z+, FREE_PTR_0
	st Z+, FREE_PTR_1
	add FREE_PTR_0, r20
	adc FREE_PTR_1, r1
	adiw r26, 1
	inc NEXT_PART_SIZE_0
	brne 531f
	inc NEXT_PART_SIZE_1
531:
	cp  r26, T_0
	cpc r27, T_1
	brsh 63f
	st Z+, LEAF_PTR_0
	st Z+, LEAF_PTR_1
	adiw LEAF_PTR_0, 2
	adiw r26, 1
	rjmp 631f
63:
	st Z+, FREE_PTR_0
	st Z+, FREE_PTR_1
	add FREE_PTR_0, r20
	adc FREE_PTR_1, r1
	adiw r26, 1
	inc NEXT_PART_SIZE_0
	brne 631f
	inc NEXT_PART_SIZE_1
631:
	sbiw r24, 1
	brne 51b /* closing minor loop */
	movw PART_SIZE_0, NEXT_PART_SIZE_0
	push r30
	push r31
	movw r22, T_0
	lsl r22
	rol r23
	sub LEAF_PTR_0, r22
	sbc LEAF_PTR_1, r23
	movw r24, T_0
	adiw r24, 0
	breq 71f /* T==0 */
65:
	rcall read_byte
	st Y+, r24
	st Y+, r1
	dec T_0
	brne 65b
	tst T_1
	breq 71f
	dec T_1
	dec T_0
	rjmp 65b
71:
	pop r31
	pop r30
;	cp  LEAF_PTR_0, LEAF_END_PTR_0
;	cpc LEAF_PTR_1, LEAF_END_PTR_1
	movw r24, NEXT_PART_SIZE_0
	adiw r24, 0
	breq 72f
	rjmp 5b /* closing major loop*/
72:
	movw r30, LEAF_PTR_0
	ldi r22, V_EOF_HIGH
	st -Z, r22
	ldi r22, V_EOF_LOW
	st -Z, r22
90:
	pop_range 28, 29
	pop_range 2, 17
	ret

read_byte:
	push_range 28, 29
	movw r28, CTX_0
	ldd r30, Y+4
	ldd r31, Y+5
	ldd r24, Y+6
	ldd r25, Y+7
	movw r22, r24
	adiw r24, 1
	std Y+6, r24
	std Y+7, r25
	movw r24, r22
	icall
	pop_range 28, 29
	sbrs r25, 0
	ret
1:
	ldi r24, 1
	pop r30 /* remove old return addr from stack */
	pop r30
	rjmp 90b


/******************************************************************************/
/* void huffman_dec_set_addr(huffman_dec_ctx_t* ctx, uint16_t addr);
 *  param ctx:  r24:r25
 *  param addr: r22:r23
 */
.global huffman_dec_set_addr
huffman_dec_set_addr:
	movw r30, r24
	std Z+6, r22
	std Z+7, r23
	ret


