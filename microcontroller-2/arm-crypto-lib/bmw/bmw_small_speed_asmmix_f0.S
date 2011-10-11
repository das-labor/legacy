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


	.text
	.align 2
	.thumb
	.thumb_func
	.type	bmw_small_f0, %function
	.global bmw_small_f0
bmw_small_f0:
	push {r4, r5, r6, r7, r8, r9, r10, r11, r12, r14}

#include "f0_small_autogen_mix.i"

	pop {r4, r5, r6, r7, r8, r9, r10, r11, r12, PC}

