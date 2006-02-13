#ifndef MAIN_TOOLS_H_
#define MAIN_TOOLS_H_

/****************************************************
 *  MACROS
 ****************************************************/

#include "config.h"
#include "i2c.h" 

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
void main_tools_init();

/* specific menus */
void console_dbg();
void i2c_detect(i2c_detect_t table);
void thermo_dump(i2c_addr_t);

/* console helping functions */
void uart_putbyte(uint8_t b);

/* helping functions */
void eeprom_dump_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_dump_page(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length);






#endif /*MAIN_TOOLS_H_*/
