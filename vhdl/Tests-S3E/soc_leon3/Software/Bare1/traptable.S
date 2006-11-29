/*************************************************************************
 * Spike TrapTable
 *
 * Has to be located at the very beginning (0x00000000)
 */

  .seg    "text"
  .global _trap_table, start, _hardreset, _window_overflow, _window_underflow
  .global  _irq_enty

#define TRAP(H)  mov %psr, %l0; sethi %hi(H), %l4; jmp %l4+%lo(H); nop;
#define TRAPL(H)  mov %g0, %g4; sethi %hi(H), %g4; jmp %g4+%lo(H); nop;
#define TRAP_ENTRY(H) rd %psr, %l0; b H; rd %wim, %l3; nop;

/* Unexcpected trap will halt the processor by forcing it to error state */
#define BAD_TRAP ta 0; nop; nop; nop;

#define SOFT_TRAP BAD_TRAP

#define TRAP_ENTRY_INTERRUPT(int_level) \
        mov int_level, %l7; mov %psr, %l0; b _irq_entry; rd %wim, %l3;

/***************** TrapTable *********************************************/

start: 
_trap_table:
  TRAPL(_hardreset);            ! 00 reset trap 
  BAD_TRAP;                     ! 01 instruction_access_exception (in mmu_asm.S)
  BAD_TRAP;                     ! 02 illegal_instruction
  BAD_TRAP;                     ! 03 priveleged_instruction
  BAD_TRAP;                     ! 04 fp_disabled
  BAD_TRAP;                     ! 05 window_overflow
  BAD_TRAP;                     ! 06 window_underflow
  BAD_TRAP;                     ! 07 memory_address_not_aligned
  BAD_TRAP;                     ! 08 fp_exception
  BAD_TRAP;                     ! 09 data_access_exception (in mmu_asm.S)
  BAD_TRAP;                     ! 0A tag_overflow

  BAD_TRAP;                     ! 0B undefined
  BAD_TRAP;                     ! 0C undefined
  BAD_TRAP;                     ! 0D undefined
  BAD_TRAP;                     ! 0E undefined
  BAD_TRAP;                     ! 0F undefined
  BAD_TRAP;                     ! 10 undefined

  /* Interrupt entries */
  TRAP_ENTRY_INTERRUPT(1);              ! 11 interrupt level 1
  TRAP_ENTRY_INTERRUPT(2);              ! 12 interrupt level 2
  TRAP_ENTRY_INTERRUPT(3);              ! 13 interrupt level 3
  TRAP_ENTRY_INTERRUPT(4);              ! 14 interrupt level 4
  TRAP_ENTRY_INTERRUPT(5);              ! 15 interrupt level 5
  TRAP_ENTRY_INTERRUPT(6);              ! 16 interrupt level 6
  TRAP_ENTRY_INTERRUPT(7);              ! 17 interrupt level 7
  TRAP_ENTRY_INTERRUPT(8);              ! 18 interrupt level 8
  TRAP_ENTRY_INTERRUPT(9);              ! 19 interrupt level 9
  TRAP_ENTRY_INTERRUPT(10);             ! 1A interrupt level 1
  TRAP_ENTRY_INTERRUPT(11);             ! 1B interrupt level 11
  TRAP_ENTRY_INTERRUPT(12);             ! 1C interrupt level 12
  TRAP_ENTRY_INTERRUPT(13);             ! 1D interrupt level 13
  TRAP_ENTRY_INTERRUPT(14);             ! 1E interrupt level 14
  TRAP_ENTRY_INTERRUPT(15);             ! 1F interrupt level 15

/*
  BAD_TRAP;                     ! 11 interrupt level 1
  BAD_TRAP;                     ! 12 interrupt level 2
  BAD_TRAP;                     ! 13 interrupt level 3
  BAD_TRAP;                     ! 14 interrupt level 4
  BAD_TRAP;                     ! 15 interrupt level 5
  BAD_TRAP;                     ! 16 interrupt level 6
  BAD_TRAP;                     ! 17 interrupt level 7
  BAD_TRAP;                     ! 18 interrupt level 8
  BAD_TRAP;                     ! 19 interrupt level 9
  BAD_TRAP;                     ! 1A interrupt level A
  BAD_TRAP;                     ! 1B interrupt level B
  BAD_TRAP;                     ! 1C interrupt level F
  BAD_TRAP;                     ! 1D interrupt level D
  BAD_TRAP;                     ! 1E interrupt level E
  BAD_TRAP;                     ! 1F interrupt level F
*/

