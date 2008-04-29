#include "config.h"
#include "lcd_tools.h"
#include "uart.h"
#include "menu.h"
#include "keypad.h"
#include "rtc.h"
#include "prng.h"
#include "lop.h"
#include "lop_debug.h"
#include <stdint.h>
#include <util/delay.h>
#include "interface.h"

char status_string[6] = "     ";


void print_status(void){
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr("=AnonAccess=");
	lcd_writestr(status_string);
}

void waitforkey(char key){
	while(read_keypad()!=key)
		;
	_delay_ms(1);
	while(read_keypad()==key)
		;
	_delay_ms(1);
}

char waitforanykey(void){
	char key;
	while((key=read_keypad())==' ')
		;
	_delay_ms(1);
	while(read_keypad()==key)
		;
	_delay_ms(1);
	return key;
}
