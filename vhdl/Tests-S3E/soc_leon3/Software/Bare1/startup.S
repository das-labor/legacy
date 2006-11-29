#include "stack.h"

#define PSR_INIT 0x00C0   /* Disable traps, set s and ps */
#define WIM_INIT 2

  .seg    "text"
  .global _trap_table, start, _start 
  .global _hardreset, _window_overflow, _window_underflow, _irq_entry
  .global halt, irq_enable, irq_disable, jump

#define _stack (_RAM_END - 4)

_hardreset:
	set     PSR_INIT, %g1           ! Initialize PSR
	wr      %g1, %psr               ! Enable traps
	nop; nop; nop; nop

	set     _trap_table, %g2        ! Initialize TBR
	mov     %g2, %tbr

	add     %g1, 1, %g2             ! Initialize WIM
	and     %g2, 0x7, %g2 
	set     1, %g3 
	sll     %g3, %g2, %g3 
	mov     %g3, %wim    

	set     _stack, %fp             ! Initialize %sp
	andn    %fp, 0x0f, %fp
	sub     %fp, 64, %sp

	or      %g1, 0x20, %g1 
	wr      %g1, %psr               ! Enable traps

	flush							! Enable cache
	set		0x81000f, %g1
 	sta		%g1, [%g0] 2

	mov     %0, %o1                 ! argv
	mov     %0, %o0                 ! argc
	call	main                    ! Call main
	nop

/****************************************************************************
 * Interrupt entry port (from trap table)
 * 
 * %l0  PSR
 * %l1   PC
 * %l2  nPC
 * %l3  WIM
 * %l7  IRL
 */
_irq_entry:
!	mov     %psr, %l0

	call    irq_handler
	mov     %l7, %o0

	mov  	%l0, %psr
	nop; nop; nop; nop;

	jmp     %l1
	rett    %l2

/****************************************************************************
 * CPU Sleep until next IRQ
 */
halt:
	wr %g0, %asr19			! POWER DOWN 
	retl
	nop
	
irq_enable:
	mov	    %psr, %o0
	andn    %o0, 0x0f00, %o0
	retl
	mov     %o0, %psr

irq_disable:
	mov	    %psr, %o0
	or      %o0, 0x0f20, %o0
	retl
	mov     %o0, %psr

jump:
	jmp     %o0
	nop

