#ifndef _DIMMER_H
#define _DIMMER_H

extern uint8_t dim_vals_8bit[4];

extern void dimmer_init(void);
extern void set_dimmer(uint8_t channel, uint8_t bright);

extern uint8_t get_channel_status(void);
extern uint8_t get_channel_val(uint8_t chan);
extern void enable_channel(uint8_t channel, uint8_t enable);

#endif // _DIMMER_H

