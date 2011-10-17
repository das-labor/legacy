#include <avr/pgmspace.h>

#define PB(v) pgm_read_byte(&(v))
#define PW(v) pgm_read_word(&(v))
#define PD(v) pgm_read_dword(&(v))

