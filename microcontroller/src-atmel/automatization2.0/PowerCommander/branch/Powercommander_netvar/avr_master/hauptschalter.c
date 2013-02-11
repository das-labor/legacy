#include <stdint.h>

#include "config.h"
#include "../include/PowerCommander.h"
#include "i2c_com.h"


static uint8_t timeout_cnt;
static void start_main_switch_timeout(void) {
	/* 5 seconds timeout */
	timeout_cnt = 250;
}

void handle_main_switch_timeout(void) {
	if (timeout_cnt > 0)
	{
		timeout_cnt--;
		if (!timeout_cnt)
		{
			/* no need to handle other relays, they are controlled by set_lamp... */
			outputdata.ports &= ~((1<<SWA_HS) | (1<<SWA_KLO) | (1<<SWA_STECKDOSEN) | (1<<SWA_BEAMER));
			twi_send();
		}
	}
}

void hauptschalter_update(uint8_t stat) {
	if (stat) { //schalter angeschaltet
		timeout_cnt = 0; //timer stoppen
		outputdata.ports |= (1<<SWA_HS) | (1<<SWA_KLO) | (1<<SWA_STECKDOSEN); //alle power relais an
		twi_send();
	}
	else
	{
		/* start timeout, shutdown after 5 seconds */
		start_main_switch_timeout();
	}
}

