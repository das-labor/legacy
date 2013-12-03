#ifndef _IO_H
#define _IO_H

extern void io_init(void);
extern void switch_handler(void);

extern void set_output(uint8_t port, uint8_t val);
extern void set_output_all(uint8_t val);
extern void set_pwm(uint8_t port, uint8_t value);
extern void set_pwm_all(uint8_t value);
extern uint8_t get_outputs(void);
extern uint8_t get_pwm(uint8_t port);

#endif // _IO

