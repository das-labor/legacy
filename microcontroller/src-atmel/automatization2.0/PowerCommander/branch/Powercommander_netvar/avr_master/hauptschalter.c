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
			//alle power relais aus
			set_output(SWA_HS,          0);
			set_output(SWA_KLO,         0);
			set_output(SWA_STECKDOSEN,  0);
		}
	}
}

void hauptschalter_update(uint8_t stat) {
	if (stat) { //schalter angeschaltet
		timeout_cnt = 0; //timer stoppen
		
		//alle power relais an
		set_output(SWA_HS,          1);
		set_output(SWA_KLO,         1);
		set_output(SWA_STECKDOSEN,  1);		
	}
	else
	{
		/* start timeout, shutdown after 5 seconds */
		start_main_switch_timeout();
	}
}
