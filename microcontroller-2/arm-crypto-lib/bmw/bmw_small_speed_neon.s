	.cpu cortex-a8
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"bmw_small_speed_neon.c"
	.text
	.align	2
	.type	bmw_small_f1, %function
bmw_small_f1:
	@ args = 0, pretend = 0, frame = 13368
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, fp}
	add	fp, sp, #4
	sub	sp, sp, #13312
	sub	sp, sp, #56
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	str	r0, [ip, r3]
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	str	r1, [r0, r3]
	movw	r3, #52172
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #53168
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, .L2
	vldr	d17, .L2+8
	vstmia	r3, {d16-d17}
	movw	r3, #53152
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #21844
	movt	r3, 5461
	vdup.32	q8, r3
	vstmia	r2, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #12
	vldmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #40
	vldmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L2+16
	vldr	d19, .L2+24
	vstr	d18, [fp, #-20]
	vstr	d19, [fp, #-12]
	movw	r3, #63456
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63440
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-20]
	vldr	d17, [fp, #-12]
	vstmia	r3, {d16-d17}
	movw	r3, #63456
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L2+32
	vldr	d21, .L2+40
	vstr	d20, [fp, #-36]
	vstr	d21, [fp, #-28]
	movw	r3, #63424
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63408
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-36]
	vldr	d17, [fp, #-28]
	vstmia	r3, {d16-d17}
	movw	r3, #63424
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63408
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #63392
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63392
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63376
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L2+48
	vldr	d19, .L2+56
	vstr	d18, [fp, #-52]
	vstr	d19, [fp, #-44]
	movw	r3, #63360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63344
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-52]
	vldr	d17, [fp, #-44]
	vstmia	r3, {d16-d17}
	movw	r3, #63360
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L2+64
	vldr	d21, .L2+72
	vstr	d20, [fp, #-68]
	vstr	d21, [fp, #-60]
	movw	r3, #63328
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63312
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-68]
	vldr	d17, [fp, #-60]
	vstmia	r3, {d16-d17}
	movw	r3, #63328
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #63296
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63280
	movt	r3, 65535
	sub	r2, fp, #4
	b	.L3
.L4:
	.align	3
.L2:
	.word	1431655760
	.word	1521134245
	.word	1610612730
	.word	1700091215
	.word	1
	.word	2
	.word	3
	.word	4
	.word	-31
	.word	-30
	.word	-29
	.word	-28
	.word	4
	.word	5
	.word	6
	.word	7
	.word	-28
	.word	-27
	.word	-26
	.word	-25
	.word	11
	.word	12
	.word	13
	.word	14
	.word	-21
	.word	-20
	.word	-19
	.word	-18
.L3:
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L2+80
	vldr	d19, .L2+88
	vstr	d18, [fp, #-84]
	vstr	d19, [fp, #-76]
	movw	r3, #63264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63248
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-84]
	vldr	d17, [fp, #-76]
	vstmia	r3, {d16-d17}
	movw	r3, #63264
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63248
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L2+96
	vldr	d21, .L2+104
	vstr	d20, [fp, #-100]
	vstr	d21, [fp, #-92]
	mov	r3, #63232
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-100]
	vldr	d17, [fp, #-92]
	vstmia	r3, {d16-d17}
	mov	r3, #63232
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #63200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63184
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63168
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63152
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63136
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #63120
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63120
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vsub.i32	q8, q8, q10
	movw	r3, #63104
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63088
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63104
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63088
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #52172
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #28
	vldmia	r3, {d16-d17}
	movw	r3, #63072
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63072
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63056
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63040
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63024
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63040
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63024
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53168
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #28
	vldmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #56
	vldmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L5
	vldr	d19, .L5+8
	vstr	d18, [fp, #-116]
	vstr	d19, [fp, #-108]
	movw	r3, #63008
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62992
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-116]
	vldr	d17, [fp, #-108]
	vstmia	r3, {d16-d17}
	movw	r3, #63008
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L5+16
	vldr	d21, .L5+24
	vstr	d20, [fp, #-132]
	vstr	d21, [fp, #-124]
	mov	r3, #62976
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstmia	r3, {d16-d17}
	mov	r3, #62976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62960
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62944
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62928
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L5+32
	vldr	d19, .L5+40
	vstr	d18, [fp, #-148]
	vstr	d19, [fp, #-140]
	movw	r3, #62912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62896
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-148]
	vldr	d17, [fp, #-140]
	vstmia	r3, {d16-d17}
	movw	r3, #62912
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L5+48
	vldr	d21, .L5+56
	vstr	d20, [fp, #-164]
	vstr	d21, [fp, #-156]
	movw	r3, #62880
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62864
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-164]
	vldr	d17, [fp, #-156]
	vstmia	r3, {d16-d17}
	movw	r3, #62880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62864
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62848
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62832
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62848
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62832
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L5+64
	vldr	d19, .L5+72
	vstr	d18, [fp, #-180]
	vstr	d19, [fp, #-172]
	movw	r3, #62816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-180]
	vldr	d17, [fp, #-172]
	vstmia	r3, {d16-d17}
	movw	r3, #62816
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62800
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	b	.L6
.L7:
	.align	3
.L5:
	.word	5
	.word	6
	.word	7
	.word	8
	.word	-27
	.word	-26
	.word	-25
	.word	-24
	.word	8
	.word	9
	.word	10
	.word	11
	.word	-24
	.word	-23
	.word	-22
	.word	-21
	.word	15
	.word	16
	.word	1
	.word	2
	.word	-17
	.word	-16
	.word	-31
	.word	-30
.L6:
	vldr	d20, .L5+80
	vldr	d21, .L5+88
	vstr	d20, [fp, #-196]
	vstr	d21, [fp, #-188]
	movw	r3, #62784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62768
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-196]
	vldr	d17, [fp, #-188]
	vstmia	r3, {d16-d17}
	movw	r3, #62784
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62736
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62752
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	mov	r3, #62720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #62720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62704
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62688
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62672
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62672
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vsub.i32	q8, q8, q10
	movw	r3, #62656
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62640
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62640
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #52172
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #44
	vldmia	r3, {d16-d17}
	movw	r3, #62624
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62608
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53264
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62592
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62576
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62592
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62576
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53168
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #44
	vldmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #72
	vldmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L8
	vldr	d19, .L8+8
	vstr	d18, [fp, #-212]
	vstr	d19, [fp, #-204]
	movw	r3, #62560
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62544
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-212]
	vldr	d17, [fp, #-204]
	vstmia	r3, {d16-d17}
	movw	r3, #62560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62544
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L8+16
	vldr	d21, .L8+24
	vstr	d20, [fp, #-228]
	vstr	d21, [fp, #-220]
	movw	r3, #62528
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62512
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-228]
	vldr	d17, [fp, #-220]
	vstmia	r3, {d16-d17}
	movw	r3, #62528
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62512
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62496
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62480
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62496
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62480
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L8+32
	vldr	d19, .L8+40
	vstr	d18, [fp, #-244]
	vstr	d19, [fp, #-236]
	mov	r3, #62464
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62448
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-244]
	vldr	d17, [fp, #-236]
	vstmia	r3, {d16-d17}
	mov	r3, #62464
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L8+48
	vldr	d21, .L8+56
	vstr	d20, [fp, #-260]
	vstr	d21, [fp, #-252]
	movw	r3, #62432
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62416
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-260]
	vldr	d17, [fp, #-252]
	vstmia	r3, {d16-d17}
	movw	r3, #62432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62416
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62400
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62384
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L8+64
	vldr	d19, .L8+72
	vstr	d18, [fp, #-276]
	vstr	d19, [fp, #-268]
	movw	r3, #62368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62352
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-276]
	vldr	d17, [fp, #-268]
	vstmia	r3, {d16-d17}
	movw	r3, #62368
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62352
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	b	.L9
.L10:
	.align	3
.L8:
	.word	9
	.word	10
	.word	11
	.word	12
	.word	-23
	.word	-22
	.word	-21
	.word	-20
	.word	12
	.word	13
	.word	14
	.word	15
	.word	-20
	.word	-19
	.word	-18
	.word	-17
	.word	3
	.word	4
	.word	5
	.word	6
	.word	-29
	.word	-28
	.word	-27
	.word	-26
.L9:
	vldr	d20, .L8+80
	vldr	d21, .L8+88
	vstr	d20, [fp, #-292]
	vstr	d21, [fp, #-284]
	movw	r3, #62336
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62320
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-292]
	vldr	d17, [fp, #-284]
	vstmia	r3, {d16-d17}
	movw	r3, #62336
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62320
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62288
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62304
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62256
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62272
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62256
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62240
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62224
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62240
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62224
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vsub.i32	q8, q8, q10
	mov	r3, #62208
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62192
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	mov	r3, #62208
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #52172
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #60
	vldmia	r3, {d16-d17}
	movw	r3, #62176
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62176
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62160
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	mov	r3, #53248
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62128
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62128
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53168
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #60
	vldmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52176
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #88
	vldmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L11
	vldr	d19, .L11+8
	vstr	d18, [fp, #-308]
	vstr	d19, [fp, #-300]
	movw	r3, #62112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-308]
	vldr	d17, [fp, #-300]
	vstmia	r3, {d16-d17}
	movw	r3, #62112
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62096
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L11+16
	vldr	d21, .L11+24
	vstr	d20, [fp, #-324]
	vstr	d21, [fp, #-316]
	movw	r3, #62080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-324]
	vldr	d17, [fp, #-316]
	vstmia	r3, {d16-d17}
	movw	r3, #62080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62064
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62048
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62032
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L11+32
	vldr	d19, .L11+40
	vstr	d18, [fp, #-340]
	vstr	d19, [fp, #-332]
	movw	r3, #62016
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62000
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-340]
	vldr	d17, [fp, #-332]
	vstmia	r3, {d16-d17}
	movw	r3, #62016
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62000
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L11+48
	vldr	d21, .L11+56
	vstr	d20, [fp, #-356]
	vstr	d21, [fp, #-348]
	movw	r3, #61984
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61968
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-356]
	vldr	d17, [fp, #-348]
	vstmia	r3, {d16-d17}
	movw	r3, #61984
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61968
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	mov	r3, #61952
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61936
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	mov	r3, #61952
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61936
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L11+64
	vldr	d19, .L11+72
	vstr	d18, [fp, #-372]
	vstr	d19, [fp, #-364]
	movw	r3, #61920
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61904
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-372]
	vldr	d17, [fp, #-364]
	vstmia	r3, {d16-d17}
	movw	r3, #61920
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61904
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	b	.L12
.L13:
	.align	3
.L11:
	.word	13
	.word	14
	.word	15
	.word	16
	.word	-19
	.word	-18
	.word	-17
	.word	-16
	.word	16
	.word	1
	.word	2
	.word	3
	.word	-16
	.word	-31
	.word	-30
	.word	-29
	.word	7
	.word	8
	.word	9
	.word	10
	.word	-25
	.word	-24
	.word	-23
	.word	-22
.L12:
	vldr	d20, .L11+80
	vldr	d21, .L11+88
	vstr	d20, [fp, #-388]
	vstr	d21, [fp, #-380]
	movw	r3, #61888
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61872
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-388]
	vldr	d17, [fp, #-380]
	vstmia	r3, {d16-d17}
	movw	r3, #61888
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #61856
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61840
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61856
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61840
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61824
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61808
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #61776
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61792
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61776
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vsub.i32	q8, q8, q10
	movw	r3, #61760
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61744
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61760
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61744
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #52172
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #12
	vldmia	r3, {d16-d17}
	movw	r3, #61728
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61712
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61728
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61712
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53232
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	mov	r3, #61696
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61680
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	mov	r3, #61696
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61680
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53168
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L14
	vldr	d19, .L14+8
	vstr	d18, [fp, #-404]
	vstr	d19, [fp, #-396]
	movw	r3, #61664
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61648
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-404]
	vldr	d17, [fp, #-396]
	vstmia	r3, {d16-d17}
	movw	r3, #61664
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61648
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L14+80
	vldr	d21, .L14+88
	vstr	d20, [fp, #-420]
	vstr	d21, [fp, #-412]
	movw	r3, #61632
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61616
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-420]
	vldr	d17, [fp, #-412]
	vstmia	r3, {d16-d17}
	movw	r3, #61632
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61616
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #61600
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61584
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61600
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61584
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L14+96
	vldr	d21, .L14+104
	vstr	d20, [fp, #-436]
	vstr	d21, [fp, #-428]
	movw	r3, #61568
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61552
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-436]
	vldr	d17, [fp, #-428]
	vstmia	r3, {d16-d17}
	movw	r3, #61568
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61552
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L14+16
	vldr	d23, .L14+24
	vstr	d22, [fp, #-452]
	vstr	d23, [fp, #-444]
	movw	r3, #61536
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61520
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-452]
	vldr	d17, [fp, #-444]
	vstmia	r3, {d16-d17}
	movw	r3, #61536
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61520
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #61504
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #61488
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	movw	r3, #61488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L14+32
	vldr	d23, .L14+40
	vstr	d22, [fp, #-468]
	vstr	d23, [fp, #-460]
	movw	r3, #61472
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61456
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-468]
	vldr	d17, [fp, #-460]
	vstmia	r3, {d16-d17}
	movw	r3, #61472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L14+48
	vldr	d25, .L14+56
	vstr	d24, [fp, #-484]
	vstr	d25, [fp, #-476]
	mov	r3, #61440
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61424
	movt	r3, 65535
	sub	r1, fp, #4
	b	.L15
.L16:
	.align	3
.L14:
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
.L15:
	add	r3, r1, r3
	vldr	d16, [fp, #-484]
	vldr	d17, [fp, #-476]
	vstmia	r3, {d16-d17}
	mov	r3, #61440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #61408
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d22-d23}
	movw	r3, #61392
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	movw	r3, #61392
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #61376
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d20-d21}
	movw	r3, #61360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61376
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #61360
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #61344
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61328
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61344
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61328
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L14+64
	vldr	d19, .L14+72
	vstr	d18, [fp, #-500]
	vstr	d19, [fp, #-492]
	movw	r3, #61312
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-500]
	vldr	d17, [fp, #-492]
	vstmia	r3, {d16-d17}
	movw	r3, #61312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61296
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L14+80
	vldr	d21, .L14+88
	vstr	d20, [fp, #-516]
	vstr	d21, [fp, #-508]
	movw	r3, #61280
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61264
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-516]
	vldr	d17, [fp, #-508]
	vstmia	r3, {d16-d17}
	movw	r3, #61280
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61264
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #61248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61232
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61248
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61232
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L14+96
	vldr	d21, .L14+104
	vstr	d20, [fp, #-532]
	vstr	d21, [fp, #-524]
	movw	r3, #61216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-532]
	vldr	d17, [fp, #-524]
	vstmia	r3, {d16-d17}
	movw	r3, #61216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L17
	vldr	d23, .L17+8
	vstr	d22, [fp, #-548]
	vstr	d23, [fp, #-540]
	mov	r3, #61184
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61168
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-548]
	vldr	d17, [fp, #-540]
	vstmia	r3, {d16-d17}
	mov	r3, #61184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61168
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #61152
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #61136
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61152
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #61136
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L17+96
	vldr	d23, .L17+104
	vstr	d22, [fp, #-564]
	vstr	d23, [fp, #-556]
	movw	r3, #61120
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61104
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-564]
	vldr	d17, [fp, #-556]
	vstmia	r3, {d16-d17}
	movw	r3, #61120
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61104
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L17+16
	vldr	d25, .L17+24
	vstr	d24, [fp, #-580]
	vstr	d25, [fp, #-572]
	movw	r3, #61088
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61072
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-580]
	vldr	d17, [fp, #-572]
	vstmia	r3, {d16-d17}
	movw	r3, #61088
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61072
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #61056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #61040
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61056
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	movw	r3, #61040
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #61024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #61008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61024
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	movw	r3, #61008
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #60992
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60992
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60976
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60960
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60944
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	b	.L18
.L19:
	.align	3
.L17:
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
.L18:
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L17+32
	vldr	d19, .L17+40
	vstr	d18, [fp, #-596]
	vstr	d19, [fp, #-588]
	mov	r3, #60928
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60912
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-596]
	vldr	d17, [fp, #-588]
	vstmia	r3, {d16-d17}
	mov	r3, #60928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60912
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L17+48
	vldr	d21, .L17+56
	vstr	d20, [fp, #-612]
	vstr	d21, [fp, #-604]
	movw	r3, #60896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-612]
	vldr	d17, [fp, #-604]
	vstmia	r3, {d16-d17}
	movw	r3, #60896
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60880
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #60864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60864
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60848
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L17+64
	vldr	d21, .L17+72
	vstr	d20, [fp, #-628]
	vstr	d21, [fp, #-620]
	movw	r3, #60832
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60816
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-628]
	vldr	d17, [fp, #-620]
	vstmia	r3, {d16-d17}
	movw	r3, #60832
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L17+80
	vldr	d23, .L17+88
	vstr	d22, [fp, #-644]
	vstr	d23, [fp, #-636]
	movw	r3, #60800
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60784
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-644]
	vldr	d17, [fp, #-636]
	vstmia	r3, {d16-d17}
	movw	r3, #60800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #60768
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #60752
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #60752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L17+96
	vldr	d23, .L17+104
	vstr	d22, [fp, #-660]
	vstr	d23, [fp, #-652]
	movw	r3, #60736
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-660]
	vldr	d17, [fp, #-652]
	vstmia	r3, {d16-d17}
	movw	r3, #60736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L20
	vldr	d25, .L20+8
	vstr	d24, [fp, #-676]
	vstr	d25, [fp, #-668]
	movw	r3, #60704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-676]
	vldr	d17, [fp, #-668]
	vstmia	r3, {d16-d17}
	movw	r3, #60704
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60688
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	mov	r3, #60672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #60656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #60672
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	movw	r3, #60656
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #60640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #60624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60640
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	movw	r3, #60624
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #60608
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60592
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60592
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60576
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60576
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60560
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L20+16
	vldr	d19, .L20+24
	vstr	d18, [fp, #-692]
	vstr	d19, [fp, #-684]
	movw	r3, #60544
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60528
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-692]
	vldr	d17, [fp, #-684]
	vstmia	r3, {d16-d17}
	movw	r3, #60544
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60528
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L20+32
	vldr	d21, .L20+40
	vstr	d20, [fp, #-708]
	vstr	d21, [fp, #-700]
	movw	r3, #60512
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60496
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-708]
	vldr	d17, [fp, #-700]
	vstmia	r3, {d16-d17}
	movw	r3, #60512
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60496
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #60480
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60464
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	b	.L21
.L22:
	.align	3
.L20:
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
	.word	-9
	.word	-7
	.word	-3
	.word	-13
.L21:
	vstmia	r3, {d16-d17}
	movw	r3, #60480
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60464
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L20+48
	vldr	d21, .L20+56
	vstr	d20, [fp, #-724]
	vstr	d21, [fp, #-716]
	movw	r3, #60448
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60432
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-724]
	vldr	d17, [fp, #-716]
	vstmia	r3, {d16-d17}
	movw	r3, #60448
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L20+64
	vldr	d23, .L20+72
	vstr	d22, [fp, #-740]
	vstr	d23, [fp, #-732]
	mov	r3, #60416
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60400
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-740]
	vldr	d17, [fp, #-732]
	vstmia	r3, {d16-d17}
	mov	r3, #60416
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60400
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #60384
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #60368
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60384
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #60368
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L20+80
	vldr	d23, .L20+88
	vstr	d22, [fp, #-756]
	vstr	d23, [fp, #-748]
	movw	r3, #60352
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-756]
	vldr	d17, [fp, #-748]
	vstmia	r3, {d16-d17}
	movw	r3, #60352
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60336
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L20+96
	vldr	d25, .L20+104
	vstr	d24, [fp, #-772]
	vstr	d25, [fp, #-764]
	movw	r3, #60320
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-772]
	vldr	d17, [fp, #-764]
	vstmia	r3, {d16-d17}
	movw	r3, #60320
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60304
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #60288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #60272
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60288
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	movw	r3, #60272
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #60256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #60240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60256
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	movw	r3, #60240
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #60224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60208
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60224
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60192
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60176
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #60160
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #60160
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60136
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d18, [r3, #0]
	movw	r3, #60128
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #60136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	movw	r3, #60128
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #64
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	movw	r3, #60120
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #60120
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r1, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #60112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #60112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r1, r1, r3
	movw	r3, #53280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60096
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vmov.32	r3, d16[0]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #4
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L23
	vldr	d19, .L23+8
	vstr	d18, [fp, #-788]
	vstr	d19, [fp, #-780]
	movw	r3, #60080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-788]
	vldr	d17, [fp, #-780]
	vstmia	r3, {d16-d17}
	movw	r3, #60080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60064
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L23+80
	vldr	d21, .L23+88
	vstr	d20, [fp, #-804]
	vstr	d21, [fp, #-796]
	movw	r3, #60048
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60032
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-804]
	vldr	d17, [fp, #-796]
	vstmia	r3, {d16-d17}
	movw	r3, #60048
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #60032
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #60016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #60000
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #60016
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #60000
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L23+96
	vldr	d21, .L23+104
	vstr	d20, [fp, #-820]
	vstr	d21, [fp, #-812]
	movw	r3, #59984
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59968
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-820]
	vldr	d17, [fp, #-812]
	vstmia	r3, {d16-d17}
	movw	r3, #59984
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59968
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L23+16
	vldr	d23, .L23+24
	vstr	d22, [fp, #-836]
	vstr	d23, [fp, #-828]
	movw	r3, #59952
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59936
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-836]
	vldr	d17, [fp, #-828]
	vstmia	r3, {d16-d17}
	movw	r3, #59952
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59936
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #59920
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	mov	r3, #59904
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59920
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	mov	r3, #59904
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L23+32
	vldr	d23, .L23+40
	vstr	d22, [fp, #-852]
	vstr	d23, [fp, #-844]
	movw	r3, #59888
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-852]
	vldr	d17, [fp, #-844]
	vstmia	r3, {d16-d17}
	movw	r3, #59888
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59872
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L23+48
	vldr	d25, .L23+56
	vstr	d24, [fp, #-868]
	vstr	d25, [fp, #-860]
	movw	r3, #59856
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59840
	movt	r3, 65535
	sub	r2, fp, #4
	b	.L24
.L25:
	.align	3
.L23:
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
.L24:
	add	r3, r2, r3
	vldr	d16, [fp, #-868]
	vldr	d17, [fp, #-860]
	vstmia	r3, {d16-d17}
	movw	r3, #59856
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59840
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #59824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #59808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59824
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	movw	r3, #59808
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #59792
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #59776
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d20-d21}
	movw	r3, #59776
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #59760
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #59744
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59760
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #59744
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #20
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L23+64
	vldr	d19, .L23+72
	vstr	d18, [fp, #-884]
	vstr	d19, [fp, #-876]
	movw	r3, #59728
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59712
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-884]
	vldr	d17, [fp, #-876]
	vstmia	r3, {d16-d17}
	movw	r3, #59728
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59712
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L23+80
	vldr	d21, .L23+88
	vstr	d20, [fp, #-900]
	vstr	d21, [fp, #-892]
	movw	r3, #59696
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59680
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-900]
	vldr	d17, [fp, #-892]
	vstmia	r3, {d16-d17}
	movw	r3, #59696
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59680
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #59664
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	mov	r3, #59648
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59664
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	mov	r3, #59648
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L23+96
	vldr	d21, .L23+104
	vstr	d20, [fp, #-916]
	vstr	d21, [fp, #-908]
	movw	r3, #59632
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59616
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-916]
	vldr	d17, [fp, #-908]
	vstmia	r3, {d16-d17}
	movw	r3, #59632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59616
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L26
	vldr	d23, .L26+8
	vstr	d22, [fp, #-932]
	vstr	d23, [fp, #-924]
	movw	r3, #59600
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59584
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-932]
	vldr	d17, [fp, #-924]
	vstmia	r3, {d16-d17}
	movw	r3, #59600
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59584
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #59568
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d20-d21}
	movw	r3, #59552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59568
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #59552
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L26+96
	vldr	d23, .L26+104
	vstr	d22, [fp, #-948]
	vstr	d23, [fp, #-940]
	movw	r3, #59536
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59520
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-948]
	vldr	d17, [fp, #-940]
	vstmia	r3, {d16-d17}
	movw	r3, #59536
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59520
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L26+16
	vldr	d25, .L26+24
	vstr	d24, [fp, #-964]
	vstr	d25, [fp, #-956]
	movw	r3, #59504
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59488
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-964]
	vldr	d17, [fp, #-956]
	vstmia	r3, {d16-d17}
	movw	r3, #59504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #59472
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d22-d23}
	movw	r3, #59456
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59472
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d22-d23}
	movw	r3, #59456
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #59440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #59424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59440
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d20-d21}
	movw	r3, #59424
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #59408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	mov	r3, #59392
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59408
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	mov	r3, #59392
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #59360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59376
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59360
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r1, fp, #4
	b	.L27
.L28:
	.align	3
.L26:
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
.L27:
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #36
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L26+32
	vldr	d19, .L26+40
	vstr	d18, [fp, #-980]
	vstr	d19, [fp, #-972]
	movw	r3, #59344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-980]
	vldr	d17, [fp, #-972]
	vstmia	r3, {d16-d17}
	movw	r3, #59344
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59328
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L26+48
	vldr	d21, .L26+56
	vstr	d20, [fp, #-996]
	vstr	d21, [fp, #-988]
	movw	r3, #59312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59296
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldr	d16, [fp, #-996]
	vldr	d17, [fp, #-988]
	vstmia	r3, {d16-d17}
	movw	r3, #59312
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #59280
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #59264
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #59264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L26+64
	vldr	d21, .L26+72
	vstr	d20, [fp, #-1012]
	vstr	d21, [fp, #-1004]
	movw	r3, #59248
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59232
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d16, [fp, #-1012]
	vldr	d17, [fp, #-1004]
	vstmia	r3, {d16-d17}
	movw	r3, #59248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59232
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	mov	r3, #64512
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d22, .L26+80
	vldr	d23, .L26+88
	vstmia	r3, {d22-d23}
	movw	r3, #59216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	mov	r3, #64512
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #59216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #59184
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #59168
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #59168
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64496
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d22, .L26+96
	vldr	d23, .L26+104
	vstmia	r3, {d22-d23}
	movw	r3, #59152
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	mov	r3, #59136
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64496
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #59152
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #59136
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64480
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d24, .L29
	vldr	d25, .L29+8
	vstmia	r3, {d24-d25}
	movw	r3, #59120
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59104
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64480
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #59120
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #59104
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #59088
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d22-d23}
	movw	r3, #59072
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59088
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #59072
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #59056
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #59040
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #59040
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #59024
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #59008
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #59024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #59008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58976
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58992
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58976
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #52
	vldmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64464
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d18, .L29+16
	vldr	d19, .L29+24
	vstmia	r3, {d18-d19}
	movw	r3, #58960
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58944
	movt	r3, 65535
	sub	r0, fp, #4
	add	r2, r0, r3
	movw	r3, #64464
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58944
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d20, .L29+32
	vldr	d21, .L29+40
	vstmia	r3, {d20-d21}
	movw	r3, #58928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58912
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64448
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58928
	movt	r3, 65535
	sub	r1, fp, #4
	b	.L30
.L31:
	.align	3
.L29:
	.word	-9
	.word	-7
	.word	-3
	.word	-13
	.word	-1
	.word	-2
	.word	-2
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	3
	.word	8
	.word	12
	.word	15
	.word	4
	.word	-24
	.word	-20
	.word	-17
	.word	-28
	.word	23
	.word	25
	.word	29
	.word	19
	.word	-9
	.word	-7
	.word	-3
	.word	-13
.L30:
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #58896
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	mov	r3, #58880
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	mov	r3, #58880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64432
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L29+48
	vldr	d21, .L29+56
	vstmia	r3, {d20-d21}
	movw	r3, #58864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58848
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64432
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64416
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d22, .L29+64
	vldr	d23, .L29+72
	vstmia	r3, {d22-d23}
	movw	r3, #58832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58816
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64416
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58816
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #58800
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #58784
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #58784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64400
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d22, .L29+80
	vldr	d23, .L29+88
	vstmia	r3, {d22-d23}
	movw	r3, #58768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58752
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64400
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64384
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d24, .L29+96
	vldr	d25, .L29+104
	vstmia	r3, {d24-d25}
	movw	r3, #58736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64384
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #58704
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d22-d23}
	movw	r3, #58688
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #58688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #58672
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d20-d21}
	movw	r3, #58656
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #58656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #58640
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	mov	r3, #58624
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	mov	r3, #58624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58608
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58592
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58608
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58592
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53184
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58576
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58576
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58560
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58560
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #58544
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #58552
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58544
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #68
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58536
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #58536
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r1, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58528
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #58528
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r1, r1, r3
	movw	r3, #53280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58512
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58512
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vmov.32	r3, d16[1]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r2, r3, #8
	movw	r3, #58508
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #58508
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #53048
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53048
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #40
	movw	r3, #58504
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #58504
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #53016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53016
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #53080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58500
	movt	r3, 65535
	mov	r2, #0
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #58480
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58500
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r2, [r1, r3]
	movw	r3, #58480
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vmov.32	d17[1], r2
	movw	r3, #53080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53112
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58464
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58448
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58464
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58448
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58432
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58416
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58416
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d18, [r3, #0]
	movw	r3, #58400
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #58408
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #58400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58392
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #58392
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r2, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58384
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #58384
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #72
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #64
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #64
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r1, fp, #4
	add	r3, r1, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d18, .L32
	vldr	d19, .L32+8
	vstmia	r3, {d18-d19}
	mov	r3, #58368
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58352
	movt	r3, 65535
	sub	r0, fp, #4
	add	r2, r0, r3
	movw	r3, #64360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	mov	r3, #58368
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58352
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d20, .L32+16
	vldr	d21, .L32+24
	vstmia	r3, {d20-d21}
	movw	r3, #58336
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58320
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64344
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58320
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #58304
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58288
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58304
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #58288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64328
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L32+32
	vldr	d19, .L32+40
	vstmia	r3, {d18-d19}
	movw	r3, #58272
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58256
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64328
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58256
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64312
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L32+48
	vldr	d21, .L32+56
	vstmia	r3, {d20-d21}
	movw	r3, #58240
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58224
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64312
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #58240
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58224
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #58208
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58192
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58208
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #58192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58176
	b	.L33
.L34:
	.align	3
.L32:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L33:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58160
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58176
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58128
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58128
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58120
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	mov	r3, #58112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #58120
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	mov	r3, #58112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #72
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #72
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #58104
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #58104
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #58096
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #58096
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #12
	movw	r3, #58092
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #58092
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52984
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53296
	movt	r2, 65535
	movw	r3, #52984
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53296
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #44
	movw	r3, #58088
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #58088
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52952
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53328
	movt	r2, 65535
	movw	r3, #52952
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53328
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #53080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58084
	movt	r3, 65535
	mov	r2, #0
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #58064
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58084
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r2, [r1, r3]
	movw	r3, #58064
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	vmov.32	d17[1], r2
	movw	r3, #53080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53112
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58048
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #58032
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58048
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58032
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58016
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #58000
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #58000
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d18, [r3, #0]
	movw	r3, #57984
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57992
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #57984
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57976
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #57976
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r2, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57968
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fstd	d16, [r3, #0]
	movw	r3, #57968
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #76
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #68
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r3, r3, #68
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r1, fp, #4
	add	r3, r1, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d18, .L35
	vldr	d19, .L35+8
	vstmia	r3, {d18-d19}
	movw	r3, #57952
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57936
	movt	r3, 65535
	sub	r0, fp, #4
	add	r2, r0, r3
	movw	r3, #64288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57952
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57936
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d20, .L35+16
	vldr	d21, .L35+24
	vstmia	r3, {d20-d21}
	movw	r3, #57920
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57904
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64272
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57920
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57904
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57888
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57872
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57888
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	mov	r3, #64256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L35+32
	vldr	d19, .L35+40
	vstmia	r3, {d18-d19}
	mov	r3, #57856
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57840
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	mov	r3, #64256
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	mov	r3, #57856
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57840
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64240
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L35+48
	vldr	d21, .L35+56
	vstmia	r3, {d20-d21}
	movw	r3, #57824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57808
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64240
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57776
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57792
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57776
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57760
	b	.L36
.L37:
	.align	3
.L35:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L36:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57744
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57760
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57744
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57728
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57728
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57712
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57712
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #57696
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #57704
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57696
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #76
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #76
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #57688
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57688
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57680
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57680
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #16
	movw	r3, #57676
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #57676
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52920
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53360
	movt	r2, 65535
	movw	r3, #52920
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53360
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #48
	movw	r3, #57672
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #57672
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52888
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53392
	movt	r2, 65535
	movw	r3, #52888
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53392
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #64
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #8
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #80
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #72
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #72
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L38
	vldr	d19, .L38+8
	vstmia	r3, {d18-d19}
	movw	r3, #57656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57640
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64224
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57640
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L38+16
	vldr	d21, .L38+24
	vstmia	r3, {d20-d21}
	movw	r3, #57624
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57624
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57608
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57592
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57576
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57592
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57576
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64192
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L38+32
	vldr	d19, .L38+40
	vstmia	r3, {d18-d19}
	movw	r3, #57560
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57544
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64192
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57544
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64176
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L38+48
	vldr	d21, .L38+56
	vstmia	r3, {d20-d21}
	movw	r3, #57528
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57512
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64176
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57528
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57512
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57496
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57480
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57496
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57480
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57464
	b	.L39
.L40:
	.align	3
.L38:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L39:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57448
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57464
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57432
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57416
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57416
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57408
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #57400
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #57408
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57400
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #80
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #80
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #57392
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57392
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57384
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57384
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #20
	movw	r3, #57380
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #57380
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52856
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53424
	movt	r2, 65535
	movw	r3, #52856
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53424
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #52
	movw	r3, #57376
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #57376
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53456
	movt	r2, 65535
	movw	r3, #52824
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53456
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #68
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #12
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #84
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #76
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #76
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L41
	vldr	d19, .L41+8
	vstmia	r3, {d18-d19}
	movw	r3, #57360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #57344
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64160
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #57344
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L41+16
	vldr	d21, .L41+24
	vstmia	r3, {d20-d21}
	movw	r3, #57328
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57312
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57328
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57296
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57280
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64128
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L41+32
	vldr	d19, .L41+40
	vstmia	r3, {d18-d19}
	movw	r3, #57264
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57248
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64128
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57248
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L41+48
	vldr	d21, .L41+56
	vstmia	r3, {d20-d21}
	movw	r3, #57232
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57232
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57184
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #57184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57168
	b	.L42
.L43:
	.align	3
.L41:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L42:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #57152
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57168
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57152
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57136
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57120
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57120
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57112
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #57104
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #57112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #57104
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #84
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #84
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #57096
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #57096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	mov	r3, #57088
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	mov	r3, #57088
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #24
	movw	r3, #57084
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #57084
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53488
	movt	r2, 65535
	movw	r3, #52792
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53488
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #56
	movw	r3, #57080
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #57080
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52760
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53520
	movt	r2, 65535
	movw	r3, #52760
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53520
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #72
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #16
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #88
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #80
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #80
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64096
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L44
	vldr	d19, .L44+8
	vstmia	r3, {d18-d19}
	movw	r3, #57064
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57048
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L44+16
	vldr	d21, .L44+24
	vstmia	r3, {d20-d21}
	movw	r3, #57032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57016
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64080
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #57032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #57016
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #57000
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56984
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #57000
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56984
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L44+32
	vldr	d19, .L44+40
	vstmia	r3, {d18-d19}
	movw	r3, #56968
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56952
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64064
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56968
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56952
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64048
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L44+48
	vldr	d21, .L44+56
	vstmia	r3, {d20-d21}
	movw	r3, #56936
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56920
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64048
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56936
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56920
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56904
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56888
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56904
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56888
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56872
	b	.L45
.L46:
	.align	3
.L44:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L45:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56856
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56872
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56856
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56840
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56840
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56824
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56824
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #56808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #56816
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56808
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #88
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #88
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #56800
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56800
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56792
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #28
	movw	r3, #56788
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #56788
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52728
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53552
	movt	r2, 65535
	movw	r3, #52728
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53552
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #60
	movw	r3, #56784
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #56784
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52696
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53584
	movt	r2, 65535
	movw	r3, #52696
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53584
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #76
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #20
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #92
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L47
	vldr	d19, .L47+8
	vstmia	r3, {d18-d19}
	movw	r3, #56768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56752
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64032
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #64016
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L47+16
	vldr	d21, .L47+24
	vstmia	r3, {d20-d21}
	movw	r3, #56736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #64016
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56704
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56688
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	mov	r3, #64000
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L47+32
	vldr	d19, .L47+40
	vstmia	r3, {d18-d19}
	movw	r3, #56672
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56656
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	mov	r3, #64000
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63984
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L47+48
	vldr	d21, .L47+56
	vstmia	r3, {d20-d21}
	movw	r3, #56640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56624
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63984
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56592
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56608
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56592
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #56576
	b	.L48
.L49:
	.align	3
.L47:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L48:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56560
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	mov	r3, #56576
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56544
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56544
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56528
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56528
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56520
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #56512
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #56520
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56512
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #92
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #92
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #56504
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56496
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56496
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #32
	movw	r3, #56492
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #56492
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52664
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53616
	movt	r2, 65535
	movw	r3, #52664
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53616
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #64
	movw	r3, #56488
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #56488
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53648
	movt	r2, 65535
	movw	r3, #52632
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53648
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #80
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #24
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #96
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #88
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #88
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63968
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L50
	vldr	d19, .L50+8
	vstmia	r3, {d18-d19}
	movw	r3, #56472
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56456
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63968
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63952
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L50+16
	vldr	d21, .L50+24
	vstmia	r3, {d20-d21}
	movw	r3, #56440
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63952
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56440
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56424
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56408
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56392
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56408
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56392
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63936
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L50+32
	vldr	d19, .L50+40
	vstmia	r3, {d18-d19}
	movw	r3, #56376
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56360
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63936
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63920
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L50+48
	vldr	d21, .L50+56
	vstmia	r3, {d20-d21}
	movw	r3, #56344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56328
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63920
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56312
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56296
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56312
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56296
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56280
	b	.L51
.L52:
	.align	3
.L50:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L51:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56264
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56280
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56248
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56232
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56232
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #56216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #56224
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56216
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #96
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #96
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #56208
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #56200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #56200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #36
	movw	r3, #56196
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #56196
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52600
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53680
	movt	r2, 65535
	movw	r3, #52600
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53680
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #68
	movw	r3, #56192
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #56192
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52568
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53712
	movt	r2, 65535
	movw	r3, #52568
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53712
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #84
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #28
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #100
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #92
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #92
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63904
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L53
	vldr	d19, .L53+8
	vstmia	r3, {d18-d19}
	movw	r3, #56176
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56160
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63904
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56160
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63888
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L53+16
	vldr	d21, .L53+24
	vstmia	r3, {d20-d21}
	movw	r3, #56144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56128
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63888
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56128
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56112
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56112
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56096
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L53+32
	vldr	d19, .L53+40
	vstmia	r3, {d18-d19}
	movw	r3, #56080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #56064
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63872
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56080
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #56064
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63856
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L53+48
	vldr	d21, .L53+56
	vstmia	r3, {d20-d21}
	movw	r3, #56048
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56032
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63856
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #56048
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #56032
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #56016
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #56000
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #56016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #56000
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55984
	b	.L54
.L55:
	.align	3
.L53:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L54:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55968
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55984
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55968
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55952
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55952
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55936
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55936
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #55920
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #55928
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55920
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #100
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #100
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #55912
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55912
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55904
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55904
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #40
	movw	r3, #55900
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #55900
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52536
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53744
	movt	r2, 65535
	movw	r3, #52536
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53744
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #72
	movw	r3, #55896
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #55896
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52504
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53776
	movt	r2, 65535
	movw	r3, #52504
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53776
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #88
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #32
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #104
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #96
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #96
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63840
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L56
	vldr	d19, .L56+8
	vstmia	r3, {d18-d19}
	movw	r3, #55880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55864
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63840
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55864
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63824
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L56+16
	vldr	d21, .L56+24
	vstmia	r3, {d20-d21}
	movw	r3, #55848
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55832
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63824
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55848
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55832
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55816
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55800
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63808
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L56+32
	vldr	d19, .L56+40
	vstmia	r3, {d18-d19}
	movw	r3, #55784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55768
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63808
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63792
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L56+48
	vldr	d21, .L56+56
	vstmia	r3, {d20-d21}
	movw	r3, #55752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55736
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63792
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55704
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55688
	b	.L57
.L58:
	.align	3
.L56:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L57:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55672
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55688
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55656
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55640
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55640
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #55624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #55632
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55624
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #104
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #104
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #55616
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55616
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55608
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #44
	movw	r3, #55604
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #55604
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52472
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53808
	movt	r2, 65535
	movw	r3, #52472
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53808
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #76
	movw	r3, #55600
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #55600
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52440
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53840
	movt	r2, 65535
	movw	r3, #52440
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53840
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #92
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #36
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #108
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #100
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #100
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63776
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L59
	vldr	d19, .L59+8
	vstmia	r3, {d18-d19}
	movw	r3, #55584
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55568
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63776
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55584
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55568
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63760
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L59+16
	vldr	d21, .L59+24
	vstmia	r3, {d20-d21}
	mov	r3, #55552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55536
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63760
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	mov	r3, #55552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55536
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55520
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55520
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55504
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	mov	r3, #63744
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L59+32
	vldr	d19, .L59+40
	vstmia	r3, {d18-d19}
	movw	r3, #55488
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55472
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	mov	r3, #63744
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55472
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63728
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L59+48
	vldr	d21, .L59+56
	vstmia	r3, {d20-d21}
	movw	r3, #55456
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55440
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63728
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55456
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55408
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55424
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55392
	b	.L60
.L61:
	.align	3
.L59:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L60:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55376
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55392
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55360
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55344
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55344
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #55328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #55336
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55328
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #108
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #108
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #55320
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55320
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55312
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55312
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #48
	movw	r3, #55308
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #55308
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52408
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53872
	movt	r2, 65535
	movw	r3, #52408
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53872
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #80
	movw	r3, #55304
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #55304
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53904
	movt	r2, 65535
	movw	r3, #52376
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53904
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #96
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #40
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #112
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #104
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #104
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63712
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L62
	vldr	d19, .L62+8
	vstmia	r3, {d18-d19}
	movw	r3, #55288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55272
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63712
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55272
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63696
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L62+16
	vldr	d21, .L62+24
	vstmia	r3, {d20-d21}
	movw	r3, #55256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55240
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63696
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55224
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55208
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63680
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L62+32
	vldr	d19, .L62+40
	vstmia	r3, {d18-d19}
	movw	r3, #55192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55176
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63680
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55192
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55176
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63664
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L62+48
	vldr	d21, .L62+56
	vstmia	r3, {d20-d21}
	movw	r3, #55160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63664
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #55160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #55128
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55112
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55128
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #55112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55096
	b	.L63
.L64:
	.align	3
.L62:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L63:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #55080
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55096
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55080
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55064
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #55048
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #55048
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	mov	r3, #55040
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #55032
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	mov	r3, #55040
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55032
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #112
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #112
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #55024
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55024
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #55016
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #55016
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #52
	movw	r3, #55012
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #55012
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52344
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53936
	movt	r2, 65535
	movw	r3, #52344
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #53936
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #84
	movw	r3, #55008
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #55008
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52312
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #53968
	movt	r2, 65535
	movw	r3, #52312
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #53968
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #100
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #44
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #116
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #108
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #108
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63648
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L65
	vldr	d19, .L65+8
	vstmia	r3, {d18-d19}
	movw	r3, #54992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54976
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63648
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54992
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63632
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L65+16
	vldr	d21, .L65+24
	vstmia	r3, {d20-d21}
	movw	r3, #54960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54944
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63632
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54928
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54912
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63616
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L65+32
	vldr	d19, .L65+40
	vstmia	r3, {d18-d19}
	movw	r3, #54896
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54880
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63616
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63600
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L65+48
	vldr	d21, .L65+56
	vstmia	r3, {d20-d21}
	movw	r3, #54864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54848
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63600
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54832
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54816
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54816
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54800
	b	.L66
.L67:
	.align	3
.L65:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L66:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	mov	r3, #54784
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54800
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	mov	r3, #54784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54768
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54752
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54752
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54744
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #54736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #54744
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54736
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #116
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #116
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #54728
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54728
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54720
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #56
	movw	r3, #54716
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #54716
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52280
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #54000
	movt	r2, 65535
	movw	r3, #52280
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #54000
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #88
	movw	r3, #54712
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #54712
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #54032
	movt	r2, 65535
	movw	r3, #52248
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #54032
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #104
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #48
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1160]
	add	r3, r2, r3
	str	r3, [fp, #-1160]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #120
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #112
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #112
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1160]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63584
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L68
	vldr	d19, .L68+8
	vstmia	r3, {d18-d19}
	movw	r3, #54696
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54680
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63584
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54696
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54680
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63568
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L68+16
	vldr	d21, .L68+24
	vstmia	r3, {d20-d21}
	movw	r3, #54664
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54648
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63568
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54664
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54648
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54632
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54616
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54616
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L68+32
	vldr	d19, .L68+40
	vstmia	r3, {d18-d19}
	movw	r3, #54600
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54584
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63552
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54600
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54584
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63536
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L68+48
	vldr	d21, .L68+56
	vstmia	r3, {d20-d21}
	movw	r3, #54568
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54552
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63536
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54568
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54552
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54536
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54520
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54536
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54520
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54504
	b	.L69
.L70:
	.align	3
.L68:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L69:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54488
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54472
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54456
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54456
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #54440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #54448
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54440
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #120
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #120
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #54432
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54432
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54424
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #60
	movw	r3, #54420
	movt	r3, 65535
	sub	r1, fp, #4
	str	r2, [r1, r3]
	movw	r3, #54420
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d19}
	movw	r2, #54064
	movt	r2, 65535
	movw	r3, #52216
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53112
	movt	r2, 65535
	movw	r3, #54064
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #92
	movw	r3, #54416
	movt	r3, 65535
	sub	ip, fp, #4
	str	r2, [ip, r3]
	movw	r3, #54416
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	vld2.32	{d16-d19}, [r3]
	movw	r3, #52184
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d19}
	movw	r2, #54096
	movt	r2, 65535
	movw	r3, #52184
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r2, #53080
	movt	r2, 65535
	movw	r3, #54096
	movt	r3, 65535
	sub	r0, fp, #4
	add	ip, r0, r2
	sub	r1, fp, #4
	add	r4, r1, r3
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r3, r3, #108
	ldr	r2, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #52
	ldr	r3, [r3, #0]
	rsb	r3, r3, r2
	ldr	r2, [fp, #-1232]
	add	r3, r2, r3
	str	r3, [fp, #-1232]
	movw	r3, #52180
	movt	r3, 65535
	sub	r0, fp, #4
	ldr	r3, [r0, r3]
	add	r2, r3, #124
	movw	r3, #52180
	movt	r3, 65535
	sub	r1, fp, #4
	ldr	r3, [r1, r3]
	add	r3, r3, #116
	ldr	r3, [r3, #0]
	mov	r1, r3, lsr #1
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #116
	ldr	r3, [r3, #0]
	orr	r1, r1, r3
	ldr	r3, [fp, #-1232]
	add	r3, r1, r3
	str	r3, [r2, #0]
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63520
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L71
	vldr	d19, .L71+8
	vstmia	r3, {d18-d19}
	movw	r3, #54400
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54384
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63520
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53112
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L71+16
	vldr	d21, .L71+24
	vstmia	r3, {d20-d21}
	movw	r3, #54368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54352
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63504
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54352
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54336
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54320
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54320
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53216
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	r0, fp, #4
	add	r3, r0, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	mov	r3, #63488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d18, .L71+32
	vldr	d19, .L71+40
	vstmia	r3, {d18-d19}
	movw	r3, #54304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54288
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	mov	r3, #63488
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	movw	r3, #54304
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #53080
	movt	r3, 65535
	mov	r2, #16
	sub	ip, fp, #4
	add	r3, ip, r3
	add	r3, r3, r2
	vldmia	r3, {d16-d17}
	movw	r3, #63472
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldr	d20, .L71+48
	vldr	d21, .L71+56
	vstmia	r3, {d20-d21}
	mov	r3, #54272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54256
	movt	r3, 65535
	sub	ip, fp, #4
	add	r2, ip, r3
	movw	r3, #63472
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	mov	r3, #54272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54256
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #54240
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54224
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54240
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #54224
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d18-d19}
	movw	r3, #53216
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54208
	b	.L72
.L73:
	.align	3
.L71:
	.word	3
	.word	7
	.word	13
	.word	16
	.word	-29
	.word	-25
	.word	-19
	.word	-16
	.word	19
	.word	23
	.word	27
	.word	0
	.word	-13
	.word	-9
	.word	-5
	.word	-2
.L72:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #54192
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54208
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54192
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #53200
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54176
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov	d16, d17
	vmov	d18, d16  @ v2si
	movw	r3, #53200
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54160
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	vstmia	r3, {d16-d17}
	movw	r3, #54160
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	vldmia	r3, {d16-d17}
	movw	r3, #54152
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d18, [r3, #0]
	movw	r3, #54144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #54152
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d17, [r3, #0]
	vadd.i32	d16, d16, d17
	movw	r3, #53144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #52180
	movt	r3, 65535
	sub	r2, fp, #4
	ldr	r3, [r2, r3]
	add	r2, r3, #124
	movw	r3, #52180
	movt	r3, 65535
	sub	ip, fp, #4
	ldr	r3, [ip, r3]
	add	r3, r3, #124
	ldr	r1, [r3, #0]
	movw	r3, #53144
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	movw	r3, #54136
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54136
	movt	r3, 65535
	sub	r0, fp, #4
	add	r3, r0, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[0]
	mov	r0, r3
	movw	r3, #53144
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	movw	r3, #54128
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fstd	d16, [r3, #0]
	movw	r3, #54128
	movt	r3, 65535
	sub	ip, fp, #4
	add	r3, ip, r3
	fldd	d16, [r3, #0]
	vmov.32	r3, d16[1]
	add	r3, r0, r3
	add	r3, r1, r3
	str	r3, [r2, #0]
	sub	sp, fp, #4
	ldmfd	sp!, {r4, fp}
	bx	lr
	.size	bmw_small_f1, .-bmw_small_f1
	.align	2
	.type	bmw_small_f0, %function
bmw_small_f0:
	@ args = 0, pretend = 0, frame = 3928
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #3920
	sub	sp, sp, #12
	str	r0, [fp, #-3920]
	str	r1, [fp, #-3924]
	str	r2, [fp, #-3928]
	ldr	r3, [fp, #-3924]
	vldmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3928]
	vldmia	r3, {d16-d17}
	movw	r3, #61736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3928]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	movw	r3, #61720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3928]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #61704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3928]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #61688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-756]
	vstr	d19, [fp, #-748]
	vstr	d16, [fp, #-772]
	vstr	d17, [fp, #-764]
	vldr	d18, [fp, #-756]
	vldr	d19, [fp, #-748]
	vldr	d16, [fp, #-772]
	vldr	d17, [fp, #-764]
	veor	q8, q9, q8
	movw	r3, #61672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-788]
	vstr	d19, [fp, #-780]
	vstr	d16, [fp, #-804]
	vstr	d17, [fp, #-796]
	vldr	d18, [fp, #-788]
	vldr	d19, [fp, #-780]
	vldr	d16, [fp, #-804]
	vldr	d17, [fp, #-796]
	veor	q8, q9, q8
	movw	r3, #61656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-820]
	vstr	d19, [fp, #-812]
	vstr	d16, [fp, #-836]
	vstr	d17, [fp, #-828]
	vldr	d18, [fp, #-820]
	vldr	d19, [fp, #-812]
	vldr	d16, [fp, #-836]
	vldr	d17, [fp, #-828]
	veor	q8, q9, q8
	movw	r3, #61640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-852]
	vstr	d19, [fp, #-844]
	vstr	d16, [fp, #-868]
	vstr	d17, [fp, #-860]
	vldr	d18, [fp, #-852]
	vldr	d19, [fp, #-844]
	vldr	d16, [fp, #-868]
	vldr	d17, [fp, #-860]
	veor	q8, q9, q8
	movw	r3, #61624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	movw	r2, #61672
	movt	r2, 65535
	sub	r1, fp, #4
	add	r2, r1, r2
	vldmia	r2, {d16-d17}
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r2, r3, #16
	movw	r3, #61656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r2, r3, #32
	movw	r3, #61640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r2, r3, #48
	movw	r3, #61624
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r2, r3, #64
	movw	r3, #61672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #20
	vldmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #52
	vldmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-884]
	vstr	d19, [fp, #-876]
	vstr	d16, [fp, #-900]
	vstr	d17, [fp, #-892]
	vldr	d16, [fp, #-884]
	vldr	d17, [fp, #-876]
	vldr	d18, [fp, #-900]
	vldr	d19, [fp, #-892]
	vadd.i32	q8, q8, q9
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov.i64	q9, #-4294967296  @ v4si
	vstr	d18, [fp, #-20]
	vstr	d19, [fp, #-12]
	vstr	d16, [fp, #-916]
	vstr	d17, [fp, #-908]
	vldr	d16, [fp, #-20]
	vldr	d17, [fp, #-12]
	vstr	d16, [fp, #-932]
	vstr	d17, [fp, #-924]
	vldr	d18, [fp, #-916]
	vldr	d19, [fp, #-908]
	vldr	d16, [fp, #-932]
	vldr	d17, [fp, #-924]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L75
	vldr	d21, .L75+8
	vstr	d20, [fp, #-36]
	vstr	d21, [fp, #-28]
	vstr	d16, [fp, #-948]
	vstr	d17, [fp, #-940]
	vldr	d16, [fp, #-36]
	vldr	d17, [fp, #-28]
	vstr	d16, [fp, #-964]
	vstr	d17, [fp, #-956]
	vldr	d20, [fp, #-948]
	vldr	d21, [fp, #-940]
	vldr	d16, [fp, #-964]
	vldr	d17, [fp, #-956]
	veor	q8, q10, q8
	vstr	d18, [fp, #-980]
	vstr	d19, [fp, #-972]
	vstr	d16, [fp, #-996]
	vstr	d17, [fp, #-988]
	vldr	d16, [fp, #-980]
	vldr	d17, [fp, #-972]
	vldr	d18, [fp, #-996]
	vldr	d19, [fp, #-988]
	vadd.i32	q8, q8, q9
	movw	r3, #61704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #36
	vldmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #4
	vldmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L75+16
	vldr	d19, .L75+24
	vstr	d18, [fp, #-52]
	vstr	d19, [fp, #-44]
	vstr	d16, [fp, #-1012]
	vstr	d17, [fp, #-1004]
	mov	r3, #64512
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-52]
	vldr	d17, [fp, #-44]
	vstmia	r3, {d16-d17}
	vldr	d18, [fp, #-1012]
	vldr	d19, [fp, #-1004]
	mov	r3, #64512
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L75+32
	vldr	d21, .L75+40
	vstr	d20, [fp, #-68]
	vstr	d21, [fp, #-60]
	movw	r3, #64496
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64480
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-68]
	vldr	d17, [fp, #-60]
	vstmia	r3, {d16-d17}
	movw	r3, #64496
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64480
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #64464
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64464
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64448
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov.i64	q9, #-4294967296  @ v4si
	vstr	d18, [fp, #-84]
	vstr	d19, [fp, #-76]
	movw	r3, #64432
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64416
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-84]
	vldr	d17, [fp, #-76]
	vstmia	r3, {d16-d17}
	movw	r3, #64432
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64416
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L75
	vldr	d21, .L75+8
	vstr	d20, [fp, #-100]
	vstr	d21, [fp, #-92]
	movw	r3, #64400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-100]
	vldr	d17, [fp, #-92]
	vstmia	r3, {d16-d17}
	movw	r3, #64400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #64368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64352
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64352
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	b	.L76
.L77:
	.align	3
.L75:
	.word	-1
	.word	0
	.word	0
	.word	0
	.word	-1
	.word	-1
	.word	0
	.word	0
	.word	-1
	.word	-1
	.word	-1
	.word	0
	.word	0
	.word	-1
	.word	0
	.word	0
	.word	-1
	.word	0
	.word	0
	.word	0
.L76:
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #40
	vldmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #56
	vldmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #8
	vldmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #24
	vldmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L75+48
	vldr	d19, .L75+56
	vstr	d18, [fp, #-116]
	vstr	d19, [fp, #-108]
	movw	r3, #64336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64320
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-116]
	vldr	d17, [fp, #-108]
	vstmia	r3, {d16-d17}
	movw	r3, #64336
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64320
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov.i64	q10, #4294967295  @ v4si
	vstr	d20, [fp, #-132]
	vstr	d21, [fp, #-124]
	movw	r3, #64304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstmia	r3, {d16-d17}
	movw	r3, #64304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64288
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #64272
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	mov	r3, #64256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64272
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	mov	r3, #64256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61672
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov.i64	q9, #-4294967296  @ v4si
	vstr	d18, [fp, #-148]
	vstr	d19, [fp, #-140]
	movw	r3, #64240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-148]
	vldr	d17, [fp, #-140]
	vstmia	r3, {d16-d17}
	movw	r3, #64240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L75+64
	vldr	d21, .L75+72
	vstr	d20, [fp, #-164]
	vstr	d21, [fp, #-156]
	movw	r3, #64208
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-164]
	vldr	d17, [fp, #-156]
	vstmia	r3, {d16-d17}
	movw	r3, #64208
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #64176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64160
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64160
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L78
	vldr	d19, .L78+8
	vstr	d18, [fp, #-180]
	vstr	d19, [fp, #-172]
	movw	r3, #64144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64128
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-180]
	vldr	d17, [fp, #-172]
	vstmia	r3, {d16-d17}
	movw	r3, #64144
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64128
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L78+16
	vldr	d21, .L78+24
	vstr	d20, [fp, #-196]
	vstr	d21, [fp, #-188]
	movw	r3, #64112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64096
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-196]
	vldr	d17, [fp, #-188]
	vstmia	r3, {d16-d17}
	movw	r3, #64112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64096
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #64080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61640
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L78+32
	vldr	d19, .L78+40
	vstr	d18, [fp, #-212]
	vstr	d19, [fp, #-204]
	movw	r3, #64048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-212]
	vldr	d17, [fp, #-204]
	vstmia	r3, {d16-d17}
	movw	r3, #64048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64032
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L78+48
	vldr	d21, .L78+56
	vstr	d20, [fp, #-228]
	vstr	d21, [fp, #-220]
	movw	r3, #64016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	mov	r3, #64000
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-228]
	vldr	d17, [fp, #-220]
	vstmia	r3, {d16-d17}
	movw	r3, #64016
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	mov	r3, #64000
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #63984
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63968
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63984
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63968
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61624
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63952
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63936
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63952
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63936
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	b	.L79
.L80:
	.align	3
.L78:
	.word	-1
	.word	-1
	.word	-1
	.word	0
	.word	0
	.word	0
	.word	-1
	.word	-1
	.word	-1
	.word	0
	.word	0
	.word	-1
	.word	0
	.word	-1
	.word	0
	.word	0
	.word	-1
	.word	0
	.word	-1
	.word	-1
	.word	2
	.word	1
	.word	2
	.word	1
.L79:
	vldmia	r3, {d18-d19}
	movw	r3, #61656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63920
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63904
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63920
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63904
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63888
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63888
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63856
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63840
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63856
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63840
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #28
	vldmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #44
	vldmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #60
	vldmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	sub	r3, fp, #3712
	sub	r3, r3, #4
	sub	r3, r3, #8
	add	r3, r3, #12
	vldmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L78+64
	vldr	d19, .L78+72
	vstr	d18, [fp, #-244]
	vstr	d19, [fp, #-236]
	movw	r3, #63824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-244]
	vldr	d17, [fp, #-236]
	vstmia	r3, {d16-d17}
	movw	r3, #63824
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63808
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vldr	d18, .L78+80
	vldr	d19, .L78+88
	vstr	d18, [fp, #-260]
	vstr	d19, [fp, #-252]
	movw	r3, #63792
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63776
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-260]
	vldr	d17, [fp, #-252]
	vstmia	r3, {d16-d17}
	movw	r3, #63792
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63776
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L81
	vldr	d19, .L81+8
	vstr	d18, [fp, #-276]
	vstr	d19, [fp, #-268]
	movw	r3, #63760
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	mov	r3, #63744
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-276]
	vldr	d17, [fp, #-268]
	vstmia	r3, {d16-d17}
	movw	r3, #63760
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	mov	r3, #63744
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vldr	d18, .L81+16
	vldr	d19, .L81+24
	vstr	d18, [fp, #-292]
	vstr	d19, [fp, #-284]
	movw	r3, #63728
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63712
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-292]
	vldr	d17, [fp, #-284]
	vstmia	r3, {d16-d17}
	movw	r3, #63728
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63712
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L81+32
	vldr	d19, .L81+40
	vstr	d18, [fp, #-308]
	vstr	d19, [fp, #-300]
	movw	r3, #63696
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63680
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-308]
	vldr	d17, [fp, #-300]
	vstmia	r3, {d16-d17}
	movw	r3, #63696
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63680
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vldr	d18, .L81+48
	vldr	d19, .L81+56
	vstr	d18, [fp, #-324]
	vstr	d19, [fp, #-316]
	movw	r3, #63664
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63648
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-324]
	vldr	d17, [fp, #-316]
	vstmia	r3, {d16-d17}
	movw	r3, #63664
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63648
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L81+64
	vldr	d19, .L81+72
	vstr	d18, [fp, #-340]
	vstr	d19, [fp, #-332]
	movw	r3, #63632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63616
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-340]
	vldr	d17, [fp, #-332]
	vstmia	r3, {d16-d17}
	movw	r3, #63632
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63616
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vldr	d18, .L81+80
	vldr	d19, .L81+88
	vstr	d18, [fp, #-356]
	vstr	d19, [fp, #-348]
	movw	r3, #63600
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63584
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-356]
	vldr	d17, [fp, #-348]
	vstmia	r3, {d16-d17}
	movw	r3, #63600
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63584
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61624
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61672
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63568
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63568
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63552
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	b	.L82
.L83:
	.align	3
.L81:
	.word	-1
	.word	0
	.word	-1
	.word	-1
	.word	4
	.word	3
	.word	2
	.word	2
	.word	-1
	.word	-1
	.word	0
	.word	-1
	.word	3
	.word	3
	.word	2
	.word	3
	.word	-1
	.word	-1
	.word	0
	.word	0
	.word	3
	.word	3
	.word	0
	.word	2
	.word	-1
	.word	-1
	.word	-2
	.word	-2
	.word	3
	.word	2
	.word	1
	.word	2
	.word	4
	.word	8
	.word	12
	.word	15
.L82:
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61656
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63536
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63520
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63536
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63520
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63504
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	mov	r3, #63488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63504
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	mov	r3, #63488
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L81+96
	vldr	d19, .L81+104
	vstr	d18, [fp, #-372]
	vstr	d19, [fp, #-364]
	movw	r3, #63440
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63424
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-372]
	vldr	d17, [fp, #-364]
	vstmia	r3, {d16-d17}
	movw	r3, #63440
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63424
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #61736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L81+112
	vldr	d21, .L81+120
	vstr	d20, [fp, #-388]
	vstr	d21, [fp, #-380]
	movw	r3, #63408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63392
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-388]
	vldr	d17, [fp, #-380]
	vstmia	r3, {d16-d17}
	movw	r3, #63408
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63392
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #63376
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63376
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63360
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L81+128
	vldr	d21, .L81+136
	vstr	d20, [fp, #-404]
	vstr	d21, [fp, #-396]
	movw	r3, #63344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-404]
	vldr	d17, [fp, #-396]
	vstmia	r3, {d16-d17}
	movw	r3, #63344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #61736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L84
	vldr	d23, .L84+8
	vstr	d22, [fp, #-420]
	vstr	d23, [fp, #-412]
	movw	r3, #63312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-420]
	vldr	d17, [fp, #-412]
	vstmia	r3, {d16-d17}
	movw	r3, #63312
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63296
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #63280
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #63264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63280
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63264
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L84+16
	vldr	d23, .L84+24
	vstr	d22, [fp, #-436]
	vstr	d23, [fp, #-428]
	movw	r3, #63248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	mov	r3, #63232
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-436]
	vldr	d17, [fp, #-428]
	vstmia	r3, {d16-d17}
	movw	r3, #63248
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #63232
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #61736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L84+32
	vldr	d25, .L84+40
	vstr	d24, [fp, #-452]
	vstr	d25, [fp, #-444]
	movw	r3, #63216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-452]
	vldr	d17, [fp, #-444]
	vstmia	r3, {d16-d17}
	movw	r3, #63216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #63184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d22-d23}
	movw	r3, #63168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	movw	r3, #63168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #63152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #63136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #63120
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63104
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63120
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63104
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L84+48
	vldr	d19, .L84+56
	vstr	d18, [fp, #-468]
	vstr	d19, [fp, #-460]
	movw	r3, #63088
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63072
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-468]
	vldr	d17, [fp, #-460]
	vstmia	r3, {d16-d17}
	movw	r3, #63088
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63072
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #61720
	movt	r3, 65535
	b	.L85
.L86:
	.align	3
.L84:
	.word	-28
	.word	-24
	.word	-20
	.word	-17
	.word	19
	.word	23
	.word	25
	.word	29
	.word	-13
	.word	-9
	.word	-7
	.word	-3
	.word	-1
	.word	-1
	.word	-1
	.word	-2
	.word	0
	.word	3
	.word	2
	.word	1
	.word	0
	.word	4
	.word	8
	.word	12
	.word	0
	.word	-28
	.word	-24
	.word	-20
	.word	0
	.word	19
	.word	23
	.word	25
	.word	0
	.word	-13
	.word	-9
	.word	-7
.L85:
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L84+64
	vldr	d21, .L84+72
	vstr	d20, [fp, #-484]
	vstr	d21, [fp, #-476]
	movw	r3, #63056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63040
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-484]
	vldr	d17, [fp, #-476]
	vstmia	r3, {d16-d17}
	movw	r3, #63056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63040
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #63024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L84+80
	vldr	d21, .L84+88
	vstr	d20, [fp, #-500]
	vstr	d21, [fp, #-492]
	movw	r3, #62992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	mov	r3, #62976
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-500]
	vldr	d17, [fp, #-492]
	vstmia	r3, {d16-d17}
	movw	r3, #62992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	mov	r3, #62976
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #61720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L84+96
	vldr	d23, .L84+104
	vstr	d22, [fp, #-516]
	vstr	d23, [fp, #-508]
	movw	r3, #62960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-516]
	vldr	d17, [fp, #-508]
	vstmia	r3, {d16-d17}
	movw	r3, #62960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #62928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #62912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L84+112
	vldr	d23, .L84+120
	vstr	d22, [fp, #-532]
	vstr	d23, [fp, #-524]
	movw	r3, #62896
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-532]
	vldr	d17, [fp, #-524]
	vstmia	r3, {d16-d17}
	movw	r3, #62896
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #61720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L84+128
	vldr	d25, .L84+136
	vstr	d24, [fp, #-548]
	vstr	d25, [fp, #-540]
	movw	r3, #62864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-548]
	vldr	d17, [fp, #-540]
	vstmia	r3, {d16-d17}
	movw	r3, #62864
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #62832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #62816
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #62816
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #62800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #62784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #62768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L87
	vldr	d19, .L87+8
	vstr	d18, [fp, #-564]
	vstr	d19, [fp, #-556]
	movw	r3, #62736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	mov	r3, #62720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-564]
	vldr	d17, [fp, #-556]
	vstmia	r3, {d16-d17}
	movw	r3, #62736
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	mov	r3, #62720
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #61704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L87+16
	vldr	d21, .L87+24
	vstr	d20, [fp, #-580]
	vstr	d21, [fp, #-572]
	movw	r3, #62704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-580]
	vldr	d17, [fp, #-572]
	vstmia	r3, {d16-d17}
	movw	r3, #62704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62672
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62672
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62656
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L87+32
	vldr	d21, .L87+40
	vstr	d20, [fp, #-596]
	vstr	d21, [fp, #-588]
	movw	r3, #62640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-596]
	vldr	d17, [fp, #-588]
	vstmia	r3, {d16-d17}
	movw	r3, #62640
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62624
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #61704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L87+48
	vldr	d23, .L87+56
	vstr	d22, [fp, #-612]
	vstr	d23, [fp, #-604]
	movw	r3, #62608
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62592
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-612]
	vldr	d17, [fp, #-604]
	vstmia	r3, {d16-d17}
	movw	r3, #62608
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62592
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #62576
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62576
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #62560
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L87+64
	vldr	d23, .L87+72
	vstr	d22, [fp, #-628]
	vstr	d23, [fp, #-620]
	movw	r3, #62544
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62528
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-628]
	vldr	d17, [fp, #-620]
	vstmia	r3, {d16-d17}
	movw	r3, #62544
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62528
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #61704
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L87+80
	vldr	d25, .L87+88
	vstr	d24, [fp, #-644]
	vstr	d25, [fp, #-636]
	movw	r3, #62512
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62496
	movt	r3, 65535
	sub	r1, fp, #4
	b	.L88
.L89:
	.align	3
.L87:
	.word	-2
	.word	-1
	.word	-1
	.word	-1
	.word	2
	.word	0
	.word	3
	.word	2
	.word	15
	.word	0
	.word	4
	.word	8
	.word	-17
	.word	0
	.word	-28
	.word	-24
	.word	29
	.word	0
	.word	19
	.word	23
	.word	-3
	.word	0
	.word	-13
	.word	-9
	.word	-2
	.word	-2
	.word	-1
	.word	-1
	.word	1
	.word	2
	.word	0
	.word	3
	.word	12
	.word	15
	.word	0
	.word	4
	.word	-20
	.word	-17
	.word	0
	.word	-28
.L88:
	add	r3, r1, r3
	vldr	d16, [fp, #-644]
	vldr	d17, [fp, #-636]
	vstmia	r3, {d16-d17}
	movw	r3, #62512
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62496
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #62480
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d22-d23}
	mov	r3, #62464
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62480
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	mov	r3, #62464
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #62448
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62448
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #62432
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #62416
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62416
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62400
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #61768
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L87+96
	vldr	d19, .L87+104
	vstr	d18, [fp, #-660]
	vstr	d19, [fp, #-652]
	movw	r3, #62384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-660]
	vldr	d17, [fp, #-652]
	vstmia	r3, {d16-d17}
	movw	r3, #62384
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62368
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #61688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L87+112
	vldr	d21, .L87+120
	vstr	d20, [fp, #-676]
	vstr	d21, [fp, #-668]
	movw	r3, #62352
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62336
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-676]
	vldr	d17, [fp, #-668]
	vstmia	r3, {d16-d17}
	movw	r3, #62352
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62336
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #62320
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62320
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62304
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L87+128
	vldr	d21, .L87+136
	vstr	d20, [fp, #-692]
	vstr	d21, [fp, #-684]
	movw	r3, #62288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-692]
	vldr	d17, [fp, #-684]
	vstmia	r3, {d16-d17}
	movw	r3, #62288
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62272
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	vmov	q10, q8  @ v4si
	movw	r3, #61688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L87+144
	vldr	d23, .L87+152
	vstr	d22, [fp, #-708]
	vstr	d23, [fp, #-700]
	movw	r3, #62256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-708]
	vldr	d17, [fp, #-700]
	vstmia	r3, {d16-d17}
	movw	r3, #62256
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62240
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	movw	r3, #62224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	mov	r3, #62208
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62224
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	mov	r3, #62208
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L90
	vldr	d23, .L90+8
	vstr	d22, [fp, #-724]
	vstr	d23, [fp, #-716]
	movw	r3, #62192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-724]
	vldr	d17, [fp, #-716]
	vstmia	r3, {d16-d17}
	movw	r3, #62192
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62176
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #61688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d24, .L90+16
	vldr	d25, .L90+24
	vstr	d24, [fp, #-740]
	vstr	d25, [fp, #-732]
	movw	r3, #62160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-740]
	vldr	d17, [fp, #-732]
	vstmia	r3, {d16-d17}
	movw	r3, #62160
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62144
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d24-d25}
	vshl.u32	q8, q8, q12
	movw	r3, #62128
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #62112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62128
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #62112
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #62096
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #62080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62096
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #62080
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	movw	r3, #62064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #62048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62064
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #62048
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #4
	vldmia	r3, {d16-d17}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #20
	vldmia	r3, {d16-d17}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #36
	vldmia	r3, {d16-d17}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	add	r3, r3, #52
	vldmia	r3, {d16-d17}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-3924]
	ldr	r2, [r3, #0]
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	str	r2, [fp, #-3496]
	movw	r3, #62024
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r2, [fp, #-3496]
	movw	r3, #62024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vmov.32	d17[1], r2
	movw	r3, #61688
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61736
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #62008
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #62008
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61992
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	b	.L91
.L92:
	.align	3
.L90:
	.word	25
	.word	29
	.word	0
	.word	19
	.word	-7
	.word	-3
	.word	0
	.word	-13
.L91:
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61720
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61784
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61704
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61800
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #61688
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #61896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #61912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #61896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #61752
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r2, [fp, #-3920]
	movw	r3, #61800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-3920]
	add	r2, r3, #16
	movw	r3, #61784
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-3920]
	add	r2, r3, #32
	movw	r3, #61768
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-3920]
	add	r2, r3, #48
	movw	r3, #61752
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	bmw_small_f0, .-bmw_small_f0
	.align	2
	.type	bmw_small_f2, %function
bmw_small_f2:
	@ args = 0, pretend = 0, frame = 1752
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #1744
	sub	sp, sp, #12
	str	r0, [fp, #-1744]
	str	r1, [fp, #-1748]
	str	r2, [fp, #-1752]
	ldr	r3, [fp, #-1748]
	add	r3, r3, #64
	vldmia	r3, {d16-d17}
	movw	r3, #63928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #80
	vldmia	r3, {d16-d17}
	movw	r3, #63912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #96
	vldmia	r3, {d16-d17}
	movw	r3, #63896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #112
	vldmia	r3, {d16-d17}
	movw	r3, #63880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-180]
	vstr	d19, [fp, #-172]
	vstr	d16, [fp, #-196]
	vstr	d17, [fp, #-188]
	vldr	d18, [fp, #-180]
	vldr	d19, [fp, #-172]
	vldr	d16, [fp, #-196]
	vldr	d17, [fp, #-188]
	veor	q8, q9, q8
	movw	r3, #63960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63896
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #63880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-212]
	vstr	d19, [fp, #-204]
	vstr	d16, [fp, #-228]
	vstr	d17, [fp, #-220]
	vldr	d18, [fp, #-212]
	vldr	d19, [fp, #-204]
	vldr	d16, [fp, #-228]
	vldr	d17, [fp, #-220]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #63960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-244]
	vstr	d19, [fp, #-236]
	vstr	d16, [fp, #-260]
	vstr	d17, [fp, #-252]
	vldr	d18, [fp, #-244]
	vldr	d19, [fp, #-236]
	vldr	d16, [fp, #-260]
	vldr	d17, [fp, #-252]
	veor	q8, q9, q8
	movw	r3, #63944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d16, [fp, #-276]
	vstr	d17, [fp, #-268]
	vldr	d16, [fp, #-276]
	vldr	d17, [fp, #-268]
	vmov	d18, d16  @ v2si
	movw	r3, #63960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d16, [fp, #-292]
	vstr	d17, [fp, #-284]
	vldr	d16, [fp, #-292]
	vldr	d17, [fp, #-284]
	vmov	d16, d17
	fstd	d18, [fp, #-300]
	fstd	d16, [fp, #-308]
	fldd	d17, [fp, #-300]
	fldd	d16, [fp, #-308]
	veor	d16, d17, d16
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d17, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fldd	d16, [r3, #0]
	fstd	d16, [fp, #-316]
	fldd	d16, [fp, #-316]
	fmrrd	r2, r3, d16	@ int
	str	r2, [fp, #-324]
	str	r3, [fp, #-320]
	fldd	d16, [fp, #-324]	@ int
	vshr.u64	d16, d16, #32
	fmrrd	r2, r3, d16	@ int
	str	r2, [fp, #-332]
	str	r3, [fp, #-328]
	fldd	d16, [fp, #-332]	@ int
	fstd	d17, [fp, #-340]
	fstd	d16, [fp, #-348]
	fldd	d17, [fp, #-340]
	fldd	d16, [fp, #-348]
	veor	d16, d17, d16
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fldd	d16, [r3, #0]
	fstd	d16, [fp, #-356]
	fldd	d16, [fp, #-356]
	vdup.32	q8, d16[0]
	movw	r3, #63960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d16, [fp, #-372]
	vstr	d17, [fp, #-364]
	vldr	d16, [fp, #-372]
	vldr	d17, [fp, #-364]
	vmov	d18, d16  @ v2si
	movw	r3, #63944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d16, [fp, #-388]
	vstr	d17, [fp, #-380]
	vldr	d16, [fp, #-388]
	vldr	d17, [fp, #-380]
	vmov	d16, d17
	fstd	d18, [fp, #-396]
	fstd	d16, [fp, #-404]
	fldd	d17, [fp, #-396]
	fldd	d16, [fp, #-404]
	veor	d16, d17, d16
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fstd	d16, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fldd	d17, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fldd	d16, [r3, #0]
	fstd	d16, [fp, #-412]
	fldd	d16, [fp, #-412]
	fmrrd	r2, r3, d16	@ int
	str	r2, [fp, #-420]
	str	r3, [fp, #-416]
	fldd	d16, [fp, #-420]	@ int
	vshr.u64	d16, d16, #32
	fmrrd	r2, r3, d16	@ int
	str	r2, [fp, #-428]
	str	r3, [fp, #-424]
	fldd	d16, [fp, #-428]	@ int
	fstd	d17, [fp, #-436]
	fstd	d16, [fp, #-444]
	fldd	d17, [fp, #-436]
	fldd	d16, [fp, #-444]
	veor	d16, d17, d16
	movw	r3, #63872
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	fstd	d16, [r3, #0]
	movw	r3, #63872
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	fldd	d16, [r3, #0]
	fstd	d16, [fp, #-452]
	fldd	d16, [fp, #-452]
	vdup.32	q8, d16[0]
	movw	r3, #63944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L94
	vldr	d19, .L94+8
	vstr	d18, [fp, #-20]
	vstr	d19, [fp, #-12]
	vstr	d16, [fp, #-468]
	vstr	d17, [fp, #-460]
	vldr	d16, [fp, #-20]
	vldr	d17, [fp, #-12]
	vstr	d16, [fp, #-484]
	vstr	d17, [fp, #-476]
	vldr	d16, [fp, #-468]
	vldr	d17, [fp, #-460]
	vldr	d18, [fp, #-484]
	vldr	d19, [fp, #-476]
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #63928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L94+16
	vldr	d21, .L94+24
	vstr	d20, [fp, #-36]
	vstr	d21, [fp, #-28]
	vstr	d16, [fp, #-500]
	vstr	d17, [fp, #-492]
	vldr	d16, [fp, #-36]
	vldr	d17, [fp, #-28]
	vstr	d16, [fp, #-516]
	vstr	d17, [fp, #-508]
	vldr	d16, [fp, #-500]
	vldr	d17, [fp, #-492]
	vldr	d20, [fp, #-516]
	vldr	d21, [fp, #-508]
	vshl.u32	q8, q8, q10
	vstr	d18, [fp, #-532]
	vstr	d19, [fp, #-524]
	vstr	d16, [fp, #-548]
	vstr	d17, [fp, #-540]
	vldr	d18, [fp, #-532]
	vldr	d19, [fp, #-524]
	vldr	d16, [fp, #-548]
	vldr	d17, [fp, #-540]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	ldr	r3, [fp, #-1752]
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-564]
	vstr	d19, [fp, #-556]
	vstr	d16, [fp, #-580]
	vstr	d17, [fp, #-572]
	vldr	d18, [fp, #-564]
	vldr	d19, [fp, #-556]
	vldr	d16, [fp, #-580]
	vldr	d17, [fp, #-572]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #63960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63896
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstr	d20, [fp, #-596]
	vstr	d21, [fp, #-588]
	vstr	d16, [fp, #-612]
	vstr	d17, [fp, #-604]
	vldr	d20, [fp, #-596]
	vldr	d21, [fp, #-588]
	vldr	d16, [fp, #-612]
	vldr	d17, [fp, #-604]
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	ldr	r3, [fp, #-1748]
	vldmia	r3, {d16-d17}
	vstr	d20, [fp, #-628]
	vstr	d21, [fp, #-620]
	vstr	d16, [fp, #-644]
	vstr	d17, [fp, #-636]
	vldr	d20, [fp, #-628]
	vldr	d21, [fp, #-620]
	vldr	d16, [fp, #-644]
	vldr	d17, [fp, #-636]
	veor	q8, q10, q8
	vstr	d18, [fp, #-660]
	vstr	d19, [fp, #-652]
	vstr	d16, [fp, #-676]
	vstr	d17, [fp, #-668]
	vldr	d16, [fp, #-660]
	vldr	d17, [fp, #-652]
	vldr	d18, [fp, #-676]
	vldr	d19, [fp, #-668]
	vadd.i32	q8, q8, q9
	movw	r3, #63848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L94+32
	vldr	d19, .L94+40
	vstr	d18, [fp, #-52]
	vstr	d19, [fp, #-44]
	vstr	d16, [fp, #-692]
	vstr	d17, [fp, #-684]
	vldr	d16, [fp, #-52]
	vldr	d17, [fp, #-44]
	vstr	d16, [fp, #-708]
	vstr	d17, [fp, #-700]
	vldr	d16, [fp, #-692]
	vldr	d17, [fp, #-684]
	vldr	d18, [fp, #-708]
	vldr	d19, [fp, #-700]
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #63912
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L94+48
	vldr	d21, .L94+56
	vstr	d20, [fp, #-68]
	vstr	d21, [fp, #-60]
	vstr	d16, [fp, #-724]
	vstr	d17, [fp, #-716]
	vldr	d16, [fp, #-68]
	vldr	d17, [fp, #-60]
	vstr	d16, [fp, #-740]
	vstr	d17, [fp, #-732]
	vldr	d16, [fp, #-724]
	vldr	d17, [fp, #-716]
	vldr	d20, [fp, #-740]
	vldr	d21, [fp, #-732]
	vshl.u32	q8, q8, q10
	vstr	d18, [fp, #-756]
	vstr	d19, [fp, #-748]
	vstr	d16, [fp, #-772]
	vstr	d17, [fp, #-764]
	vldr	d18, [fp, #-756]
	vldr	d19, [fp, #-748]
	vldr	d16, [fp, #-772]
	vldr	d17, [fp, #-764]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	ldr	r3, [fp, #-1752]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	vstr	d18, [fp, #-788]
	vstr	d19, [fp, #-780]
	vstr	d16, [fp, #-804]
	vstr	d17, [fp, #-796]
	vldr	d18, [fp, #-788]
	vldr	d19, [fp, #-780]
	vldr	d16, [fp, #-804]
	vldr	d17, [fp, #-796]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #63960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63880
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vstr	d20, [fp, #-820]
	vstr	d21, [fp, #-812]
	vstr	d16, [fp, #-836]
	vstr	d17, [fp, #-828]
	vldr	d20, [fp, #-820]
	vldr	d21, [fp, #-812]
	vldr	d16, [fp, #-836]
	vldr	d17, [fp, #-828]
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	ldr	r3, [fp, #-1748]
	add	r3, r3, #16
	vldmia	r3, {d16-d17}
	vstr	d20, [fp, #-852]
	vstr	d21, [fp, #-844]
	vstr	d16, [fp, #-868]
	vstr	d17, [fp, #-860]
	vldr	d20, [fp, #-852]
	vldr	d21, [fp, #-844]
	vldr	d16, [fp, #-868]
	vldr	d17, [fp, #-860]
	veor	q8, q10, q8
	vstr	d18, [fp, #-884]
	vstr	d19, [fp, #-876]
	vstr	d16, [fp, #-900]
	vstr	d17, [fp, #-892]
	vldr	d16, [fp, #-884]
	vldr	d17, [fp, #-876]
	vldr	d18, [fp, #-900]
	vldr	d19, [fp, #-892]
	vadd.i32	q8, q8, q9
	movw	r3, #63832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #60
	vldmia	r3, {d16-d17}
	movw	r3, #63928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #92
	ldr	r2, [r3, #0]
	movw	r3, #63928
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	str	r2, [fp, #-904]
	vstr	d16, [fp, #-924]
	vstr	d17, [fp, #-916]
	ldr	r3, [fp, #-904]
	vldr	d16, [fp, #-924]
	vldr	d17, [fp, #-916]
	vmov.32	d16[0], r3
	movw	r3, #63928
	b	.L95
.L96:
	.align	3
.L94:
	.word	5
	.word	-7
	.word	-5
	.word	-1
	.word	-5
	.word	8
	.word	5
	.word	5
	.word	-3
	.word	6
	.word	-4
	.word	-11
	.word	0
	.word	-6
	.word	6
	.word	2
	.word	9
	.word	10
	.word	11
	.word	12
	.word	-23
	.word	-22
	.word	-21
	.word	-20
	.word	8
	.word	-6
	.word	6
	.word	4
.L95:
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L94+64
	vldr	d19, .L94+72
	vstr	d18, [fp, #-84]
	vstr	d19, [fp, #-76]
	vstr	d16, [fp, #-940]
	vstr	d17, [fp, #-932]
	vldr	d16, [fp, #-84]
	vldr	d17, [fp, #-76]
	vstr	d16, [fp, #-956]
	vstr	d17, [fp, #-948]
	vldr	d16, [fp, #-940]
	vldr	d17, [fp, #-932]
	vldr	d18, [fp, #-956]
	vldr	d19, [fp, #-948]
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #63832
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L94+80
	vldr	d21, .L94+88
	vstr	d20, [fp, #-100]
	vstr	d21, [fp, #-92]
	vstr	d16, [fp, #-972]
	vstr	d17, [fp, #-964]
	vldr	d16, [fp, #-100]
	vldr	d17, [fp, #-92]
	vstr	d16, [fp, #-988]
	vstr	d17, [fp, #-980]
	vldr	d16, [fp, #-972]
	vldr	d17, [fp, #-964]
	vldr	d20, [fp, #-988]
	vldr	d21, [fp, #-980]
	vshl.u32	q8, q8, q10
	vstr	d18, [fp, #-1004]
	vstr	d19, [fp, #-996]
	vstr	d16, [fp, #-1020]
	vstr	d17, [fp, #-1012]
	vldr	d18, [fp, #-1004]
	vldr	d19, [fp, #-996]
	vldr	d16, [fp, #-1020]
	vldr	d17, [fp, #-1012]
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #63944
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63896
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	ldr	r3, [fp, #-1752]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #64504
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #64488
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64504
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #64488
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #64472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #64456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64472
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64456
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #63960
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L94+96
	vldr	d23, .L94+104
	vstr	d22, [fp, #-116]
	vstr	d23, [fp, #-108]
	movw	r3, #64440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64424
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-116]
	vldr	d17, [fp, #-108]
	vstmia	r3, {d16-d17}
	movw	r3, #64440
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64424
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	movw	r3, #63928
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d24-d25}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #32
	vldmia	r3, {d16-d17}
	movw	r3, #64408
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d24-d25}
	movw	r3, #64392
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64408
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d24-d25}
	movw	r3, #64392
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q12, q8
	movw	r3, #64376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #64360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64376
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #64360
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #64344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #64328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64344
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64328
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vadd.i32	q8, q8, q10
	movw	r3, #64312
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64296
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64312
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64296
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #63816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63848
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d18, .L97
	vldr	d19, .L97+8
	vstr	d18, [fp, #-132]
	vstr	d19, [fp, #-124]
	movw	r3, #64280
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64264
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstmia	r3, {d16-d17}
	movw	r3, #64280
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64264
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vshl.u32	q8, q8, q9
	vmov	q9, q8  @ v4si
	movw	r3, #63848
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vldr	d20, .L97+16
	vldr	d21, .L97+24
	vstr	d20, [fp, #-148]
	vstr	d21, [fp, #-140]
	movw	r3, #64248
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64232
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldr	d16, [fp, #-148]
	vldr	d17, [fp, #-140]
	vstmia	r3, {d16-d17}
	movw	r3, #64248
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64232
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d20-d21}
	vshl.u32	q8, q8, q10
	movw	r3, #64216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d18-d19}
	movw	r3, #64200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64216
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	movw	r3, #64200
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q9, q8
	vmov	q9, q8  @ v4si
	movw	r3, #63944
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #63880
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	ldr	r3, [fp, #-1752]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #64184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d22-d23}
	movw	r3, #64168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64184
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	movw	r3, #64168
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #64152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d20-d21}
	movw	r3, #64136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64152
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	movw	r3, #64136
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	veor	q8, q10, q8
	vmov	q10, q8  @ v4si
	movw	r3, #63960
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	vldr	d22, .L97+32
	vldr	d23, .L97+40
	vstr	d22, [fp, #-164]
	vstr	d23, [fp, #-156]
	movw	r3, #64120
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64104
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldr	d16, [fp, #-164]
	vldr	d17, [fp, #-156]
	vstmia	r3, {d16-d17}
	movw	r3, #64120
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64104
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d22-d23}
	vshl.u32	q8, q8, q11
	vmov	q11, q8  @ v4si
	ldr	r3, [fp, #-1748]
	add	r3, r3, #76
	vldmia	r3, {d24-d25}
	ldr	r3, [fp, #-1748]
	add	r3, r3, #48
	vldmia	r3, {d16-d17}
	movw	r3, #64088
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d24-d25}
	movw	r3, #64072
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64088
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d24-d25}
	movw	r3, #64072
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q12, q8
	movw	r3, #64056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d22-d23}
	movw	r3, #64040
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64056
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d22-d23}
	movw	r3, #64040
	movt	r3, 65535
	b	.L98
.L99:
	.align	3
.L97:
	.word	13
	.word	14
	.word	15
	.word	16
	.word	-19
	.word	-18
	.word	-17
	.word	-16
	.word	-3
	.word	-4
	.word	-7
	.word	-2
.L98:
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d16-d17}
	veor	q8, q11, q8
	movw	r3, #64024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d20-d21}
	movw	r3, #64008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #64024
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #64008
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d20-d21}
	vadd.i32	q8, q8, q10
	movw	r3, #63992
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d18-d19}
	movw	r3, #63976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vstmia	r3, {d16-d17}
	movw	r3, #63992
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	movw	r3, #63976
	movt	r3, 65535
	sub	r2, fp, #4
	add	r3, r2, r3
	vldmia	r3, {d18-d19}
	vadd.i32	q8, q8, q9
	movw	r3, #63800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vstmia	r3, {d16-d17}
	ldr	r2, [fp, #-1744]
	movw	r3, #63848
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-1744]
	add	r2, r3, #16
	movw	r3, #63832
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-1744]
	add	r2, r3, #32
	movw	r3, #63816
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	ldr	r3, [fp, #-1744]
	add	r2, r3, #48
	movw	r3, #63800
	movt	r3, 65535
	sub	r1, fp, #4
	add	r3, r1, r3
	vldmia	r3, {d16-d17}
	vstmia	r2, {d16-d17}
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	bmw_small_f2, .-bmw_small_f2
	.align	2
	.global	bmw_small_nextBlock
	.type	bmw_small_nextBlock, %function
bmw_small_nextBlock:
	@ args = 0, pretend = 0, frame = 136
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #136
	str	r0, [fp, #-136]
	str	r1, [fp, #-140]
	ldr	r2, [fp, #-136]
	ldr	r3, [fp, #-140]
	sub	r1, fp, #132
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f0
	ldr	r2, [fp, #-140]
	ldr	r3, [fp, #-136]
	sub	r1, fp, #132
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f1
	ldr	r1, [fp, #-136]
	ldr	r3, [fp, #-140]
	sub	r2, fp, #132
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f2
	ldr	r3, [fp, #-136]
	ldr	r3, [r3, #64]
	add	r2, r3, #1
	ldr	r3, [fp, #-136]
	str	r2, [r3, #64]
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw_small_nextBlock, .-bmw_small_nextBlock
	.align	2
	.global	bmw_small_lastBlock
	.type	bmw_small_lastBlock, %function
bmw_small_lastBlock:
	@ args = 0, pretend = 0, frame = 216
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #216
	str	r0, [fp, #-216]
	str	r1, [fp, #-220]
	mov	r3, r2
	strh	r3, [fp, #-222]	@ movhi
	b	.L102
.L103:
	ldr	r0, [fp, #-216]
	ldr	r1, [fp, #-220]
	bl	bmw_small_nextBlock
	ldrh	r3, [fp, #-222]	@ movhi
	sub	r3, r3, #512
	strh	r3, [fp, #-222]	@ movhi
	ldr	r3, [fp, #-220]
	add	r3, r3, #64
	str	r3, [fp, #-220]
.L102:
	ldrh	r2, [fp, #-222]
	movw	r3, #511
	cmp	r2, r3
	bhi	.L103
	sub	r3, fp, #80
	mov	r0, r3
	mov	r1, #0
	mov	r2, #64
	bl	memset
	ldrh	r3, [fp, #-222]
	add	r3, r3, #7
	add	r2, r3, #7
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #3
	sub	r2, fp, #80
	mov	r0, r2
	ldr	r1, [fp, #-220]
	mov	r2, r3
	bl	memcpy
	ldrh	r3, [fp, #-222]
	mov	r3, r3, lsr #3
	uxth	r3, r3
	mov	r1, r3
	ldrh	r3, [fp, #-222]
	mov	r3, r3, lsr #3
	uxth	r3, r3
	mov	r2, r3
	mvn	r3, #67
	sub	r0, fp, #12
	add	r2, r0, r2
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	uxtb	r2, r3
	ldrh	r3, [fp, #-222]
	and	r3, r3, #7
	mov	r0, #128
	mov	r3, r0, asr r3
	uxtb	r3, r3
	orr	r3, r2, r3
	uxtb	r3, r3
	uxtb	r2, r3
	mvn	r3, #67
	sub	r0, fp, #12
	add	r1, r0, r1
	add	r3, r1, r3
	strb	r2, [r3, #0]
	ldrh	r3, [fp, #-222]
	add	r3, r3, #1
	cmp	r3, #448
	ble	.L104
	sub	r3, fp, #80
	ldr	r0, [fp, #-216]
	mov	r1, r3
	bl	bmw_small_nextBlock
	sub	r3, fp, #80
	mov	r0, r3
	mov	r1, #0
	mov	r2, #56
	bl	memset
	ldr	r3, [fp, #-216]
	ldr	r3, [r3, #64]
	sub	r2, r3, #1
	ldr	r3, [fp, #-216]
	str	r2, [r3, #64]
.L104:
	sub	r3, fp, #80
	add	ip, r3, #56
	ldr	r3, [fp, #-216]
	ldr	r3, [r3, #64]
	mov	r2, r3
	mov	r3, #0
	mov	r1, r2, lsr #23
	mov	r5, r3, asl #9
	orr	r5, r1, r5
	mov	r4, r2, asl #9
	mov	r0, r4
	mov	r1, r5
	ldrh	r3, [fp, #-222]
	mov	r2, r3
	mov	r3, #0
	adds	r2, r2, r0
	adc	r3, r3, r1
	strd	r2, [ip]
	sub	r3, fp, #80
	ldr	r0, [fp, #-216]
	mov	r1, r3
	bl	bmw_small_nextBlock
	sub	r3, fp, #80
	mov	r0, r3
	mov	r1, #170
	mov	r2, #64
	bl	memset
	mov	r3, #0
	strb	r3, [fp, #-13]
	b	.L105
.L106:
	ldrb	r3, [fp, #-13]	@ zero_extendqisi2
	mov	r1, r3, asl #2
	ldrb	r3, [fp, #-13]
	sub	r3, r3, #96
	uxtb	r2, r3
	mvn	r3, #67
	sub	r0, fp, #12
	add	r1, r0, r1
	add	r3, r1, r3
	strb	r2, [r3, #0]
	ldrb	r3, [fp, #-13]
	add	r3, r3, #1
	strb	r3, [fp, #-13]
.L105:
	ldrb	r3, [fp, #-13]	@ zero_extendqisi2
	cmp	r3, #15
	bls	.L106
	sub	r2, fp, #80
	ldr	r3, [fp, #-216]
	sub	r1, fp, #208
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f0
	ldr	r2, [fp, #-216]
	sub	r3, fp, #80
	sub	r1, fp, #208
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f1
	sub	r1, fp, #80
	ldr	r3, [fp, #-216]
	sub	r2, fp, #208
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	bmw_small_f2
	ldr	r2, [fp, #-216]
	sub	r3, fp, #80
	mov	r0, r2
	mov	r1, r3
	mov	r2, #64
	bl	memcpy
	sub	sp, fp, #12
	ldmfd	sp!, {r4, r5, fp, pc}
	.size	bmw_small_lastBlock, .-bmw_small_lastBlock
	.align	2
	.global	bmw224_init
	.type	bmw224_init, %function
bmw224_init:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	ldr	r2, [fp, #-16]
	movw	r3, #515
	movt	r3, 1
	str	r3, [r2, #0]
	mov	r3, #1
	strb	r3, [fp, #-5]
	b	.L108
.L109:
	ldrb	r1, [fp, #-5]	@ zero_extendqisi2
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	sub	r2, r3, #1
	ldr	r3, [fp, #-16]
	ldr	r2, [r3, r2, asl #2]
	movw	r3, #1028
	movt	r3, 1028
	add	r3, r2, r3
	ldr	r2, [fp, #-16]
	str	r3, [r2, r1, asl #2]
	ldrb	r3, [fp, #-5]
	add	r3, r3, #1
	strb	r3, [fp, #-5]
.L108:
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	cmp	r3, #15
	bls	.L109
	ldr	r3, [fp, #-16]
	mov	r2, #0
	str	r2, [r3, #64]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	bmw224_init, .-bmw224_init
	.align	2
	.global	bmw256_init
	.type	bmw256_init, %function
bmw256_init:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	ldr	r2, [fp, #-16]
	movw	r3, #16963
	movt	r3, 16449
	str	r3, [r2, #0]
	mov	r3, #1
	strb	r3, [fp, #-5]
	b	.L111
.L112:
	ldrb	r1, [fp, #-5]	@ zero_extendqisi2
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	sub	r2, r3, #1
	ldr	r3, [fp, #-16]
	ldr	r2, [r3, r2, asl #2]
	movw	r3, #1028
	movt	r3, 1028
	add	r3, r2, r3
	ldr	r2, [fp, #-16]
	str	r3, [r2, r1, asl #2]
	ldrb	r3, [fp, #-5]
	add	r3, r3, #1
	strb	r3, [fp, #-5]
.L111:
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	cmp	r3, #15
	bls	.L112
	ldr	r3, [fp, #-16]
	mov	r2, #0
	str	r2, [r3, #64]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	bmw256_init, .-bmw256_init
	.align	2
	.global	bmw224_nextBlock
	.type	bmw224_nextBlock, %function
bmw224_nextBlock:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r0, [fp, #-8]
	ldr	r1, [fp, #-12]
	bl	bmw_small_nextBlock
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw224_nextBlock, .-bmw224_nextBlock
	.align	2
	.global	bmw256_nextBlock
	.type	bmw256_nextBlock, %function
bmw256_nextBlock:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r0, [fp, #-8]
	ldr	r1, [fp, #-12]
	bl	bmw_small_nextBlock
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw256_nextBlock, .-bmw256_nextBlock
	.align	2
	.global	bmw224_lastBlock
	.type	bmw224_lastBlock, %function
bmw224_lastBlock:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	mov	r3, r2
	strh	r3, [fp, #-14]	@ movhi
	ldrh	r3, [fp, #-14]
	ldr	r0, [fp, #-8]
	ldr	r1, [fp, #-12]
	mov	r2, r3
	bl	bmw_small_lastBlock
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw224_lastBlock, .-bmw224_lastBlock
	.align	2
	.global	bmw256_lastBlock
	.type	bmw256_lastBlock, %function
bmw256_lastBlock:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	mov	r3, r2
	strh	r3, [fp, #-14]	@ movhi
	ldrh	r3, [fp, #-14]
	ldr	r0, [fp, #-8]
	ldr	r1, [fp, #-12]
	mov	r2, r3
	bl	bmw_small_lastBlock
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw256_lastBlock, .-bmw256_lastBlock
	.align	2
	.global	bmw224_ctx2hash
	.type	bmw224_ctx2hash, %function
bmw224_ctx2hash:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r3, [fp, #-12]
	add	r3, r3, #36
	ldr	r0, [fp, #-8]
	mov	r1, r3
	mov	r2, #28
	bl	memcpy
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw224_ctx2hash, .-bmw224_ctx2hash
	.align	2
	.global	bmw256_ctx2hash
	.type	bmw256_ctx2hash, %function
bmw256_ctx2hash:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r3, [fp, #-12]
	add	r3, r3, #32
	ldr	r0, [fp, #-8]
	mov	r1, r3
	mov	r2, #32
	bl	memcpy
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw256_ctx2hash, .-bmw256_ctx2hash
	.align	2
	.global	bmw224
	.type	bmw224, %function
bmw224:
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #88
	str	r0, [fp, #-80]
	str	r1, [fp, #-84]
	str	r2, [fp, #-88]
	sub	r3, fp, #72
	mov	r0, r3
	bl	bmw224_init
	b	.L120
.L121:
	sub	r3, fp, #72
	mov	r0, r3
	ldr	r1, [fp, #-84]
	bl	bmw_small_nextBlock
	ldr	r3, [fp, #-88]
	sub	r3, r3, #512
	str	r3, [fp, #-88]
	ldr	r3, [fp, #-84]
	add	r3, r3, #64
	str	r3, [fp, #-84]
.L120:
	ldr	r2, [fp, #-88]
	movw	r3, #511
	cmp	r2, r3
	bhi	.L121
	ldr	r3, [fp, #-88]
	uxth	r3, r3
	sub	r2, fp, #72
	mov	r0, r2
	ldr	r1, [fp, #-84]
	mov	r2, r3
	bl	bmw_small_lastBlock
	sub	r3, fp, #72
	ldr	r0, [fp, #-80]
	mov	r1, r3
	bl	bmw224_ctx2hash
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw224, .-bmw224
	.align	2
	.global	bmw256
	.type	bmw256, %function
bmw256:
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #88
	str	r0, [fp, #-80]
	str	r1, [fp, #-84]
	str	r2, [fp, #-88]
	sub	r3, fp, #72
	mov	r0, r3
	bl	bmw256_init
	b	.L123
.L124:
	sub	r3, fp, #72
	mov	r0, r3
	ldr	r1, [fp, #-84]
	bl	bmw_small_nextBlock
	ldr	r3, [fp, #-88]
	sub	r3, r3, #512
	str	r3, [fp, #-88]
	ldr	r3, [fp, #-84]
	add	r3, r3, #64
	str	r3, [fp, #-84]
.L123:
	ldr	r2, [fp, #-88]
	movw	r3, #511
	cmp	r2, r3
	bhi	.L124
	ldr	r3, [fp, #-88]
	uxth	r3, r3
	sub	r2, fp, #72
	mov	r0, r2
	ldr	r1, [fp, #-84]
	mov	r2, r3
	bl	bmw_small_lastBlock
	sub	r3, fp, #72
	ldr	r0, [fp, #-80]
	mov	r1, r3
	bl	bmw256_ctx2hash
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	bmw256, .-bmw256
	.ident	"GCC: (GNU) 4.5.1"
