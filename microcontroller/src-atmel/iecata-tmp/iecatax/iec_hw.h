#ifndef IEC_HW_H
#define IEC_HW_H

#include <inttypes.h>
#include "fifo.h"





#define LISTEN    0x20
#define TALK      0x40
#define UNLISTEN  0x3f
#define UNTALK    0x5f

#define OPEN      0xf0
#define CLOSE     0xe0
#define DATA      0x60

#define FLAG_EOI   0x01
#define FLAG_ATN   0x02
#define FLAG_BREAK 0x04
#define FLAG_TXOK  0x08
#define FLAG_MODE  0x80

#define MODE_RELEASE		0
#define MODE_LISTEN			1
#define MODE_TALK			2
#define MODE_TALK_RELEASE	3


#ifndef IEC_HW_C
AVRX_EXTERNTASK(iecAtnTask);
AVRX_EXT_FIFO(iec_rx_fifo);
AVRX_EXT_FIFO(iec_tx_fifo);


void iec_hw_init();

#endif

#endif //IEC_HW_H
