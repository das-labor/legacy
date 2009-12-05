/* -*- Mode: C; tab-width: 2 -*- */
#ifndef _IR_CODE_H
#define _IR_CODE_H

//one tick is 16 µS long
#define IR_TICK_US 16

//teufel signal definitions
//see datasheet of PT2248 chip
//all calculations are done in µS
// a is the base time unit
// a = (1/fOSC) * 192 -> fOSC typically is 455KHz
#define PT2248_A 431 //431 fits best, empirically
// bit 0 = a on, 3*a off
#define PT_OFF (PT2248_A / IR_TICK_US), ((3*PT2248_A) / IR_TICK_US)
// bit 1 = 3*a on, a off
#define PT_ON ((3*PT2248_A) / IR_TICK_US), (PT2248_A / IR_TICK_US)


// Extended NEC Protocol
// Pulse distance encoding
// one additional bit for distance

// AGC Burst 9ms + 4,5ms pause
#define PNEC_AGC_ON  (9000 / IR_TICK_US)
#define PNEC_AGC_OFF (4500 / IR_TICK_US)
#define PNEC_AGC_BURST (PNEC_AGC_ON), (PNEC_AGC_OFF)

// bit 0 560µs on + 560µs off
#define PNEC_OFF (560 / IR_TICK_US), (560 / IR_TICK_US)

// bit 1 560µs on + 1,69ms off
#define PNEC_ON (560 / IR_TICK_US), (1690 / IR_TICK_US)

//macro for generating extended nec encodings
//x is the destination array, y is the input code, z is the bit count
#define IR_GEN_NECEXT(x, y, z) (ir_genCode((uint16_t *)(x + 2 * sizeof(uint16_t)), PNEC_ON, PNEC_OFF, y, z) + 4); x[0] = PNEC_AGC_ON; x[1] = PNEC_AGC_OFF; x[z * 2 + 2] = (560 / IR_TICK_US)

#endif // 
