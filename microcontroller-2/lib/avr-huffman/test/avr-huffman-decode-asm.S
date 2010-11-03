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

#define AUTO_LPM_ZERO 0 /* this allows automatic load from flash if read-function pointer is zero */

/*
typedef struct {
	void* tree;
	uint8_t  rbuffer;
	uint8_t  rbuffer_index;
	uint16_t(*read_byte)(uint16_t addr);
	uint16_t addr;
} huffman_dec_ctx_t;
*/

/*
 * param ctx: r24:r25
 */
read_bit:
	adiw r26, 3		/* X points at rbuffer_index */
	ld r25, X		/* load rbuffer_index to r25 */
	cpi r25, 8
	brne 5f			/* there is data in the buffer */
	st X, r1
	push r26
	push r27
	adiw r26, 1
	rcall read_byte_core
	pop r27
	pop r26
	tst r25
	breq 2f
	/* we got EOF */
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
	clr r23
	rol r23
	inc r25
	st X, r25
	clr r25
	ret


/******************************************************************************/
/* uint16_t huffman_dec_byte(huffman_dec_ctx_t* ctx)
 * param ctx: r24:r25
 */
NODE_PTR_0 = 28
NODE_PTR_1 = 29
TREE_PTR_0 = 16
TREE_PTR_1 = 17
.global huffman_dec_byte
huffman_dec_byte:
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
	push TREE_PTR_0
	push TREE_PTR_1
	movw TREE_PTR_0, r30
6:
	movw r26, TREE_PTR_0
	rcall read_bit
	tst r25
	brne 9f /* eof occured */
	ldd r25, Y+0
load_left:
	ldd r24, Y+1
	sbrs r23, 0
	rjmp post_load_dec
load_right:
	swap r25
	ldd r24, Y+2
post_load_dec:
	andi r25, 0x0f
	breq 8f /* value found */
	cpi r25, 0x0f
	brne link_found
	cpi r24, 0xff
	breq 9f /* found eof */
link_found:
	andi r25, 0x07
	add NODE_PTR_0, r24
	adc NODE_PTR_1, r25
	rjmp 6b
8:	pop TREE_PTR_1
	pop TREE_PTR_0
	pop r29
	pop r28
	ret
9:
	movw r30, TREE_PTR_0
	ld r24, Z+
	ld r25, Z+
	call free
;	movw r30, TREE_PTR_0 ; xxx
;	st Z+, r1       ; xxx
;	st Z+, r1       ; xxx
	ser r25
	ser r24
	rjmp 8b

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
 *
 *        +----*----+--------+--------+
 * node:  |  head   | left   | right  |
 *        +----*----+--------+--------+
 *          |    |      |        |
 *          |    V      V        |
 *          |  +-------------+   |
 *          |  |    left     |   |
 *          |  +-------------+   |
 *          |                    V
 *          |          +-------------+
 *          +--------->|    right    |
 *                     +-------------+
 *
 */
LEFT_USED        =  9
T_0              = 10
T_1              = 11
FREE_IDX_0       = 12
FREE_IDX_1       = 13
CTX_0            = 14
CTX_1            = 15
PART_SIZE_0      = 16
PART_SIZE_1      = 17
NODE_PTR_0       = 28
NODE_PTR_1       = 29
.global build_tree
build_tree:
	push_range 9, 17
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
	clr T_1
	rol T_1
	rcall read_byte
	mov r25, T_1 /* r24:r25 contains the amount of leafs */
	mov T_0, r24
	lsl r24        /* we need space for leafs and nodes */
	rol r25        /* (leaf_count-1)*3 */
	add r24, T_0
	adc r25, T_1
	clr r23
	ldi r22, 1
	call calloc
	movw r30, CTX_0
	st Z+, r24
	st Z+, r25
	adiw r24, 0
	brne 4f
	/* calloc returned NULL */
	ldi r24, 4
	rjmp 90f
4:	/* now we have allocated memory for the tree */
	movw NODE_PTR_0, r24
	clr PART_SIZE_1
	ldi PART_SIZE_0, 2
	movw FREE_IDX_0, PART_SIZE_0
	inc FREE_IDX_0
	clr LEFT_USED
5: /* major loop (loading new T) and generating new level */
	rcall read_byte
	clr T_1
	mov T_0, r24
	cpi r24, 0xFF
	brne 501f
	rcall read_byte
	add T_0, r24
	adc T_1, r1
	lsl PART_SIZE_0
	rol PART_SIZE_1
501: /* minor loop load&store leaf & crate links */
	movw r24, T_0
	adiw r24, 0
	breq create_links
502:
	rcall read_byte
	sbrc LEFT_USED, 0
	rjmp store_right_leaf
store_left_leaf:
	std Y+1, r24
	rjmp post_store_leaf
store_right_leaf:
	std Y+2, r24
	adiw NODE_PTR_0, 3
	movw r24, FREE_IDX_0
	sbiw r24, 3
	movw FREE_IDX_0, r24
post_store_leaf:
	inc LEFT_USED
	subi PART_SIZE_0, 1
	sbc PART_SIZE_1, r1
	movw r24, T_0
	sbiw r24, 1
	movw T_0, r24
	brne 502b
create_links:
	movw r24, PART_SIZE_0
	adiw r24, 0
	breq tree_build_end
	push PART_SIZE_0
	push PART_SIZE_1
510:
	movw r24, FREE_IDX_0
	ori r25, 0x08
	sbrc LEFT_USED, 0
	rjmp store_right_link
store_left_link:
	std Y+0, r25
	std Y+1, r24
	movw r24, FREE_IDX_0
	adiw r24, 3
	movw FREE_IDX_0, r24
	rjmp post_store_link
store_right_link:
	ldd r0, Y+0
	swap r25
	or r25, r0
	std Y+0, r25
	std Y+2, r24
	adiw NODE_PTR_0, 3
post_store_link:
	inc LEFT_USED
	movw r24, PART_SIZE_0
	sbiw r24, 1
	movw PART_SIZE_0, r24
	brne 510b
	/* end of minor loop */
	pop PART_SIZE_1
	pop PART_SIZE_0
	lsl PART_SIZE_0
	rol PART_SIZE_1
	rjmp 5b /* close of major loop */
tree_build_end:
	/* set last leaf to EOF */
	sbiw NODE_PTR_0, 3
	ldi r23, 0xF0
	std Y+0, r23
;	clr r24
90:
	pop_range 28, 29
	pop_range 9, 17
	ret

read_byte:
	movw r26, CTX_0
	adiw r26, 4
	rcall read_byte_core
	sbrs r25, 0
	ret
1:
	ldi r24, 1
	pop r30 /* remove old return addr from stack */
	pop r30
	rjmp 90b

read_byte_core:
	ld r30, X+
	ld r31, X+
	ld r24, X+
	ld r25, X+
	movw r22, r24
	adiw r24, 1
	st -X, r25
	st -X, r24
#if AUTO_LPM_ZERO
	adiw r30, 0
	brne 2f
	movw r30, r22
	lpm
	ret
#endif
2:
	movw r24, r22
	ijmp


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


