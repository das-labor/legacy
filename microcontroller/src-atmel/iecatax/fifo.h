#ifndef _AvrXFifo_h_
#define _AvrXFifo_h_

#include <inttypes.h>
#include "avrx.h"
/*
    Buffered BYTE oriented FIFO

	Uses providers/consumer model.  When an interrupt
	drives the consumer/provider side use the interrupt calls

	The fifo needs to be both declared and initialized.
*/

#include "types.h"

typedef iec_word_t fifo_data_t;

typedef struct AvrXFifo
{
	Mutex	Producer;
	Mutex	Consumer;
	uint8_t in;
	uint8_t out;
	uint8_t size;
	fifo_data_t buf[1];
}
AvrXFifo, *pAvrXFifo;

enum
{
	FIFO_OK,
	FIFO_ERR = 0xff
};

#define AVRX_DECL_FIFO(Name, Size)					\
uint8_t Name##Fifo[Size*sizeof(fifo_data_t) + sizeof(AvrXFifo) - 1];	\
const pAvrXFifo Name = (pAvrXFifo)Name##Fifo;		\
static const uint8_t Name##FifoSz = Size;

#define AVRX_INIT_FIFO(Name)		\
	AvrXFlushFifo(Name);			\
	Name->size = Name##FifoSz

#define AVRX_EXT_FIFO(Name)			\
	extern uint8_t Name##Fifo[];	\
	static const pAvrXFifo Name = (pAvrXFifo)Name##Fifo

int16_t AvrXPutFifo(pAvrXFifo, fifo_data_t);
fifo_data_t AvrXPullFifo(pAvrXFifo);
void AvrXWaitPutFifo(pAvrXFifo, fifo_data_t);
fifo_data_t AvrXWaitPullFifo(pAvrXFifo);
fifo_data_t AvrXPeekFifo(pAvrXFifo);
int16_t AvrXStatFifo(pAvrXFifo);
void AvrXFlushFifo(pAvrXFifo);

#endif	// _AvrXFifo_h_
