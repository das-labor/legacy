
#include <avr/pgmspace.h>

void sed1565_init();
void scommand(unsigned char data);
void sdata(unsigned char data);
void sed1565_bitmap(PGM_P bmp);
