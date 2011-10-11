	.syntax unified
	.thumb
	.file	"bmw_small-asm-cstub.c"
	.text
	.align	2
	.global	s32_0
	.thumb
	.thumb_func
	.type	s32_0, %function
s32_0:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	lsrs	r0, r0, #1
	eor	r0, r0, r3, lsl #3
	eor	r0, r0, r3, ror #28
	eor	r0, r0, r3, ror #13
	bx	lr
	.size	s32_0, .-s32_0
	.align	2
	.global	s32_1
	.thumb
	.thumb_func
	.type	s32_1, %function
s32_1:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	lsrs	r0, r0, #1
	eor	r0, r0, r3, lsl #2
	eor	r0, r0, r3, ror #24
	eor	r0, r0, r3, ror #9
	bx	lr
	.size	s32_1, .-s32_1
	.align	2
	.global	s32_2
	.thumb
	.thumb_func
	.type	s32_2, %function
s32_2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	lsrs	r0, r0, #2
	eor	r0, r0, r3, lsl #1
	eor	r0, r0, r3, ror #20
	eor	r0, r0, r3, ror #7
	bx	lr
	.size	s32_2, .-s32_2
	.align	2
	.global	s32_3
	.thumb
	.thumb_func
	.type	s32_3, %function
s32_3:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	lsrs	r0, r0, #2
	eor	r0, r0, r3, lsl #2
	eor	r0, r0, r3, ror #17
	eor	r0, r0, r3, ror #3
	bx	lr
	.size	s32_3, .-s32_3
	.align	2
	.global	s32_4
	.thumb
	.thumb_func
	.type	s32_4, %function
s32_4:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	eor	r0, r0, r0, lsr #1
	bx	lr
	.size	s32_4, .-s32_4
	.align	2
	.global	s32_5
	.thumb
	.thumb_func
	.type	s32_5, %function
s32_5:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	eor	r0, r0, r0, lsr #2
	bx	lr
	.size	s32_5, .-s32_5
	.align	2
	.thumb
	.thumb_func
	.type	bmw_small_expand1, %function
