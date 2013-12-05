#ifndef UART_HANDLER_H
#define UART_HANDLER_H

extern void uart_handler(void);
extern void uart_send_status(uint8_t taster_status);
extern void uart_get_led_output(void);

#endif
