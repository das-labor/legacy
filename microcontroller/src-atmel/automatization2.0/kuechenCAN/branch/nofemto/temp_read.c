#include <avr/io.h>

#include "can/can.h"
#include "can/lap.h"
#include "i2c_temp.h"

static uint8_t temp_old;

void temp_sensor_read() {
	//static can_message msg = {0x04, 0x00, 0x10, 0x10, 2, {0}};
	uint8_t data[2] = {0, 0};
	get_temp(data, 0x96);
	if (data[1] != temp_old) {
	//	msg.data[0] = data[0];
	//	msg.data[1] = data[1];
	//	msg.addr_src = myaddr;
	//	can_transmit(&msg);
		temp_old = data[1];
	}
}

