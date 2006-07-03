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
	.import		__CODE_LOAD__,__CODE_RUN__;
	.import		__CODE_SIZE__,__RODATA_SIZE__,__DATA_SIZE__;

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
;patch basic to jump to us when boot is finished
	lda	#$4c ;jmp abs
	sta	$a480
	lda	#<startupwt
	sta	$a481
	lda	#>startupwt
	sta	$a482

;change memory configuration to cart, basic and kernal from ram
	lda	#$e5
	sta	$01
	lda	#$2f
	sta	$00
;continue reset as if there was no cartridge
	ldx	#$05
	jmp	$fcef

startupwt:
;now we copy the code and data segment to the ram under the kernal
	sei
	lda	#<(__CODE_LOAD__)
	sta	$fb
	lda	#>(__CODE_LOAD__)
	sta	$fc
	lda	#<(__CODE_RUN__)
	sta	$fd
	lda	#>(__CODE_RUN__)
	sta	$fe
	ldy	#$00
	ldx	#>(__CODE_SIZE__+__RODATA_SIZE__+__DATA_SIZE__)
scpylp:
	lda	($fb),y
	sta	($fd),y
	iny
	bne	scpylp
	inc	$fc
	inc	$fe
	dex
	bne scpylp
scpylp1:
	lda	($fb),y
	sta	($fd),y
	iny
	cpy	#<(__CODE_SIZE__+__RODATA_SIZE__+__DATA_SIZE__)
	bne	scpylp1

;now we do the rest of the C startup stuff
	ldx    	#zpspace-1
L1:	lda	sp,x
   	sta	zpsave,x	; Save the zero page locations we need
	dex
	bpl	L1

; Clear the BSS data

	jsr	zerobss

; Save system settings and setup the stack

       	tsx
       	stx    	spsave 		; Save the system stack ptr

	lda    	#<(__RAM_START__ + __RAM_SIZE__)
	sta	sp
	lda	#>(__RAM_START__ + __RAM_SIZE__)
       	sta	sp+1   		; Set argument stack ptr

; Call module constructors

	jsr	initlib

;push return address in code segment and callmain
	lda #>(finished-1)
	pha
	lda #<(finished-1)
	pha
	jmp callmain
	
.code
	finished:
; Call module destructors. This is also the _exit entry.

_exit:	jsr	donelib		; Run module destructors

; Restore system stuff

  	ldx	spsave
	txs	   	  	; Restore stack pointer
    
; Copy back the zero page stuff

       	ldx	#zpspace-1
L2:	lda	zpsave,x
	sta	sp,x
	dex
       	bpl	L2

;put "sta $01" to location $febf, because $fec1 will have a
;rti when the memory config is switched back.
	lda #$85
	sta $febf
	lda #$01
	sta $fec0

;location of the ready & warmstart routine
	lda #>($a474)
	pha
	lda #<($a474)
	pha
	lda #$00;all flags 0
	pha

;switch memory configuration back to normal and return to the keyget loop
	lda #$e7
	jmp	$febf


; ------------------------------------------------------------------------
; Data

.data

zpsave:	.res	zpspace

.bss

spsave:	.res	1
