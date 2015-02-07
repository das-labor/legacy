#ifndef _BEAMER_RS232_H
#define _BEAMER_RS232_H

extern void rs232_receive_handler(void);
extern void rs232_send_command(uint8_t cmd);
extern void poll_beamer_state(void);
extern void start_shutdown(void);
extern void set_beamer_power(uint8_t status);

#endif // _BEAMER_RS232_H
