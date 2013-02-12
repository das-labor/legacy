
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "dc_com.h"

void hardware_reset(){
	dc_byte_put(DC_SHOW_HYPNOTOAD);
	cli();
	wdt_enable(0);
	while (1);
}
