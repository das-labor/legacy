	.file	"bmw_small_speed.c"
	.text
	.align	2
	.type	bmw_small_f1, %function
bmw_small_f1:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldmib	r0, {r5, r6}	@ phole ldm
	ldr	r7, [r0, #0]
	ldr	r8, [r0, #12]
	mov	r9, r5, lsr #2
	mov	r3, r6, lsr #2
	ldr	sl, [r0, #16]
	eor	r9, r9, r5, asl #1
	eor	r3, r3, r6, asl #2
	mov	ip, r7, lsr #1
	ldr	fp, [r0, #20]
	eor	r9, r9, r5, ror #20
	eor	r3, r3, r6, ror #17
	eor	ip, ip, r7, asl #2
	mov	r4, r8, lsr #1
	eor	r3, r3, r6, ror #3
	eor	r9, r9, r5, ror #7
	ldr	r6, [r0, #24]
	eor	ip, ip, r7, ror #24
	eor	r4, r4, r8, asl #3
	mov	r5, sl, lsr #1
	eor	ip, ip, r7, ror #9
	add	r9, r9, r3
	ldr	r7, [r0, #28]
	eor	r4, r4, r8, ror #28
	eor	r5, r5, sl, asl #2
	mov	r3, fp, lsr #2
	eor	r4, r4, r8, ror #13
	add	r9, r9, ip
	eor	r5, r5, sl, ror #24
	eor	r3, r3, fp, asl #1
	mov	ip, r6, lsr #2
	eor	r5, r5, sl, ror #9
	add	r9, r9, r4
	eor	r3, r3, fp, ror #20
	eor	ip, ip, r6, asl #2
	mov	r4, r7, lsr #1
	ldr	r8, [r0, #32]
	eor	r3, r3, fp, ror #7
	add	r9, r9, r5
	eor	ip, ip, r6, ror #17
	eor	r4, r4, r7, asl #3
	ldr	sl, [r0, #36]
	eor	ip, ip, r6, ror #3
	add	r9, r9, r3
	ldr	r6, [r0, #44]
	eor	r4, r4, r7, ror #28
	ldr	fp, [r0, #40]
	eor	r4, r4, r7, ror #13
	mov	r5, r8, lsr #1
	ldr	r7, [r0, #48]
	add	r9, r9, ip
	eor	r5, r5, r8, asl #2
	mov	r3, sl, lsr #2
	add	r9, r9, r4
	mov	r4, r6, lsr #1
	eor	r5, r5, r8, ror #24
	eor	r3, r3, sl, asl #1
	mov	ip, fp, lsr #2
	eor	r4, r4, r6, asl #3
	mov	r6, r7, lsr #1
	eor	r5, r5, r8, ror #9
	eor	r3, r3, sl, ror #20
	ldr	r8, [r0, #44]
	eor	ip, ip, fp, asl #2
	eor	r6, r6, r7, asl #2
	eor	r3, r3, sl, ror #7
	add	r9, r9, r5
	eor	ip, ip, fp, ror #17
	eor	r6, r6, r7, ror #24
	ldr	sl, [r0, #52]
	eor	ip, ip, fp, ror #3
	eor	r6, r6, r7, ror #9
	add	r9, r9, r3
	ldr	r7, [r0, #8]
	eor	r4, r4, r8, ror #28
	eor	r4, r4, r8, ror #13
	add	r9, r9, ip
	mov	r3, sl, lsr #2
	add	r9, r9, r4
	mov	r5, r7, lsr #2
	ldr	r4, [r0, #60]
	ldr	r7, [r0, #4]
	eor	r3, r3, sl, asl #1
	add	r9, r9, r6
	ldr	r6, [r0, #52]
	ldr	ip, [r1, #0]
	ldr	fp, [r0, #56]
	eor	r3, r3, sl, ror #20
	mov	sl, r4, lsr #1
	mov	r4, r7, lsr #1
	ldr	r7, .L3
	sub	sp, sp, #24
	eor	r3, r3, r6, ror #7
	str	r3, [sp, #0]
	mov	r8, fp, lsr #2
	ldr	r3, [r0, #60]
	add	r7, r7, ip, ror #31
	ldr	ip, [r0, #56]
	eor	r8, r8, fp, asl #2
	ldr	r6, [r0, #8]
	eor	r8, r8, ip, ror #17
	eor	sl, sl, r3, asl #3
	ldr	ip, [r0, #4]
	ldr	r3, [r0, #12]
	eor	r5, r5, r6, asl #1
	eor	r6, r4, ip, asl #2
	mov	ip, r3, lsr #2
	ldr	r3, [sp, #0]
	add	r9, r9, r3
	ldr	r3, [r0, #56]
	ldr	fp, [r1, #12]
	eor	r8, r8, r3, ror #3
	ldr	r3, [r0, #8]
	add	r7, r7, fp, ror #28
	eor	r5, r5, r3, ror #20
	ldr	fp, [r0, #60]
	ldr	r3, [r0, #4]
	ldr	r4, [r1, #40]
	eor	sl, sl, fp, ror #28
	eor	fp, r6, r3, ror #24
	ldr	r6, [r0, #12]
	ldr	r3, [r0, #16]
	eor	ip, ip, r6, asl #2
	add	r9, r9, r8
	mov	r6, r3, lsr #1
	ldr	r8, [r0, #60]
	ldr	r3, [r0, #8]
	sub	r7, r7, r4, ror #21
	ldr	r4, [r0, #4]
	eor	sl, sl, r8, ror #13
	eor	r5, r5, r3, ror #7
	eor	r8, fp, r4, ror #9
	ldr	r3, [r0, #16]
	ldr	fp, [r0, #12]
	add	r9, r9, sl
	ldr	sl, [r0, #16]
	eor	ip, ip, fp, ror #17
	eor	r6, r6, r3, asl #3
	ldr	fp, [r0, #20]
	add	r5, r5, r8
	ldr	r8, [r0, #12]
	ldr	r3, [r2, #28]
	eor	r6, r6, sl, ror #28
	eor	ip, ip, r8, ror #3
	eor	r6, r6, sl, ror #13
	ldr	r8, [r0, #24]
	ldr	sl, [r0, #20]
	mov	r4, fp, lsr #1
	eor	r7, r7, r3
	eor	r4, r4, fp, asl #2
	add	r9, r9, r7
	mov	r3, r8, lsr #2
	eor	r4, r4, sl, ror #24
	ldr	fp, [r0, #32]
	ldr	r7, [r0, #36]
	eor	r4, r4, sl, ror #9
	eor	r3, r3, r8, asl #1
	ldr	r8, [r0, #28]
	str	r9, [r0, #64]
	ldr	sl, [r0, #24]
	add	r5, r5, ip
	add	r5, r5, r6
	eor	r3, r3, sl, ror #20
	eor	r3, r3, sl, ror #7
	mov	ip, r8, lsr #2
	add	r5, r5, r4
	eor	ip, ip, r8, asl #2
	mov	r6, fp, lsr #1
	add	r5, r5, r3
	ldr	r3, [r0, #28]
	eor	ip, ip, r8, ror #17
	eor	r6, r6, fp, asl #3
	eor	ip, ip, r3, ror #3
	mov	r8, r7, lsr #1
	ldr	r3, [r0, #40]
	eor	r6, r6, fp, ror #28
	eor	r6, r6, fp, ror #13
	eor	r8, r8, r7, asl #2
	add	r5, r5, ip
	mov	sl, r3, lsr #2
	eor	r8, r8, r7, ror #24
	add	r5, r5, r6
	ldr	r6, [r0, #40]
	ldr	r4, [r1, #16]
	eor	r8, r8, r7, ror #9
	eor	sl, sl, r3, asl #1
	ldr	fp, [r0, #44]
	ldr	r3, [r1, #4]
	eor	sl, sl, r6, ror #20
	add	r5, r5, r8
	ldr	r6, [r0, #48]
	ldr	r8, [r0, #40]
	mov	r4, r4, ror #27
	eor	sl, sl, r8, ror #7
	mov	ip, fp, lsr #2
	mov	r7, r6, lsr #1
	add	r4, r4, r3, ror #30
	ldr	r3, [r0, #44]
	eor	ip, ip, fp, asl #2
	eor	r7, r7, r6, asl #3
	add	r5, r5, sl
	ldr	r6, [r0, #52]
	ldr	sl, [r0, #44]
	eor	ip, ip, r3, ror #17
	ldr	fp, [r0, #56]
	eor	ip, ip, sl, ror #3
	mov	r3, r6, lsr #1
	add	r6, r0, #48
	ldmia	r6, {r6, sl}	@ phole ldm
	ldr	r8, [r0, #60]
	add	r5, r5, ip
	ldr	ip, [r0, #48]
	eor	r7, r7, r6, ror #28
	eor	r3, r3, sl, asl #2
	mov	r6, fp, lsr #2
	add	r4, r4, #1509949440
	eor	r7, r7, ip, ror #13
	eor	r3, r3, sl, ror #24
	eor	r6, r6, fp, asl #1
	mov	ip, r8, lsr #2
	add	r4, r4, #11141120
	eor	r3, r3, sl, ror #9
	add	r5, r5, r7
	ldr	sl, [r1, #44]
	eor	r6, r6, fp, ror #20
	eor	ip, ip, r8, asl #2
	add	r4, r4, #43520
	ldr	r7, [r2, #32]
	eor	r6, r6, fp, ror #7
	add	r5, r5, r3
	eor	ip, ip, r8, ror #17
	mov	r3, r9, lsr #1
	add	r4, r4, #165
	eor	ip, ip, r8, ror #3
	add	r5, r5, r6
	sub	r4, r4, sl, ror #20
	eor	r3, r3, r9, asl #3
	eor	r4, r4, r7
	add	r5, r5, ip
	eor	r3, r3, r9, ror #28
	eor	r3, r3, r9, ror #13
	add	r5, r5, r4
	ldr	ip, [r0, #12]
	add	r5, r5, r3
	ldr	r3, [r0, #20]
	str	r5, [r0, #68]
	ldr	r4, [r0, #8]
	ldr	r6, [r0, #28]
	ldr	r8, [r0, #16]
	mov	ip, ip, ror #29
	ldr	r7, [r0, #24]
	ldr	sl, [r0, #36]
	add	ip, ip, r3, ror #25
	add	r8, r8, r4
	ldr	r3, [r1, #20]
	ldr	r4, [r0, #32]
	add	ip, ip, r6, ror #19
	ldr	r6, [r0, #44]
	ldr	fp, [r1, #8]
	add	r8, r8, r7
	add	ip, ip, sl, ror #16
	ldr	r7, [r0, #40]
	add	r9, r0, #48
	ldmia	r9, {r9, sl}	@ phole ldm
	add	r8, r8, r4
	add	ip, ip, r6, ror #13
	ldr	r4, [r0, #64]
	ldr	r6, [r0, #60]
	mov	r3, r3, ror #26
	add	r8, r8, r7
	add	ip, ip, sl, ror #9
	ldr	r7, [r0, #56]
	ldr	sl, [r1, #48]
	add	r3, r3, fp, ror #29
	add	r8, r8, r9
	add	ip, ip, r6, ror #5
	eor	r4, r4, r4, lsr #1
	ldr	r6, [r2, #36]
	sub	r3, r3, #-1610612730
	add	r8, r8, r7
	add	ip, ip, r4
	sub	r3, r3, sl, ror #19
	eor	r3, r3, r6
	add	ip, ip, r8
	add	ip, ip, r3
	eor	r5, r5, r5, lsr #2
	add	ip, ip, r5
	str	ip, [r0, #72]
	ldr	r5, [r0, #16]
	ldr	r4, [r0, #24]
	ldr	r3, [r1, #24]
	ldr	r6, [r0, #12]
	ldr	r7, [r0, #20]
	mov	r5, r5, ror #29
	ldr	r9, [r1, #12]
	add	r5, r5, r4, ror #25
	add	r4, r0, #28
	ldmia	r4, {r4, sl}	@ phole ldm
	add	r7, r7, r6
	mov	r3, r3, ror #25
	add	r6, r0, #36
	ldmia	r6, {r6, fp}	@ phole ldm
	add	r3, r3, r9, ror #28
	add	r7, r7, r4
	ldr	r9, [r0, #44]
	add	r5, r5, sl, ror #19
	add	r7, r7, r6
	ldr	sl, [r0, #48]
	add	r5, r5, fp, ror #16
	add	r7, r7, r9
	add	r3, r3, #1711276032
	ldr	r9, [r0, #56]
	ldr	fp, [r0, #52]
	ldr	r6, [r0, #64]
	add	r5, r5, sl, ror #13
	ldr	r4, [r0, #68]
	sub	r3, r3, #11141120
	ldr	sl, [r0, #60]
	add	r5, r5, r9, ror #9
	sub	r3, r3, #43520
	ldr	r9, [r1, #52]
	add	r7, r7, fp
	add	r5, r5, r6, ror #5
	eor	r4, r4, r4, lsr #1
	ldr	r6, [r2, #40]
	sub	r3, r3, #177
	add	r7, r7, sl
	add	r5, r5, r4
	sub	r3, r3, r9, ror #18
	eor	r3, r3, r6
	add	r5, r5, r7
	add	r5, r5, r3
	eor	ip, ip, ip, lsr #2
	add	r5, r5, ip
	str	r5, [r0, #76]
	ldr	ip, [r0, #20]
	ldr	r4, [r0, #28]
	ldr	r3, [r1, #28]
	ldr	r6, [r0, #36]
	mov	ip, ip, ror #29
	ldr	sl, [r1, #16]
	add	ip, ip, r4, ror #25
	ldr	r4, [r0, #44]
	add	ip, ip, r6, ror #19
	mov	r3, r3, ror #24
	add	r3, r3, sl, ror #27
	add	ip, ip, r4, ror #16
	ldr	sl, [r0, #60]
	ldr	r9, [r0, #68]
	add	ip, ip, fp, ror #13
	add	r3, r3, #1778384896
	add	ip, ip, sl, ror #9
	add	r3, r3, #11141120
	ldr	r4, [r0, #72]
	add	ip, ip, r9, ror #5
	add	r3, r3, #43520
	ldr	r9, [r1, #56]
	ldr	fp, [r0, #64]
	ldr	sl, [r2, #44]
	ldr	r6, [r0, #8]
	add	r3, r3, #164
	sub	r3, r3, r9, ror #17
	eor	r4, r4, r4, lsr #1
	eor	r3, r3, sl
	rsb	r6, r6, fp
	add	ip, ip, r4
	add	r6, r6, r8
	add	ip, ip, r3
	add	ip, ip, r6
	eor	r5, r5, r5, lsr #2
	str	r6, [sp, #4]
	add	ip, ip, r5
	ldr	r4, [r0, #24]
	str	ip, [r0, #80]
	ldr	r5, [r0, #32]
	ldr	r6, [r0, #40]
	mov	r4, r4, ror #29
	ldr	r3, [r1, #32]
	add	r4, r4, r5, ror #25
	ldr	r5, [r0, #48]
	ldr	r8, [r1, #20]
	add	r4, r4, r6, ror #19
	ldr	r6, [r0, #56]
	add	r4, r4, r5, ror #16
	mov	r3, r3, ror #23
	ldr	r9, [r1, #60]
	add	r4, r4, r6, ror #13
	add	r3, r3, r8, ror #26
	ldr	r5, [r0, #76]
	ldr	r8, [r0, #72]
	ldr	sl, [r2, #48]
	add	r4, r4, fp, ror #9
	ldr	r6, [r0, #12]
	ldr	fp, [r0, #68]
	sub	r3, r3, #-1879048185
	add	r4, r4, r8, ror #5
	sub	r3, r3, r9, ror #16
	eor	r5, r5, r5, lsr #1
	eor	r3, r3, sl
	rsb	r6, r6, fp
	add	r4, r4, r5
	add	fp, r6, r7
	add	r4, r4, r3
	add	r4, r4, fp
	eor	ip, ip, ip, lsr #2
	add	r4, r4, ip
	str	r4, [r0, #84]
	ldr	r5, [r0, #28]
	ldr	r3, [r1, #36]
	ldr	r7, [r1, #24]
	ldr	r6, [r0, #36]
	ldr	ip, [r0, #44]
	mov	r5, r5, ror #29
	mov	r3, r3, ror #22
	add	r5, r5, r6, ror #25
	add	r3, r3, r7, ror #25
	ldr	r6, [r0, #52]
	add	r5, r5, ip, ror #19
	add	r3, r3, #1979711488
	ldr	ip, [r0, #60]
	add	r5, r5, r6, ror #16
	sub	r3, r3, #11141120
	ldr	r6, [r0, #68]
	ldr	sl, [r1, #0]
	ldr	r8, [r0, #76]
	add	r5, r5, ip, ror #13
	sub	r3, r3, #43520
	ldr	ip, [r0, #80]
	ldr	r9, [r0, #72]
	ldr	r7, [r2, #52]
	add	r5, r5, r6, ror #9
	sub	r3, r3, #178
	ldr	r6, [r0, #16]
	add	r5, r5, r8, ror #5
	sub	r3, r3, sl, ror #31
	eor	ip, ip, ip, lsr #1
	ldr	sl, [sp, #4]
	eor	r3, r3, r7
	rsb	r6, r6, r9
	add	r5, r5, ip
	add	r6, r6, sl
	add	r5, r5, r3
	add	r5, r5, r6
	eor	r4, r4, r4, lsr #2
	add	r5, r5, r4
	str	r5, [r0, #88]
	ldr	ip, [r0, #32]
	ldr	r3, [r1, #40]
	ldr	r7, [r1, #28]
	ldr	r4, [r0, #40]
	str	r6, [sp, #8]
	mov	ip, ip, ror #29
	ldr	r6, [r0, #48]
	mov	r3, r3, ror #21
	add	ip, ip, r4, ror #25
	add	r3, r3, r7, ror #24
	ldr	r4, [r0, #56]
	add	ip, ip, r6, ror #19
	add	r3, r3, #2063597568
	ldr	r6, [r0, #64]
	add	ip, ip, r4, ror #16
	sub	r3, r3, #5570560
	ldr	sl, [r1, #4]
	ldr	r8, [r0, #80]
	add	ip, ip, r6, ror #13
	ldr	r4, [r0, #84]
	sub	r3, r3, #21760
	ldr	r7, [r2, #56]
	ldr	r6, [r0, #20]
	add	ip, ip, r9, ror #9
	sub	r3, r3, #93
	ldr	r9, [r0, #76]
	add	ip, ip, r8, ror #5
	sub	r3, r3, sl, ror #30
	eor	r4, r4, r4, lsr #1
	eor	r3, r3, r7
	rsb	r9, r6, r9
	add	ip, ip, r4
	add	r9, r9, fp
	add	ip, ip, r3
	add	ip, ip, r9
	eor	r5, r5, r5, lsr #2
	add	ip, ip, r5
	ldr	r4, [r0, #36]
	str	ip, [r0, #92]
	ldr	r5, [r0, #44]
	ldr	r6, [r0, #52]
	mov	r4, r4, ror #29
	ldr	r3, [r1, #44]
	add	r4, r4, r5, ror #25
	ldr	r5, [r0, #60]
	ldr	r7, [r1, #32]
	add	r4, r4, r6, ror #19
	ldr	r6, [r0, #68]
	ldr	r8, [r0, #76]
	add	r4, r4, r5, ror #16
	mov	r3, r3, ror #20
	ldr	sl, [r1, #8]
	ldr	fp, [r0, #80]
	add	r4, r4, r6, ror #13
	add	r3, r3, r7, ror #23
	ldr	r5, [r0, #88]
	ldr	r7, [r0, #84]
	ldr	r6, [r0, #24]
	add	r4, r4, r8, ror #9
	sub	r3, r3, #-2147483640
	ldr	r8, [r2, #60]
	add	r4, r4, r7, ror #5
	sub	r3, r3, sl, ror #29
	eor	r5, r5, r5, lsr #1
	rsb	r6, r6, fp
	ldr	fp, [sp, #8]
	eor	r3, r3, r8
	add	r4, r4, r5
	add	r6, r6, fp
	add	r4, r4, r3
	add	r4, r4, r6
	eor	ip, ip, ip, lsr #2
	add	r4, r4, ip
	str	r4, [r0, #96]
	ldr	r5, [r0, #40]
	ldr	r3, [r1, #48]
	ldr	r7, [r1, #36]
	str	r6, [sp, #12]
	ldr	r6, [r0, #48]
	ldr	ip, [r0, #56]
	mov	r5, r5, ror #29
	mov	r3, r3, ror #19
	add	r5, r5, r6, ror #25
	add	r3, r3, r7, ror #22
	ldr	r6, [r0, #64]
	add	r5, r5, ip, ror #19
	add	r3, r3, #-2063597568
	ldr	ip, [r0, #72]
	add	r5, r5, r6, ror #16
	add	r3, r3, #5570560
	ldr	r6, [r0, #80]
	ldr	sl, [r1, #12]
	ldr	r8, [r0, #88]
	add	r5, r5, ip, ror #13
	add	r3, r3, #21760
	ldr	ip, [r0, #92]
	ldr	fp, [r0, #84]
	ldr	r7, [r2, #0]
	add	r5, r5, r6, ror #9
	add	r3, r3, #77
	ldr	r6, [r0, #28]
	add	r5, r5, r8, ror #5
	sub	r3, r3, sl, ror #28
	eor	ip, ip, ip, lsr #1
	eor	r3, r3, r7
	rsb	r6, r6, fp
	add	r5, r5, ip
	add	r6, r6, r9
	add	r5, r5, r3
	add	r5, r5, r6
	eor	r4, r4, r4, lsr #2
	add	r5, r5, r4
	str	r5, [r0, #100]
	ldr	ip, [r0, #44]
	ldr	r3, [r1, #52]
	ldr	r4, [r0, #52]
	ldr	r7, [r1, #40]
	str	r6, [sp, #16]
	mov	ip, ip, ror #29
	ldr	r6, [r0, #60]
	add	ip, ip, r4, ror #25
	mov	r3, r3, ror #18
	ldr	r4, [r0, #68]
	add	r3, r3, r7, ror #21
	add	ip, ip, r6, ror #19
	ldr	r6, [r0, #76]
	add	ip, ip, r4, ror #16
	add	r3, r3, #-1979711488
	ldr	r8, [r0, #92]
	add	ip, ip, r6, ror #13
	ldr	r4, [r0, #96]
	add	r3, r3, #11141120
	ldr	sl, [r1, #16]
	add	ip, ip, fp, ror #9
	add	r3, r3, #43520
	ldr	r7, [r2, #4]
	ldr	r6, [r0, #32]
	ldr	r9, [r0, #88]
	add	ip, ip, r8, ror #5
	eor	r4, r4, r4, lsr #1
	add	r3, r3, #162
	sub	r3, r3, sl, ror #27
	add	ip, ip, r4
	ldr	r4, [sp, #12]
	eor	r3, r3, r7
	rsb	r9, r6, r9
	add	r9, r9, r4
	add	ip, ip, r3
	add	ip, ip, r9
	eor	r5, r5, r5, lsr #2
	add	ip, ip, r5
	ldr	r4, [r0, #48]
	str	ip, [r0, #104]
	ldr	r5, [r0, #56]
	ldr	r6, [r0, #64]
	mov	r4, r4, ror #29
	ldr	r3, [r1, #56]
	add	r4, r4, r5, ror #25
	ldr	r5, [r0, #72]
	ldr	r7, [r1, #44]
	add	r4, r4, r6, ror #19
	ldr	r6, [r0, #80]
	ldr	r8, [r0, #88]
	add	r4, r4, r5, ror #16
	mov	r3, r3, ror #17
	add	r4, r4, r6, ror #13
	add	r3, r3, r7, ror #20
	ldr	r5, [r0, #100]
	ldr	r7, [r0, #96]
	ldr	sl, [r1, #20]
	add	r4, r4, r8, ror #9
	ldr	fp, [r0, #92]
	ldr	r8, [r2, #8]
	ldr	r6, [r0, #36]
	add	r4, r4, r7, ror #5
	eor	r5, r5, r5, lsr #1
	sub	r3, r3, #1879048201
	sub	r3, r3, sl, ror #26
	add	r4, r4, r5
	ldr	r5, [sp, #16]
	eor	r3, r3, r8
	rsb	r6, r6, fp
	add	r6, r6, r5
	add	r4, r4, r3
	add	r4, r4, r6
	eor	ip, ip, ip, lsr #2
	add	r4, r4, ip
	str	r4, [r0, #108]
	ldr	r5, [r0, #52]
	ldr	r3, [r1, #60]
	ldr	r7, [r1, #48]
	str	r6, [sp, #20]
	ldr	r6, [r0, #60]
	ldr	ip, [r0, #68]
	mov	r5, r5, ror #29
	mov	r3, r3, ror #16
	add	r5, r5, r6, ror #25
	add	r3, r3, r7, ror #19
	ldr	r6, [r0, #76]
	add	r5, r5, ip, ror #19
	add	r3, r3, #-1795162112
	ldr	ip, [r0, #84]
	add	r5, r5, r6, ror #16
	add	r3, r3, #5570560
	ldr	sl, [r1, #24]
	ldr	r8, [r0, #100]
	add	r5, r5, ip, ror #13
	add	r3, r3, #21760
	ldr	ip, [r0, #104]
	ldr	r7, [r2, #12]
	add	r5, r5, fp, ror #9
	ldr	r6, [r0, #40]
	ldr	fp, [r0, #96]
	add	r3, r3, #76
	add	r5, r5, r8, ror #5
	sub	r3, r3, sl, ror #25
	eor	ip, ip, ip, lsr #1
	eor	r3, r3, r7
	rsb	r6, r6, fp
	add	r5, r5, ip
	add	fp, r6, r9
	add	r5, r5, r3
	eor	r4, r4, r4, lsr #2
	ldr	ip, [r0, #56]
	add	r5, r5, fp
	add	r5, r5, r4
	ldr	r3, [r0, #64]
	str	r5, [r0, #112]
	ldr	r4, [r0, #72]
	mov	ip, ip, ror #29
	ldr	r6, [r0, #80]
	add	ip, ip, r3, ror #25
	ldr	r7, [r1, #0]
	add	ip, ip, r4, ror #19
	ldr	r3, .L3+4
	ldr	r4, [r0, #88]
	ldr	sl, [r1, #52]
	add	ip, ip, r6, ror #16
	ldr	r6, [r0, #96]
	ldr	r9, [r1, #28]
	add	ip, ip, r4, ror #13
	add	r3, r3, r7, ror #31
	ldr	r4, [r0, #108]
	ldr	r7, [r0, #104]
	ldr	r8, [r2, #16]
	add	ip, ip, r6, ror #9
	add	r3, r3, sl, ror #18
	ldr	r6, [r0, #44]
	ldr	sl, [r0, #100]
	add	ip, ip, r7, ror #5
	sub	r3, r3, r9, ror #24
	ldr	r7, [sp, #20]
	eor	r4, r4, r4, lsr #1
	rsb	r6, r6, sl
	eor	r3, r3, r8
	add	ip, ip, r4
	add	r9, r6, r7
	add	ip, ip, r3
	ldr	r4, [r0, #68]
	ldr	r3, [r0, #60]
	eor	r5, r5, r5, lsr #2
	add	ip, ip, r9
	add	ip, ip, r5
	ldr	r6, [r0, #104]
	mov	r4, r4, ror #25
	str	ip, [r0, #116]
	add	r4, r4, r3, ror #29
	ldr	r5, [r0, #76]
	add	r4, r4, r6
	ldr	r7, [r0, #84]
	ldr	r3, [r1, #4]
	add	r4, r4, r5, ror #19
	ldr	r5, [r0, #92]
	ldr	r8, [r1, #56]
	add	r4, r4, r7, ror #16
	add	r4, r4, r5, ror #13
	ldr	r7, [r0, #108]
	mov	r3, r3, ror #30
	add	r4, r4, sl, ror #9
	add	r3, r3, r8, ror #17
	ldr	sl, [r1, #32]
	ldr	r6, [r0, #48]
	ldr	r5, [r0, #112]
	add	r4, r4, r7, ror #5
	sub	r3, r3, #1610612746
	ldr	r7, [r2, #20]
	rsb	r4, r6, r4
	eor	r5, r5, r5, lsr #1
	sub	r3, r3, sl, ror #23
	eor	r3, r3, r7
	add	r4, r4, r5
	add	r4, r4, r3
	eor	ip, ip, ip, lsr #2
	add	r4, r4, fp
	add	r4, r4, ip
	str	r4, [r0, #120]
	ldr	r3, [r0, #72]
	ldr	r5, [r0, #64]
	ldr	ip, [r1, #8]
	ldr	r6, [r0, #108]
	mov	r3, r3, ror #25
	ldr	r8, [r1, #60]
	add	r3, r3, r5, ror #29
	ldr	r7, [r0, #80]
	add	r3, r3, r6
	ldr	r5, [r0, #88]
	mov	ip, ip, ror #29
	add	r3, r3, r7, ror #19
	add	ip, ip, r8, ror #16
	ldr	r6, [r0, #96]
	add	r3, r3, r5, ror #16
	add	ip, ip, #-1526726656
	ldr	r5, [r0, #104]
	add	r3, r3, r6, ror #13
	add	ip, ip, #5570560
	ldr	r6, [r0, #112]
	ldr	r8, [r1, #36]
	add	r3, r3, r5, ror #9
	add	ip, ip, #21760
	ldr	r1, [r0, #116]
	ldr	r5, [r0, #52]
	ldr	r7, [r2, #24]
	add	r3, r3, r6, ror #5
	add	ip, ip, #75
	rsb	r3, r5, r3
	sub	ip, ip, r8, ror #22
	eor	r1, r1, r1, lsr #1
	add	r3, r3, r1
	eor	ip, ip, r7
	add	r3, r3, ip
	add	r3, r3, r9
	eor	r4, r4, r4, lsr #2
	add	r3, r3, r4
	str	r3, [r0, #124]
	add	sp, sp, #24
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
.L4:
	.align	2
.L3:
	.word	1431655760
	.word	-1700091231
	.size	bmw_small_f1, .-bmw_small_f1
	.align	2
	.global	bmw_small_f0
	.type	bmw_small_f0, %function
bmw_small_f0:
	@ args = 0, pretend = 0, frame = 84
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldr	r3, [r1, #28]
	ldr	r7, [r2, #28]
	sub	sp, sp, #84
	ldr	fp, [r2, #20]
	ldr	r9, [r2, #40]
	ldr	sl, [r2, #52]
	ldr	r4, [r1, #52]
	eor	r3, r7, r3
	ldr	r6, [r1, #20]
	ldr	r5, [r1, #40]
	ldr	r8, [r2, #56]
	str	r3, [sp, #76]
	ldr	ip, [r1, #56]
	eor	sl, sl, r4
	eor	r5, r9, r5
	eor	r6, fp, r6
	ldr	r4, [sp, #76]
	ldr	r9, [r2, #32]
	str	r5, [sp, #72]
	eor	ip, r8, ip
	add	r3, r5, r6
	ldr	r5, [r1, #32]
	rsb	r8, r4, ip
	add	r3, r3, sl
	add	r3, r3, r8
	eor	r5, r9, r5
	str	r5, [sp, #56]
	str	r6, [sp, #68]
	ldr	r5, [r1, #4]
	ldr	r6, [r2, #4]
	str	ip, [sp, #80]
	mov	ip, r3, lsr #1
	eor	ip, ip, r3, asl #3
	eor	r6, r5, r6
	eor	ip, ip, r3, ror #28
	str	r6, [sp, #44]
	eor	ip, ip, r3, ror #13
	ldr	r7, [r2, #0]
	ldr	r4, [r1, #0]
	ldr	r6, [sp, #56]
	str	ip, [sp, #4]
	ldr	r3, [sp, #44]
	ldr	ip, [sp, #72]
	eor	r7, r7, r4
	ldr	r5, [r1, #16]
	add	r4, r6, sl
	add	r6, ip, r3
	ldr	ip, [r2, #16]
	eor	r5, ip, r5
	ldr	r3, [sp, #4]
	str	r5, [sp, #48]
	ldr	r5, [r1, #4]
	ldr	fp, [r2, #44]
	add	ip, r3, r5
	ldr	r3, [r1, #44]
	eor	fp, fp, r3
	ldr	r9, [r1, #12]
	str	fp, [sp, #64]
	add	r3, r2, #8
	ldmia	r3, {r3, r5}	@ phole ldm
	eor	fp, r5, r9
	ldr	r5, [r1, #8]
	str	r3, [sp, #40]
	ldr	r3, [r2, #24]
	str	r5, [sp, #36]
	ldr	r5, [r1, #24]
	str	r3, [sp, #32]
	ldr	r3, [r2, #36]
	str	r5, [sp, #28]
	ldr	r5, [r1, #36]
	str	r3, [sp, #24]
	ldr	r3, [r2, #48]
	ldr	r9, [r2, #60]
	str	r5, [sp, #20]
	ldr	r2, [r1, #60]
	ldr	r5, [r1, #48]
	add	r4, r4, r7
	rsb	r4, r6, r4
	str	r3, [sp, #16]
	ldr	r3, [sp, #48]
	str	ip, [r0, #0]
	str	r5, [sp, #12]
	str	r2, [sp, #8]
	ldr	r5, [sp, #64]
	mov	r2, r4, lsr #2
	rsb	ip, r7, r3
	eor	r2, r2, r4, asl #2
	rsb	r3, r5, sl
	rsb	ip, fp, ip
	ldr	r5, [r1, #16]
	eor	r2, r2, r4, ror #17
	add	ip, ip, r3
	eor	r2, r2, r4, ror #3
	add	r2, r2, r5
	mov	r3, ip, lsr #1
	str	r2, [r0, #12]
	eor	r3, r3, ip, asl #2
	eor	r3, r3, ip, ror #24
	ldr	r2, [r1, #28]
	ldr	r4, [sp, #28]
	eor	r3, r3, ip, ror #9
	ldr	ip, [sp, #32]
	add	r3, r3, r2
	eor	ip, ip, r4
	rsb	r2, fp, ip
	str	ip, [sp, #52]
	str	r3, [r0, #24]
	ldr	ip, [sp, #20]
	ldr	r5, [sp, #24]
	add	r8, r8, r2
	eor	r5, r5, ip
	ldr	r3, [r1, #40]
	add	r8, r8, r7
	rsb	r6, r5, r6
	eor	r8, r8, r8, lsr #1
	rsb	r6, r2, r6
	add	r8, r8, r3
	str	r5, [sp, #60]
	mov	r2, r6, lsr #2
	str	r8, [r0, #36]
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #12]
	eor	r2, r2, r6, asl #1
	eor	r8, r3, r4
	ldr	r5, [sp, #52]
	ldr	r3, [r1, #52]
	ldr	ip, [sp, #60]
	eor	r2, r2, r6, ror #20
	eor	r2, r2, r6, ror #7
	rsb	sl, r5, sl
	add	r2, r2, r3
	rsb	r5, ip, r8
	ldr	r3, [sp, #48]
	add	r4, sl, r5
	ldr	r6, [sp, #8]
	rsb	r4, r3, r4
	str	r2, [r0, #48]
	eor	r9, r9, r6
	mov	r3, r4, lsr #1
	ldr	r6, [sp, #76]
	eor	r3, r3, r4, asl #3
	ldr	r2, [r1, #0]
	add	ip, r9, r6
	eor	r3, r3, r4, ror #28
	eor	r3, r3, r4, ror #13
	add	ip, ip, r7
	rsb	ip, r5, ip
	add	r3, r3, r2
	str	r3, [r0, #60]
	ldr	r4, [sp, #36]
	ldr	r3, [sp, #40]
	mov	r2, ip, lsr #2
	eor	r2, r2, ip, asl #1
	eor	r6, r3, r4
	eor	r2, r2, ip, ror #20
	rsb	r5, r6, r9
	rsb	fp, r8, fp
	eor	r2, r2, ip, ror #7
	ldr	ip, [sp, #72]
	ldr	r3, [r1, #12]
	add	r4, r5, fp
	add	r4, r4, ip
	add	r2, r2, r3
	mov	r3, r4, lsr #1
	eor	r3, r3, r4, asl #3
	eor	r3, r3, r4, ror #28
	str	r3, [sp, #0]
	ldr	r3, [sp, #68]
	str	r2, [r0, #8]
	rsb	ip, r3, #0
	ldr	r3, [sp, #0]
	ldr	r2, [r1, #24]
	eor	r4, r3, r4, ror #13
	add	r3, r4, r2
	add	sl, ip, sl
	rsb	sl, r5, sl
	str	r3, [r0, #20]
	ldr	r5, [sp, #56]
	ldr	r3, [sp, #68]
	mov	r2, sl, lsr #2
	rsb	r4, r3, r5
	ldr	r5, [sp, #60]
	eor	r2, r2, sl, asl #2
	ldr	r3, [r1, #36]
	rsb	r7, r7, r5
	eor	r2, r2, sl, ror #17
	add	r7, r7, r4
	eor	r2, r2, sl, ror #3
	rsb	r7, r6, r7
	add	r2, r2, r3
	str	r2, [r0, #32]
	mov	r3, r7, lsr #1
	eor	r3, r3, r7, asl #2
	ldr	r2, [r1, #48]
	eor	r3, r3, r7, ror #24
	eor	r3, r3, r7, ror #9
	add	r3, r3, r2
	ldr	r7, [sp, #64]
	ldr	r5, [sp, #80]
	str	r3, [r0, #44]
	add	r3, r4, fp
	rsb	r2, r7, r3
	add	r4, r5, r7
	ldr	r7, [sp, #56]
	add	r9, r9, r7
	ldr	r7, [sp, #52]
	ldr	r3, [r1, #60]
	eor	r5, r2, r2, lsr #1
	add	r2, r4, r7
	add	r3, r5, r3
	rsb	r5, r9, r2
	str	r3, [r0, #56]
	mov	r3, r5, lsr #1
	eor	r3, r3, r5, asl #2
	ldr	r2, [r1, #8]
	eor	r3, r3, r5, ror #24
	eor	r3, r3, r5, ror #9
	add	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [sp, #44]
	ldr	r5, [sp, #60]
	add	r2, r6, r3
	ldr	r7, [sp, #80]
	add	r2, r2, r5
	rsb	ip, r7, ip
	rsb	r2, r4, r2
	ldr	r4, [sp, #48]
	add	ip, ip, r3
	rsb	ip, r4, ip
	ldr	r3, [r1, #20]
	rsb	ip, r8, ip
	eor	r2, r2, r2, lsr #1
	add	r2, r2, r3
	mov	r3, ip, lsr #2
	eor	r3, r3, ip, asl #1
	str	r2, [r0, #16]
	eor	r3, r3, ip, ror #20
	ldr	r5, [sp, #76]
	eor	r3, r3, ip, ror #7
	ldr	ip, [sp, #44]
	ldr	r2, [r1, #32]
	rsb	r9, ip, r9
	mov	r7, r4
	add	r4, r5, r4
	rsb	r9, r4, r9
	add	r3, r3, r2
	str	r3, [r0, #28]
	mov	r2, r9, lsr #1
	ldr	r3, [sp, #64]
	eor	r2, r2, r9, asl #3
	ldr	r5, [sp, #72]
	add	ip, r3, r6
	eor	r2, r2, r9, ror #28
	ldr	r3, [r1, #44]
	eor	r2, r2, r9, ror #13
	add	r6, ip, r5
	add	r6, r6, r4
	add	r2, r2, r3
	str	r2, [r0, #40]
	mov	r3, r6, lsr #2
	eor	r3, r3, r6, asl #2
	ldr	r2, [r1, #56]
	eor	r3, r3, r6, ror #17
	eor	r3, r3, r6, ror #3
	add	r3, r3, r2
	str	r3, [r0, #52]
	add	sp, sp, #84
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.size	bmw_small_f0, .-bmw_small_f0
	.align	2
	.global	bmw224_init
	.type	bmw224_init, %function
bmw224_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, #65536
	add	r3, r3, #512
	add	r3, r3, #3
	str	r3, [r0, #0]
	mov	r2, r0
	add	r1, r0, #60
.L8:
	ldr	r3, [r2, #0]
	add	r3, r3, #67108864
	add	r3, r3, #262144
	add	r3, r3, #1024
	add	r3, r3, #4
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L8
	mov	r3, #0
	str	r3, [r0, #64]
	bx	lr
	.size	bmw224_init, .-bmw224_init
	.align	2
	.global	bmw256_init
	.type	bmw256_init, %function
bmw256_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L16
	str	r3, [r0, #0]
	mov	r2, r0
	add	r1, r0, #60
.L13:
	ldr	r3, [r2, #0]
	add	r3, r3, #67108864
	add	r3, r3, #262144
	add	r3, r3, #1024
	add	r3, r3, #4
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L13
	mov	r3, #0
	str	r3, [r0, #64]
	bx	lr
.L17:
	.align	2
.L16:
	.word	1078018627
	.size	bmw256_init, .-bmw256_init
	.align	2
	.global	bmw256_ctx2hash
	.type	bmw256_ctx2hash, %function
bmw256_ctx2hash:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r1, r1, #32
	mov	r2, #32
	b	memcpy
	.size	bmw256_ctx2hash, .-bmw256_ctx2hash
	.align	2
	.global	bmw224_ctx2hash
	.type	bmw224_ctx2hash, %function
bmw224_ctx2hash:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r1, r1, #36
	mov	r2, #28
	b	memcpy
	.size	bmw224_ctx2hash, .-bmw224_ctx2hash
	.align	2
	.global	bmw_small_nextBlock
	.type	bmw_small_nextBlock, %function
bmw_small_nextBlock:
	@ args = 0, pretend = 0, frame = 192
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #192
	mov	r7, r0
	mov	r8, r1
	add	r4, sp, #64
	mov	r0, r4
	mov	r1, r7
	mov	r2, r8
	bl	bmw_small_f0
	mov	r0, r4
	mov	r1, r8
	mov	r2, r7
	bl	bmw_small_f1
	add	r1, sp, #128
	ldmia	r1, {r1, r2, r3, ip}	@ phole ldm
	str	ip, [sp, #48]
	ldr	lr, [sp, #168]
	eor	ip, r1, r2
	str	r1, [sp, #60]
	ldr	r1, [sp, #144]
	str	r2, [sp, #56]
	ldr	r2, [sp, #172]
	str	lr, [sp, #28]
	str	r1, [sp, #44]
	str	r3, [sp, #52]
	ldr	r1, [sp, #176]
	eor	ip, ip, r3
	ldr	r3, [sp, #48]
	ldr	fp, [sp, #164]
	str	r2, [sp, #24]
	ldr	lr, [sp, #148]
	ldr	r2, [sp, #28]
	str	r1, [sp, #20]
	eor	ip, ip, r3
	ldr	r1, [sp, #152]
	ldr	r3, [sp, #44]
	str	lr, [sp, #40]
	eor	r0, r2, fp
	ldr	lr, [sp, #24]
	ldr	r2, [sp, #180]
	str	r1, [sp, #36]
	eor	ip, ip, r3
	ldr	r1, [sp, #184]
	ldr	r3, [sp, #40]
	str	r2, [sp, #16]
	eor	r0, r0, lr
	ldr	lr, [sp, #20]
	eor	ip, ip, r3
	str	r1, [sp, #12]
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #188]
	eor	r0, r0, lr
	ldr	r2, [sp, #36]
	ldr	lr, [sp, #156]
	eor	r0, r0, r3
	str	r1, [sp, #8]
	ldr	r3, [sp, #12]
	eor	ip, ip, r2
	eor	r0, r0, r3
	ldr	r2, [sp, #160]
	eor	ip, ip, lr
	ldr	r9, [r8, #0]
	eor	r0, r0, r1
	add	r1, sp, #60
	ldmia	r1, {r1, r3}	@ phole ldm
	str	r2, [sp, #32]
	eor	r2, ip, r2
	eor	r0, r0, r2
	eor	r9, r9, r1, lsr #5
	eor	r2, r2, r3
	eor	r9, r9, r0, asl #5
	add	r9, r9, r2
	str	r9, [r7, #0]
	ldr	r3, [sp, #68]
	ldr	sl, [r8, #4]
	ldr	r2, [sp, #56]
	eor	r3, r3, fp
	eor	sl, sl, r2, asl #8
	eor	r3, r3, ip
	eor	sl, sl, r0, lsr #7
	add	sl, sl, r3
	str	sl, [r7, #4]
	ldr	r3, [sp, #72]
	ldr	r1, [sp, #28]
	ldr	r2, [r8, #8]
	eor	r3, r3, r1
	ldr	r1, [sp, #52]
	eor	r2, r2, r1, asl #5
	eor	r2, r2, r0, lsr #5
	eor	r3, r3, ip
	add	r3, r2, r3
	str	r3, [r7, #8]
	ldr	r1, [sp, #24]
	str	r3, [sp, #4]
	ldr	r3, [sp, #76]
	ldr	r2, [r8, #12]
	eor	r3, r3, r1
	ldr	r1, [sp, #48]
	eor	r2, r2, r1, asl #5
	eor	r2, r2, r0, lsr #1
	eor	r3, r3, ip
	add	r3, r2, r3
	str	r3, [r7, #12]
	ldr	r2, [sp, #20]
	str	r3, [sp, #0]
	ldr	r3, [sp, #80]
	ldr	r1, [r8, #16]
	eor	r3, r3, r2
	ldr	r2, [sp, #44]
	eor	r1, r1, r2
	eor	r3, r3, ip
	eor	r1, r1, r0, lsr #3
	add	r1, r1, r3
	str	r1, [r7, #16]
	ldr	r3, [sp, #84]
	ldr	r2, [sp, #16]
	ldr	r4, [r8, #20]
	eor	r3, r3, r2
	ldr	r2, [sp, #40]
	eor	r4, r4, r2, lsr #6
	eor	r3, r3, ip
	eor	r4, r4, r0, asl #6
	add	r4, r4, r3
	str	r4, [r7, #20]
	ldr	r3, [sp, #36]
	ldr	r5, [r8, #24]
	ldr	r2, [sp, #12]
	eor	r5, r5, r3, asl #6
	ldr	r3, [sp, #88]
	eor	r3, r3, r2
	eor	r3, r3, ip
	eor	r5, r5, r0, lsr #4
	add	r5, r5, r3
	str	r5, [r7, #24]
	ldr	r6, [r8, #28]
	ldr	r3, [sp, #92]
	ldr	r2, [sp, #8]
	eor	r6, r6, lr, asl #2
	eor	r3, r3, r2
	eor	r3, r3, ip
	eor	r6, r6, r0, lsr #11
	add	r6, r6, r3
	str	r6, [r7, #28]
	ldr	r3, [sp, #96]
	ldr	r2, [r8, #32]
	eor	r3, r3, lr
	ldr	lr, [sp, #32]
	eor	r3, r3, ip, asl #8
	eor	r2, r2, lr
	add	r3, r3, r1, ror #23
	eor	r2, r2, r0
	add	r3, r3, r2
	str	r3, [r7, #32]
	ldr	r3, [sp, #100]
	ldr	r1, [sp, #60]
	ldr	r2, [r8, #36]
	eor	r3, r3, r1
	eor	r3, r3, ip, lsr #6
	eor	r2, r2, fp
	eor	r2, r2, r0
	add	r3, r3, r4, ror #22
	add	r3, r3, r2
	str	r3, [r7, #36]
	ldr	r3, [sp, #104]
	ldr	lr, [sp, #56]
	ldr	r2, [r8, #40]
	ldr	r1, [sp, #28]
	eor	r3, r3, lr
	eor	r2, r2, r1
	eor	r3, r3, ip, asl #6
	eor	r2, r2, r0
	add	r3, r3, r5, ror #21
	add	r3, r3, r2
	str	r3, [r7, #40]
	ldr	r3, [sp, #108]
	ldr	lr, [sp, #52]
	ldr	r2, [r8, #44]
	ldr	r1, [sp, #24]
	eor	r3, r3, lr
	eor	r2, r2, r1
	eor	r3, r3, ip, asl #4
	eor	r2, r2, r0
	add	r3, r3, r6, ror #20
	add	r3, r3, r2
	str	r3, [r7, #44]
	ldr	r3, [sp, #112]
	ldr	lr, [sp, #48]
	ldr	r2, [r8, #48]
	ldr	r1, [sp, #20]
	eor	r3, r3, lr
	eor	r2, r2, r1
	eor	r3, r3, ip, lsr #3
	eor	r2, r2, r0
	add	r3, r3, r9, ror #19
	add	r3, r3, r2
	str	r3, [r7, #48]
	ldr	r3, [sp, #116]
	ldr	lr, [sp, #44]
	ldr	r2, [r8, #52]
	ldr	r1, [sp, #16]
	eor	r3, r3, lr
	eor	r2, r2, r1
	eor	r3, r3, ip, lsr #4
	eor	r2, r2, r0
	add	r3, r3, sl, ror #18
	add	r3, r3, r2
	str	r3, [r7, #52]
	ldr	lr, [sp, #40]
	ldr	r3, [sp, #120]
	ldr	r1, [sp, #12]
	ldr	r2, [r8, #56]
	eor	r3, r3, lr
	ldr	lr, [sp, #4]
	eor	r2, r2, r1
	eor	r3, r3, ip, lsr #7
	add	r3, r3, lr, ror #17
	eor	r2, r2, r0
	add	r3, r3, r2
	str	r3, [r7, #56]
	ldr	r3, [sp, #124]
	ldr	r2, [sp, #36]
	eor	r3, r3, r2
	ldr	r1, [r8, #60]
	eor	r3, r3, ip, lsr #2
	ldr	ip, [sp, #8]
	ldr	lr, [sp, #0]
	ldr	r2, [r7, #64]
	eor	r1, r1, ip
	add	r3, r3, lr, ror #16
	eor	r1, r1, r0
	add	r3, r3, r1
	add	r2, r2, #1
	str	r2, [r7, #64]
	str	r3, [r7, #60]
	add	sp, sp, #192
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
	.size	bmw_small_nextBlock, .-bmw_small_nextBlock
	.align	2
	.global	bmw224_nextBlock
	.type	bmw224_nextBlock, %function
bmw224_nextBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	bmw_small_nextBlock
	.size	bmw224_nextBlock, .-bmw224_nextBlock
	.align	2
	.global	bmw256_nextBlock
	.type	bmw256_nextBlock, %function
bmw256_nextBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	bmw_small_nextBlock
	.size	bmw256_nextBlock, .-bmw256_nextBlock
	.align	2
	.global	bmw_small_lastBlock
	.type	bmw_small_lastBlock, %function
bmw_small_lastBlock:
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r2, r2, asl #16
	mov	ip, #508
	mov	r4, r2, lsr #16
	add	ip, ip, #3
	cmp	r4, ip
	sub	sp, sp, #400
	mov	fp, r0
	mov	r8, r1
	bls	.L29
	mov	r7, ip
	mov	r6, r1
	mov	r5, r4
.L30:
	mov	r1, r6
	mov	r0, fp
	bl	bmw_small_nextBlock
	sub	r3, r5, #512
	mov	r3, r3, asl #16
	mov	r5, r3, lsr #16
	cmp	r5, r7
	add	r6, r6, #64
	bhi	.L30
	sub	r2, r4, #512
	mov	r2, r2, asl #16
	mov	r3, r2, lsr #25
	add	r3, r3, #1
	mov	r2, r2, lsr #16
	mov	r3, r3, asl #6
	mov	r4, r2, asl #23
	add	r8, r8, r3
	mov	r4, r4, lsr #23
.L29:
	add	r9, sp, #336
	mov	r1, #0
	mov	r2, #64
	mov	r0, r9
	bl	memset
	add	r2, r4, #7
	mov	r1, r8
	mov	r2, r2, asr #3
	mov	r0, r9
	bl	memcpy
	add	r0, sp, #400
	add	ip, r0, r4, lsr #3
	ldrb	r2, [ip, #-64]	@ zero_extendqisi2
	and	r1, r4, #7
	mov	r3, #128
	orr	r2, r2, r3, asr r1
	cmp	r4, #448
	strb	r2, [ip, #-64]
	bge	.L37
	add	r2, sp, #348
	add	r3, sp, #352
	add	r7, sp, #356
	add	r8, sp, #360
	add	ip, sp, #364
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r7, [sp, #4]
	str	r8, [sp, #20]
	str	ip, [sp, #16]
	ldr	r1, [fp, #64]
	add	r5, sp, #340
	add	r6, sp, #344
.L31:
	mov	r2, #512
	mov	r3, r4
	mov	r4, #0
	umlal	r3, r4, r2, r1
	mov	r0, fp
	mov	r1, r9
	str	r3, [r9, #56]
	str	r4, [r9, #60]
	bl	bmw_small_nextBlock
	mov	r2, #64
	mov	r0, r9
	mov	r1, #170
	bl	memset
	mov	r2, #0
	mov	r3, #160
.L32:
	strb	r3, [r9, r2]
	add	r2, r2, #4
	add	r3, r3, #1
	cmp	r2, #64
	and	r3, r3, #255
	bne	.L32
	add	r4, sp, #208
	mov	r0, r4
	mov	r1, r9
	mov	r2, fp
	bl	bmw_small_f0
	mov	r0, r4
	mov	r1, fp
	mov	r2, r9
	bl	bmw_small_f1
	add	r8, sp, #272
	ldmia	r8, {r8, ip, lr}	@ phole ldm
	add	r2, sp, #308
	ldmia	r2, {r2, r3}	@ phole ldm
	eor	r1, ip, r8
	str	ip, [sp, #200]
	ldr	ip, [sp, #292]
	ldr	r0, [sp, #284]
	str	r8, [sp, #204]
	ldr	r8, [sp, #316]
	ldr	r7, [sp, #288]
	str	ip, [sp, #184]
	str	lr, [sp, #196]
	eor	ip, r3, r2
	eor	r1, r1, lr
	ldr	lr, [sp, #320]
	str	r2, [sp, #172]
	eor	r1, r1, r0
	ldr	r2, [sp, #324]
	eor	ip, ip, r8
	str	r3, [sp, #168]
	str	r7, [sp, #188]
	ldr	r3, [sp, #184]
	eor	r1, r1, r7
	eor	ip, ip, lr
	ldr	r7, [sp, #328]
	eor	ip, ip, r2
	eor	r1, r1, r3
	str	r7, [sp, #152]
	eor	ip, ip, r7
	ldr	r3, [fp, #0]
	ldr	r7, [sp, #204]
	str	r0, [sp, #192]
	str	r8, [sp, #164]
	ldr	r0, [sp, #296]
	ldr	r8, [sp, #332]
	str	lr, [sp, #160]
	add	sl, sp, #300
	ldmia	sl, {sl, lr}	@ phole ldm
	eor	r3, r3, r7, lsr #5
	str	r0, [sp, #180]
	str	r2, [sp, #156]
	eor	r1, r1, r0
	ldr	r2, [fp, #4]
	str	r8, [sp, #148]
	str	lr, [sp, #176]
	eor	ip, ip, r8
	str	r3, [sp, #144]
	ldr	r8, [sp, #200]
	eor	r1, r1, sl
	eor	r0, r1, lr
	ldr	r3, [sp, #172]
	eor	lr, r2, r8, asl #8
	ldr	r2, [sp, #212]
	ldr	r7, [sp, #144]
	eor	r2, r3, r2
	ldr	r3, [sp, #208]
	eor	ip, ip, r0
	eor	r7, r7, ip, asl #5
	eor	r0, r0, r3
	eor	r2, r2, r1
	eor	r3, lr, ip, lsr #7
	add	r0, r7, r0
	add	r2, r3, r2
	str	r0, [r9, #0]
	str	r2, [r5, #0]
	ldr	r8, [fp, #8]
	ldr	r3, [sp, #216]
	ldr	lr, [sp, #168]
	str	r0, [sp, #136]
	ldr	r0, [sp, #196]
	eor	r3, lr, r3
	eor	r8, r8, r0, asl #5
	eor	r3, r3, r1
	eor	r8, r8, ip, lsr #5
	add	r8, r8, r3
	str	r8, [r6, #0]
	ldr	r7, [fp, #12]
	ldr	r3, [sp, #220]
	ldr	r5, [sp, #192]
	str	r2, [sp, #132]
	ldr	r2, [sp, #164]
	eor	r7, r7, r5, asl #5
	eor	r3, r2, r3
	eor	r3, r3, r1
	ldr	lr, [sp, #12]
	eor	r7, r7, ip, lsr #1
	add	r7, r7, r3
	str	r7, [lr, #0]
	ldr	r6, [fp, #16]
	ldr	r3, [sp, #224]
	ldr	r0, [sp, #160]
	ldr	r2, [sp, #188]
	eor	r3, r0, r3
	eor	r6, r2, r6
	eor	r3, r3, r1
	eor	r6, r6, ip, lsr #3
	add	r6, r6, r3
	ldr	r3, [sp, #8]
	str	r6, [r3, #0]
	ldr	r5, [fp, #20]
	ldr	lr, [sp, #184]
	ldr	r3, [sp, #228]
	ldr	r0, [sp, #156]
	eor	r5, r5, lr, lsr #6
	eor	r3, r0, r3
	eor	r3, r3, r1
	ldr	r2, [sp, #4]
	eor	r5, r5, ip, asl #6
	add	r5, r5, r3
	str	r5, [r2, #0]
	ldr	lr, [sp, #152]
	ldr	r0, [sp, #180]
	ldr	r4, [fp, #24]
	ldr	r3, [sp, #232]
	eor	r4, r4, r0, asl #6
	eor	r3, lr, r3
	eor	r3, r3, r1
	eor	r4, r4, ip, lsr #4
	ldr	r2, [sp, #20]
	add	r4, r4, r3
	str	r4, [r2, #0]
	ldr	r0, [fp, #28]
	ldr	r3, [sp, #236]
	ldr	lr, [sp, #148]
	eor	r0, r0, sl, asl #2
	eor	r3, lr, r3
	eor	r3, r3, r1
	ldr	r2, [sp, #16]
	eor	r0, r0, ip, lsr #11
	add	r0, r0, r3
	str	r0, [r2, #0]
	ldr	r3, [sp, #240]
	ldr	r2, [sp, #244]
	eor	sl, sl, r3
	ldr	r3, [sp, #204]
	ldr	lr, [sp, #200]
	eor	r2, r3, r2
	ldr	r3, [sp, #248]
	eor	r3, lr, r3
	str	r2, [sp, #124]
	str	r3, [sp, #112]
	ldr	r2, [sp, #252]
	ldr	r3, [sp, #196]
	ldr	lr, [sp, #192]
	eor	r2, r3, r2
	ldr	r3, [sp, #256]
	eor	r3, lr, r3
	str	r2, [sp, #100]
	str	r3, [sp, #88]
	ldr	r2, [sp, #260]
	ldr	r3, [sp, #188]
	ldr	lr, [sp, #184]
	eor	r2, r3, r2
	ldr	r3, [sp, #264]
	eor	r3, lr, r3
	str	r2, [sp, #72]
	str	r3, [sp, #52]
	ldr	r2, [sp, #268]
	ldr	r3, [sp, #180]
	eor	lr, r3, r2
	ldr	r3, [fp, #32]
	ldr	r2, [sp, #176]
	eor	r2, r2, r3
	str	r2, [sp, #128]
	ldr	r3, [fp, #36]
	ldr	r2, [sp, #172]
	eor	r2, r2, r3
	str	r2, [sp, #116]
	ldr	r3, [fp, #40]
	ldr	r2, [sp, #168]
	eor	r2, r2, r3
	str	r2, [sp, #104]
	ldr	r3, [fp, #44]
	ldr	r2, [sp, #164]
	eor	r2, r2, r3
	str	r2, [sp, #92]
	ldr	r3, [fp, #48]
	ldr	r2, [sp, #160]
	eor	r2, r2, r3
	str	r2, [sp, #76]
	ldr	r3, [fp, #52]
	ldr	r2, [sp, #156]
	eor	r2, r2, r3
	str	r2, [sp, #60]
	ldr	r3, [fp, #56]
	ldr	r2, [sp, #152]
	eor	lr, lr, r1, lsr #2
	eor	r2, r2, r3
	str	r2, [sp, #44]
	str	lr, [sp, #36]
	ldr	r2, [fp, #60]
	ldr	lr, [sp, #148]
	ldr	r3, [sp, #124]
	eor	lr, lr, r2
	ldr	r2, [sp, #112]
	str	lr, [sp, #28]
	eor	lr, r3, r1, lsr #6
	ldr	r3, [sp, #100]
	eor	r2, r2, r1, asl #6
	str	r2, [sp, #0]
	eor	r2, r3, r1, asl #4
	ldr	r3, [sp, #88]
	eor	r3, r3, r1, lsr #3
	str	r3, [sp, #84]
	ldr	r3, [sp, #72]
	eor	r3, r3, r1, lsr #4
	str	r3, [sp, #68]
	ldr	r3, [sp, #52]
	add	lr, lr, r5, ror #22
	add	r2, r2, r0, ror #20
	ldr	r5, [sp, #0]
	ldr	r0, [sp, #136]
	eor	sl, sl, r1, asl #8
	str	lr, [sp, #120]
	eor	r1, r3, r1, lsr #7
	str	r2, [sp, #96]
	ldr	r3, [sp, #132]
	ldr	lr, [sp, #84]
	ldr	r2, [sp, #68]
	add	r4, r5, r4, ror #21
	add	lr, lr, r0, ror #19
	add	r2, r2, r3, ror #18
	str	r4, [sp, #108]
	str	lr, [sp, #80]
	str	r2, [sp, #64]
	ldr	r5, [sp, #36]
	add	r7, r5, r7, ror #16
	str	r7, [sp, #32]
	ldr	r7, [sp, #28]
	add	r1, r1, r8, ror #17
	ldr	r0, [sp, #116]
	ldr	r8, [sp, #128]
	ldr	r2, [sp, #104]
	ldr	r3, [sp, #92]
	eor	r7, r7, ip
	str	r7, [sp, #24]
	eor	lr, r8, ip
	eor	r5, r0, ip
	eor	r8, r3, ip
	eor	r7, r2, ip
	ldr	r0, [sp, #76]
	ldr	r3, [sp, #44]
	ldr	r2, [sp, #60]
	add	sl, sl, r6, ror #23
	eor	r2, r2, ip
	eor	r6, r0, ip
	add	sl, sl, lr
	eor	ip, r3, ip
	ldr	lr, [sp, #108]
	str	ip, [sp, #40]
	str	r1, [sp, #48]
	str	r2, [sp, #56]
	ldr	ip, [sp, #120]
	add	r7, lr, r7
	mov	r4, r9
	ldmia	r4!, {r0, r1, r2, r3}
	str	r7, [r9, #40]
	ldr	r7, [sp, #96]
	add	r5, ip, r5
	ldr	lr, [sp, #56]
	ldr	ip, [sp, #64]
	str	r5, [r9, #36]
	add	r5, r7, r8
	ldr	r8, [sp, #80]
	add	r7, r8, r6
	add	r6, ip, lr
	ldr	ip, [sp, #48]
	ldr	lr, [sp, #40]
	add	r8, ip, lr
	ldr	ip, [sp, #32]
	ldr	lr, [sp, #24]
	add	ip, ip, lr
	str	ip, [r9, #60]
	mov	ip, fp
	str	sl, [r9, #32]
	str	r5, [r9, #44]
	str	r7, [r9, #48]
	str	r6, [r9, #52]
	str	r8, [r9, #56]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	add	sp, sp, #400
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
.L37:
	mov	r0, fp
	mov	r1, r9
	bl	bmw_small_nextBlock
	ldr	r1, [fp, #64]
	mov	r3, #0
	str	r3, [sp, #388]
	str	r3, [sp, #336]
	str	r3, [sp, #340]
	str	r3, [sp, #344]
	str	r3, [sp, #348]
	str	r3, [sp, #352]
	str	r3, [sp, #356]
	str	r3, [sp, #360]
	str	r3, [sp, #364]
	str	r3, [sp, #368]
	str	r3, [sp, #372]
	str	r3, [sp, #376]
	str	r3, [sp, #380]
	str	r3, [sp, #384]
	sub	r1, r1, #1
	add	lr, sp, #348
	add	r0, sp, #352
	add	r2, sp, #356
	add	r3, sp, #360
	add	r7, sp, #364
	str	r1, [fp, #64]
	add	r5, sp, #340
	add	r6, sp, #344
	str	lr, [sp, #12]
	str	r0, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp, #20]
	str	r7, [sp, #16]
	b	.L31
	.size	bmw_small_lastBlock, .-bmw_small_lastBlock
	.align	2
	.global	bmw256_lastBlock
	.type	bmw256_lastBlock, %function
bmw256_lastBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r2, asl #16
	mov	r2, r2, lsr #16
	b	bmw_small_lastBlock
	.size	bmw256_lastBlock, .-bmw256_lastBlock
	.align	2
	.global	bmw224_lastBlock
	.type	bmw224_lastBlock, %function
bmw224_lastBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r2, asl #16
	mov	r2, r2, lsr #16
	b	bmw_small_lastBlock
	.size	bmw224_lastBlock, .-bmw224_lastBlock
	.align	2
	.global	bmw256
	.type	bmw256, %function
bmw256:
	@ args = 0, pretend = 0, frame = 68
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	ldr	r3, .L49
	sub	sp, sp, #68
	add	r6, sp, #68
	str	r3, [r6, #-68]!
	mov	sl, r1
	mov	r7, r2
	mov	r9, r0
	mov	r2, sp
	add	r1, sp, #60
.L43:
	ldr	r3, [r2, #0]
	add	r3, r3, #67108864
	add	r3, r3, #262144
	add	r3, r3, #1024
	add	r3, r3, #4
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L43
	mov	r2, #508
	add	r2, r2, #3
	mov	r3, #0
	cmp	r7, r2
	str	r3, [sp, #64]
	bls	.L44
	mov	r8, r2
	mov	r5, sl
	mov	r4, r7
.L45:
	mov	r1, r5
	sub	r4, r4, #512
	mov	r0, sp
	bl	bmw_small_nextBlock
	cmp	r4, r8
	add	r5, r5, #64
	bhi	.L45
	sub	r2, r7, #512
	mov	r3, r2, lsr #9
	add	r3, r3, #1
	mov	r3, r3, asl #6
	mov	r7, r2, asl #23
	add	sl, sl, r3
	mov	r7, r7, lsr #23
.L44:
	mov	r2, r7, asl #16
	mov	r1, sl
	mov	r0, sp
	mov	r2, r2, lsr #16
	bl	bmw_small_lastBlock
	mov	r0, r9
	add	r1, sp, #32
	mov	r2, #32
	bl	memcpy
	add	sp, sp, #68
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
.L50:
	.align	2
.L49:
	.word	1078018627
	.size	bmw256, .-bmw256
	.align	2
	.global	bmw224
	.type	bmw224, %function
bmw224:
	@ args = 0, pretend = 0, frame = 68
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	mov	r3, #65536
	sub	sp, sp, #68
	add	r3, r3, #512
	add	r3, r3, #3
	add	r6, sp, #68
	str	r3, [r6, #-68]!
	mov	sl, r1
	mov	r7, r2
	mov	r9, r0
	mov	r2, sp
	add	r1, sp, #60
.L52:
	ldr	r3, [r2, #0]
	add	r3, r3, #67108864
	add	r3, r3, #262144
	add	r3, r3, #1024
	add	r3, r3, #4
	str	r3, [r2, #4]!
	cmp	r2, r1
	bne	.L52
	mov	r2, #508
	add	r2, r2, #3
	mov	r3, #0
	cmp	r7, r2
	str	r3, [sp, #64]
	bls	.L53
	mov	r8, r2
	mov	r5, sl
	mov	r4, r7
.L54:
	mov	r1, r5
	sub	r4, r4, #512
	mov	r0, sp
	bl	bmw_small_nextBlock
	cmp	r4, r8
	add	r5, r5, #64
	bhi	.L54
	sub	r2, r7, #512
	mov	r3, r2, lsr #9
	add	r3, r3, #1
	mov	r3, r3, asl #6
	mov	r7, r2, asl #23
	add	sl, sl, r3
	mov	r7, r7, lsr #23
.L53:
	mov	r2, r7, asl #16
	mov	r1, sl
	mov	r0, sp
	mov	r2, r2, lsr #16
	bl	bmw_small_lastBlock
	mov	r0, r9
	add	r1, sp, #36
	mov	r2, #28
	bl	memcpy
	add	sp, sp, #68
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
	.size	bmw224, .-bmw224
	.ident	"GCC: (GNU) 4.3.2"
