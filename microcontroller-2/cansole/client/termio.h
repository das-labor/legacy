#ifndef TERMIO
#define TERMIO
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "termio.h"

static struct termios stored_settings;
void set_keypress(void);
void reset_keypress(void);

#endif
