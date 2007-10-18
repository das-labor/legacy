
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "debug.h"
//#include "sha256.h"

/*
 *  EEPROM-MAP:
 * 
 *  +----+----+----+----+
 *  |    |    |    |    |
 *  +----+----+----+----+
 */

#include <avr/eeprom.h>

#define SETUP_DONE_FIELD 0x0000
#define SETUP_DONE_VALUE 0x23

bool setup_done(){
	eeprom_busy_wait();
	return (eeprom_read_byte((void*)SETUP_DONE_FIELD) == SETUP_DONE_VALUE)  
}

int main(){
	if (!setup_done()){
		setup_system();
	}
	init_system();
	while (1){
		action = getActionReq();
		if (validAction(action)){
			giveNewTokens();
			doAction(action);
		} else {
			doNotActio(action);
		}
	}
	return 0;
}


