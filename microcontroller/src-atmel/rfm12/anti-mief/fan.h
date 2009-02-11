#include <avr/io.h>
#include <stdint.h>

#ifndef FAN_H
#define LPORT PORTB
#define LDDR DDRB
#define LPIN (_BV(PB0))


void fan_on(uint16_t);
void fan_off(uint16_t);
void fan_init();
void fan_kick (uint16_t);
void fan_delaykick (uint16_t);

#define FAN_H
#endif
