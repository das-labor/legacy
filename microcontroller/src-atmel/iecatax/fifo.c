/*
    Buffered BYTE oriented FIFO

	Uses providers/consumer model.  When an interrupt
	drives the consumer/provider side use the interrupt calls

	The fifo needs to be both declared and initialized.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrx.h"
#include "fifo.h"

typedef uint8_t BOOL;

// Since there is only one process modifying each side of the
// FIFO, I am pretty sure critical sections are not needed.
// The only race is if the out pointer changes after the load
// and before the test. But this is not a race that will cause
// fifo corruption.

int16_t AvrXPutFifo(pAvrXFifo p, fifo_data_t c)
{
	uint8_t t = p->in+1;
	if (t >= p->size)
		t = 0;
	if (t == p->out)	// isFull()
		return FIFO_ERR;
	p->buf[p->in] = c;
	p->in = t;
	AvrXSetSemaphore(&p->Producer);
	return FIFO_OK;
}

fifo_data_t AvrXPullFifo(pAvrXFifo p)
{
	fifo_data_t c;
	if (p->in == p->out){	// isEmpty()
		c.flags = FIFO_ERR;
		return c;
	}
	c = p->buf[p->out];
	uint8_t t = p->out+1;
	if (t >= p->size)
		t = 0;
	p->out = t;
	AvrXSetSemaphore(&p->Consumer);
	return c;
}

void AvrXWaitPutFifo(pAvrXFifo p, fifo_data_t c)
{
	while (AvrXPutFifo(p, c) == FIFO_ERR)
		AvrXWaitSemaphore(&p->Consumer);
}

fifo_data_t AvrXWaitPullFifo(pAvrXFifo p)
{
	fifo_data_t c;
	while ((c = AvrXPullFifo(p)).flags == FIFO_ERR)
		AvrXWaitSemaphore(&p->Producer);
	return c;
}

void AvrXFlushFifo(pAvrXFifo p)
{
	p->in = 0;
	p->out = 0;
	AvrXResetSemaphore(&p->Consumer);
	AvrXResetSemaphore(&p->Producer);
}

// Return next character to be removed (without removing it)

fifo_data_t AvrXPeekFifo(pAvrXFifo p)
{
	if (p->in == p->out){
		fifo_data_t c;
		c.flags = FIFO_ERR;
		return c;
	}else
		return p->buf[p->out];
}

// Return size of FIFO (difference between in & out

int16_t AvrXStatFifo(pAvrXFifo p)
{
	uint8_t sreg = SREG;
	cli();
	int16_t c = (uint16_t)p->in - (uint16_t)p->out;
	SREG = sreg;
	if (c < 0)
		c += p->size;
	return c;
}
