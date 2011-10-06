#include <avr/io.h>

.include "rfm12_common.S"

; rfm12_int (rfm12_ctx_t *in_ctx)
;
; function to handle the rfm12 interrupt
;
; in_ctx  given in r25 (msb), r24 (lsb)
;
.global rfm12_int
rfm12_int:
	push REG_SPI_DATA
	push r11
	push r12
	
	; read status reister
	ldi REG_SPI_DATA, 0x00
	rcall rfm12_spi_read
	
	sbis REG_SPI_DATA, (RFM12_STATUS_FFIT_BIT-8)
	rjmp rfm12_int_end ; we don't about other int reasons (for now)

	; FIFO INT handling
	in r30, SPL
	in r31, SPH
	sbiw r30, 20 ; ctx size
	in r0, SREG
	cli
	out SPL, r30
	out SREG, r0 ; 1 cycle 'til ints are re-enabled
	out SPH, r31


	; restore stack
	in r30, SPL
	in r31, SPH
	andiw r30, 20 ; ctx size
	in r0, SREG
	cli
	out SPL, r30
	out SREG, r0
	out SPH, r31	
	
rfm12_int_end:
	pop r12
	pop r11
	pop REG_SPI_DATA
	ret

