#include <config.h>

#if CONF_WITH_I2CEEPROM==1
	.arch ARCH
DDRB=0x17
EE_SCL=1
EE_SDA=0
PINB=0x16
PORTB=0x18
	.text
	.global xeread, xewrt
XEAddr:
	rcall  XEStrt
	clc
	ldi	   r19,0xA0
	rcall  XEEOut
	rcall  XE0Bit
	mov	   r19,r31
	rcall  XEEOut
	rcall  XE0Bit
	mov	   r19,r30
	rcall  XEEOut
	rcall  XE0Bit
	ret

; address r25:r24 
; result r25(=0):r24
xeread:
	push	r31
	push	r30
	push	r19
	push	r18
	push	r16
	push	r1
	push	r0
	mov	r31,r25
	mov	r30,r24
; Start
	rcall	xereadlocal
; Done
	clr	r25
	mov	r24,r0
	pop	r0
	pop	r1
	pop	r16
	pop	r18
	pop	r19
	pop	r30
	pop	r31
	ret

; address r31:r30 
; result r0 = XE(Z+)
xereadlocal:
	rcall	XEAddr
	rcall	XEStrt
	clc
	ldi		r19,0xA1
	rcall	XEEOut
	rcall	XE0Bit
	rcall	XEEIn
	rcall	XE1Bit
	rcall	XEStop
	ret

; address r25:r24 
; byte r23(=0):r22
xewrt:
	push	r31
	push	r30
	push	r19
	push	r18
	push	r16
	push	r1
	push	r0
	mov	r31,r25
	mov	r30,r24
; Start
; address r31:r30 
; result XE(Z+) = r22
	rcall	xereadlocal
	cp	r0,r22
	breq	dontwrite
	rcall	XEAddr
	mov	r19,r22
	rcall	XEEOut
	rcall	XE0Bit
	rcall	XEStop
	rcall	XEDly
dontwrite: 
; Done
	pop	r0
	pop	r1
	pop	r16
	pop	r18
	pop	r19
	pop	r30
	pop	r31
	ret

XEDly:
	ldi	  r25,0x20
	mov	  r1,r25
avr3B9:
	ldi	  r25,0xFF
avr3BA: 
	dec	  r25
	brne  avr3BA
	dec	  r1
	brne  avr3B9
	ret

XEStrt: 
	rcall	ClrPB0
	rcall	SetPB2
	rcall	SetPB0
	rcall	ClrPB2
	rcall	ClrPB0
	ret

XEStop: 
	rcall	ClrPB0
	rcall	ClrPB2
	rcall	SetPB0
	rcall	SetPB2
	rcall	ClrPB0
	ret

XEEIn:
	clr r0
	ldi r16,0x08
	rcall	SetPB2
	rcall	PB2In
avr3CF: 
	rcall SetPB0
	sbic  PINB,EE_SDA
	rjmp  avr3D5
	clc
	rol r0
	rjmp	avr3D7
avr3D5: sec
	rol r0
avr3D7: rcall	ClrPB0
	dec r16
	brne	avr3CF
	rcall	PB2Out
	ret

XEEOut: 
	ldi	  r16,0x08
	mov	  r0,r19
avr3DE: 
	clc
	rol	  r0
	brlo  avr3E4
	rcall ClrPB2
	rcall ClkPls
	rjmp  avr3E6
avr3E4: 
	rcall SetPB2
	rcall ClkPls
avr3E6: 
	dec	  r16
	brne  avr3DE
	ret

XE1Bit: rcall	SetPB2
	rcall	ClkPls
	ret

XE0Bit: 
	rcall	ClrPB2
	rcall	ClkPls
	ret

ClkPls: 
	rcall	SetPB0
	rcall	ClrPB0
	ret

SetPB2:
	sbi	 PORTB,EE_SDA
	rjmp PBExit

ClrPB2:
	cbi	 PORTB,EE_SDA
	rjmp PBExit

SetPB0:
	sbi	 PORTB,EE_SCL
	rjmp PBExit

ClrPB0: 
	cbi PORTB,EE_SCL
	rjmp	PBExit

PB2Out: 
	sbi	 DDRB,EE_SDA
	rjmp PBExit

PB2In:
	cbi	 DDRB,EE_SDA
	rjmp PBExit

PBExit:
	nop
	nop
	nop
	ret
#endif /* CONF_WITH_I2CEEPROM==1 */

