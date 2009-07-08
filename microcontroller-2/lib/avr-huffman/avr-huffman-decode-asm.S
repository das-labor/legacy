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
	push r28
	push r29
	movw r30, r24
	ld  r28, Z
	ldd r29, Z+1
	tst r28
	brne 5f
	tst r29
	brne 5f
	push r30
	push r31
	rcall build_tree
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
	tst r24
	breq 7f /* take the left way */
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
;	movw r30, CTX_0
;	st Z+, r1
;	st Z+, r1
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
CTX_0       =  2
CTX_1       =  3
PREFIX_0    =  4
PREFIX_1    =  5
TMP_0       =  6
TMP_1       =  7
DEPTH_0     =  8
DEPTH_1     =  9
CDEPTH_0    = 10
CDEPTH_1    = 11
COUNT_0     = 12
COUNT_1     = 13
TREEINDEX_0 = 14
TREEINDEX_1 = 15
build_tree:
	push_range 2, 15
	push_range 28, 29
	stack_alloc 32
	adiw r30, 1
	movw PREFIX_0, r30
	/* set prefix to 0  */
	ldi r22, 32
1:
	st Z+, r1
	dec r22
	brne 1b
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
	clr TMP_0
	rol TMP_0
	rcall read_byte
	mov r25, TMP_0
	clr r23
	ldi r22, 6
	rcall calloc
	movw r30, CTX_0
	st Z+, r24
	st Z+, r25
	tst r24
	brne 4f
	tst r25
	brne 4f
	/* calloc returned NULL */
	ldi r24, 4
	rjmp 90f
4:	/* now we have allocated memory for the tree */
	clr DEPTH_0
	clr DEPTH_1
	clr CDEPTH_0
	clr CDEPTH_1
	clr COUNT_0
	clr COUNT_1
	clr TREEINDEX_0
	clr TREEINDEX_1
	movw r28, r24
	ldi r25, V_NODE_HIGH
	ldi r24, V_NODE_LOW
	std Y+0, r24
	std Y+1, r25
5:
	tst COUNT_0
	brne 60f
	tst COUNT_1
	brne 60f
	inc DEPTH_0
	brne 51f
	inc DEPTH_1
51:	
	rcall read_byte
	mov COUNT_0, r24
	cpi r24, 0xff
	brne 5b
	rcall read_byte
	add COUNT_0, r24
	adc COUNT_1, r1
	rjmp 5b	
60:
	rcall read_byte
	mov TMP_0, r24		/* store value in TMP_0 */
	movw r24, COUNT_0
	sbiw r24, 1
	movw COUNT_0, r24
61:	cp DEPTH_0, CDEPTH_0
	brne 62f
	cp DEPTH_1, CDEPTH_1
	breq 65f
62:
	/* prefix shift left */
	movw r30, PREFIX_0
	clc
	ldi r22, 32
621:
	ld r23, Z
	rol r23
	st Z+, r23
	dec r22
	brne 621b	
	inc CDEPTH_0
	brne 622f
	inc CDEPTH_1
622:	
	rjmp 61b
65:	
	/* insert node into tree */
	movw r30, CTX_0
	ld r28, Z+ /* set Y to point to tree */
	ld r29, Z
	/* append value to tree */
	movw r24, DEPTH_0
70:
	sbiw r24, 1
	movw r22, r24
	lsr r23 /* set r22 to (r24:r25)/8 and r21 to (r24:r25)%8 */	
	ror r22
	lsr r22
	lsr r22
	mov r21, r24
	andi r21, 0x07
	movw r30, PREFIX_0
	add r30, r22
	adc r31, r1
	ld TMP_1, Z
	ldi r23, 1

	tst r21
	breq 702f
701:
	lsl r23
	dec r21
	brne 701b
702:	
	and TMP_1, r23 /* decide if left or right */
	breq 710f
	adiw r28, 2
710:
	adiw r28, 2
	ldd r22, Y+0
	ldd r23, Y+1
	tst r22
	brne 790f
	tst r23
	brne 790f
	/* generate new node */
	movw r26, TREEINDEX_0 /* (r22:r23) = 6*TREEINDEX */
	adiw r26, 1
	movw TREEINDEX_0, r26
	lsl r26
	rol r27
	movw r22, r26
	lsl r26
	rol r27
	add r22, r26
	adc r23, r27
	movw r30, CTX_0 /* add ctx->tree to (r22:r23) */
	ld r20, Z+
	ld r21, Z+
	add r22, r20
	adc r23, r21
	std Y+0, r22	/* store new node in tree */
	std Y+1, r23
	movw r28, r22
	ldi r22, V_NODE_LOW
	ldi r23, V_NODE_HIGH
	std Y+0, r22
	std Y+1, r23
	rjmp 791f
790:
	movw r28, r22
791:	
	tst r24		/* check if count is 0 */
	brne 70b
	tst r25
	brne 70b		
80:
	std Y+0, TMP_0 /* store value in node, making it a leaf */
	std Y+1, r1	
	/* increment prefix */
	movw r30, PREFIX_0
801:
	ld r22, Z
	inc r22
	st Z+, r22
	breq 801b
	movw r22, DEPTH_0
	mov r24, r22
	andi r24, 0x07
	lsr r23
	ror r22
	lsr r22
	lsr r22
	movw r30, PREFIX_0
	add r30, r22
	adc r31, r1
	ld TMP_1, Z
	ldi r25, 1
	tst r24
	breq 804f
802:
	lsl r25
	dec r24
	brne 802b
804:
	and TMP_1, r25
	brne 890f
	rjmp 5b
890:	
	/* set last to EOF */
	ldi r22, V_EOF_LOW
	ldi r23, V_EOF_HIGH
	std Y+0, r22
	std Y+1, r23
	clr r25
	clr r24
90:
	stack_free 32	
	pop_range 28, 29
	pop_range 2, 15
	ret

read_byte:
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
	tst r25
	brne 1f
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
	

