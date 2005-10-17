	.LEVEL 1.1
.globl count
	.data
	.align 8
	.type	count,@object
	.size	count,8
count:
	.word 0
	.word 0
.globl divisor
	.align 8
	.type	divisor,@object
	.size	divisor,8
divisor:
	.word 0
	.word 1024
.globl automatic
	.align 4
	.type	automatic,@object
	.size	automatic,4
automatic:
	.word 1
.globl suffix
	.align 4
	.type	suffix,@object
	.size	suffix,1
suffix:
	.byte 98
.globl suf_list
	.section	.rodata
	.align 4
.LC0:
.stringz"bKMGTE"
	.data
	.align 4
	.type	suf_list,@object
	.size	suf_list,4
suf_list:
	.word .LC0
.globl append
	.align 4
	.type	append,@object
	.size	append,4
append:
	.word 0
	.align 4
	.type	t_div___0,@object
	.size	t_div___0,4
t_div___0:
	.word 1
	.align 4
	.type	suf_index___1,@object
	.size	suf_index___1,4
suf_index___1:
	.word 0
.globl __udivdi3
.globl __cmpdi2
	.section	.rodata
	.align 4
.LC1:
.stringz"%'15.3f %c\n"
	.text
	.align 4
.globl writer_auto_append
		.type		 writer_auto_append,@function
writer_auto_append:
	.PROC
	.CALLINFO FRAME=128,CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=3
	.ENTRY
	stw %r2,-20(%r30)
	copy %r3,%r1
	copy %r30,%r3
	stwm %r1,128(%r30)
	nop
.L2:
	addil LR't_div___0-$global$,%r27
	ldo RR't_div___0-$global$(%r1),%r22
	ldw 0(%r22),%r19
	extrs %r19,0,1,%r19
	ldw 0(%r22),%r21
	copy %r19,%r20
	addil LR'count-$global$,%r27
	ldw RR'count-$global$(%r1),%r25
	ldw RR'count-$global$+4(%r1),%r26
	copy %r20,%r23
	copy %r21,%r24
	bl __udivdi3,%r2
	nop
	stw %r28,8(%r3)
	stw %r29,12(%r3)
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	ldw 8(%r3),%r20
	comb,<<,n %r20,%r19,.L3
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	ldw 8(%r3),%r20
	comb,<>,n %r20,%r19,.L4
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	ldw 12(%r3),%r20
	comb,<<,n %r20,%r19,.L3
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	ldw 12(%r3),%r20
.L4:
	addil LR't_div___0-$global$,%r27
	ldo RR't_div___0-$global$(%r1),%r20
	ldw 0(%r20),%r19
	extrs %r19,0,1,%r19
	fldws 0(%r20),%fr22R
	stw %r19,-16(%r30)
	fldws -16(%r30),%fr22L
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r22
	fldws 4(%r22),%fr23L
	xmpyu %fr22R,%fr23L,%fr23
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	fcpy,sgl %fr22R,%fr24L
	fldws 0(%r22),%fr23L
	xmpyu %fr24L,%fr23L,%fr23
	fstws %fr23R,-16(%r30)
	ldw -16(%r30),%r1
	copy %r1,%r19
	addl %r20,%r19,%r20
	fldws 4(%r22),%fr23L
	xmpyu %fr23L,%fr22L,%fr22
	fstws %fr22R,-16(%r30)
	ldw -16(%r30),%r22
	copy %r22,%r19
	addl %r20,%r19,%r20
	addil LR't_div___0-$global$,%r27
	stw %r21,RR't_div___0-$global$(%r1)
	addil LR'suf_index___1-$global$,%r27
	ldw RR'suf_index___1-$global$(%r1),%r19
	ldo 1(%r19),%r23
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	stw %r19,-16(%r30)
	fldws -16(%r30),%fr22L
	stw %r23,-16(%r30)
	fldws -16(%r30),%fr23L
	xmpyu %fr23L,%fr22L,%fr22
	fstws %fr22L,-16(%r30)
	ldw -16(%r30),%r1
	extru %r1,31,32,%r21
	ldi 0,%r20
	extrs %r23,0,1,%r22
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	and %r22,%r19,%r19
	sub %r21,%r19,%r21
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	extrs %r19,0,1,%r19
	and %r19,%r23,%r19
	sub %r21,%r19,%r20
	extrs %r23,0,1,%r19
	sub %r20,%r19,%r20
	copy %r20,%r19
	zdep %r19,30,31,%r19
	addl %r19,%r20,%r19
	zdep %r19,30,31,%r19
	sub %r23,%r19,%r19
	addil LR'suf_index___1-$global$,%r27
	stw %r19,RR'suf_index___1-$global$(%r1)
	b,n .L2
