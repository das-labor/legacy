#ifndef I2C_PRINTER_H_
#define I2C_PRINTER_H_

#include "config.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "mcp23017.h"
#include "i2c.h"
#include "i2c_tools.h"

void printer_char(char c);
void printer_str(const char *str);
void printer_str_P(PGM_P str);
void printer_hexdump(const void* buffer, uint16_t length);
void printer_reset(void);
void printer_init(void);


#endif /*I2C_PRINTER_H_*/
