
typedef struct{
	uint8_t data;
	uint8_t flags;
}iec_word_t;

#define FLAG_EOI   0x01
#define FLAG_ATN   0x02
#define FLAG_BREAK 0x04

#define MODE_LISTEN 0x01
#define MODE_TALK 0x02

#ifndef IEC_HW_C
AVRX_EXTERNTASK(iecAtnTask);

AVRX_EXT_FIFO(iec_rx_fifo);
AVRX_EXT_FIFO(iec_tx_fifo);

void iec_hw_init();

#endif
