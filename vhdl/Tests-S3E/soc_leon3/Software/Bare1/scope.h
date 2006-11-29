#ifndef SCOPE_H
#define SCOPE_H

/***************************************************************************
 * SCOPE -- LogicAnalyzer
 */
#define SCOPE_SIZE	(8*4096-16)

#define SCOPE_SPEN	1
#define SCOPE_IRQEN	2
#define SCOPE_SDONE	4

typedef struct {
	volatile uint32_t cr;		// control register
	volatile uint32_t sp;       // sample pointer
	volatile uint32_t sc;		// sample counter
	volatile uint32_t divisor;	// sampleing divisor
	volatile uint8_t  channel0; // 
	volatile uint8_t  channel1;
	volatile uint8_t  channel2;
	volatile uint8_t  channel3;
} scope_t;

extern scope_t *scope0;
extern uint8_t *scope0_data;

/***************************************************************************
 * Routines
 */
void scope_init(int chan0, int chan1, int chan2, int chan3);
void scope_send();

static inline void scope_start()
{
	scope0->sp = 0;
	scope0->sc = SCOPE_SIZE;
	scope0->cr = SCOPE_SPEN;
};

void scope_wait();

static inline int scope_done()
{
	return (scope0->cr & SCOPE_SDONE) ? 1 : 0;
};

#endif // SCOPE_H
