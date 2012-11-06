#ifndef _IO_H
#define _IO_H

#define SREG_PORT	PORTC
#define SREG_CK		_BV(PC4)
#define SREG_DATA	_BV(PC5)
#define SREG_STROBE	_BV(PA5)

extern uint8_t sreg;

void init_switch();
void switch_handler();

#endif // _IO

