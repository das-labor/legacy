#ifndef _IO_H
#define _IO_H

extern uint8_t sreg;

extern void switch_handler();
extern void change_shift_reg(uint8_t sreg);
extern void pwm_set(volatile uint8_t *port, uint8_t value);
extern uint8_t pwm_get(volatile uint8_t *port);
extern void init_io();

#endif // _IO