bmw_small_expand1:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	uxtb	r0, r0
	and	ip, r0, #15
	push	{r4, r5, r6, r7, r8, r9, sl, fp}
	ldr	r4, [r2, ip, lsl #2]
	mvn	sl, ip
	add	sl, sl, #32
	add	ip, ip, #1
	lsr	sl, r4, sl
	lsl	r4, r4, ip
	add	fp, r0, #3
	add	r7, r0, #10
	and	r5, fp, #15
	and	r8, r7, #15
	ldr	r6, [r2, r5, lsl #2]
	ldr	r9, [r2, r8, lsl #2]
	ldr	r2, .L15
	orr	sl, sl, r4
	ldr	ip, [r2, r0, lsl #2]
	mvn	r2, r5
	add	sl, sl, ip
	adds	r5, r5, #1
	adds	r2, r2, #32
	mvn	ip, r8
	lsrs	r2, r6, r2
	add	r8, r8, #1
	lsls	r6, r6, r5
	add	ip, ip, #32
	lsr	ip, r9, ip
	lsl	r9, r9, r8
	sub	sp, sp, #32
	orrs	r2, r2, r6
	str	r7, [sp, #0]
	adds	r7, r0, #7
	orr	ip, ip, r9
	add	sl, sl, r2
	and	r2, r7, #15
	rsb	sl, ip, sl
	ldr	ip, [r3, r2, lsl #2]
	adds	r2, r0, #2
	ldr	r4, [r1, r2, lsl #2]
	adds	r3, r0, #1
	adds	r2, r0, #5
	ldr	r9, [r1, r2, lsl #2]
	eor	sl, sl, ip
	add	r2, r0, #9
	ldr	ip, [r1, r3, lsl #2]
	adds	r3, r0, #4
	ldr	r8, [r1, r3, lsl #2]
	ldr	r2, [r1, r2, lsl #2]
	adds	r3, r0, #6
	ldr	r6, [r1, fp, lsl #2]
	ldr	fp, [r1, r3, lsl #2]
	add	r3, r0, #8
	ldr	r3, [r1, r3, lsl #2]
	str	r2, [sp, #8]
	ldr	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [r1, r2, lsl #2]
	add	r3, r0, #11
	ldr	r3, [r1, r3, lsl #2]
	str	r2, [sp, #12]
	add	r2, r0, #12
	ldr	r2, [r1, r2, lsl #2]
	str	r3, [sp, #16]
	add	r3, r0, #13
	ldr	r3, [r1, r3, lsl #2]
	ldr	r5, [r1, r0, lsl #2]
	str	r2, [sp, #20]
	add	r2, r0, #14
	adds	r0, r0, #15
	ldr	r7, [r1, r7, lsl #2]
	ldr	r2, [r1, r2, lsl #2]
	str	r3, [sp, #24]
	ldr	r1, [r1, r0, lsl #2]
	lsrs	r3, r4, #2
	lsr	r0, ip, #2
	eor	r0, r0, ip, lsl #1
	eor	r3, r3, r4, lsl #2
	eor	r0, r0, ip, ror #20
	eor	r3, r3, r4, ror #17
	str	r2, [sp, #28]
	eor	r3, r3, r4, ror #3
	lsrs	r2, r5, #1
	eor	r0, r0, ip, ror #7
	adds	r0, r0, r3
	eor	r2, r2, r5, lsl #2
	lsrs	r3, r6, #1
	eor	r2, r2, r5, ror #24
	eor	r3, r3, r6, lsl #3
	eor	r2, r2, r5, ror #9
	eor	r3, r3, r6, ror #28
	adds	r0, r0, r2
	eor	r3, r3, r6, ror #13
	lsr	r2, r8, #1
	adds	r0, r0, r3
	eor	r2, r2, r8, lsl #2
	lsr	r3, r9, #2
	eor	r2, r2, r8, ror #24
	eor	r3, r3, r9, lsl #1
	eor	r2, r2, r8, ror #9
	eor	r3, r3, r9, ror #20
	adds	r0, r0, r2
	eor	r3, r3, r9, ror #7
	lsr	r2, fp, #2
	adds	r0, r0, r3
	eor	r2, r2, fp, lsl #2
	lsrs	r3, r7, #1
	eor	r2, r2, fp, ror #17
	eor	r3, r3, r7, lsl #3
	eor	r2, r2, fp, ror #3
	eor	r3, r3, r7, ror #28
	adds	r0, r0, r2
	eor	r3, r3, r7, ror #13
	adds	r0, r0, r3
	ldr	r3, [sp, #4]
	ldr	r7, [sp, #8]
	lsrs	r2, r3, #1
	eor	r2, r2, r3, lsl #2
	eor	r2, r2, r3, ror #24
	eor	r2, r2, r3, ror #9
	lsrs	r3, r7, #2
	eor	r3, r3, r7, lsl #1
	eor	r3, r3, r7, ror #20
	adds	r0, r0, r2
	eor	r3, r3, r7, ror #7
	adds	r0, r0, r3
	ldr	r3, [sp, #12]
	ldr	r7, [sp, #16]
	lsrs	r2, r3, #2
	eor	r2, r2, r3, lsl #2
	eor	r2, r2, r3, ror #17
	eor	r2, r2, r3, ror #3
	lsrs	r3, r7, #1
	eor	r3, r3, r7, lsl #3
	eor	r3, r3, r7, ror #28
	adds	r0, r0, r2
	eor	r3, r3, r7, ror #13
	adds	r0, r0, r3
	ldr	r3, [sp, #20]
	ldr	r7, [sp, #24]
	lsrs	r2, r3, #1
	eor	r2, r2, r3, lsl #2
	eor	r2, r2, r3, ror #24
	eor	r2, r2, r3, ror #9
	adds	r0, r0, r2
	lsrs	r3, r7, #2
	eor	r3, r3, r7, lsl #1
	eor	r3, r3, r7, ror #20
	eor	r3, r3, r7, ror #7
	adds	r0, r0, r3
	ldr	r3, [sp, #28]
	lsrs	r2, r3, #2
	eor	r2, r2, r3, lsl #2
	eor	r2, r2, r3, ror #17
	eor	r2, r2, r3, ror #3
	lsrs	r3, r1, #1
	eor	r3, r3, r1, lsl #3
	eor	r3, r3, r1, ror #28
	adds	r0, r0, r2
	eor	r3, r3, r1, ror #13
	adds	r0, r0, r3
	add	r0, r0, sl
	add	sp, sp, #32
	pop	{r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
.L16:
	.align	2
.L15:
	.word	k_lut
	.size	bmw_small_expand1, .-bmw_small_expand1
	.align	2
	.thumb
	.thumb_func
	.type	bmw_small_f0, %function
bmw_small_f0:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5}
	mov	r5, r0
	mov	r4, r1
	mov	r0, r2
	movs	r1, #0
.L18:
	ldr	r3, [r4, r1]
	ldr	r2, [r0, r1]
	eors	r3, r3, r2
	str	r3, [r4, r1]
	adds	r1, r1, #4
	cmp	r1, #64
	bne	.L18
	ldr	r2, [r4, #20]
	ldr	r3, [r4, #40]
	ldr	r1, [r4, #52]
	adds	r3, r3, r2
	ldr	r2, [r4, #56]
	adds	r3, r3, r1
	ldr	r1, [r4, #28]
	adds	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r3, [r4, #24]
	ldr	r2, [r4, #44]
	ldr	r1, [r4, #56]
	adds	r2, r2, r3
	ldr	r3, [r4, #60]
	adds	r2, r2, r1
	ldr	r1, [r4, #32]
	subs	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #4]
	ldr	r2, [r4, #36]
	ldr	r3, [r4, #28]
	ldr	r1, [r4, #0]
	adds	r3, r3, r2
	ldr	r2, [r4, #60]
	adds	r3, r3, r1
	ldr	r1, [r4, #48]
	adds	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #8]
	ldr	r3, [r4, #52]
	ldr	r2, [r4, #32]
	ldr	r1, [r4, #0]
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	adds	r2, r2, r1
	ldr	r1, [r4, #40]
	subs	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #12]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]
	ldr	r1, [r4, #36]
	adds	r3, r3, r2
	ldr	r2, [r4, #56]
	adds	r3, r3, r1
	ldr	r1, [r4, #44]
	subs	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r3, [r4, #12]
	ldr	r2, [r4, #40]
	ldr	r1, [r4, #60]
	adds	r2, r2, r3
	ldr	r3, [r4, #8]
	adds	r2, r2, r1
	ldr	r1, [r4, #48]
	subs	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #20]
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #44]
	subs	r3, r3, r2
	ldr	r2, [r4, #52]
	subs	r3, r3, r1
	ldr	r1, [r4, #12]
	adds	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #24]
	ldr	r3, [r4, #16]
	ldr	r2, [r4, #4]
	ldr	r1, [r4, #20]
	subs	r2, r2, r3
	ldr	r3, [r4, #48]
	subs	r2, r2, r1
	ldr	r1, [r4, #56]
	subs	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #28]
	ldr	r2, [r4, #20]
	ldr	r3, [r4, #8]
	ldr	r1, [r4, #60]
	subs	r3, r3, r2
	ldr	r2, [r4, #52]
	subs	r3, r3, r1
	ldr	r1, [r4, #24]
	adds	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #32]
	ldr	r3, [r4, #56]
	ldr	r2, [r4, #24]
	ldr	r1, [r4, #0]
	adds	r2, r2, r3
	ldr	r3, [r4, #12]
	adds	r2, r2, r1
	ldr	r1, [r4, #28]
	subs	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #36]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #32]
	ldr	r1, [r4, #28]
	subs	r3, r3, r2
	ldr	r2, [r4, #60]
	subs	r3, r3, r1
	ldr	r1, [r4, #16]
	adds	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #40]
	ldr	r3, [r4, #0]
	ldr	r2, [r4, #32]
	ldr	r1, [r4, #20]
	subs	r2, r2, r3
	ldr	r3, [r4, #36]
	subs	r2, r2, r1
	ldr	r1, [r4, #8]
	adds	r2, r2, r3
	subs	r2, r2, r1
	str	r2, [r5, #44]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #12]
	ldr	r1, [r4, #40]
	adds	r3, r3, r2
	ldr	r2, [r4, #36]
	adds	r3, r3, r1
	ldr	r1, [r4, #24]
	subs	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #48]
	ldr	r3, [r4, #8]
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #28]
	adds	r2, r2, r3
	ldr	r3, [r4, #40]
	adds	r2, r2, r1
	ldr	r1, [r4, #44]
	adds	r2, r2, r3
	adds	r2, r2, r1
	str	r2, [r5, #52]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #32]
	ldr	r1, [r4, #48]
	adds	r3, r3, r2
	ldr	r2, [r4, #44]
	subs	r3, r3, r1
	ldr	r1, [r4, #20]
	subs	r3, r3, r2
	subs	r3, r3, r1
	str	r3, [r5, #56]
	ldr	r2, [r4, #36]
	ldr	r3, [r4, #24]
	ldr	r1, [r4, #52]
	rsb	r2, r2, #0
	subs	r2, r2, r3
	adds	r2, r2, r1
	ldr	r3, [r4, #48]
	ldr	r1, [r4, #16]
	subs	r3, r3, r1
	ldr	r1, [r5, #0]
	adds	r2, r2, r3
	lsrs	r3, r1, #1
	eor	r3, r3, r1, lsl #3
	str	r2, [r5, #60]
	eor	r3, r3, r1, ror #28
	ldr	r2, [r5, #4]
	eor	r3, r3, r1, ror #13
	str	r3, [r5, #0]
	lsrs	r3, r2, #1
	eor	r3, r3, r2, lsl #2
	eor	r3, r3, r2, ror #24
	eor	r3, r3, r2, ror #9
	ldr	r2, [r5, #8]
	str	r3, [r5, #4]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #1
	eor	r3, r3, r2, ror #20
	eor	r3, r3, r2, ror #7
	ldr	r2, [r5, #12]
	str	r3, [r5, #8]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #2
	eor	r3, r3, r2, ror #17
	eor	r3, r3, r2, ror #3
	ldr	r2, [r5, #16]
	str	r3, [r5, #12]
	eor	r2, r2, r2, lsr #1
	str	r2, [r5, #16]
	ldr	r1, [r5, #20]
	ldr	r2, [r5, #24]
	lsrs	r3, r1, #1
	eor	r3, r3, r1, lsl #3
	eor	r3, r3, r1, ror #28
	eor	r3, r3, r1, ror #13
	str	r3, [r5, #20]
	lsrs	r3, r2, #1
	eor	r3, r3, r2, lsl #2
	eor	r3, r3, r2, ror #24
	eor	r3, r3, r2, ror #9
	ldr	r2, [r5, #28]
	str	r3, [r5, #24]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #1
	eor	r3, r3, r2, ror #20
	eor	r3, r3, r2, ror #7
	ldr	r2, [r5, #32]
	str	r3, [r5, #28]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #2
	ldr	r1, [r5, #40]
	eor	r3, r3, r2, ror #17
	eor	r3, r3, r2, ror #3
	ldr	r2, [r5, #36]
	str	r3, [r5, #32]
	lsrs	r3, r1, #1
	eor	r2, r2, r2, lsr #1
	eor	r3, r3, r1, lsl #3
	eor	r3, r3, r1, ror #28
	str	r2, [r5, #36]
	ldr	r2, [r5, #44]
	eor	r3, r3, r1, ror #13
	str	r3, [r5, #40]
	lsrs	r3, r2, #1
	eor	r3, r3, r2, lsl #2
	eor	r3, r3, r2, ror #24
	eor	r3, r3, r2, ror #9
	ldr	r2, [r5, #48]
	str	r3, [r5, #44]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #1
	eor	r3, r3, r2, ror #20
	eor	r3, r3, r2, ror #7
	ldr	r2, [r5, #52]
	str	r3, [r5, #48]
	lsrs	r3, r2, #2
	eor	r3, r3, r2, lsl #2
	ldr	r1, [r5, #60]
	eor	r3, r3, r2, ror #17
	eor	r3, r3, r2, ror #3
	str	r3, [r5, #52]
	lsrs	r3, r1, #1
	ldr	r2, [r5, #56]
	eor	r3, r3, r1, lsl #3
	eor	r3, r3, r1, ror #28
	eor	r3, r3, r1, ror #13
	eor	r2, r2, r2, lsr #1
	str	r2, [r5, #56]
	str	r3, [r5, #60]
	movs	r1, #0
.L19:
	ldr	r3, [r4, r1]
	ldr	r2, [r0, r1]
	eors	r3, r3, r2
	str	r3, [r4, r1]
	adds	r1, r1, #4
	cmp	r1, #64
	bne	.L19
	mov	ip, #0
	movs	r0, #1
.L20:
	and	r3, r0, #15
	ldr	r2, [r5, ip]
	ldr	r1, [r4, r3, lsl #2]
	adds	r0, r0, #1
	adds	r2, r2, r1
	str	r2, [r5, ip]
	add	ip, ip, #4
	cmp	ip, #64
	bne	.L20
	pop	{r4, r5}
	bx	lr
	.size	bmw_small_f0, .-bmw_small_f0
	.align	2
	.thumb
	.thumb_func
	.type	bmw_small_f1, %function
bmw_small_f1:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #4
	mov	r4, r0
	mov	fp, r1
	str	r2, [sp, #0]
	mov	r1, r4
	mov	r2, fp
	ldr	r3, [sp, #0]
	movs	r0, #0
	bl	bmw_small_expand1
	mov	r1, r4
	str	r0, [r4, #64]
	mov	r2, fp
	movs	r0, #1
	ldr	r3, [sp, #0]
	bl	bmw_small_expand1
	ldr	r9, .L30
	str	r0, [r4, #68]
	mov	lr, r4
	movs	r6, #0
	mov	sl, #2
	mov	r8, #5
	movs	r7, #12
.L27:
	and	r3, sl, #15
	ldr	r2, [fp, r3, lsl #2]
	mvn	r0, r3
	and	r1, r8, #15
	adds	r3, r3, #1
	adds	r0, r0, #32
	ldr	ip, [fp, r1, lsl #2]
	lsrs	r0, r2, r0
	lsls	r2, r2, r3
	mvn	r3, r1
	adds	r3, r3, #32
	adds	r1, r1, #1
	lsr	r3, ip, r3
	lsl	ip, ip, r1
	and	r4, r7, #15
	orrs	r0, r0, r2
	orr	r3, r3, ip
	ldr	r5, [fp, r4, lsl #2]
	adds	r0, r0, r3
	mvn	r3, r4
	adds	r3, r3, #32
	adds	r4, r4, #1
	lsrs	r3, r5, r3
	lsls	r5, r5, r4
	ldr	r2, [r9, #8]
	ldr	r1, [sp, #0]
	adds	r0, r0, r2
	add	r2, r6, #9
	orrs	r3, r3, r5
	and	r2, r2, #15
	subs	r0, r0, r3
	ldr	r3, [r1, r2, lsl #2]
	ldr	r2, [lr, #8]
	eors	r0, r0, r3
	ldr	r3, [lr, #16]
	ldr	r1, [lr, #24]
	adds	r3, r3, r2
	ldr	r2, [lr, #32]
	adds	r3, r3, r1
	ldr	r1, [lr, #40]
	adds	r3, r3, r2
	ldr	r2, [lr, #48]
	adds	r3, r3, r1
	ldr	r1, [lr, #56]
	adds	r3, r3, r2
	ldr	r2, [lr, #12]
	adds	r3, r3, r1
	ldr	r1, [lr, #20]
	add	r3, r3, r2, ror #29
	ldr	r2, [lr, #28]
	add	r3, r3, r1, ror #25
	ldr	r1, [lr, #36]
	add	r3, r3, r2, ror #19
	ldr	r2, [lr, #44]
	add	r3, r3, r1, ror #16
	ldr	r1, [lr, #52]
	ldr	ip, [lr, #64]
	add	r3, r3, r2, ror #13
	ldr	r2, [lr, #60]
	ldr	r4, [lr, #68]
	add	r3, r3, r1, ror #9
	add	r3, r3, r2, ror #5
	eor	ip, ip, ip, lsr #1
	add	r3, r3, ip
	eor	r4, r4, r4, lsr #2
	adds	r3, r3, r4
	adds	r6, r6, #1
	adds	r3, r3, r0
	adds	r7, r7, #1
	cmp	r6, #14
	str	r3, [lr, #72]
	add	sl, sl, #1
	add	r8, r8, #1
	add	r9, r9, #4
	add	lr, lr, #4
	bne	.L27
	add	sp, sp, #4
	pop	{r4, r5, r6, r7, r8, r9, sl, fp, pc}
.L31:
	.align	2
.L30:
	.word	k_lut
	.size	bmw_small_f1, .-bmw_small_f1
	.align	2
	.global	bmw224_init
	.thumb
	.thumb_func
	.type	bmw224_init, %function
bmw224_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L36
	mov	r2, r0
	str	r3, [r0, #0]
	add	r1, r0, #60
.L33:
	ldr	r3, [r2, #0]
	add	r3, r3, #67372036
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L33
	movs	r3, #0
	str	r3, [r0, #64]
	bx	lr
.L37:
	.align	2
.L36:
	.word	66051
	.size	bmw224_init, .-bmw224_init
	.align	2
	.global	bmw256_init
	.thumb
	.thumb_func
	.type	bmw256_init, %function
bmw256_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L42
	mov	r2, r0
	str	r3, [r0, #0]
	add	r1, r0, #60
.L39:
	ldr	r3, [r2, #0]
	add	r3, r3, #67372036
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L39
	movs	r3, #0
	str	r3, [r0, #64]
	bx	lr
.L43:
	.align	2
.L42:
	.word	1078018627
	.size	bmw256_init, .-bmw256_init
	.align	2
	.global	bmw256_ctx2hash
	.thumb
	.thumb_func
	.type	bmw256_ctx2hash, %function
bmw256_ctx2hash:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	adds	r1, r1, #32
	movs	r2, #32
	bl	memcpy
	pop	{pc}
	.size	bmw256_ctx2hash, .-bmw256_ctx2hash
	.align	2
	.global	bmw224_ctx2hash
	.thumb
	.thumb_func
	.type	bmw224_ctx2hash, %function
bmw224_ctx2hash:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	adds	r1, r1, #36
	movs	r2, #28
	bl	memcpy
	pop	{pc}
	.size	bmw224_ctx2hash, .-bmw224_ctx2hash
	.align	2
	.thumb
	.thumb_func
	.type	bmw_small_f2, %function
bmw_small_f2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r8, #0
	mov	r5, r0
	mov	r6, r1
	mov	r0, r8
	mov	r1, r2
.L49:
	add	r3, r6, r0
	ldr	r2, [r3, #64]
	adds	r0, r0, #4
	cmp	r0, #32
	eor	r8, r8, r2
	bne	.L49
	mov	r7, r8
	subs	r0, r0, #32
.L50:
	add	r3, r6, r0
	ldr	r2, [r3, #96]
	adds	r0, r0, #4
	eors	r7, r7, r2
	cmp	r0, #32
	bne	.L50
	movs	r2, #64
	mov	r0, r5
	bl	memcpy
	ldr	r2, [r6, #64]
	ldr	r3, [r5, #0]
	movs	r0, #0
	eor	r3, r3, r2, lsr #5
	eor	r3, r3, r7, lsl #5
	str	r3, [r5, #0]
	ldr	r2, [r6, #68]
	ldr	r3, [r5, #4]
	eor	r3, r3, r2, lsl #8
	eor	r3, r3, r7, lsr #7
	str	r3, [r5, #4]
	ldr	r2, [r6, #72]
	ldr	r3, [r5, #8]
	eor	r3, r3, r2, lsl #5
	eor	r3, r3, r7, lsr #5
	str	r3, [r5, #8]
	ldr	r2, [r6, #76]
	ldr	r3, [r5, #12]
	eor	r3, r3, r2, lsl #5
	eor	r3, r3, r7, lsr #1
	str	r3, [r5, #12]
	ldr	r2, [r6, #80]
	ldr	r3, [r5, #16]
	eors	r2, r2, r3
	eor	r2, r2, r7, lsr #3
	str	r2, [r5, #16]
	ldr	r1, [r6, #84]
	ldr	r3, [r5, #20]
	eor	r3, r3, r1, lsr #6
	eor	r3, r3, r7, lsl #6
	str	r3, [r5, #20]
	ldr	r2, [r6, #88]
	ldr	r3, [r5, #24]
	eor	r3, r3, r2, lsl #6
	eor	r3, r3, r7, lsr #4
	str	r3, [r5, #24]
	ldr	r2, [r6, #92]
	ldr	r3, [r5, #28]
	eor	r3, r3, r2, lsl #2
	eor	r3, r3, r7, lsr #11
	str	r3, [r5, #28]
.L51:
	add	r1, r6, r0
	ldr	r2, [r6, r0]
	ldr	r3, [r1, #96]
	eors	r2, r2, r3
	ldr	r3, [r5, r0]
	eor	r2, r2, r8
	adds	r3, r3, r2
	str	r3, [r5, r0]
	adds	r0, r0, #4
	cmp	r0, #32
	bne	.L51
	mov	lr, r5
	mov	r4, r6
	mov	ip, #4
.L52:
	ldr	r3, [lr, #32]
	ldr	r0, [r4, #96]
	and	r2, ip, #7
	eors	r0, r0, r3
	eors	r0, r0, r7
	str	r0, [lr, #32]
	ldr	r1, [r5, r2, lsl #2]
	rsb	r3, ip, #27
	add	r2, ip, #5
	lsrs	r3, r1, r3
	lsls	r1, r1, r2
	add	ip, ip, #1
	orrs	r3, r3, r1
	adds	r3, r3, r0
	adds	r4, r4, #4
	cmp	ip, #12
	str	r3, [lr, #32]
	add	lr, lr, #4
	bne	.L52
	movs	r2, #28
	add	r0, r6, #36
	add	r1, r6, #64
	bl	memxor
	ldr	r3, [r6, #32]
	ldr	r2, [r6, #92]
	eors	r2, r2, r3
	str	r2, [r6, #32]
	ldr	r3, [r5, #32]
	eor	r2, r2, r8, lsl #8
	adds	r3, r3, r2
	str	r3, [r5, #32]
	ldr	r2, [r6, #36]
	ldr	r3, [r5, #36]
	eor	r2, r2, r8, lsr #6
	adds	r3, r3, r2
	str	r3, [r5, #36]
	ldr	r2, [r6, #40]
	ldr	r3, [r5, #40]
	eor	r2, r2, r8, lsl #6
	adds	r3, r3, r2
	str	r3, [r5, #40]
	ldr	r2, [r6, #44]
	ldr	r3, [r5, #44]
	eor	r2, r2, r8, lsl #4
	adds	r3, r3, r2
	str	r3, [r5, #44]
	ldr	r2, [r6, #48]
	ldr	r3, [r5, #48]
	eor	r2, r2, r8, lsr #3
	adds	r3, r3, r2
	str	r3, [r5, #48]
	ldr	r2, [r6, #52]
	ldr	r3, [r5, #52]
	eor	r2, r2, r8, lsr #4
	adds	r3, r3, r2
	str	r3, [r5, #52]
	ldr	r2, [r6, #56]
	ldr	r3, [r5, #56]
	eor	r2, r2, r8, lsr #7
	adds	r3, r3, r2
	str	r3, [r5, #56]
	ldr	r2, [r6, #60]
	ldr	r3, [r5, #60]
	eor	r2, r2, r8, lsr #2
	adds	r3, r3, r2
	str	r3, [r5, #60]
	pop	{r4, r5, r6, r7, r8, pc}
	.size	bmw_small_f2, .-bmw_small_f2
	.align	2
	.global	bmw_small_nextBlock
	.thumb
	.thumb_func
	.type	bmw_small_nextBlock, %function
bmw_small_nextBlock:
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r5, r0
	sub	sp, sp, #128
	mov	r6, r1
	mov	r0, sp
	mov	r1, r5
	mov	r2, r6
	bl	bmw_small_f0
	mov	r0, sp
	mov	r1, r6
	mov	r2, r5
	bl	bmw_small_f1
	mov	r0, r5
	mov	r1, sp
	mov	r2, r6
	bl	bmw_small_f2
	ldr	r3, [r5, #64]
	mov	r4, sp
	adds	r3, r3, #1
	str	r3, [r5, #64]
	add	sp, sp, #128
	pop	{r4, r5, r6, pc}
	.size	bmw_small_nextBlock, .-bmw_small_nextBlock
	.align	2
	.global	bmw256_nextBlock
	.thumb
	.thumb_func
	.type	bmw256_nextBlock, %function
bmw256_nextBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	bl	bmw_small_nextBlock
	pop	{pc}
	.size	bmw256_nextBlock, .-bmw256_nextBlock
	.align	2
	.global	bmw224_nextBlock
	.thumb
	.thumb_func
	.type	bmw224_nextBlock, %function
bmw224_nextBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	bl	bmw_small_nextBlock
	pop	{pc}
	.size	bmw224_nextBlock, .-bmw224_nextBlock
	.align	2
	.global	bmw_small_lastBlock
	.thumb
	.thumb_func
	.type	bmw_small_lastBlock, %function
bmw_small_lastBlock:
	@ args = 0, pretend = 0, frame = 192
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, sl, lr}
	movw	r3, #511
	uxth	r8, r2
	cmp	r8, r3
	sub	sp, sp, #192
	mov	r6, r0
	mov	sl, r1
	bls	.L65
	mov	r5, r1
	mov	r4, r8
	mov	r7, r3
.L66:
	mov	r1, r5
	mov	r0, r6
	bl	bmw_small_nextBlock
	sub	r3, r4, #512
	uxth	r4, r3
	adds	r5, r5, #64
	cmp	r4, r7
	bhi	.L66
	sub	r2, r8, #512
	uxth	r2, r2
	lsrs	r3, r2, #9
	adds	r3, r3, #1
	lsls	r3, r3, #6
	lsl	r8, r2, #23
	add	sl, sl, r3
	lsr	r8, r8, #23
.L65:
	add	r5, sp, #128
	movs	r1, #0
	movs	r2, #64
	mov	r0, r5
	bl	memset
	add	r2, r8, #7
	asrs	r2, r2, #3
	mov	r1, sl
	mov	r0, r5
	bl	memcpy
	add	r3, sp, #192
	add	ip, r3, r8, lsr #3
	and	r1, r8, #7
	movs	r3, #128
	asrs	r3, r3, r1
	ldrb	r2, [ip, #-64]	@ zero_extendqisi2
	orrs	r3, r3, r2
	strb	r3, [ip, #-64]
	cmp	r8, #448
	it	lt
	ldrlt	r1, [r6, #64]
	bge	.L74
.L67:
	mov	r2, #512
	mov	r3, r8
	mov	r4, #0
	umlal	r3, r4, r2, r1
	mov	r0, r6
	str	r3, [r5, #56]
	str	r4, [r5, #60]
	mov	r1, r5
	bl	bmw_small_nextBlock
	movs	r2, #64
	mov	r0, r5
	movs	r1, #170
	bl	memset
	movs	r2, #0
	movs	r3, #160
.L68:
	strb	r3, [r5, r2]
	adds	r2, r2, #4
	adds	r3, r3, #1
	cmp	r2, #64
	uxtb	r3, r3
	bne	.L68
	mov	r1, r5
	mov	r0, sp
	mov	r2, r6
	bl	bmw_small_f0
	mov	r2, r5
	mov	r0, sp
	mov	r1, r6
	bl	bmw_small_f1
	mov	r4, r6
	mov	r0, r5
	mov	r1, sp
	mov	r2, r6
	bl	bmw_small_f2
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	mov	ip, r5
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	stmia	r4, {r0, r1, r2, r3}
	add	sp, sp, #192
	pop	{r4, r5, r6, r7, r8, sl, pc}
.L74:
	mov	r1, r5
	mov	r0, r6
	bl	bmw_small_nextBlock
	ldr	r1, [r6, #64]
	movs	r3, #0
	subs	r1, r1, #1
	str	r3, [sp, #128]
	str	r3, [sp, #132]
	str	r3, [sp, #136]
	str	r3, [sp, #140]
	str	r3, [sp, #144]
	str	r3, [sp, #148]
	str	r3, [sp, #152]
	str	r3, [sp, #156]
	str	r3, [sp, #160]
	str	r3, [sp, #164]
	str	r3, [sp, #168]
	str	r3, [sp, #172]
	str	r3, [sp, #176]
	str	r3, [sp, #180]
	str	r1, [r6, #64]
	b	.L67
	.size	bmw_small_lastBlock, .-bmw_small_lastBlock
	.align	2
	.global	bmw256
	.thumb
	.thumb_func
	.type	bmw256, %function
bmw256:
	@ args = 0, pretend = 0, frame = 68
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, sl, lr}
	ldr	r3, .L82
	sub	sp, sp, #68
	add	r6, sp, #68
	mov	sl, r1
	mov	r7, r2
	mov	r9, r0
	str	r3, [r6, #-68]!
	mov	r2, sp
	add	r1, sp, #60
.L76:
	ldr	r3, [r2, #0]
	add	r3, r3, #67372036
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L76
	movs	r3, #0
	str	r3, [sp, #64]
	movw	r3, #511
	cmp	r7, r3
	bls	.L77
	mov	r5, sl
	mov	r4, r7
	mov	r8, r3
.L78:
	mov	r1, r5
	mov	r0, sp
	sub	r4, r4, #512
	bl	bmw_small_nextBlock
	adds	r5, r5, #64
	cmp	r4, r8
	bhi	.L78
	sub	r2, r7, #512
	lsrs	r3, r2, #9
	adds	r3, r3, #1
	lsls	r3, r3, #6
	lsls	r7, r2, #23
	add	sl, sl, r3
	lsrs	r7, r7, #23
.L77:
	uxth	r2, r7
	mov	r0, sp
	mov	r1, sl
	bl	bmw_small_lastBlock
	mov	r0, r9
	add	r1, sp, #32
	movs	r2, #32
	bl	memcpy
	add	sp, sp, #68
	pop	{r4, r5, r6, r7, r8, r9, sl, pc}
.L83:
	.align	2
.L82:
	.word	1078018627
	.size	bmw256, .-bmw256
	.align	2
	.global	bmw224
	.thumb
	.thumb_func
	.type	bmw224, %function
bmw224:
	@ args = 0, pretend = 0, frame = 68
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, sl, lr}
	ldr	r3, .L91
	sub	sp, sp, #68
	add	r6, sp, #68
	mov	sl, r1
	mov	r7, r2
	mov	r9, r0
	str	r3, [r6, #-68]!
	mov	r2, sp
	add	r1, sp, #60
.L85:
	ldr	r3, [r2, #0]
	add	r3, r3, #67372036
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L85
	movs	r3, #0
	str	r3, [sp, #64]
	movw	r3, #511
	cmp	r7, r3
	bls	.L86
	mov	r5, sl
	mov	r4, r7
	mov	r8, r3
.L87:
	mov	r1, r5
	mov	r0, sp
	sub	r4, r4, #512
	bl	bmw_small_nextBlock
	adds	r5, r5, #64
	cmp	r4, r8
	bhi	.L87
	sub	r2, r7, #512
	lsrs	r3, r2, #9
	adds	r3, r3, #1
	lsls	r3, r3, #6
	lsls	r7, r2, #23
	add	sl, sl, r3
	lsrs	r7, r7, #23
.L86:
	uxth	r2, r7
	mov	r0, sp
	mov	r1, sl
	bl	bmw_small_lastBlock
	mov	r0, r9
	add	r1, sp, #36
	movs	r2, #28
	bl	memcpy
	add	sp, sp, #68
	pop	{r4, r5, r6, r7, r8, r9, sl, pc}
.L92:
	.align	2
.L91:
	.word	66051
	.size	bmw224, .-bmw224
	.align	2
	.global	bmw256_lastBlock
	.thumb
	.thumb_func
	.type	bmw256_lastBlock, %function
bmw256_lastBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	uxth	r2, r2
	bl	bmw_small_lastBlock
	pop	{pc}
	.size	bmw256_lastBlock, .-bmw256_lastBlock
	.align	2
	.global	bmw224_lastBlock
	.thumb
	.thumb_func
	.type	bmw224_lastBlock, %function
bmw224_lastBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
	uxth	r2, r2
	bl	bmw_small_lastBlock
	pop	{pc}
	.size	bmw224_lastBlock, .-bmw224_lastBlock
	.section	.rodata
	.align	2
	.type	k_lut, %object
	.size	k_lut, 64
k_lut:
	.word	1431655760
	.word	1521134245
	.word	1610612730
	.word	1700091215
	.word	1789569700
	.word	1879048185
	.word	1968526670
	.word	2058005155
	.word	2147483640
	.word	-2058005171
	.word	-1968526686
	.word	-1879048201
	.word	-1789569716
	.word	-1700091231
	.word	-1610612746
	.word	-1521134261
	.ident	"GCC: (GNU) 4.3.2"
