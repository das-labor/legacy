#include "console.h"
#include <avr/pgmspace.h>
#include "util.h"
#include "uart.h"
#include "ps2.h"
#include "console.h"
 
c_function fkt__console__console__cfkt PROGMEM={
	(void(*)(void))&console, {vd, 0},0,{}
};
 
const static char fkt__console__console__name[] PROGMEM = "console";
 
const static command fkt__console__console__command PROGMEM={
	function, fkt__console__console__name, (void *)&fkt__console__console__cfkt
};
 
 
c_function fkt__console__console_init__cfkt PROGMEM={
	(void(*)(void))&console_init, {vd, 0},0,{}
};
 
const static char fkt__console__console_init__name[] PROGMEM = "console_init";
 
const static command fkt__console__console_init__command PROGMEM={
	function, fkt__console__console_init__name, (void *)&fkt__console__console_init__cfkt
};
 
const static command_table fkt__console__table PROGMEM={
	2, {	&fkt__console__console__command,
	&fkt__console__console_init__command,
}
};
 
const static char fkt__console____table__name[] PROGMEM = "console";
 
const static command fkt__console____table__command PROGMEM={
	table, fkt__console____table__name, (void *)&fkt__console__table
};
 
const static char fkt__ps2__key_press_key[] PROGMEM = "key";
 
c_function fkt__ps2__key_press__cfkt PROGMEM={
	(void(*)(void))&key_press, {vd, 0},1,{{ptr,fkt__ps2__key_press_key},}
};
 
const static char fkt__ps2__key_press__name[] PROGMEM = "key_press";
 
const static command fkt__ps2__key_press__command PROGMEM={
	function, fkt__ps2__key_press__name, (void *)&fkt__ps2__key_press__cfkt
};
 
const static char fkt__ps2__key_make_key[] PROGMEM = "key";
 
c_function fkt__ps2__key_make__cfkt PROGMEM={
	(void(*)(void))&key_make, {vd, 0},1,{{ptr,fkt__ps2__key_make_key},}
};
 
const static char fkt__ps2__key_make__name[] PROGMEM = "key_make";
 
const static command fkt__ps2__key_make__command PROGMEM={
	function, fkt__ps2__key_make__name, (void *)&fkt__ps2__key_make__cfkt
};
 
const static char fkt__ps2__key_break_key[] PROGMEM = "key";
 
c_function fkt__ps2__key_break__cfkt PROGMEM={
	(void(*)(void))&key_break, {vd, 0},1,{{ptr,fkt__ps2__key_break_key},}
};
 
const static char fkt__ps2__key_break__name[] PROGMEM = "key_break";
 
const static command fkt__ps2__key_break__command PROGMEM={
	function, fkt__ps2__key_break__name, (void *)&fkt__ps2__key_break__cfkt
};
 
 
c_function fkt__ps2__init_ps2__cfkt PROGMEM={
	(void(*)(void))&init_ps2, {vd, 0},0,{}
};
 
const static char fkt__ps2__init_ps2__name[] PROGMEM = "init_ps2";
 
const static command fkt__ps2__init_ps2__command PROGMEM={
	function, fkt__ps2__init_ps2__name, (void *)&fkt__ps2__init_ps2__cfkt
};
 
const static command_table fkt__ps2__table PROGMEM={
	4, {	&fkt__ps2__key_press__command,
	&fkt__ps2__key_make__command,
	&fkt__ps2__key_break__command,
	&fkt__ps2__init_ps2__command,
}
};
 
const static char fkt__ps2____table__name[] PROGMEM = "ps2";
 
