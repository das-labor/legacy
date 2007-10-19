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
#include "uart.h"
#include "i2c.h"
#include "24CBlockDev.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "prng.h"
#include "main_test_tools.h"


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
	"f - to test blockDev driver\r\n"\
	"z - to set all bytes to 0xff in blockDev\r\n"\
	"c - to learn about your terminal\r\n"\
	"s - to test shabea implementation\r\n"  \
	"i - to init the database\r\n" \
	"n - to add a user to datatabase (hard)\r\n" \
	"m - to set admin flag\r\n" \
	"j - to clear admin flag\r\n" \
	"+ - to add a user\r\n" \
	"# - to do action\r\n" \
	"b - to enter LA-Sim mode" \
	"\r\n"


/****************************************************
 * main()
 * **************************************************/
int main (void)
{
	i2c_detect_t dev_table;
	uart_init();
    // DEBUG_INIT();
    uart_putstr_P(PSTR("\r\nAnonAccess test system - MAIN\r\n"));
	main_tools_init();
    /*******************/
    i2c_set_speed(I2C_SPEED_100);
	/******************/

    uart_putstr_P(PSTR(HELP_STR));
    while(1){
    		switch (uart_getc()){
    //			case 't': thermo_dump(0x90); break;
    			case 'a': i2c_detect(dev_table); break;
    			case 'd': eeprom_dump_page(0xA0, 0, 512); break;
    			case 'l': eeprom_dump_page(0xA0, 0,2048); break;
    			case 'z': E24C_blockdev_setBlock(0,0x00, 1024); break;
    			// {int i; for (i=0; i<1024; ++i){ E24C_blockdev_writeByte(i, 0xFF);}}; break;
    			case 'c': console_dbg(); break;
    			case 's': test_shabea256(); break;
    			case 'f': ticketdb_format(TICKETDB_SIZE);/* break;*/
    			case 'i': ticketdb_init();dump_dbstats(); break;
    			case 'n': console_adduser_db(); break;
    			case 'm': console_setadmin(); break;
    			case 'j': console_clearadmin(); break;
    			
    			case '+': console_adduser(); break;
    			case '#': console_verifyuser(); break;
    			case 'b': console_lasim(); break;
    		/*	
    			case '1': E24C_blockdev_setBlock(0,'1',42); break;
    			case '2': E24C_blockdev_setBlock(0,'2',128); break;
    			case '3': E24C_blockdev_setBlock(1,'3',130); break;
    			case '4': E24C_blockdev_setBlock(14,'4',114); break;
    			case '5': E24C_blockdev_setBlock(0,'#',128LL*1024); break;
    		*/	
    			default: uart_putstr_P(PSTR(HELP_STR)); break;
    		}	
    		uart_putstr_P(PSTR("\r\ndone\r\n"));
    }

    return (0);
}


