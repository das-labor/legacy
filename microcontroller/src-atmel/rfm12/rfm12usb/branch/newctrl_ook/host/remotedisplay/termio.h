#ifndef TERMIO
#define TERMIO
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>

void set_keypress(void);
void reset_keypress(void);
inline void termflush (int in_fd);

#endif
