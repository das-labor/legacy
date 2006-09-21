#define MAX_BRITH 64
#define PORT_DATA 0x15
#define PORT_ADRESS_L 0x1B	// PORTA
#define PORT_ADRESS_H 0x18	// PORTB
; config.h einbinden..


#include <avr/io.h>

#define         Xl              R26
#define         Xh              R27
#define         Yl              R28
#define         Yh              R29
#define         Zl              R30
#define         Zh              R31

cnt1   = 16
cnt2   = 25
value1 = 18
value2 = 24
tmp    = 17
mask   = 21 


.global writeRam
.func writeRam
writeRam:
        push cnt1
		push cnt2
		push value1
		push value2
		push tmp
		push mask
		push Xl
		push Xh
		push Zl
		push Zh
; SORT		
		;bitmask initialisieren
		ldi mask, 0xfe
		ldi Xl, lo8(masks) 
		ldi Xh, hi8(masks) 
		st  X+, mask
		sec
		rol mask
		st  X+,  mask
		rol mask
		st  X+,  mask
		rol mask
		st  X+,  mask
		rol mask
		st  X+,  mask
		rol mask
		st  X+,  mask
		rol mask
		st  X+,  mask
		rol mask
		st  X,   mask
		
		;sortieren mit Bubblesort
		ldi cnt1, 6
sort_for_a:			
		ldi Xl, lo8(values) 
		ldi Xh, hi8(values)
		ldi Zl, lo8(masks) 
		ldi Zh, hi8(masks)	
		clr cnt2
sort_for_i:
		ld value1, X+
		ld value2, X
		ld tmp, Z+
		cp value2, value1
		brsh sort_for_i2
		; tausche die Werte
		st -X,  value2
		st  X+, value2
		st  X,  value1
		ld  value1, Z
		st -Z,  value1
		st  Z+, value1
		st  Z,  tmp
sort_for_i2:			
		inc  cnt2
		cp   cnt1, cnt2
		brpl sort_for_i
		dec  cnt1
		brpl sort_for_a
; Writedata for one Latch to RAM		
		ldi Xl, lo8(values) 
		ldi Xh, hi8(values)
		ldi Zl, lo8(masks) 
		ldi Zh, hi8(masks)
		in value2, PORT_ADRESS_L
		in cnt2, PORT_ADRESS_H
		ld  value1, X+ 
		ldi cnt1, 0xff
		ldi mask, 0xff
		out PORT_DATA, mask
		
writeRamLoop:
		inc cnt1
		cpi cnt1, MAX_BRITH
		brsh end
		out PORT_ADRESS_H, cnt2
		out PORT_ADRESS_L, value2
		adiw value2, 1
		
writeRamLoop1:
 		cp  cnt1, value1
		brcs writeRamLoop
		ld tmp, Z+ 
		and mask, tmp 
		out PORT_DATA, mask
		ld value1, X+
		rjmp writeRamLoop1
end:		
	    pop Zh
		pop Zl
		pop Xh
		pop Xl
		pop mask
		pop tmp
		pop value2
		pop value1
		pop cnt2
		pop cnt1
		ret
.endfunc
