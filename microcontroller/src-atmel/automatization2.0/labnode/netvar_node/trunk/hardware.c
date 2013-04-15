#include <avr/io.h>
#include <avr/wdt.h>

void hardware_reset() {
	cli();
	wdt_enable(0);
	while (1);
}

