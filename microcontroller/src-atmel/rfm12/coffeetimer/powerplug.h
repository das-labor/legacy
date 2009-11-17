#pragma once 
#include <stdint.h>

/* size of the code queue */
#define CQ_SZ 4
void pp_send (uint8_t in_onoff, uint32_t in_code);
void pp_init();
void pp_tick (void);