const static command fkt__ps2____table__command PROGMEM={
	table, fkt__ps2____table__name, (void *)&fkt__ps2__table
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
 
 
c_function fkt__uart__uart_getc__cfkt PROGMEM={
	(void(*)(void))&uart_getc, {c, 0},0,{}
};
 
const static char fkt__uart__uart_getc__name[] PROGMEM = "uart_getc";
 
const static command fkt__uart__uart_getc__command PROGMEM={
	function, fkt__uart__uart_getc__name, (void *)&fkt__uart__uart_getc__cfkt
};
 
const static char fkt__uart__uart_getc_nb_c[] PROGMEM = "c";
 
c_function fkt__uart__uart_getc_nb__cfkt PROGMEM={
	(void(*)(void))&uart_getc_nb, {uc, 0},1,{{str,fkt__uart__uart_getc_nb_c},}
};
 
const static char fkt__uart__uart_getc_nb__name[] PROGMEM = "uart_getc_nb";
 
const static command fkt__uart__uart_getc_nb__command PROGMEM={
	function, fkt__uart__uart_getc_nb__name, (void *)&fkt__uart__uart_getc_nb__cfkt
};
 
const static char fkt__uart__uart_putstr_str[] PROGMEM = "str";
 
c_function fkt__uart__uart_putstr__cfkt PROGMEM={
	(void(*)(void))&uart_putstr, {vd, 0},1,{{str,fkt__uart__uart_putstr_str},}
};
 
const static char fkt__uart__uart_putstr__name[] PROGMEM = "uart_putstr";
 
const static command fkt__uart__uart_putstr__command PROGMEM={
	function, fkt__uart__uart_putstr__name, (void *)&fkt__uart__uart_putstr__cfkt
};
 
 
c_function fkt__uart__uart_getline_nb__cfkt PROGMEM={
	(void(*)(void))&uart_getline_nb, {str, 0},0,{}
};
 
const static char fkt__uart__uart_getline_nb__name[] PROGMEM = "uart_getline_nb";
 
const static command fkt__uart__uart_getline_nb__command PROGMEM={
	function, fkt__uart__uart_getline_nb__name, (void *)&fkt__uart__uart_getline_nb__cfkt
};
 
const static char fkt__uart__uart_putstr_P_str[] PROGMEM = "str";
 
c_function fkt__uart__uart_putstr_P__cfkt PROGMEM={
	(void(*)(void))&uart_putstr_P, {vd, 0},1,{{ptr,fkt__uart__uart_putstr_P_str},}
};
 
const static char fkt__uart__uart_putstr_P__name[] PROGMEM = "uart_putstr_P";
 
const static command fkt__uart__uart_putstr_P__command PROGMEM={
	function, fkt__uart__uart_putstr_P__name, (void *)&fkt__uart__uart_putstr_P__cfkt
};
 
const static command_table fkt__uart__table PROGMEM={
	7, {	&fkt__uart__uart_init__command,
	&fkt__uart__uart_putc__command,
	&fkt__uart__uart_getc__command,
	&fkt__uart__uart_getc_nb__command,
	&fkt__uart__uart_putstr__command,
	&fkt__uart__uart_getline_nb__command,
	&fkt__uart__uart_putstr_P__command,
}
};
 
const static char fkt__uart____table__name[] PROGMEM = "uart";
 
const static command fkt__uart____table__command PROGMEM={
	table, fkt__uart____table__name, (void *)&fkt__uart__table
};
 
const static char fkt__util__wait_ms[] PROGMEM = "ms";
 
c_function fkt__util__wait__cfkt PROGMEM={
	(void(*)(void))&wait, {vd, 0},1,{{i,fkt__util__wait_ms},}
};
 
const static char fkt__util__wait__name[] PROGMEM = "wait";
 
const static command fkt__util__wait__command PROGMEM={
	function, fkt__util__wait__name, (void *)&fkt__util__wait__cfkt
};
 
const static command_table fkt__util__table PROGMEM={
	1, {	&fkt__util__wait__command,
}
};
 
const static char fkt__util____table__name[] PROGMEM = "util";
 
const static command fkt__util____table__command PROGMEM={
	table, fkt__util____table__name, (void *)&fkt__util__table
};
 
const command_table main_table PROGMEM={
	4 ,{	&fkt__console____table__command,
	&fkt__ps2____table__command,
	&fkt__uart____table__command,
	&fkt__util____table__command,
	}
}; 
