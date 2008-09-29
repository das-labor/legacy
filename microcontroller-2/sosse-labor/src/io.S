; Simple Operating system for Smart cards
; Copyright (C) 2002  Matthias Bruestle <m@mbsks.franken.de>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

;========================================================================
; T=0 character I/O routines for 9600bps at 3.58 MHz
;========================================================================

#include <config.h>

	.arch	ARCH
IO_PIN=6
PINB=0x16
DDRB=0x17
PORTB=0x18
	.text
	.global recbytet0, sendbytet0, delay1etu
	.comm	direction,1,1
;========================================================================
; Wait loops.
; 70 cycles delay for intrabit delay
intrabitdelay:
	ldi		r22, 21			; 1
; Wait t17*3+7 cycles
delay:
	dec		r22				; 1
	brne	delay			; 1/2
	ret						; 4

delay1etu:
	ldi		r22, 121		; 1
	rjmp	delay			; 2

;========================================================================
; Receive a byte with T=0 error correction.
; result r25(=0):r24
recbytet0:
	push	r23				; 2 - getbit
	push	r22				; 2 - delay
	push	r21				; 2 - loop counter
	push	r20				; 2 - parity counter

	; Set direction bit, to indicate, that we received a byte
	ldi		r22, 1
	sts		direction,r22

restartrecbyte:
	; Setup IN direction
	cbi		DDRB, 6			; 2
	cbi		PORTB, 6		; 2

; Wait for start bit.
waitforstart:
	; Bit begins here.
	sbic	PINB, IO_PIN	; 1/2!
	rjmp	waitforstart	; 2/0
	sbic	PINB, IO_PIN	; 1/2! - Recheck for spike
	rjmp	waitforstart	; 2/0
	; Sample start bit
	clr		r24				; 1
	clr		r25				; 1 - Clear zero byte for ADC
	ldi		r22, 31			; 1
	rcall	delay			; 100
	rcall	getbit			; 3 (16bit PC)
	;brcs	waitforstart	; 1/2 - Go on, even if not valid a start bit?
	nop						; 1 - For brcs
; Receive now 9 bits
	ldi		r21, 0x09		; 1
	clr		r20				; 1
	ldi		r22, 66			; 1
	nop						; 1
	nop						; 1
rnextbit:
	rcall	delay			; 205/202
	rcall	getbit			; 3
	add		r20, r23		; 1
	clc						; 1
	sbrc	r23, 0			; 1/2
	sec						; 1/0
	ror		r24				; 1
	ldi		r22, 65			; 1
	dec		r21				; 1
	brne	rnextbit		; 1/2
; Check parity
	rol		r24				; 1 - We've rotated one to much
	sbrc	r20, 0			; 1/2
	rjmp	regetbyte		; 2/0

	; Wait halve etu
	ldi		r22, 76			; 1
	rcall	delay			; 235 - Precise enough

	clr		r25
	pop		r20				; 2 - parity counter
	pop		r21				; 2 - loop counter
	pop		r22				; 2 - delay
	pop		r23				; 2 - getbit
	ret

regetbyte:
	; Wait halve etu
	ldi		r22, 76			; 1
	rcall	delay			; 235 - Precise enough
	; Set OUT direction
	sbi		DDRB, 6			; 2
	; Signal low
	cbi		PORTB, 6		; 2
	ldi		r22, 182		; 2
	rcall	delay			; 553 - about 1.5 etu
	rjmp	restartrecbyte	; 2

;========================================================================
; Read a bit.
; Uses r23, r25
; Returns bit in r23.0.
; 5 cycles before first bit
; 8 cycles after last bit.
getbit:
	clr		r23				; 1
	clc						; 1
	; At start + 112 cycles
	sbic	PINB, IO_PIN	; 1/2
	sec						; 1/0
	adc		r23, r25		; 1
	rcall	intrabitdelay	; 70
	clc						; 1
	; At start + 186 cycles
	sbic	PINB, IO_PIN	; 1/2
	sec						; 1/0
	adc		r23, r25		; 1
	rcall	intrabitdelay	; 70
	clc						; 1
	; At start + 260 cycles
	sbic	PINB, IO_PIN	; 1/2
	sec						; 1/0
	adc		r23, r25		; 1
	; Get second bit of the sum.
	lsr		r23				; 1
	ret						; 4	(with 16bit PC)
;========================================================================
; Send a byte with T=0 error correction.
; byte r25(=0):r24
sendbytet0:
	push	r22				; 2 - delay
	push	r23				; 2 - parity counter

	lds		r22,direction
	tst		r22
	breq	resendbytet0
	rcall	delay1etu		;
	rcall	delay1etu		;
	; Clear direction bit, to indicate, that we sent a byte
	ldi		r22, 0
	sts		direction,r22

resendbytet0:
	; Set OUT direction
	sbi		PORTB, 6		; 2
	sbi		DDRB, 6			; 2
	; Send start bit
	cbi		PORTB, IO_PIN	; 2
	ldi		r22, 119		; 1
	rcall	delay			; 364
	; Send now 8 bits
	ldi		r25, 0x08		; 1
	clr		r23				; 1
snextbit:
	ror		r24				; 1
	brcs	sendbit1		; 1/2
	cbi		PORTB, IO_PIN	; 2
	rjmp	bitset			; 2
sendbit1:
	sbi		PORTB, IO_PIN	; 2
	inc		r23				; 1
bitset:
	ldi		r22, 118		; 1
	rcall	delay			; 361
	nop						; 1
	dec		r25				; 1
	brne	snextbit		; 1/2
	; Send parity
	sbrc	r23, 0			; 1/2
	rjmp	sendparity1		; 2
	nop						; 1
	nop						; 1
	cbi		PORTB, IO_PIN	; 2
	rjmp	delayparity		; 2
sendparity1:
	nop						; 1
	sbi		PORTB, IO_PIN	; 2
	nop						; 1
	nop						; 1
delayparity:
	ldi		r22, 112		; 1
	rcall	delay			; 343
	; Stop bit
	sbi		PORTB, IO_PIN	; 2
	ldi		r22, 119		; 1
	rcall	delay			; 364
	; Set IN direction
	cbi		DDRB, 6			; 2
	cbi		PORTB, 6		; 2
	; Look for error signal
	clc						; 1
	sbic	PINB, IO_PIN	; 1/2
	sec						; 1/0
	brcs	retsendbytet0	; 1/2
	; Resend byte
	; Bring byte to starting position
	ror		r24				; 1
	; Wait for end of error signal
waitforendoferror:
	sbic	PINB, IO_PIN	; 1/2!
	rjmp	waitforendoferror	; 2/0
	; Wait then a halve etu
	ldi		r22, 58			; 1
	rcall	delay			; 181
	rjmp	resendbytet0	; 2
	; return
retsendbytet0:
	ldi		r22, 116		; 1
	rcall	delay			; 355
	pop		r23				; 2 - parity counter
	pop		r22				; 2 - delay
	ret						; 4
;========================================================================

