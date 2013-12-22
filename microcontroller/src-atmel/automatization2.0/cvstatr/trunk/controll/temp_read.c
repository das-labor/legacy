#include <stdint.h>

#include "can_handler.h"
#include "ds1631.h"
#include "config.h"


void temp_sensor_read(void)
{
	uint8_t data[2] = {255, 255};

	ds1631_get_temp(data, I2C_ADRESSE_DS1631);
	can_send_temp_data(data);
}

