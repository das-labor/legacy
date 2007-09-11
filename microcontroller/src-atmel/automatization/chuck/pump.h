#include "fifo.h"

#ifndef PUMP_C
	AVRX_EXT_FIFO(pumpstat); 
#endif

#ifndef PUMP_H
	AVRX_EXTERNTASK(pumptask);

	#define STAT_PUMP_ENABLE 0x01

	#define PUMP_PIN (_BV(PB0))
	#define PUMP_PORT (PORTB)
	#define PUMP_DDR (DDRB)


	// pumping time == (PUMPTIME * PUMP_TICKS)/1000 [s]
	#define CONF_PUMP_TICKS 5
	#define CONF_PUMPTIME 60000
	
	#define PUMP_C
#endif

