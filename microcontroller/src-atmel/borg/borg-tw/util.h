#ifndef UTIL_H
#define UTIL_H

#include <avr/io.h>

extern char UartInTxt [SCROLLTEXT_STRING_SIZE];
extern uint8_t string_ready_state;

void wait(int ms);

#endif
