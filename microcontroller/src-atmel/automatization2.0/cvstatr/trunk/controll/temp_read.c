#include <avr/io.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "i2c_temp.h"

uint8_t temp_old;

void temp_sensor_read() {
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	uint8_t data[2] = {0, 0};
	get_temp(data, 0x96);
	if (data[1] != temp_old) {
		msg.data[0] = data[0];
		msg.data[1] = data[1];
		msg.dlc = 2;
		msg.port_dst = 0x10;
		msg.port_src = 0x10;
		msg.addr_src = myaddr;
		msg.addr_dst = 0x00;
		can_transmit(&msg);
	}
	temp_old = data[1];
}
