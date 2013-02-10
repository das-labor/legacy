#ifndef _BACKLIGHT_H
#define _BACKLIGHT_H

void backlight();
void init_backlight();
void backlight_pi_controller_update(uint16_t adc);
#endif // _BACKLIGHT_H
