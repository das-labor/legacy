#include "config.h"

#define delay(us)  _delay_ms (us/1000)		// wartet �s

char read_keypad(void); 
void keypad_init(void);
