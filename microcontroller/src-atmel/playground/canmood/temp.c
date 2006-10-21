#include <avr/io.h>
#include <string.h>

/*
	temperaturen werden gemulticastet wenn aktiv und verändert zum vorwert
	temp als i2c wert senden, mit sensorcode, stop tempcasating
*/

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "pwm.h"
#ifdef I2C
#include "i2cmaster.h"
#include <stdlib.h>
#endif
#define ADRESSE 0x90
//#define wo ist der sensor

//uint8_t tempact;
TimerControlBlock switchtimer3;
								// sendeport?
								// 
								// 


AVRX_GCC_TASKDEF(temp, 20, 4) {
  	char buffer[8];
	static can_message_t temp = {0, 0xa0, PORT_TEMP, PORT_REMOTE, 7, {0}};
	temp.addr_src = myaddr;
	//tempact = 0;
	//temp.addr_dst = rx_msg.addr_src;
	while (1) {
		//if(tempact > 0) {
			AvrXDelay(&switchtimer3, 1000);
			if (i2c_start (ADRESSE+I2C_READ)) {			// set device address and read mode
				i2c_stop();							// stop condition
				strcpy (buffer, "err");
			}
			else {
				itoa ((char) i2c_readAck (), buffer, 10);	// convert to string, cast for negative tempratures
				if ((i2c_readNak () & 0x80) == 0x80)		// test for msb
					strcat (buffer, ".5");				// ok for neg vaules?
				else {
					strcat (buffer, ".0");
				}
				i2c_stop();
			}
			if(strcmp(buffer, temp.data)) {
				strcpy (temp.data, buffer);
				can_put (&temp);
			}
			//tempact--;
		//}
	}
}
