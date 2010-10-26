/* usbhw.S */
/*
    This file is part of the AVR-USBHost-Lib.
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

#include <avr/io.h>

#define USB_PORT (_SFR_IO_ADDR(PORTC))
#define USB_DDR  (_SFR_IO_ADDR(DDRC))

#define USB_MASK 0x03
#define USB_SOP  0x02

#define EXTRA_NOPS rjmp .+0

usb_tx_init:

stuffcounter = 23
tmpreg = 24
usb_mask_reg = 25

send_stuff_zero:
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	EXTRA_NOPS
	EXTRA_NOPS
	ldi stuffcounter, 7
	ret

.global usb_tx_block
usb_tx_block:
	movw r26, r24
	ldi stuffcounter, 7
	ldi usb_mask_reg, USB_MASK
	in tmpreg, USB_DDR
	or tmpreg, usb_mask_reg
	out USB_DDR, tmpreg
	in tmpreg, USB_PORT
	andi tmpreg, ~USB_MASK
	ori tmpreg, USB_SOP
	out USB_PORT, tmpreg
	EXTRA_NOPS
	EXTRA_NOPS
	EXTRA_NOPS
	ld r20, X+

tx_loop:
	mov r21, r20
bit0:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	ld r20, X+
	nop
10:
	dec stuffcounter
	brne bit1
	rcall send_stuff_zero
bit1:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit2
10:
	dec stuffcounter
	brne bit2
	rcall send_stuff_zero
bit2:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit3
10:
	dec stuffcounter
	brne bit3
	rcall send_stuff_zero
bit3:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit4
10:
	dec stuffcounter
	brne bit4
	rcall send_stuff_zero
bit4:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit5
10:
	dec stuffcounter
	brne bit5
	rcall send_stuff_zero

bit5:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit6
10:
	dec stuffcounter
	brne bit6
	rcall send_stuff_zero
bit6:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	EXTRA_NOPS
	rjmp bit7
10:
	dec stuffcounter
	brne bit7
	rcall send_stuff_zero
bit7:
	lsr r21
	brcs 10f
	eor tmpreg, usb_mask_reg
	out USB_PORT, tmpreg
	ldi stuffcounter, 7
	nop
	dec r22
	breq tx_block_exit
	rjmp tx_loop
10:
	dec stuffcounter
	brne 11f
	rcall send_stuff_zero
11:
	dec r22
	breq tx_block_exit
	rjmp tx_loop
tx_block_exit:
	ret













