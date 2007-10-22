#ifndef MAIN_TOOLS_H_
#define MAIN_TOOLS_H_

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
void eeprom_dump_page(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length);
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length);
void crypto_eeprom_dump(uint32_t start, uint16_t length);
void getnames(void ** data);
void getuid(void ** data);
void console_toggleadmin(void);
void console_lasim(void);
void console_setadmin(void);
void console_clearadmin(void);


#endif /*MAIN_TOOLS_H_*/
