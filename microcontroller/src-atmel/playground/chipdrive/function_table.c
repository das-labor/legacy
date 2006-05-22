#include "console.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "i2csw.h"
#include "i2cswconf.h"
#include "global.h"
#include "foo.h"
#include "console.h"
#include "config.h"
#include "avrlibtypes.h"
#include "avrlibdefs.h"
const static command_table fkt__avrlibdefs__table PROGMEM={
	0, {}
};
 
const static char fkt__avrlibdefs____table__name[] PROGMEM = "avrlibdefs";
 
const static command fkt__avrlibdefs____table__command PROGMEM={
	table, fkt__avrlibdefs____table__name, (void *)&fkt__avrlibdefs__table
};
 
const static command_table fkt__avrlibtypes__table PROGMEM={
	0, {}
};
 
const static char fkt__avrlibtypes____table__name[] PROGMEM = "avrlibtypes";
 
const static command fkt__avrlibtypes____table__command PROGMEM={
	table, fkt__avrlibtypes____table__name, (void *)&fkt__avrlibtypes__table
};
 
const static command_table fkt__config__table PROGMEM={
	0, {}
};
 
const static char fkt__config____table__name[] PROGMEM = "config";
 
const static command fkt__config____table__command PROGMEM={
	table, fkt__config____table__name, (void *)&fkt__config__table
};
 
 
c_function fkt__console__console__cfkt PROGMEM={
	(void(*)(void))&console, {vd, 0},0,{}
};
 
const static char fkt__console__console__name[] PROGMEM = "console";
 
const static command fkt__console__console__command PROGMEM={
	function, fkt__console__console__name, (void *)&fkt__console__console__cfkt
};
 
const static command_table fkt__console__table PROGMEM={
	1, {	&fkt__console__console__command,
}
};
 
const static char fkt__console____table__name[] PROGMEM = "console";
 
const static command fkt__console____table__command PROGMEM={
	table, fkt__console____table__name, (void *)&fkt__console__table
};
 
const static char fkt__foo__malloc_size[] PROGMEM = "size";
 
c_function fkt__foo__malloc__cfkt PROGMEM={
	(void(*)(void))&malloc, {ptr, 0},1,{{ui,fkt__foo__malloc_size},}
};
 
const static char fkt__foo__malloc__name[] PROGMEM = "malloc";
 
const static command fkt__foo__malloc__command PROGMEM={
	function, fkt__foo__malloc__name, (void *)&fkt__foo__malloc__cfkt
};
 
const static char fkt__foo__hexinput_buf[] PROGMEM = "buf";
 
c_function fkt__foo__hexinput__cfkt PROGMEM={
	(void(*)(void))&hexinput, {vd, 0},1,{{ptr,fkt__foo__hexinput_buf},}
};
 
const static char fkt__foo__hexinput__name[] PROGMEM = "hexinput";
 
const static command fkt__foo__hexinput__command PROGMEM={
	function, fkt__foo__hexinput__name, (void *)&fkt__foo__hexinput__cfkt
};
 
const static char fkt__foo__hexdump_addr[] PROGMEM = "addr";
const static char fkt__foo__hexdump_size[] PROGMEM = "size";
 
c_function fkt__foo__hexdump__cfkt PROGMEM={
	(void(*)(void))&hexdump, {vd, 0},2,{{ptr,fkt__foo__hexdump_addr},{uc,fkt__foo__hexdump_size},}
};
 
const static char fkt__foo__hexdump__name[] PROGMEM = "hexdump";
 
const static command fkt__foo__hexdump__command PROGMEM={
	function, fkt__foo__hexdump__name, (void *)&fkt__foo__hexdump__cfkt
};
 
const static command_table fkt__foo__table PROGMEM={
	3, {	&fkt__foo__malloc__command,
	&fkt__foo__hexinput__command,
	&fkt__foo__hexdump__command,
}
};
 
const static char fkt__foo____table__name[] PROGMEM = "foo";
 
