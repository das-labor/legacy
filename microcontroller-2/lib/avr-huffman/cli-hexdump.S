/* cli-hexdump.S */
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

/******************************************************************************/
/* cli_hexdump 
 *   param data:   r24:r25
 *   param length: r22:r23
 */
.global cli_hexdump
cli_hexdump:
	push r28
	push r29
	push r16
	push r17
	push r2
	movw r28, r24
	movw r16, r22
1:
	tst r16
	brne 2f
	tst r17
	breq hexdump_exit
2:
	ld r2, Y+
	mov r24, r2
	swap r24
	ldi r25, 0x0f
	and r24, r25
	and r2, r25
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r24
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r2
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	subi r16, 1
	sbci r17, 0
	rjmp 1b
hexdump_exit:	
	pop r2
	pop r17
	pop r16
	pop r29
	pop r28
	ret
	
/******************************************************************************/
/* cli_hexdump 
 *   param data:   r24:r25
 *   param length: r22:r23
 */
.global cli_hexdump_rev
cli_hexdump_rev:
	push r28
	push r29
	push r16
	push r17
	push r2
	movw r28, r24
	movw r16, r22
	add r28, r22
	adc r29, r23
1:
	tst r16
	brne 2f
	tst r17
	breq hexdump_exit
2:
	ld r2, -Y
	mov r24, r2
	swap r24
	ldi r25, 0x0f
	and r24, r25
	and r2, r25
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r24
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r2
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	
	subi r16, 1
	sbci r17, 0
	rjmp 1b
	
/******************************************************************************/
/* cli_hexdump 
 *   param data:   r24:r25
 *   param length: r22:r23
 */
.global cli_hexdump2
cli_hexdump2:
	push r28
	push r29
	push r16
	push r17
	push r2
	movw r28, r24
	movw r16, r22
1:
	tst r16
	brne 2f
	tst r17
	breq hexdump_exit
2:
	ld r2, Y+
	mov r24, r2
	swap r24
	ldi r25, 0x0f
	and r24, r25
	and r2, r25
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r24
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	ldi r30, lo8(hexdigit_tab_P)
	ldi r31, hi8(hexdigit_tab_P)
	add r30, r2
	adc r31, r1
	clr r25
	lpm r24, Z
	rcall cli_putc
	clr r25
	ldi r24,' '
	rcall cli_putc
	
	subi r16, 1
	sbci r17, 0
	rjmp 1b
	
