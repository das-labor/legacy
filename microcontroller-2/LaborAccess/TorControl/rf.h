#include "fifo.h"

AVRX_EXTERNTASK(rftxtask);

#ifndef RF_C
AVRX_EXT_FIFO(rftxfifo); 
#endif

typedef struct{
	uint8_t b[4];
} rf_code_t;

void rf_init();
