#include <avr/io.h>

#define         Xl              R26
#define         Xh              R27
#define         Yl              R28
#define         Yh              R29
#define         Zl              R30
#define         Zh              R31

work1  = 16
work2  = 17
value1 = 18
value2 = 19
tmp    = 20



.global sort
.func sort
sort:
        push work1
		push work2
		push value1
		push value2
		push tmp
		push Xl
		push Xh
		push Zl
		push Zh
		
		;bitmask initialisieren
		ldi work1, 0xfe
		ldi Xl, lo8(masks) 
		ldi Xh, hi8(masks) 
		st  X+, work1
		sec
		rol work1
		st  X+,  work1
		rol work1
		st  X+,  work1
		rol work1
		st  X+,  work1
		rol work1
		st  X+,  work1
		rol work1
		st  X+,  work1
		rol work1
		st  X+,  work1
		rol work1
		st  X,   work1
		
		;sortieren
sort_for_a:			
		ldi Xl, lo8(masks) 
		ldi Xh, hi8(masks)
		ldi Zl, lo8(values) 
		ldi Zh, hi8(values)
		ldi work1, 7	
		clr work2
sort_for_i:
		ld value1, X+
		ld value2, X
		ld tmp, Z+
		cp value2, value1
		brsh sort_for_i2
		; tausche die Werte
		st  -X, value2
		st  X+, value2
		st  X,  value1
		ld  value1, Z
		st -Z,  value1
		st Z+,  value1
		st  Z,   tmp
sort_for_i2:			
		inc work2
		cp work1, work2
		brlo sort_for_i
		dec work1
		brvc sort_for_a
		
	    pop Zh
		pop Zl
		pop Xh
		pop Xl
		pop tmp
		pop value2
		pop value1
		pop work2
		pop work1
		ret
.endfunc
