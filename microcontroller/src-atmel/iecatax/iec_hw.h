
#include <inttypes.h>

typedef struct{
	uint8_t data;
	uint8_t flags;
}iec_word_t;

#define FLAG_EOI   0x01
#define FLAG_ATN   0x02
#define FLAG_BREAK 0x04
#define FLAG_TXOK  0x08

#define LISTEN    0x20
#define TALK      0x40
#define UNLISTEN  0x3f
#define UNTALK    0x5f

#define OPEN      0xf0
#define CLOSE     0xe0
#define DATA      0x60

#ifndef IEC_HW_C
AVRX_EXTERNTASK(iecAtnTask);

extern volatile uint8_t iec_mode;
volatile iec_word_t iec_tx_word, iec_rx_word;
extern Mutex iec_tx_mutex, iec_rx_mutex;

void iec_hw_init();

#endif
