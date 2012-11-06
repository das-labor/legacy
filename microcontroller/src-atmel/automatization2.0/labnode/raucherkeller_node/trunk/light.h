#ifndef _LIGHT_H
#define _LIGHT_H

void light_init();
void enable_channel(uint8_t channel, uint8_t bright);
uint8_t get_channel_status();

#endif // _LIGHT_H

