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

#include "i2cmaster.h"
#include <stdlib.h>
#include <avr/eeprom.h>

//#define wo ist der sensor

//uint8_t tempact;
TimerControlBlock switchtimer3;
								// sendeport?
								// 
								// 


AVRX_GCC_TASKDEF(temp, 40, 3) {
	uint8_t sensors = eeprom_read_byte(0x01);
	uint8_t sensor[sensors], hbyte, lbyte;
  	char buffer[sensors][2];
	for (uint8_t i = 0; i < sensors; i++) {
		sensor[i] = eeprom_read_byte(2 + i);
	}
	static can_message_t temp = {0, 0x00, PORT_TEMP, PORT_REMOTE, 8, {}};
	temp.addr_src = myaddr;
	//tempact = 0;
	//temp.addr_dst = rx_msg.addr_src;
	while (1) {
		AvrXDelay (&switchtimer3, 1000);
		//if (tempact > 0) {
			for (uint8_t j = 0; j < sensors; j++) {
				if (i2c_start(sensor[j] + I2C_READ)) {			// set device address and read mode
					i2c_stop();					// stop condition
					hbyte = 0xff;
				}
				else {
					hbyte = i2c_readAck();
					lbyte = i2c_readNak();				// msb byte2 
					i2c_stop();
				}
				if (buffer[j][0] != hbyte && buffer[j][1] != lbyte) {
					temp.data[0] = sensor[j];
					temp.data[1] = buffer[j][0] = hbyte;
					temp.data[2] = buffer[j][1] = lbyte;
					can_put (&temp);
				}
			}
			//tempact--;
		//}
	}
}
