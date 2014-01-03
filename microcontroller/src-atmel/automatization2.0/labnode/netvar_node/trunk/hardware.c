#include <avr/wdt.h>
#include <avr/interrupt.h>

void hardware_reset() {
	cli();
	wdt_enable(WDTO_15MS);
	while (1);
}