const static command fkt__foo____table__command PROGMEM={
	table, fkt__foo____table__name, (void *)&fkt__foo__table
};
 
const static command_table fkt__global__table PROGMEM={
	0, {}
};
 
const static char fkt__global____table__name[] PROGMEM = "global";
 
const static command fkt__global____table__command PROGMEM={
	table, fkt__global____table__name, (void *)&fkt__global__table
};
 
const static command_table fkt__i2cswconf__table PROGMEM={
	0, {}
};
 
const static char fkt__i2cswconf____table__name[] PROGMEM = "i2cswconf";
 
const static command fkt__i2cswconf____table__command PROGMEM={
	table, fkt__i2cswconf____table__name, (void *)&fkt__i2cswconf__table
};
 
 
c_function fkt__i2csw__i2cStart__cfkt PROGMEM={
	(void(*)(void))&i2cStart, {vd, 0},0,{}
};
 
const static char fkt__i2csw__i2cStart__name[] PROGMEM = "i2cStart";
 
const static command fkt__i2csw__i2cStart__command PROGMEM={
	function, fkt__i2csw__i2cStart__name, (void *)&fkt__i2csw__i2cStart__cfkt
};
 
 
c_function fkt__i2csw__i2cStop__cfkt PROGMEM={
	(void(*)(void))&i2cStop, {vd, 0},0,{}
};
 
const static char fkt__i2csw__i2cStop__name[] PROGMEM = "i2cStop";
 
const static command fkt__i2csw__i2cStop__command PROGMEM={
	function, fkt__i2csw__i2cStop__name, (void *)&fkt__i2csw__i2cStop__cfkt
};
 
const static char fkt__i2csw__i2cPutbyte_b[] PROGMEM = "b";
 
c_function fkt__i2csw__i2cPutbyte__cfkt PROGMEM={
	(void(*)(void))&i2cPutbyte, {ptr, 0},1,{{uc,fkt__i2csw__i2cPutbyte_b},}
};
 
const static char fkt__i2csw__i2cPutbyte__name[] PROGMEM = "i2cPutbyte";
 
const static command fkt__i2csw__i2cPutbyte__command PROGMEM={
	function, fkt__i2csw__i2cPutbyte__name, (void *)&fkt__i2csw__i2cPutbyte__cfkt
};
 
const static char fkt__i2csw__i2cGetbyte_last[] PROGMEM = "last";
 
c_function fkt__i2csw__i2cGetbyte__cfkt PROGMEM={
	(void(*)(void))&i2cGetbyte, {ptr, 0},1,{{uc,fkt__i2csw__i2cGetbyte_last},}
};
 
const static char fkt__i2csw__i2cGetbyte__name[] PROGMEM = "i2cGetbyte";
 
const static command fkt__i2csw__i2cGetbyte__command PROGMEM={
	function, fkt__i2csw__i2cGetbyte__name, (void *)&fkt__i2csw__i2cGetbyte__cfkt
};
 
const static char fkt__i2csw__i2cInit_[] PROGMEM = "";
 
c_function fkt__i2csw__i2cInit__cfkt PROGMEM={
	(void(*)(void))&i2cInit, {vd, 0},1,{{vd,fkt__i2csw__i2cInit_},}
};
 
const static char fkt__i2csw__i2cInit__name[] PROGMEM = "i2cInit";
 
const static command fkt__i2csw__i2cInit__command PROGMEM={
	function, fkt__i2csw__i2cInit__name, (void *)&fkt__i2csw__i2cInit__cfkt
};
 
const static char fkt__i2csw__i2cSend_device[] PROGMEM = "device";
const static char fkt__i2csw__i2cSend_sub[] PROGMEM = "sub";
const static char fkt__i2csw__i2cSend_length[] PROGMEM = "length";
const static char fkt__i2csw__i2cSend_data[] PROGMEM = "data";
 
