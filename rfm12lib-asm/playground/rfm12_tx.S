#include <avr/io.h>

.include "rfm12_common.S"

; rfm12_tx (rfm12_ctx_t *in_ctx, uint8_t in_len, uint8_t *in_data);
;
; function to transmit data
;
; [arg0_H][arg0_L]   ptr to context
; [arg1]             length (1..256 bytes)
; [arg2_H][arg2_L]   ptr to data
;
.global rfm12_tx 
rfm12_tx:
	ret

; load another byte into the transmit buffer
.macro rfm12_tx_tick
	
.endmacro

; rfm12_tick (rfm12_ctx_t *in_ctx)
;
; tick function
;
; [arg0_H][arg0_L]   ptr to context
;
.global rfm12_tick
	ret

; rfm12_int (rfm12_ctx_t *in_ctx)
;
; function to handle the rfm12 interrupt
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

	
rfm12_int_end:
	pop r12
	pop r11
	pop REG_SPI_DATA
	ret
