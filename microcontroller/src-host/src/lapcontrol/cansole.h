#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>

#include "can.h"
#include "../../../../microcontroller-2/cansole/client/termio.h"

cmd_cansole (int argc, char* argv[]);
void cansole_exit (int in_signal);
