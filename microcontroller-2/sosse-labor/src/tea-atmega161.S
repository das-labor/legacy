	/* Compile arguments: -mmcu=atmega161 -Os -mcall-prologues */
	/* Changes are marked with "(C)". */
	.file	"tea.c"
	.arch atmega161
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__tmp_reg__ = 0
__zero_reg__ = 1
_PC_ = 2
	.text
.global	tea_func
	.type	tea_func,@function
tea_func:
/* prologue: frame size=0 */
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
/* prologue end (size=6) */
/*
	r25:r24: &in
	r23:r22: &sum
	r21:r20: &k (network byte order)
*/
	movw r30,r24	/* Setup Address */
	movw r12,r22
	movw r22,r20
	ld r14,Z		/* Load in */
	ldd r15,Z+1
	ldd r16,Z+2
	ldd r17,Z+3
	movw r20,r16	/* Copy in */
	movw r18,r14
	ldi r25,4		/* in<<4 */
1:	lsl r18
	rol r19
	rol r20
	rol r21
	dec r25
	brne 1b
	movw r30,r22	/* Setup address */
	ld r27,Z		/* (C) Load k[0] (in network byte order) */
	ldd r26,Z+1		/* (C) */
	ldd r25,Z+2		/* (C) */
	ldd r24,Z+3		/* (C) */
	add r18,r24		/* Add k[0] */
	adc r19,r25
	adc r20,r26
	adc r21,r27
	movw r30,r12	/* Setup address */
	ld r24,Z		/* Load sum */
	ldd r25,Z+1
	ldd r26,Z+2
	ldd r27,Z+3
	add r24,r14		/* Add in */
	adc r25,r15
	adc r26,r16
	adc r27,r17
	eor r18,r24		/* xor */
	eor r19,r25
	eor r20,r26
	eor r21,r27
	ldi r24,5		/* in>>5 */
1:	lsr r17
	ror r16
	ror r15
	ror r14
	dec r24
	brne 1b
	movw r30,r22	/* Setup address */
	ldd r27,Z+4		/* (C) Load k[2] (in network byte order) */
	ldd r26,Z+5		/* (C) */
	ldd r25,Z+6		/* (C) */
	ldd r24,Z+7		/* (C) */
	add r14,r24		/* Add in>>5 */
	adc r15,r25
	adc r16,r26
	adc r17,r27
	eor r18,r14		/* xor */
	eor r19,r15
	eor r20,r16
	eor r21,r17
	movw r24,r20	/* return */
	movw r22,r18
/* epilogue: frame size=0 */
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
/* epilogue end (size=7) */
.Lfe1:
	.size	tea_func,.Lfe1-tea_func
.global	tea_enc
	.type	tea_enc,@function
tea_enc:
/* prologue: frame size=12 */
	ldi r26,lo8(12)
	ldi r27,hi8(12)
	ldi r30,pm_lo8(.L_tea_enc_body)
	ldi r31,pm_hi8(.L_tea_enc_body)
	jmp __prologue_saves__+22
.L_tea_enc_body:
/* prologue end (size=6) */
/*
	r25:r24 &v (network byte order)
	r23:r22 &k (network byte order)
*/
	movw r16,r24	/* r17:r16 = &v */
	movw r14,r22	/* r15:r14 = &k */
	std Y+1,__zero_reg__
	std Y+2,__zero_reg__
	std Y+3,__zero_reg__
	std Y+4,__zero_reg__
	movw r30,r16	/* Z = &v */
	ld r27,Z		/* (C) y=v[0] */
	ldd r26,Z+1		/* (C) */
	ldd r25,Z+2		/* (C) */
	ldd r24,Z+3		/* (C) */
	std Y+9,r24
	std Y+10,r25
	std Y+11,r26
	std Y+12,r27
    ldd r27,Z+4		/* (C) z=v[1] */
    ldd r26,Z+5		/* (C) */
    ldd r25,Z+6		/* (C) */
    ldd r24,Z+7		/* (C) */
	std Y+5,r24
	std Y+6,r25
	std Y+7,r26
	std Y+8,r27
	ldi r18,lo8(31)	/* n=ROUNDS */
	mov r13,r18