c_function fkt__i2csw__i2cSend__cfkt PROGMEM={
	(void(*)(void))&i2cSend, {vd, 0},4,{{ptr,fkt__i2csw__i2cSend_device},{ptr,fkt__i2csw__i2cSend_sub},{ptr,fkt__i2csw__i2cSend_length},{ptr,fkt__i2csw__i2cSend_data},}
};
 
const static char fkt__i2csw__i2cSend__name[] PROGMEM = "i2cSend";
 
const static command fkt__i2csw__i2cSend__command PROGMEM={
	function, fkt__i2csw__i2cSend__name, (void *)&fkt__i2csw__i2cSend__cfkt
};
 
const static char fkt__i2csw__i2cReceive_device[] PROGMEM = "device";
const static char fkt__i2csw__i2cReceive_sub[] PROGMEM = "sub";
const static char fkt__i2csw__i2cReceive_length[] PROGMEM = "length";
const static char fkt__i2csw__i2cReceive_data[] PROGMEM = "data";
 
c_function fkt__i2csw__i2cReceive__cfkt PROGMEM={
	(void(*)(void))&i2cReceive, {vd, 0},4,{{ptr,fkt__i2csw__i2cReceive_device},{ptr,fkt__i2csw__i2cReceive_sub},{ptr,fkt__i2csw__i2cReceive_length},{ptr,fkt__i2csw__i2cReceive_data},}
};
 
const static char fkt__i2csw__i2cReceive__name[] PROGMEM = "i2cReceive";
 
const static command fkt__i2csw__i2cReceive__command PROGMEM={
	function, fkt__i2csw__i2cReceive__name, (void *)&fkt__i2csw__i2cReceive__cfkt
};
 
const static command_table fkt__i2csw__table PROGMEM={
	7, {	&fkt__i2csw__i2cStart__command,
	&fkt__i2csw__i2cStop__command,
	&fkt__i2csw__i2cPutbyte__command,
	&fkt__i2csw__i2cGetbyte__command,
	&fkt__i2csw__i2cInit__command,
	&fkt__i2csw__i2cSend__command,
	&fkt__i2csw__i2cReceive__command,
}
};
 
const static char fkt__i2csw____table__name[] PROGMEM = "i2csw";
 
const static command fkt__i2csw____table__command PROGMEM={
	table, fkt__i2csw____table__name, (void *)&fkt__i2csw__table
};
 
 
c_function fkt__uart__uart_init__cfkt PROGMEM={
	(void(*)(void))&uart_init, {vd, 0},0,{}
};
 
const static char fkt__uart__uart_init__name[] PROGMEM = "uart_init";
 
const static command fkt__uart__uart_init__command PROGMEM={
	function, fkt__uart__uart_init__name, (void *)&fkt__uart__uart_init__cfkt
};
 
const static char fkt__uart__uart_putc_c[] PROGMEM = "c";
 
c_function fkt__uart__uart_putc__cfkt PROGMEM={
	(void(*)(void))&uart_putc, {vd, 0},1,{{c,fkt__uart__uart_putc_c},}
};
 
const static char fkt__uart__uart_putc__name[] PROGMEM = "uart_putc";
 
const static command fkt__uart__uart_putc__command PROGMEM={
	function, fkt__uart__uart_putc__name, (void *)&fkt__uart__uart_putc__cfkt
};
 
const static char fkt__uart__uart_putstr_str[] PROGMEM = "str";
 
c_function fkt__uart__uart_putstr__cfkt PROGMEM={
	(void(*)(void))&uart_putstr, {vd, 0},1,{{str,fkt__uart__uart_putstr_str},}
};
 
const static char fkt__uart__uart_putstr__name[] PROGMEM = "uart_putstr";
 
const static command fkt__uart__uart_putstr__command PROGMEM={
	function, fkt__uart__uart_putstr__name, (void *)&fkt__uart__uart_putstr__cfkt
};
 
