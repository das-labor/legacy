/* sizes */
#define PT_REGS_SZ     0x50 /* 20*4 */
#define SF_REGS_SZ     0x60 /* 24*4 */
#define RW_REGS_SZ     0x20 /* 16*4 */
#define FW_REGS_SZ     0x88 /* 34*4 */

/* Reg_window offsets */
#define RW_L0     0x00
#define RW_L1     0x04
#define RW_L2     0x08
#define RW_L3     0x0c
#define RW_L4     0x10
#define RW_L5     0x14
#define RW_L6     0x18
#define RW_L7     0x1c
#define RW_I0     0x20
#define RW_I1     0x24
#define RW_I2     0x28
#define RW_I3     0x2c
#define RW_I4     0x30
#define RW_I5     0x34
#define RW_I6     0x38
#define RW_I7     0x3c

/* These are for pt_regs. */
#define PT_PSR    0x0
#define PT_PC     0x4
#define PT_NPC    0x8
#define PT_Y      0xc
#define PT_G0     0x10
#define PT_WIM    PT_G0
#define PT_G1     0x14
#define PT_G2     0x18
#define PT_G3     0x1c
#define PT_G4     0x20
#define PT_G5     0x24
#define PT_G6     0x28
#define PT_G7     0x2c
#define PT_I0     0x30
#define PT_I1     0x34
#define PT_I2     0x38
#define PT_I3     0x3c
#define PT_I4     0x40
#define PT_I5     0x44
#define PT_I6     0x48
#define PT_FP     PT_I6
#define PT_I7     0x4c


/* Store the register window onto the 8-byte aligned area starting
 * at %reg.  It might be %sp, it might not, we don't care.
 */
#define RW_STORE(reg) \
    std %l0, [%reg + RW_L0]; \
    std %l2, [%reg + RW_L2]; \
    std %l4, [%reg + RW_L4]; \
    std %l6, [%reg + RW_L6]; \
    std %i0, [%reg + RW_I0]; \
    std %i2, [%reg + RW_I2]; \
    std %i4, [%reg + RW_I4]; \
    std %i6, [%reg + RW_I6];

/* Load a register window from the area beginning at %reg. */
#define RW_LOAD(reg) \
    ldd [%reg + RW_L0], %l0; \
    ldd [%reg + RW_L2], %l2; \
    ldd [%reg + RW_L4], %l4; \
    ldd [%reg + RW_L6], %l6; \
    ldd [%reg + RW_I0], %i0; \
    ldd [%reg + RW_I2], %i2; \
    ldd [%reg + RW_I4], %i4; \
    ldd [%reg + RW_I6], %i6;

/***************************************************************
 * 
 */
#define PT_STORE_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
		st      %pt_psr, [%base_reg + SF_REGS_SZ + PT_PSR]; \
		st      %pt_pc,  [%base_reg + SF_REGS_SZ + PT_PC];  \
		st      %pt_npc, [%base_reg + SF_REGS_SZ + PT_NPC]; \
        st      %g1, [%base_reg + SF_REGS_SZ + PT_G1];      \
        rd      %y, %scratch;                               \
        st      %scratch, [%base_reg + SF_REGS_SZ + PT_Y];  \

#define PT_LOAD_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
        ld      [%base_reg + SF_REGS_SZ + PT_Y], %scratch; \
        wr      %scratch, 0x0, %y;                         \
        ld      [%base_reg + SF_REGS_SZ + PT_G1], %g1;     \
        ld      [%base_reg + SF_REGS_SZ + PT_PSR], %pt_psr; \
        ld      [%base_reg + SF_REGS_SZ + PT_PC], %pt_pc;  \
        ld      [%base_reg + SF_REGS_SZ + PT_NPC], %pt_npc;


/*
#define PT_STORE_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
		st      %pt_psr, [%base_reg + SF_REGS_SZ + PT_PSR]; \
		st      %pt_pc,  [%base_reg + SF_REGS_SZ + PT_PC];  \
		st      %pt_npc, [%base_reg + SF_REGS_SZ + PT_NPC]; \
        st      %g1, [%base_reg + SF_REGS_SZ + PT_G1];      \
        std     %g2, [%base_reg + SF_REGS_SZ + PT_G2];      \
        std     %g4, [%base_reg + SF_REGS_SZ + PT_G4];      \
        std     %g6, [%base_reg + SF_REGS_SZ + PT_G6];      \
        rd      %y, %scratch;                               \
        st      %scratch, [%base_reg + SF_REGS_SZ + PT_Y];  \
        std     %i0, [%base_reg + SF_REGS_SZ + PT_I0];      \
        std     %i2, [%base_reg + SF_REGS_SZ + PT_I2];      \
        std     %i4, [%base_reg + SF_REGS_SZ + PT_I4];      \
        std     %i6, [%base_reg + SF_REGS_SZ + PT_I6];

#define PT_LOAD_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
        ld      [%base_reg + SF_REGS_SZ + PT_Y], %scratch; \
        wr      %scratch, 0x0, %y;                         \
        ldd     [%base_reg + SF_REGS_SZ + PT_I0], %i0;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_I2], %i2;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_I4], %i4;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_I6], %i6;     \
        ld      [%base_reg + SF_REGS_SZ + PT_G1], %g1;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_G2], %g2;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_G4], %g4;     \
        ldd     [%base_reg + SF_REGS_SZ + PT_G6], %g6;     \
        ld      [%base_reg + SF_REGS_SZ + PT_PSR], %pt_psr; \
        ld      [%base_reg + SF_REGS_SZ + PT_PC], %pt_pc;  \
        ld      [%base_reg + SF_REGS_SZ + PT_NPC], %pt_npc;
*/









