#ifndef _IO_H
#define _IO_H

extern void io_init(void);
extern void switch_netvars_init(void);
extern void lamp_out_init(void);
extern void switch_handler(void);
extern void set_leds(uint8_t val);

#endif // _IO