.L3:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L7
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r19
	fstds %fr22,0(%r19)
	b,n .L6
.L7:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr23
	fadd,dbl %fr22,%fr23,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
.L6:
	addil LR't_div___0-$global$,%r27
	ldw RR't_div___0-$global$(%r1),%r1
	stw %r1,-16(%r30)
	fldws -16(%r30),%fr22L
	fcnvxf,sgl,dbl %fr22L,%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr24
	fdiv,dbl %fr24,%fr22,%fr23
	addil LR'suf_list-$global$,%r27
	ldw RR'suf_list-$global$(%r1),%r20
	addil LR'suf_index___1-$global$,%r27
	ldw RR'suf_index___1-$global$(%r1),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r19
	stw %r19,-52(%r30)
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldil LR'.LC1,%r19
	ldo RR'.LC1(%r19),%r25
	fcpy,dbl %fr23,%fr22
	fcpy,dbl %fr22,%fr7
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	copy %r20,%r23
	copy %r21,%r24
	bl fprintf,%r2
	nop
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldi 0,%r25
	ldi 0,%r24
	bl fseek,%r2
	nop
	ldw -20(%r3),%r2
	ldo 64(%r3),%r30
	ldwm -64(%r30),%r3
	bv,n %r0(%r2)
	.EXIT
	.PROCEND
.Lfe1:
	.size	writer_auto_append,.Lfe1-writer_auto_append
	.data
	.align 4
	.type	t_div___2,@object
	.size	t_div___2,4
t_div___2:
	.word 1
	.align 4
	.type	suf_index___3,@object
	.size	suf_index___3,4
suf_index___3:
	.word 0
	.text
	.align 4
.globl writer_auto_overwr
		.type		 writer_auto_overwr,@function
writer_auto_overwr:
	.PROC
	.CALLINFO FRAME=128,CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=3
	.ENTRY
	stw %r2,-20(%r30)
	copy %r3,%r1
	copy %r30,%r3
	stwm %r1,128(%r30)
	nop
.L9:
	addil LR't_div___2-$global$,%r27
	ldo RR't_div___2-$global$(%r1),%r22
	ldw 0(%r22),%r19
	extrs %r19,0,1,%r19
	ldw 0(%r22),%r21
	copy %r19,%r20
	addil LR'count-$global$,%r27
	ldw RR'count-$global$(%r1),%r25
	ldw RR'count-$global$+4(%r1),%r26
	copy %r20,%r23
	copy %r21,%r24
	bl __udivdi3,%r2
	nop
	stw %r28,8(%r3)
	stw %r29,12(%r3)
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	ldw 8(%r3),%r20
	comb,<<,n %r20,%r19,.L10
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	ldw 8(%r3),%r20
	comb,<>,n %r20,%r19,.L11
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	ldw 12(%r3),%r20
	comb,<<,n %r20,%r19,.L10
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	ldw 12(%r3),%r20
.L11:
	addil LR't_div___2-$global$,%r27
	ldo RR't_div___2-$global$(%r1),%r20
	ldw 0(%r20),%r19
	extrs %r19,0,1,%r19
	fldws 0(%r20),%fr22R
	stw %r19,-16(%r30)
	fldws -16(%r30),%fr22L
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r22
	fldws 4(%r22),%fr23L
	xmpyu %fr22R,%fr23L,%fr23
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	fcpy,sgl %fr22R,%fr24L
	fldws 0(%r22),%fr23L
	xmpyu %fr24L,%fr23L,%fr23
	fstws %fr23R,-16(%r30)
	ldw -16(%r30),%r1
	copy %r1,%r19
	addl %r20,%r19,%r20
	fldws 4(%r22),%fr23L
	xmpyu %fr23L,%fr22L,%fr22
	fstws %fr22R,-16(%r30)
	ldw -16(%r30),%r22
	copy %r22,%r19
	addl %r20,%r19,%r20
	addil LR't_div___2-$global$,%r27
	stw %r21,RR't_div___2-$global$(%r1)
	addil LR'suf_index___3-$global$,%r27
	ldw RR'suf_index___3-$global$(%r1),%r19
	ldo 1(%r19),%r23
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	stw %r19,-16(%r30)
	fldws -16(%r30),%fr22L
	stw %r23,-16(%r30)
	fldws -16(%r30),%fr23L
	xmpyu %fr23L,%fr22L,%fr22
	fstws %fr22L,-16(%r30)
	ldw -16(%r30),%r1
	extru %r1,31,32,%r21
	ldi 0,%r20
	extrs %r23,0,1,%r22
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	and %r22,%r19,%r19
	sub %r21,%r19,%r21
	ldil L'715827883,%r19
	ldo R'715827883(%r19),%r19
	extrs %r19,0,1,%r19
	and %r19,%r23,%r19
	sub %r21,%r19,%r20
	extrs %r23,0,1,%r19
	sub %r20,%r19,%r20
	copy %r20,%r19
	zdep %r19,30,31,%r19
	addl %r19,%r20,%r19
	zdep %r19,30,31,%r19
	sub %r23,%r19,%r19
	addil LR'suf_index___3-$global$,%r27
	stw %r19,RR'suf_index___3-$global$(%r1)
	b,n .L9
