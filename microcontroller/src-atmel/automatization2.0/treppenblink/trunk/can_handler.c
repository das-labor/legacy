#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can_handler.h"
#include "can/can.h"
#include "can/lap.h"
#include "animationen.h"
#include "io.h"


uint8_t myaddr;

void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()))			//get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			switch (rx_msg->port_dst)
			{
			case PORT_MGT:
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						wdt_enable(WDTO_15MS);
						while (1);
					case FKT_MGT_PING:
						{
							can_message *tx_msg = can_buffer_get();
							tx_msg->port_src = PORT_MGT;
							tx_msg->port_dst = PORT_MGT;
							tx_msg->addr_src = myaddr;
							tx_msg->addr_dst = rx_msg->addr_src;
							tx_msg->dlc = 1;
							tx_msg->data[0] = FKT_MGT_PONG;
							can_transmit(tx_msg);
						}
						break;
				}
				break;
			case 0x0f:
				switch(rx_msg->data[0]) { // alte nummerierung in neue
					case 4: // aus
						animation = 0;
						can_send_ani_status();
						break;
					case 2: // weiss
						animation = 1;
						can_send_ani_status();
						break;
					case 8: // rgb
						animation = 2;
						can_send_ani_status();
						break;
				}
				break;
			}
		}
		if (rx_msg->addr_src == 0x02 && rx_msg->port_src == 0x02) // get relais status from powercommander for rgb led
		{
			if ((rx_msg->data[0]) & 0x20)
				set_leds(1);
			else if (!(rx_msg->data[0] & 0x20))
				set_leds(2);
		}
		can_free(rx_msg);
	}
}

void can_send_input_status(uint8_t stat_sw, uint8_t change)
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x02;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 2;
	msg->data[0] = stat_sw;
	msg->data[1] = change;
	can_transmit(msg);
}

void can_send_ani_status(void)
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x03;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 1;
	msg->data[0] = animation;
	can_transmit(msg);
}

void can_send_temp_data(uint8_t *data)
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x0f;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 2;
	msg->data[0] = data[0];
	msg->data[1] = data[1];
	can_transmit(msg);
}

static const uint8_t EE_lap_addr EEMEM = EEPROM_LAP_ADDR;

void can_read_addr()
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}