.L6:
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r26,Y+3
	ldd r27,Y+4
	subi r24,lo8(-(-1640531527))
	sbci r25,hi8(-(-1640531527))
	sbci r26,hlo8(-(-1640531527))
	sbci r27,hhi8(-(-1640531527))
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	movw r20,r14
	movw r22,r28
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	movw r24,r28
	adiw r24,5
	call tea_func
	movw r18,r22
	movw r20,r24
	ldd r24,Y+9
	ldd r25,Y+10
	ldd r26,Y+11
	ldd r27,Y+12
	add r24,r18
	adc r25,r19
	adc r26,r20
	adc r27,r21
	std Y+9,r24
	std Y+10,r25
	std Y+11,r26
	std Y+12,r27
	movw r20,r14
	subi r20,lo8(-(8))
	sbci r21,hi8(-(8))
	movw r22,r28
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	movw r24,r28
	adiw r24,9
	call tea_func
	movw r26,r24
	movw r24,r22
	ldd r18,Y+5
	ldd r19,Y+6
	ldd r20,Y+7
	ldd r21,Y+8
	add r18,r24
	adc r19,r25
	adc r20,r26
	adc r21,r27
	std Y+5,r18
	std Y+6,r19
	std Y+7,r20
	std Y+8,r21
	mov r24,r13
	dec r13
	tst r24
	brne .L6
	ldd r24,Y+9
	ldd r25,Y+10
	ldd r26,Y+11
	ldd r27,Y+12
	movw r30,r16
	st Z,r27		/* (C) v[0]=y; v[1]=z; */
	std Z+1,r26		/* (C) */
	std Z+2,r25		/* (C) */
	std Z+3,r24		/* (C) */
	std Z+4,r21		/* (C) */
	std Z+5,r20		/* (C) */
	std Z+6,r19		/* (C) */
	std Z+7,r18		/* (C) */
/* epilogue: frame size=12 */
	ldi r30,7
	adiw r28,12
	jmp __epilogue_restores__+22
/* epilogue end (size=4) */
.Lfe2:
	.size	tea_enc,.Lfe2-tea_enc
.global	tea_dec
	.type	tea_dec,@function
tea_dec:
/* prologue: frame size=12 */
	ldi r26,lo8(12)
	ldi r27,hi8(12)
	ldi r30,pm_lo8(.L_tea_dec_body)
	ldi r31,pm_hi8(.L_tea_dec_body)
	jmp __prologue_saves__+22
.L_tea_dec_body:
/* prologue end (size=6) */
/*
	r25:r24 &v (network byte order)
	r23:r22 &k (network byte order)
*/
	movw r16,r24	/* r17:r16 = &v */
	movw r14,r22	/* r15:r14 = &k */
	ldi r24,lo8(-957401312)	/* sum = DELTA*ROUNDS; */
	ldi r25,hi8(-957401312)
	ldi r26,hlo8(-957401312)
	ldi r27,hhi8(-957401312)
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	movw r30,r16
	ld r27,Z		/* (C) y=v[0]; */
	ldd r26,Z+1		/* (C) */
	ldd r25,Z+2		/* (C) */
	ldd r24,Z+3		/* (C) */
	std Y+5,r24
	std Y+6,r25
	std Y+7,r26
	std Y+8,r27
	ldd r27,Z+4		/* (C) z=v[1]; */
	ldd r26,Z+5		/* (C) */
	ldd r25,Z+6		/* (C) */
	ldd r24,Z+7		/* (C) */
	std Y+9,r24
	std Y+10,r25
	std Y+11,r26
	std Y+12,r27
	ldi r19,lo8(31)
	mov r13,r19
.L12:
	movw r20,r14
	subi r20,lo8(-(8))
	sbci r21,hi8(-(8))
	movw r22,r28
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	movw r24,r28
	adiw r24,5
	call tea_func
	movw r18,r22
	movw r20,r24
	ldd r24,Y+9
	ldd r25,Y+10
	ldd r26,Y+11
	ldd r27,Y+12
	sub r24,r18
	sbc r25,r19
	sbc r26,r20
	sbc r27,r21
	std Y+9,r24
	std Y+10,r25
	std Y+11,r26
	std Y+12,r27
	movw r20,r14
	movw r22,r28
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	movw r24,r28
	adiw r24,9
	call tea_func
	movw r26,r24
	movw r24,r22
	ldd r18,Y+5
	ldd r19,Y+6
	ldd r20,Y+7
	ldd r21,Y+8
	sub r18,r24
	sbc r19,r25
	sbc r20,r26
	sbc r21,r27
	std Y+5,r18
	std Y+6,r19
	std Y+7,r20
	std Y+8,r21
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r26,Y+3
	ldd r27,Y+4
	subi r24,lo8(-(1640531527))
	sbci r25,hi8(-(1640531527))
	sbci r26,hlo8(-(1640531527))
	sbci r27,hhi8(-(1640531527))
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	mov r24,r13
	dec r13
	tst r24
	brne .L12
	movw r30,r16
	st Z,r21		/* (C) */
	std Z+1,r20		/* (C) */
	std Z+2,r19		/* (C) */
	std Z+3,r18		/* (C) */
	ldd r24,Y+9
	ldd r25,Y+10
	ldd r26,Y+11
	ldd r27,Y+12
	std Z+4,r27		/* (C) */
	std Z+5,r26		/* (C) */
	std Z+6,r25		/* (C) */
	std Z+7,r24		/* (C) */
/* epilogue: frame size=12 */
	ldi r30,7
	adiw r28,12
	jmp __epilogue_restores__+22
/* epilogue end (size=4) */
.Lfe3:
	.size	tea_dec,.Lfe3-tea_dec
