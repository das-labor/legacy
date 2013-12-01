#ifndef UART_HANDLER_H
#define UART_HANDLER_H

extern uint8_t eingang_status;

extern void uart_handler(void);

extern void uart_set_led_output(uint8_t led, uint8_t val);
extern void uart_set_taster_led(uint8_t sreg);

#endif