.L10:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L14
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r19
	fstds %fr22,0(%r19)
	b,n .L13
.L14:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr23
	fadd,dbl %fr22,%fr23,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
.L13:
	addil LR't_div___2-$global$,%r27
	ldw RR't_div___2-$global$(%r1),%r1
	stw %r1,-16(%r30)
	fldws -16(%r30),%fr22L
	fcnvxf,sgl,dbl %fr22L,%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr24
	fdiv,dbl %fr24,%fr22,%fr23
	addil LR'suf_list-$global$,%r27
	ldw RR'suf_list-$global$(%r1),%r20
	addil LR'suf_index___3-$global$,%r27
	ldw RR'suf_index___3-$global$(%r1),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r19
	stw %r19,-52(%r30)
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldil LR'.LC1,%r19
	ldo RR'.LC1(%r19),%r25
	fcpy,dbl %fr23,%fr22
	fcpy,dbl %fr22,%fr7
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	copy %r20,%r23
	copy %r21,%r24
	bl fprintf,%r2
	nop
	ldw -20(%r3),%r2
	ldo 64(%r3),%r30
	ldwm -64(%r30),%r3
	bv,n %r0(%r2)
	.EXIT
	.PROCEND
.Lfe2:
	.size	writer_auto_overwr,.Lfe2-writer_auto_overwr
	.align 4
.globl writer_manu_append
		.type		 writer_manu_append,@function
writer_manu_append:
	.PROC
	.CALLINFO FRAME=128,CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=3
	.ENTRY
	stw %r2,-20(%r30)
	copy %r3,%r1
	copy %r30,%r3
	stwm %r1,128(%r30)
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L17
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	fstds %fr22,8(%r3)
	b,n .L16
.L17:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	fstds %fr22,8(%r3)
	fldds 8(%r3),%fr23
	fldds 8(%r3),%fr24
	fadd,dbl %fr23,%fr24,%fr23
	fstds %fr23,8(%r3)
.L16:
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L19
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r19
	fstds %fr22,0(%r19)
	b,n .L18
.L19:
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr23
	fadd,dbl %fr22,%fr23,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
.L18:
	fldds 8(%r3),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr24
	fdiv,dbl %fr22,%fr24,%fr23
	addil LR'suffix-$global$,%r27
	ldb RR'suffix-$global$(%r1),%r19
	extrs %r19,31,8,%r19
	stw %r19,-52(%r30)
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldil LR'.LC1,%r19
	ldo RR'.LC1(%r19),%r25
	fcpy,dbl %fr23,%fr22
	fcpy,dbl %fr22,%fr7
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	copy %r20,%r23
	copy %r21,%r24
	bl fprintf,%r2
	nop
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldi 0,%r25
	ldi 0,%r24
	bl fseek,%r2
	nop
	ldw -20(%r3),%r2
	ldo 64(%r3),%r30
	ldwm -64(%r30),%r3
	bv,n %r0(%r2)
	.EXIT
	.PROCEND
