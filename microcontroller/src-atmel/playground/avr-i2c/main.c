// main.c
/*
*
*/


/****************************************************
 ****************************************************
 *  INCLUDES
 ****************************************************
 ****************************************************/

#include "config.h"
#include "debug.h"
#include "uart/uart.h"
#include "i2c.h"
#include "main_tools.h"


/****************************************************
 ****************************************************
 *  MACROS
 ****************************************************
 ****************************************************/


#define TERM_LN_ERASE "\x1B[2K"
#define TERM_CUR_UP1 "\x1B[A"
#define TERM_CUR_UP (s) "\x1B["#(S)"A"

#define HELP_STR	 \
	"Press\r\n"\
	"a - to detect I2C devices\r\n"\
	"d - to dump an eeprom\r\n"\
	"t - to show temperature\r\n"\
	"c - to learn about your terminal"\
	"\r\n"


/****************************************************
 * main()
 * **************************************************/
int main (void)
{
	i2c_detect_t dev_table;
	uart_init();
    // DEBUG_INIT();
  uart_putstr("\r\nMAIN\r\n");
	
	main_tools_init();
    /*******************/
    i2c_set_speed(I2C_SPEED_100);
	/******************/
	//
    /*******************/
	//eeprom_dump_byte();      
    /*******************/
    //eeprom_dump_page(0xA0, 0, 64);
    /*******************/
    //thermo_dump(0x90);
    uart_putstr(HELP_STR);
    while(1){
    		switch (uart_getc()){
    			case 't': thermo_dump(0x90); break;
    			case 'a': i2c_detect(dev_table); break;
    			case 'd': eeprom_dump_page(0xA8, 0, 8*1024); break;
    			case 'c': console_dbg(); break;
    			default: uart_putstr(HELP_STR); break;
    		}	
    }

    return (0);
}


