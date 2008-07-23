/* debug_tools.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef DEBUG_TOOLS_H_
#define DEBUG_TOOLS_H_

/****************************************************
 *  MACROS
 ****************************************************/

#include "config.h"
#include "i2c.h" 
#include "types.h"
#include "flmDB.h"

#define TERM_LN_ERASE "\x1B[2K"
#define TERM_CUR_UP1 "\x1B[A"
#define TERM_CUR_UP (s) "\x1B["#(S)"A"
#define ROW_SIZE 16

/****************************************************
 *  TYPES
 ****************************************************/
 
typedef uint8_t  i2c_detect_t[32]; // 32*8==256 every bit represents one i2c addr.


/****************************************************
 *  FUNCTIONS
 ****************************************************/
//void main_tools_init(void);

/* specific menus */
/*void dump_dbstats(void);
void console_dbg(void);
void console_verifyuser(void);
void console_adduser_db(void);
void console_adduser(void);
void console_lasim(void);
void i2c_detect(i2c_detect_t table);
void thermo_dump(i2c_addr_t);
*/
/* console helping functions */
//void uart_putbyte(uint8_t b);

/* helping functions */
/*
void eeprom_dump_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_dump_page(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length);
*/

void console_dumpresets(void);
void console_dumptimestamp(void);
void test_shabea256(void);
uint8_t getbadrandom(void);
void prng_init(void);
void main_tools_init(void);
void dump_dbstats(void);
void console_getnstr(uint16_t n, char* dest);
uint8_t charhex2value(char c);
void asciihex2values(uint8_t* dest, char * src, int n);
void console_getnick(void * data);
void console_verifyuser(void);
void console_adduser(void);
void console_adduser_db(void);
void console_getauthblock(authblock_t *ab);
void console_dumpauthblock(authblock_t *ab);
uint8_t console_getauthcredentials(authblock_t **ab);
void console_dbg(void);
void uart_putbyte(uint8_t b);
void i2c_detect(i2c_detect_t table);
void eeprom_dump_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_dump_page(i2c_addr_t dev, uint32_t start, uint16_t length);
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length);
void crypto_eeprom_dump(uint32_t start, uint16_t length);
void getnames(void ** data);
void getuid(void ** data);
void console_toggleadmin(void);
void console_lasim(void);
void console_setadmin(void);
void console_clearadmin(void);


#endif /*DEBUG_TOOLS_H_*/
