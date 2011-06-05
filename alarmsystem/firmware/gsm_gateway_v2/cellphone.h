#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/eeprom.h>

#include "uart.h"

#define AT_DIAL_PREFIX "ATD+"
#define AT_DIAL_SUFFIX ";\r\n"
#define AT_HANGUP "ATH0;\r\n"

/* offset in eeprom */
#define NUMBER_OFFSET 8
#define NUMBER_MAXLEN 16

void cellphone_init();
void cellphone_tick();
void cellphone_dial (uint8_t in_num);
