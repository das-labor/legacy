;
; Startup code for cc65 (C64 version)
;
; This must be the *first* file on the linker command line
;

	.export		_exit
	.import		initlib, donelib
       	.import	       	zerobss, push0
	.import	     	callmain
        .import         RESTOR, BSOUT, CLRCH
	.import		__RAM_START__, __RAM_SIZE__	; Linker generated

        .include        "zeropage.inc"
	.include     	"c64.inc"


; ------------------------------------------------------------------------
; Place the startup code in a special segment.

.segment       	"STARTUP"

.word startup	;cartridge start address
.word startup	;warm start address (when is this needed?)
.byte $C3,$C2,$CD,$38,$30  ;"CBM80" Cartridge autostart

; ------------------------------------------------------------------------
; Actual code

startup:
;copy cartridge and basic to ram
	ldy	#$00
	sty	$fb
	ldx	#$80
bccpylp1:	
	stx	$fc
bccpylp:
	lda	($fb),y
	sta	($fb),y
	iny
	bne	bccpylp
	inx
	cpx	#$c0
	bne	bccpylp1

;copy kernal to ram
	ldx	#$e0
kcpylp1:	
	stx	$fc
kcpylp:
	lda	($fb),y
	sta	($fb),y
	iny
	bne	kcpylp
	inx
	bne	kcpylp1

;patch kernal to not alter the memory configuration
	lda	#$ea ;nop
	sta	$fdd7
	sta	$fdd8
;patch kernal to jump to us when basic has booted
	lda	#$4c ;jmp abs
	sta	$e5cd
	lda	#<startupwt
	sta	$e5ce
	lda	#>startupwt
	sta	$e5cf

;change memory configuration to cart, basic and kernal from ram
	lda	#$e5
	sta	$01
	lda	#$2f
	sta	$00
;continue reset as if there was no cartridge
	ldx	#$05
	jmp	$fcef

startupwt:
	lda #$41
	sta $400
	jmp	startupwt

	ldx    	#zpspace-1
L1:	lda	sp,x
   	sta	zpsave,x	; Save the zero page locations we need
	dex
       	bpl	L1

; Close open files

	jsr	CLRCH

; Switch to second charset

	lda	#14
	jsr	BSOUT

; Switch off the BASIC ROM

	lda	$01
       	pha                     ; Remember the value
	and	#$F8
       	ora	#$06		; Enable kernal+I/O, disable basic
	sta	$01

; Clear the BSS data

	jsr	zerobss

; Save system settings and setup the stack

        pla
        sta	mmusave      	; Save the memory configuration

       	tsx
       	stx    	spsave 		; Save the system stack ptr

	lda    	#<(__RAM_START__ + __RAM_SIZE__)
	sta	sp
	lda	#>(__RAM_START__ + __RAM_SIZE__)
       	sta	sp+1   		; Set argument stack ptr

; Call module constructors

	jsr	initlib

; Push arguments and call main

        jsr     callmain

; Call module destructors. This is also the _exit entry.

_exit:	jsr	donelib		; Run module destructors

; Restore system stuff

  	ldx	spsave
	txs	   	  	; Restore stack pointer
       	lda    	mmusave
	sta	$01	  	; Restore memory configuration

; Copy back the zero page stuff

       	ldx	#zpspace-1
L2:	lda	zpsave,x
	sta	sp,x
	dex
       	bpl	L2

; Reset changed vectors, back to basic

	jmp	RESTOR


; ------------------------------------------------------------------------
; Data

.data

zpsave:	.res	zpspace

.bss

spsave:	.res	1
mmusave:.res	1
