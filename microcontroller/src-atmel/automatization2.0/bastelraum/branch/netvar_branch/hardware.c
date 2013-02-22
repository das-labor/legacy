#include <avr/wdt.h>
#include <avr/interrupt.h>

void hardware_reset() {
	cli();
	wdt_enable(0);
	while (1);
}

