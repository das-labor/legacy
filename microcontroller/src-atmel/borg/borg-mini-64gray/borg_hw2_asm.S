#include <avr/io.h>
#include "config.h"

#define temp	R26
#define temp1	R27
#define temp2	R28


// An diesen Pins ist das Schieberegister
// fÃ¼r die Spalten angeschlossen.
#define COLPORT  PORTC
#define COLDDR   DDRC
#define BIT_DAT	4 //Daten-Eingang
#define BIT_CLK 5 //Takt-Eingang

#define xmappot 6

// An diesem Port sind die Zeilentreiber (8) angeschlossen.
// Ein Null-Pegel schaltet den jeweiligen Transistor an.
#define ROWPORT PORTD
#define ROWDDR   DDRD

#define ROWPORT2 PORTC
#define ROWDDR2 DDRC

// globals...
 .comm	rowmask, 1
 .comm  brightness, 1
 .comm  brightcount, 1
 .comm  row, 1

// Interrupt auf das Timer0-Überlauf Ereigniss
.global TIMER0_OVF_vect
TIMER0_OVF_vect:
	sei
	; Alles mögliche sichern
	push	R2
	in		R2, _SFR_IO_ADDR(SREG)
	push	temp
	push	temp1
	push	temp2
	push	ZL
	push	ZH
	
	; brightness nach temp2
	lds		temp2, brightness
	; temp1 = (rowmask >>= 1) | 0x80;
	lds		temp1, rowmask
	sec		
	ror		temp1
	
	; temp = row
	; row++; if(row >= NUM_ROWS) row = 0;
	lds		temp, row
	subi 	temp, -NUM_COLS
	cpi		temp, NUM_ROWS*NUM_COLS
	brlo	SO_W1
	
		ldi		temp, 0
		ldi		temp1, 0x7f
		
		; nächsten brightnesswert holen
		; if(++bright>=(1<<xmappot)) bright = 1;
		lds		temp2, brightcount
		inc		temp2
		cpi		temp2, (1<<xmappot)
		brlo	SO_W2
		ldi		temp2, 1
		
	SO_W2:	
		sts		brightcount, temp2
		; temp2 = xmap[brightcount]
		ldi		ZL, lo8(xmap)
		ldi		ZH, hi8(xmap)	
		add		ZL, temp2
		ldi		temp2, 0
		adc		ZH, temp2
		ld		temp2, Z
		sts		brightness, temp2
	
SO_W1:
	sts		row, temp
	sts		rowmask, temp1
	
	; row ist in temp, brightness ist in temp2, rowmask in temp1	
	; **********************************************************
	
	; ### TCNT0 = 0x100-(40 + (brightness>>0));
	ldi		ZL, 220
	mov		ZH, temp2
	;lsl		ZH
	sub		ZL, ZH
	out		_SFR_IO_ADDR(TCNT0), ZL
	
	; Z = &pixmap[row][0]
	ldi		ZL, lo8(pixmap)
	ldi		ZH, hi8(pixmap)	
	add		ZL, temp
	ldi		temp, 0
	adc		ZH, temp
	
	; alle Zeilentreiber aus
	in		temp, _SFR_IO_ADDR(ROWPORT)
	ori		temp, 0xf3
	out		_SFR_IO_ADDR(ROWPORT), temp
	in		temp, _SFR_IO_ADDR(ROWPORT2)
	ori		temp, 0x0c
	out		_SFR_IO_ADDR(ROWPORT2), temp
	
	; Spalte 5 einschieben
	ldd		temp, Z	+ 5
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW1
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW1:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 6 einschieben
	ldd		temp, Z	+ 6
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW2
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW2:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 7 einschieben
	ldd		temp, Z	+ 7
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW3
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW3:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 8 einschieben
	ldd		temp, Z	+ 8
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW4
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW4:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 9 einschieben
	ldd		temp, Z	+ 9
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW5
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW5:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 0 einschieben
	ldd		temp, Z	+ 0
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW6
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW6:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 1 einschieben
	ldd		temp, Z	+ 1
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW7
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW7:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 2 einschieben
	ldd		temp, Z	+ 2
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW8
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW8:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 3 einschieben
	ldd		temp, Z	+ 3
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW9
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW9:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; Zeile 4 einschieben
	ldd		temp, Z	+ 4
	cbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	cp		temp, temp2
	brsh	SO_WW10
	sbi		_SFR_IO_ADDR(COLPORT), BIT_DAT
	
SO_WW10:
	sbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	nop
	cbi		_SFR_IO_ADDR(COLPORT), BIT_CLK
	
	; nächster Zeilentreiber an (rowmask in temp1)
	in		temp, _SFR_IO_ADDR(ROWPORT)
	mov		temp2, temp1
	ori		temp2, 0x0c
	and		temp, temp2
	out		_SFR_IO_ADDR(ROWPORT), temp
	in		temp, _SFR_IO_ADDR(ROWPORT2)
	mov		temp2, temp1
	ori		temp2, 0xf3
	and		temp, temp2
	out		_SFR_IO_ADDR(ROWPORT2), temp	
	
	
SO_RAUS:	

	; Alles mögliche zurücksichern
	pop		ZH
	pop		ZL
	pop		temp2
	pop		temp1
	pop		temp 
	out		_SFR_IO_ADDR(SREG), R2
	pop		R2
	
	reti	


