
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "stuff.h"
#include "grafiklcd.h"
#include "text.h"

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(stuff, 100, 4) {
	char bla[8], bla2[8];
	sensor_t *aktSensor;
	uint8_t i;
	while(1) {
		aktSensor = sensor;
		i = 0;
		AvrXDelay(&switchtimer, 1000);

		while (aktSensor != NULL) {
			dispFillRect(80, 30 + 9 * i, 30, 8, 0);
			itoa(aktSensor->typ, bla, 10);
			strcat(bla, " ");
			itoa((int) aktSensor->wert, bla2, 10);
			strcat(bla, bla2);
			draw_Text(bla, 80, 30 + 9 * i, 0, 1, 1);
			aktSensor = aktSensor->next;
			i++;
		}
	}
}

