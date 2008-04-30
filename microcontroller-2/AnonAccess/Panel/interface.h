#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <stdint.h>
#include "rtc.h"

extern char status_string[6];

#define CHAR_SWITCH_DELAY 1000 /* 1 sec */

void print_status(void);

#endif /*INTERFACE_H_*/
