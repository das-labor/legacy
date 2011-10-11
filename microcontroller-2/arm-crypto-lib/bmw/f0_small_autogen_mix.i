  sub SP, #16*4
/*=== W[ 0] ===*/
  ldr r14, [r1, #5*4]
  ldr r3, [r2, #5*4]
  eor r14, r3
  str r14, [SP, #5*4]
  ldr r12, [r1, #7*4]
  ldr r3, [r2, #7*4]
  eor r12, r3
  str r12, [SP, #7*4]
  ldr r11, [r1, #10*4]
  ldr r3, [r2, #10*4]
  eor r11, r3
  str r11, [SP, #10*4]
  ldr r10, [r1, #13*4]
  ldr r3, [r2, #13*4]
  eor r10, r3
  str r10, [SP, #13*4]
  ldr r9, [r1, #14*4]
  ldr r3, [r2, #14*4]
  eor r9, r3
  str r9, [SP, #14*4]
  sub r8, r9, r12
/*(-- should do +10, +13, +1tr, +5 --)*/
  add r3, r11, r10
  add r3, r8
  add r3, r14
  S32_0 r7 r3
  ldr r3, [r1, #1*4]
  add r7, r3
  str r7, [r0, #0*4]
/*=== W[ 3] ===*/
  ldr r7, [r1, #0*4]
  ldr r3, [r2, #0*4]
  eor r7, r3
  str r7, [SP, #0*4]
  ldr r6, [r1, #1*4]
  ldr r3, [r2, #1*4]
  eor r6, r3
  str r6, [SP, #1*4]
  ldr r5, [r1, #8*4]
  ldr r3, [r2, #8*4]
  eor r5, r3
  str r5, [SP, #8*4]
  add r4, r6, r11
/*(-- should do +0, +13, +8, -2tr --)*/
  add r3, r7, r10
  add r3, r5
  sub r3, r4
  S32_3 r6 r3
  ldr r3, [r1, #4*4]
  add r6, r3
  str r6, [r0, #3*4]
/*=== W[ 6] ===*/
  ldr r9, [r1, #3*4]
  ldr r3, [r2, #3*4]
  eor r9, r3
  str r9, [SP, #3*4]
  ldr r6, [r1, #4*4]
  ldr r3, [r2, #4*4]
  eor r6, r3
  str r6, [SP, #4*4]
  ldr r5, [r1, #11*4]
  ldr r3, [r2, #11*4]
  eor r5, r3
  str r5, [SP, #11*4]
/*(-- should do +13, +4, -0, -11, -3 --)*/
  add r3, r10, r6
  sub r3, r7
  sub r3, r5
  sub r3, r9
  S32_1 r5 r3
  ldr r3, [r1, #7*4]
  add r5, r3
  str r5, [r0, #6*4]
/*=== W[ 9] ===*/
  ldr r5, [r1, #6*4]
  ldr r3, [r2, #6*4]
  eor r5, r3
  str r5, [SP, #6*4]
  sub r14, r5, r9
/*(-- should do +0, +0tr, +1tr --)*/
  add r3, r7, r14
  add r3, r8
  S32_4 r3
  ldr r8, [r1, #10*4]
  add r3, r8
  str r3, [r0, #9*4]
/*=== W[12] ===*/
  ldr r8, [r1, #9*4]
  ldr r3, [r2, #9*4]
  eor r8, r3
  str r8, [SP, #9*4]
/*(-- should do +2tr, -0tr, -9 --)*/
  sub r3, r4, r14
  sub r3, r8
  S32_2 r14 r3
  ldr r3, [r1, #13*4]
  add r14, r3
  str r14, [r0, #12*4]
/*=== W[15] ===*/
  ldr r14, [r1, #12*4]
  ldr r3, [r2, #12*4]
  eor r14, r3
  str r14, [SP, #12*4]
  sub r4, r14, r8
  sub r11, r10, r5
/*(-- should do +0tr, +1tr, -4 --)*/
  add r3, r4, r11
  sub r3, r6
  S32_0 r10 r3
  ldr r3, [r1, #0*4]
  add r10, r3
  str r10, [r0, #15*4]
/*=== W[ 2] ===*/
  ldr r10, [r1, #15*4]
  ldr r3, [r2, #15*4]
  eor r10, r3
  str r10, [SP, #15*4]
/*(-- should do +0, +15, +7, -0tr --)*/
  add r3, r7, r10
  add r3, r12
  sub r3, r4
  S32_2 r4 r3
  ldr r3, [r1, #3*4]
  add r4, r3
  str r4, [r0, #2*4]
/*=== W[ 5] ===*/
  ldr r12, [r1, #2*4]
  ldr r3, [r2, #2*4]
  eor r12, r3
  str r12, [SP, #2*4]
  ldr r4, [SP, #10*4]
  sub r6, r10, r12
  sub r5, r9, r14
/*(-- should do +0tr, +10, +2tr --)*/
  add r3, r6, r4
  add r3, r5
  S32_0 r9 r3
  ldr r3, [r1, #6*4]
  add r9, r3
  str r9, [r0, #5*4]
/*=== W[ 8] ===*/
  ldr r9, [SP, #5*4]
/*(-- should do +1tr, -0tr, -5 --)*/
  sub r3, r11, r6
  sub r3, r9
  S32_3 r11 r3
  ldr r3, [r1, #9*4]
  add r11, r3
  str r11, [r0, #8*4]
/*=== W[11] ===*/
  ldr r11, [SP, #8*4]
  sub r6, r11, r9
/*(-- should do +0tr, +9, -0, -2 --)*/
  add r3, r6, r8
  sub r3, r7
  sub r3, r12
  S32_1 r7 r3
  ldr r3, [r1, #12*4]
  add r7, r3
  str r7, [r0, #11*4]
/*=== W[14] ===*/
  ldr r7, [SP, #11*4]
/*(-- should do +0tr, +2tr, -11 --)*/
  add r3, r6, r5
  sub r3, r7
  S32_4 r3
  ldr r6, [r1, #15*4]
  add r3, r6
  str r3, [r0, #14*4]
/*=== W[ 1] ===*/
  ldr r6, [SP, #6*4]
  ldr r5, [SP, #14*4]
  add r4, r7, r5
  add r14, r11, r10
/*(-- should do +0tr, +6, -1tr --)*/
  add r3, r4, r6
  sub r3, r14
  S32_1 r11 r3
  ldr r3, [r1, #2*4]
  add r11, r3
  str r11, [r0, #1*4]
/*=== W[ 4] ===*/
  ldr r11, [SP, #1*4]
/*(-- should do +1, +2, +9, -0tr --)*/
  add r3, r11, r12
  add r3, r8
  sub r3, r4
  S32_4 r3
  ldr r4, [r1, #5*4]
  add r3, r4
  str r3, [r0, #4*4]
/*=== W[ 7] ===*/
  ldr r10, [SP, #4*4]
  ldr r4, [SP, #12*4]
/*(-- should do +1, -12, -14, -4, -5 --)*/
  sub r3, r11, r4
  sub r3, r5
  sub r3, r10
  sub r3, r9
  S32_2 r11 r3
  ldr r3, [r1, #8*4]
  add r11, r3
  str r11, [r0, #7*4]
/*=== W[10] ===*/
  ldr r11, [SP, #1*4]
  ldr r9, [SP, #7*4]
  add r8, r10, r9
/*(-- should do +1tr, -0tr, -1 --)*/
  sub r3, r14, r8
  sub r3, r11
  S32_0 r14 r3
  ldr r3, [r1, #11*4]
  add r14, r3
  str r14, [r0, #10*4]
/*=== W[13] ===*/
  ldr r14, [SP, #10*4]
/*(-- should do +0tr, +10, +11, +2 --)*/
  add r3, r8, r14
  add r3, r7
  add r3, r12
  S32_3 r8 r3
  ldr r3, [r1, #14*4]
  add r8, r3
  str r8, [r0, #13*4]
  add SP, #16*4
