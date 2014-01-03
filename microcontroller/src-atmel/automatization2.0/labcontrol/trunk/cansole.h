#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>

#include "cansole/client/termio.h"

void cmd_cansole (int argc, char* argv[]);
void cansole_exit (int in_signal);
