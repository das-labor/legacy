/* bmw_small-asm.S */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
/*
 * \file    bmw_small-asm.S
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2010-05-23
 * \license GPLv3 or later
 *
 */
.syntax unified
.text
.thumb
.align 2
.thumb_func
 /*
#define S32_1(x) ( (SHR32((x),   1)) ^ \
	               (SHL32((x),   2)) ^ \
	               (ROTL32((x),  8)) ^ \
	               (ROTR32((x),  9)) )

#define S32_2(x) ( (SHR32((x),   2)) ^ \
	               (SHL32((x),   1)) ^ \
	               (ROTL32((x), 12)) ^ \
	               (ROTR32((x),  7)) )

#define S32_3(x) ( (SHR32((x),   2)) ^ \
	               (SHL32((x),   2)) ^ \
	               (ROTL32((x), 15)) ^ \
	               (ROTR32((x),  3)) )

#define S32_4(x) ( (SHR32((x),   1)) ^ (x))

#define S32_5(x) ( (SHR32((x),   2)) ^ (x))

*/

.global bmw_s32_0
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_0, %function
bmw_s32_0:
 	mov r1, r0
 	lsrs r0, r0, #1
 	eor r0, r0, r1, lsl #3
 	eor r0, r0, r1, ror #28
 	eor r0, r0, r1, ror #13
	bx lr

.global bmw_s32_1
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_1, %function
bmw_s32_1:
 	mov r1, r0
 	lsrs r0, r0, #1
 	eor r0, r0, r1, lsl #2
 	eor r0, r0, r1, ror #24
 	eor r0, r0, r1, ror #9
	bx lr

.global bmw_s32_2
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_2, %function
bmw_s32_2:
 	mov r1, r0
 	lsrs r0, r0, #2
 	eor r0, r0, r1, lsl #1
 	eor r0, r0, r1, ror #20
 	eor r0, r0, r1, ror #7
	bx lr

.global bmw_s32_3
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_3, %function
bmw_s32_3:
 	mov r1, r0
 	lsrs r0, r0, #2
 	eor r0, r0, r1, lsl #2
 	eor r0, r0, r1, ror #17
 	eor r0, r0, r1, ror #3
	bx lr

.global bmw_s32_4
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_4, %function
bmw_s32_4:
	eor r0, r0, r0, lsr #1
	bx lr

.global bmw_s32_5
.text
.thumb
.align 2
.thumb_func
.type bmw_s32_5, %function
bmw_s32_5:
	eor r0, r0, r0, lsr #2
	bx lr



.global bmw_small_f0
.text
.thumb
.align 2
.thumb_func
.type bmw_small_f0, %function
/*
 * param q:  r0
 * param h:  r1
 * param m:  r2
 */
bmw_small_f0:
	push {r4-r11, r14}
	sub sp, sp, #64
	mov r3, sp
	adds r3, r3, #4
10:
	ldmia r1!, {r4,r6,r8,r10}
	ldmia r2!, {r5,r7,r9,r11}
	eors r4, r5
	eors r6, r7
	eors r8, r9
	eors r10, r11
	stmia r3!, {r4,r6,r8,r10}

	ldmia r1!, {r4,r6,r8,r10}
	ldmia r2!, {r5,r7,r9,r11}
	eors r4, r5
	eors r6, r7
	eors r8, r9
	eors r10, r11
	stmia r3!, {r4,r6,r8,r10}

	ldmia r1!, {r4,r6,r8,r10}
	ldmia r2!, {r5,r7,r9,r11}
	eors r4, r5
	eors r6, r7
	eors r8, r9
	eors r10, r11
	stmia r3!, {r4,r6,r8,r10}

	ldmia r1!, {r4,r6,r8,r10}
	ldmia r2!, {r5,r7,r9,r11}
	eors r4, r5
	eors r6, r7
	eors r8, r9
	eors r10, r11
	stmia r3!, {r4,r6,r8,r10}
/* --- */
	subs r1, r1, #64
	subs r3, r3, #64
/*
	q[ 0] = (+ h[ 5] - h[ 7] + h[10] + h[13] + h[14]);
	q[ 3] = (+ h[ 8] - h[10] + h[13] + h[ 0] - h[ 1]);
	q[ 6] = (- h[11] + h[13] - h[ 0] - h[ 3] + h[ 4]);
	q[ 9] = (+ h[14] + h[ 0] - h[ 3] + h[ 6] - h[ 7]);
	q[12] = (+ h[ 1] + h[ 3] - h[ 6] - h[ 9] + h[10]);
	q[15] = (- h[ 4] - h[ 6] - h[ 9] + h[12] + h[13]);
	q[ 2] = (+ h[ 7] + h[ 9] - h[12] + h[15] + h[ 0]);
	q[ 5] = (+ h[10] - h[12] + h[15] - h[ 2] + h[ 3]);
	q[ 8] = (+ h[13] - h[15] + h[ 2] - h[ 5] - h[ 6]);
	q[11] = (- h[ 0] - h[ 2] - h[ 5] + h[ 8] + h[ 9]);
	q[14] = (+ h[ 3] - h[ 5] + h[ 8] - h[11] - h[12]);
	q[ 1] = (+ h[ 6] - h[ 8] + h[11] + h[14] - h[15]);
	q[ 4] = (+ h[ 9] - h[11] - h[14] + h[ 1] + h[ 2]);
	q[ 7] = (- h[12] - h[14] + h[ 1] - h[ 4] - h[ 5]);
	q[10] = (+ h[15] - h[ 1] - h[ 4] - h[ 7] + h[ 8]);
	q[13] = (+ h[ 2] + h[ 4] + h[ 7] + h[10] + h[11]);
*/
	ldr r4,  [r3,  #(5*4)]
	ldr r5,  [r3,  #(7*4)]
	ldr r6,  [r3, #(10*4)]
	ldr r7,  [r3, #(13*4)]
	ldr r8,  [r3, #(14*4)]
	ldr r9,  [r3, #(8*4)]
	ldr r10, [r3, #(11*4)]
	subs r2, r4, r5
	adds r2, r2, r6
	adds r2, r2, r7
	adds r2, r2, r8
	str r2, [r0, #0]
	ldr r4, [r3, #0]
	ldr r5, [r3, #1]
	subs r2, r9, r6
	adds r2, r2, r7
	adds r2, r2, r4
	subs r2, r2, r5
	add sp, sp, #64
	pop {r4-r11, pc}
