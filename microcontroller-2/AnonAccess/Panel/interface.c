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