.Lfe3:
	.size	writer_manu_append,.Lfe3-writer_manu_append
	.align 4
.globl writer_manu_overwr
		.type		 writer_manu_overwr,@function
writer_manu_overwr:
	.PROC
	.CALLINFO FRAME=128,CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=3
	.ENTRY
	stw %r2,-20(%r30)
	copy %r3,%r1
	copy %r30,%r3
	stwm %r1,128(%r30)
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L22
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	fstds %fr22,8(%r3)
	b,n .L21
.L22:
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'count-$global$,%r27
	ldo RR'count-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	fstds %fr22,8(%r3)
	fldds 8(%r3),%fr23
	fldds 8(%r3),%fr24
	fadd,dbl %fr23,%fr24,%fr23
	fstds %fr23,8(%r3)
.L21:
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r25
	ldw 4(%r1),%r26
	copy %r0,%r23
	copy %r0,%r24
	bl __cmpdi2,%r2
	nop
	copy %r28,%r19
	comib,>,n 1,%r19,.L24
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	fldds 0(%r1),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r19
	fstds %fr22,0(%r19)
	b,n .L23
.L24:
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r22
	ldw 4(%r1),%r23
	ldi 0,%r20
	ldi 1,%r21
	and %r22,%r20,%r22
	and %r23,%r21,%r23
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r19
	zdep %r19,0,1,%r25
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 4(%r1),%r19
	extru %r19,30,31,%r19
	addil LR'divisor-$global$,%r27
	ldo RR'divisor-$global$(%r1),%r1
	ldw 0(%r1),%r24
	or %r25,%r19,%r21
	extru %r24,30,31,%r20
	or %r22,%r20,%r22
	or %r23,%r21,%r23
	stw %r22,-16(%r30)
	stw %r23,-12(%r30)
	fldds -16(%r30),%fr22
	fcnvxf,dbl,dbl %fr22,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr23
	fadd,dbl %fr22,%fr23,%fr22
	ldo 16(%r3),%r1
	fstds %fr22,0(%r1)
.L23:
	fldds 8(%r3),%fr22
	ldo 16(%r3),%r1
	fldds 0(%r1),%fr24
	fdiv,dbl %fr22,%fr24,%fr23
	addil LR'suffix-$global$,%r27
	ldb RR'suffix-$global$(%r1),%r19
	extrs %r19,31,8,%r19
	stw %r19,-52(%r30)
	addil LR'f-$global$,%r27
	ldw RR'f-$global$(%r1),%r26
	ldil LR'.LC1,%r19
	ldo RR'.LC1(%r19),%r25
	fcpy,dbl %fr23,%fr22
	fcpy,dbl %fr22,%fr7
	fstds %fr23,-16(%r30)
	ldw -16(%r30),%r20
	ldw -12(%r30),%r21
	copy %r20,%r23
	copy %r21,%r24
	bl fprintf,%r2
	nop
	ldw -20(%r3),%r2
	ldo 64(%r3),%r30
	ldwm -64(%r30),%r3
	bv,n %r0(%r2)
	.EXIT
	.PROCEND
.Lfe4:
	.size	writer_manu_overwr,.Lfe4-writer_manu_overwr
	.data
	.align 4
.LC5:
	.word P%writer_manu_overwr
	.word P%writer_manu_append
	.word P%writer_auto_overwr
	.word P%writer_auto_append
	.section	.rodata
	.align 4
.LC2:
.stringz"bkmgte"
	.align 4
.LC3:
.stringz"a+"
	.align 4
.LC4:
.stringz"w+"
	.text
	.align 4
.globl main
		.type		 main,@function
main:
	.PROC
	.CALLINFO FRAME=704,CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=3
	.ENTRY
	stw %r2,-20(%r30)
	copy %r3,%r1
	copy %r30,%r3
	stwm %r1,704(%r30)
	stw %r26,-36(%r3)
	stw %r25,-40(%r3)
	stw %r0,528(%r3)
	stw %r0,536(%r3)