const static char fkt__uart__uart_putstr_P_str[] PROGMEM = "str";
 
c_function fkt__uart__uart_putstr_P__cfkt PROGMEM={
	(void(*)(void))&uart_putstr_P, {vd, 0},1,{{ptr,fkt__uart__uart_putstr_P_str},}
};
 
const static char fkt__uart__uart_putstr_P__name[] PROGMEM = "uart_putstr_P";
 
const static command fkt__uart__uart_putstr_P__command PROGMEM={
	function, fkt__uart__uart_putstr_P__name, (void *)&fkt__uart__uart_putstr_P__cfkt
};
 
const static char fkt__uart__uart_hexdump_buf[] PROGMEM = "buf";
const static char fkt__uart__uart_hexdump_len[] PROGMEM = "len";
 
c_function fkt__uart__uart_hexdump__cfkt PROGMEM={
	(void(*)(void))&uart_hexdump, {vd, 0},2,{{str,fkt__uart__uart_hexdump_buf},{i,fkt__uart__uart_hexdump_len},}
};
 
const static char fkt__uart__uart_hexdump__name[] PROGMEM = "uart_hexdump";
 
const static command fkt__uart__uart_hexdump__command PROGMEM={
	function, fkt__uart__uart_hexdump__name, (void *)&fkt__uart__uart_hexdump__cfkt
};
 
 
c_function fkt__uart__uart_getc__cfkt PROGMEM={
	(void(*)(void))&uart_getc, {c, 0},0,{}
};
 
const static char fkt__uart__uart_getc__name[] PROGMEM = "uart_getc";
 
const static command fkt__uart__uart_getc__command PROGMEM={
	function, fkt__uart__uart_getc__name, (void *)&fkt__uart__uart_getc__cfkt
};
 
const static char fkt__uart__uart_getc_nb_c[] PROGMEM = "c";
 
c_function fkt__uart__uart_getc_nb__cfkt PROGMEM={
	(void(*)(void))&uart_getc_nb, {c, 0},1,{{str,fkt__uart__uart_getc_nb_c},}
};
 
const static char fkt__uart__uart_getc_nb__name[] PROGMEM = "uart_getc_nb";
 
const static command fkt__uart__uart_getc_nb__command PROGMEM={
	function, fkt__uart__uart_getc_nb__name, (void *)&fkt__uart__uart_getc_nb__cfkt
};
 
 
c_function fkt__uart__uart_getline_nb__cfkt PROGMEM={
	(void(*)(void))&uart_getline_nb, {str, 0},0,{}
};
 
const static char fkt__uart__uart_getline_nb__name[] PROGMEM = "uart_getline_nb";
 
const static command fkt__uart__uart_getline_nb__command PROGMEM={
	function, fkt__uart__uart_getline_nb__name, (void *)&fkt__uart__uart_getline_nb__cfkt
};
 
const static command_table fkt__uart__table PROGMEM={
	8, {	&fkt__uart__uart_init__command,
	&fkt__uart__uart_putc__command,
	&fkt__uart__uart_putstr__command,
	&fkt__uart__uart_putstr_P__command,
	&fkt__uart__uart_hexdump__command,
	&fkt__uart__uart_getc__command,
	&fkt__uart__uart_getc_nb__command,
	&fkt__uart__uart_getline_nb__command,
}
};
 
const static char fkt__uart____table__name[] PROGMEM = "uart";
 
const static command fkt__uart____table__command PROGMEM={
	table, fkt__uart____table__name, (void *)&fkt__uart__table
};
 
const command_table main_table PROGMEM={
	9 ,{	&fkt__avrlibdefs____table__command,
	&fkt__avrlibtypes____table__command,
	&fkt__config____table__command,
	&fkt__console____table__command,
	&fkt__foo____table__command,
	&fkt__global____table__command,
	&fkt__i2cswconf____table__command,
	&fkt__i2csw____table__command,
	&fkt__uart____table__command,
	}
}; 
