/* bmw_small_speed_asm_f0.S */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010 Daniel Otte (daniel.otte@rub.de)

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

	.syntax unified

.macro S32_0 out:req in:req
	lsr \out, \in, #1
	eor \out, \out, \in, LSL #3
	eor \out, \out, \in, ROR #28
	eor \out, \out, \in, ROR #13
.endm

.macro S32_1 out:req in:req
	lsr \out, \in, #1
	eor \out, \out, \in, LSL #2
	eor \out, \out, \in, ROR #24
	eor \out, \out, \in, ROR #9
.endm

.macro S32_2 out:req in:req
	lsr \out, \in, #2
	eor \out, \out, \in, LSL #1
	eor \out, \out, \in, ROR #20
	eor \out, \out, \in, ROR #7
.endm

.macro S32_3 out:req in:req
	lsr \out, \in, #2
	eor \out, \out, \in, LSL #2
	eor \out, \out, \in, ROR #17
	eor \out, \out, \in, ROR #3
.endm

.macro S32_4 in:req
	eor \in, \in, \in, LSR #1
.endm

.macro S32_5 in:req
	eor \in, \in, \in, LSR #2
.endm

#define T00_ADDR [SP, #(15-3)*4]
#define T01_ADDR [SP, #(15-2)*4]
#define T02_ADDR [SP, #(15-1)*4]
#define T03_ADDR [SP, #(15-0)*4]
#define T04_ADDR [SP, #(15-7)*4]
#define T05_ADDR [SP, #(15-6)*4]
#define T06_ADDR [SP, #(15-5)*4]
#define T07_ADDR [SP, #(15-4)*4]
#define T08_ADDR [SP, #(15-11)*4]
#define T09_ADDR [SP, #(15-10)*4]
#define T10_ADDR [SP, #(15-9)*4]
#define T11_ADDR [SP, #(15-8)*4]
#define T12_ADDR [SP, #(15-15)*4]
#define T13_ADDR [SP, #(15-14)*4]
#define T14_ADDR [SP, #(15-13)*4]
#define T15_ADDR [SP, #(15-12)*4]

	.text
	.align 2
	.thumb
	.thumb_func
	.type	bmw_small_f0, %function
	.global bmw_small_f0
bmw_small_f0:
	push {r4, r5, r6, r7, r8, r9, r10, r11, r12, r14}
	/* memxor(<STACK>, h, m, 16) */
	ldmia r1!, {r4, r5, r6, r7}
	ldmia r2!, {r8, r9, r10, r11}
	eor r4, r8
	eor r5, r9
	eor r6, r10
	eor r7, r11
	push {r4, r5, r6, r7}
	ldmia r1!, {r4, r5, r6, r7}
	ldmia r2!, {r8, r9, r10, r11}
	eor r4, r8
	eor r5, r9
	eor r6, r10
	eor r7, r11
	push {r4, r5, r6, r7}
	ldmia r1!, {r4, r5, r6, r7}
	ldmia r2!, {r8, r9, r10, r11}
	eor r4, r8
	eor r5, r9
	eor r6, r10
	eor r7, r11
	push {r4, r5, r6, r7}
	ldmia r1!, {r4, r5, r6, r7}
	ldmia r2!, {r8, r9, r10, r11}
	eor r4, r8
	eor r5, r9
	eor r6, r10
	eor r7, r11
	push {r4, r5, r6, r7}
    sub r1, #16*4

#include "f0_small_autogen.i"

	add SP, #16*4
	pop {r4, r5, r6, r7, r8, r9, r10, r11, r12, PC}