.L26:
	ldw 536(%r3),%r19
	ldo 1(%r19),%r19
	copy %r19,%r20
	stw %r20,536(%r3)
	ldw -36(%r3),%r19
	comb,<=,n %r19,%r20,.L27
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r20
	ldi 45,%r19
	comb,<>,n %r19,%r20,.L27
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r19
	ldo 1(%r19),%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r20
	ldi 45,%r19
	comb,=,n %r19,%r20,.L27
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r19
	ldo 1(%r19),%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r19
	comib,<>,n 0,%r19,.L28
	b,n .L27
.L28:
	stw %r0,540(%r3)
.L30:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	copy %r19,%r21
	ldw 540(%r3),%r19
	ldo 1(%r19),%r19
	copy %r19,%r20
	stw %r20,540(%r3)
	ldw 0(%r21),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r19
	comib,<>,n 0,%r19,.L32
	b,n .L26
.L32:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	extrs %r19,31,8,%r19
	ldo -69(%r19),%r19
	stw %r19,560(%r3)
	ldi 47,%r19
	ldw 560(%r3),%r1
	comb,<< %r19,%r1,.L30
	nop
	ldw 560(%r3),%r19
	blr %r19,%r0
	nop
.L50:
	.begin_brtab
	b .L44
	nop
	b .L30
	nop
	b .L40
	nop
	b .L46
	nop
	b .L30
	nop
	b .L30
	nop
	b .L36
	nop
	b .L30
	nop
	b .L38
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L42
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L47
	nop
	b .L34
	nop
	b .L30
	nop
	b .L30
	nop
	b .L43
	nop
	b .L30
	nop
	b .L39
	nop
	b .L45
	nop
	b .L30
	nop
	b .L30
	nop
	b .L35
	nop
	b .L30
	nop
	b .L37
	nop
	b .L30
	nop
	b .L48
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L30
	nop
	b .L41
	nop
	.end_brtab
.L34:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldi 1,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L35:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldi 1000,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L36:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldi 1024,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L37:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldil L'1000000,%r21
	ldi 0,%r20
	ldo R'1000000(%r21),%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L38:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldil L'1048576,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L39:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldil L'1000000000,%r21
	ldi 0,%r20
	ldo R'1000000000(%r21),%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L40:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldil L'1073741824,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L41:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi -1,%r20
	ldil L'-727379968,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L42:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	addil LR'divisor-$global$,%r27
	copy %r0,%r20
	copy %r0,%r21
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L43:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	ldi -1,%r20
	ldil L'-1530494976,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	b,n .L30
.L44:
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	ldw 0(%r19),%r20
	ldw 540(%r3),%r19
	addl %r20,%r19,%r19
	ldb 0(%r19),%r19
	addil LR'suffix-$global$,%r27
	stb %r19,RR'suffix-$global$(%r1)
	addil LR'automatic-$global$,%r27
	stw %r0,RR'automatic-$global$(%r1)
	addil LR'divisor-$global$,%r27
	copy %r0,%r22
	copy %r0,%r23
	stw %r22,RR'divisor-$global$(%r1)
	stw %r23,RR'divisor-$global$+4(%r1)
	b,n .L30
.L45:
	ldi 1,%r19
	addil LR'automatic-$global$,%r27
	stw %r19,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldi 1000,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	ldil LR'.LC2,%r19
	ldo RR'.LC2(%r19),%r19
	addil LR'suf_list-$global$,%r27
	stw %r19,RR'suf_list-$global$(%r1)
	b,n .L30
.L46:
	ldi 1,%r19
	addil LR'automatic-$global$,%r27
	stw %r19,RR'automatic-$global$(%r1)
	ldi 0,%r20
	ldi 1024,%r21
	addil LR'divisor-$global$,%r27
	stw %r20,RR'divisor-$global$(%r1)
	stw %r21,RR'divisor-$global$+4(%r1)
	ldil LR'.LC0,%r19
	ldo RR'.LC0(%r19),%r19
	addil LR'suf_list-$global$,%r27
	stw %r19,RR'suf_list-$global$(%r1)
	b,n .L30
.L47:
	ldi 1,%r19
	addil LR'append-$global$,%r27
	stw %r19,RR'append-$global$(%r1)
	b,n .L30
