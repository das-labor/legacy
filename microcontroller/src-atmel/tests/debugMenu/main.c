#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include "debugMenu.h"

int main () {
	uint8_t wert = 0;
	debugMenuInit();
	debugMenuSetFormat(0, DM_FRMT_VOLUME);
	debugMenuSetValue(0, 3);
	debugMenuSetFormat(1, DM_FRMT_BIN);		
	debugMenuSetValue(1, 50);   // Geschwindigkeit
	debugMenuSetFormat(2, DM_FRMT_BIN);	
	while (1) {
		debugMenuSetValue(2, wert++);  // Wert wird bei jedem Durchlauf um 1 erhöhen
		_delay_ms(debugMenuGetValue(1));
	}
	return 0;
}
