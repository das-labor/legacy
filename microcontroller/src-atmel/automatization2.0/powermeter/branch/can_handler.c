#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can/lap.h"
#include "string.h"
#include "powermeter_driver.h"
#include "can_handler.h"
#include "config.h"

#define PORT_POWERMETER 0x3C
uint8_t myaddr;


void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb())) //get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						/*I/O Protection*/
						CCP = CCP_IOREG_gc;
						//Reset
						RST.CTRL = RST_SWRST_bm;
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
			}
		}
		can_free(rx_msg);
	}
}

inline void read_can_addr()
{
	myaddr = eeprom_read_byte(EEPROM_LAP_ADDR);
}

void can_send_value_packet(can_message *template_msg, uint8_t id, void *value, size_t length)
{
	can_message *txmsg;
	txmsg = can_buffer_get();
	memcpy(txmsg, template_msg, sizeof(can_message) - 8); //header
	txmsg->data[0] = id;
	memcpy(txmsg->data + 1, value, length < 7 ? length : 7);
	can_transmit(txmsg);
}

//TODO port, id
void can_createDATAPACKET()
{
	static void *value_pointer[] =
	{
		(void *) &powermeter.powerdrawLastSecond.c1.P,
		(void *) &powermeter.powerdrawLastSecond.c2.P,
		(void *) &powermeter.powerdrawLastSecond.c3.P,
		(void *) &powermeter.powerdrawLastSecond.c1.S,
		(void *) &powermeter.powerdrawLastSecond.c2.S,
		(void *) &powermeter.powerdrawLastSecond.c3.S,
		(void *) &powermeter.powerdrawLastSecond.c1.Ueff,
		(void *) &powermeter.powerdrawLastSecond.c2.Ueff,
		(void *) &powermeter.powerdrawLastSecond.c3.Ueff,
		(void *) &powermeter.powerdrawLastSecond.c1.Ieff,
		(void *) &powermeter.powerdrawLastSecond.c2.Ieff,
		(void *) &powermeter.powerdrawLastSecond.c3.Ieff,
		(void *) &powermeter.powerdrawLastSecond.c1.E,
		(void *) &powermeter.powerdrawLastSecond.c2.E,
		(void *) &powermeter.powerdrawLastSecond.c3.E,
		(void *) &powermeter.seconds_uptime,
	};

	static can_message msg = {0, 0, PORT_POWERMETER, PORT_POWERMETER, 4, {0, 0, 0, 0}};
	can_message *txmsg;
	uint8_t id = 0, i;

	msg.addr_src = myaddr;
	msg.dlc = 4;
	*(uint16_t *)&msg.data[0] = ADCSAMPLESPERPERIOD * NET_FREQ;
	msg.data[2] = 0;
	msg.data[3] = 0;

	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message));
	can_transmit(txmsg);

	id++;
	msg.dlc = 5;

	//send all data value ram locations,
	//assuming that _ALL_ values are 4 byte
	for (i = 0; i < (sizeof(value_pointer) / sizeof(void *)); i++)
	{
		#if sendwithDELAY
			_delay_ms(2);
		#endif
		can_send_value_packet(&msg, id++, value_pointer[i], 4);
	}

	//powermeter_clearpowerdrawPerSecond();
}