.L48:
	addil LR'append-$global$,%r27
	stw %r0,RR'append-$global$(%r1)
	b,n .L30
.L27:
	ldw 536(%r3),%r20
	ldw -36(%r3),%r19
	comb,<=,n %r19,%r20,.L55
	ldw 536(%r3),%r19
	zdep %r19,29,30,%r20
	ldw -40(%r3),%r19
	addl %r20,%r19,%r19
	stw %r19,564(%r3)
	addil LR'append-$global$,%r27
	ldw RR'append-$global$(%r1),%r19
	comib,=,n 0,%r19,.L53
	ldil LR'.LC3,%r19
	ldo RR'.LC3(%r19),%r19
	stw %r19,568(%r3)
	b,n .L54
.L53:
	ldil LR'.LC4,%r19
	ldo RR'.LC4(%r19),%r19
	stw %r19,568(%r3)
.L54:
	ldw 564(%r3),%r23
	ldw 0(%r23),%r26
	ldw 568(%r3),%r25
	bl fopen,%r2
	nop
	addil LR'f-$global$,%r27
	ldo RR'f-$global$(%r1),%r19
	stw %r28,0(%r19)
	ldo RR'f-$global$(%r1),%r1
	ldw 0(%r1),%r19
	addil LR'f-$global$,%r27
	ldo RR'f-$global$(%r1),%r20
	stw %r19,0(%r20)
.L55:
	ldi 1,%r19
	addil LR'append-$global$,%r27
	stw %r19,RR'append-$global$(%r1)
	ldo 544(%r3),%r21
	stw %r21,572(%r3)
	addil LR'.LC5-$global$,%r27
	ldo RR'.LC5-$global$(%r1),%r1
	stw %r1,576(%r3)
	ldw 572(%r3),%r22
	ldw 576(%r3),%r23
	ldi 8,%r21
	ldws,ma 4(%r23),%r20
	ldws,ma 4(%r23),%r19
	stws,ma %r20,4(%r22)
	addib,>= -8,%r21,.-12
	stws,ma %r19,4(%r22)
	addil LR'automatic-$global$,%r27
	ldw RR'automatic-$global$(%r1),%r19
	zdep %r19,30,31,%r20
	addil LR'append-$global$,%r27
	ldw RR'append-$global$(%r1),%r19
	addl %r20,%r19,%r19
	zdep %r19,29,30,%r20
	ldo 544(%r3),%r19
	addl %r19,%r20,%r19
	ldw 0(%r19),%r19
	stw %r19,532(%r3)
.L57:
	ldi 0,%r26
	bl feof,%r2
	nop
	copy %r28,%r19
	comib,=,n 0,%r19,.L59
	b,n .L58
.L59:
	ldo 16(%r3),%r26
	ldi 1,%r25
	ldi 512,%r24
	ldi 0,%r23
	bl fread,%r2
	nop
	stw %r28,528(%r3)
	ldo 16(%r3),%r26
	ldi 1,%r25
	ldw 528(%r3),%r24
	ldi 0,%r23
	bl fwrite,%r2
	nop
	copy %r28,%r20
	ldw 528(%r3),%r19
	comb,=,n %r20,%r19,.L60
	ldi -1,%r26
	bl exit,%r2
	nop
.L60:
	ldw 528(%r3),%r21
	ldi 0,%r20
	addil LR'count-$global$,%r27
	ldw RR'count-$global$(%r1),%r22
	ldw RR'count-$global$+4(%r1),%r23
	add %r21,%r23,%r21
	addc %r20,%r22,%r20
	stw %r20,RR'count-$global$(%r1)
	stw %r21,RR'count-$global$+4(%r1)
	ldw 532(%r3),%r22
	.CALL	ARGW0=GR
	bl $$dyncall,%r31
	copy %r31,%r2
	b,n .L57
.L58:
	ldi 0,%r28
	ldw -20(%r3),%r2
	ldo 64(%r3),%r30
	ldwm -64(%r30),%r3
	bv,n %r0(%r2)
	.EXIT
	.PROCEND
.Lfe5:
	.size	main,.Lfe5-main
	.comm	f,4,4
	.ident	"GCC: (GNU) 3.0.4"
