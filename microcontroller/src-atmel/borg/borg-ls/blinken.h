#pragma once

//clears the current blinken backbuffer to color
void blink_clearbuf(unsigned char value);

//sets auto position increment on or off
void blink_setautopos(unsigned char val);

//set the current blinkenbuffer offset position
void blink_setpos(unsigned char pos);

//puts the current blinkenbuffer to the frontbuffer
void blink_show(void);

//puts data into the blinkenbuffer
void blink_data(unsigned char *data, unsigned char len);
