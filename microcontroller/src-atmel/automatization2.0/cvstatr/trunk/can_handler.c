#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "i2c_temp.h"

static uint8_t myaddr;

union {
	struct {
		uint8_t klingel:1; // 1 Bit für bStatus_1
		uint8_t nachtmodus:1;    // Dieses Feld ist 2 Bits breit
		uint8_t tuerkontakt:1;
		uint8_t schloss:1;
	} switches;
	uint8_t bla;
} stat_switches;

extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						TCCR2 = 0;
						wdt_enable(0);
						while (1);
			
					case FKT_MGT_PING:
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						can_transmit(&msg);
						break;
				}
			}
			else if (rx_msg->port_dst == 0x10)
			{
				uint8_t data[2] = {0, 0};
				get_temp(data, 0x96);	
				msg.data[0] = data[0];
				msg.data[1] = data[1];
				msg.dlc = 2;
				msg.port_dst = 3;
				msg.port_src = 3;
				msg.addr_src = myaddr;
				msg.addr_dst = rx_msg->addr_src;
				can_transmit(&msg);
			}
		}
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

void switch_handler()
{
// Hauptschalter
	if (!(PIND & _BV(PD6)) && stat_switches.switches.klingel)
	{
		stat_switches.switches.klingel = 0;
		_delay_ms(100);
	}
	if ((PIND & _BV(PD6)) && stat_switches.switches.klingel == 0 && stat_switches.switches.tuerkontakt)
	{
		stat_switches.switches.klingel = 1;

		static can_message msg = {0x03, 0x00, 0x00, 0x01, 1, {0}};
		
		msg.data[0] = stat_switches.bla;
		msg.addr_src = myaddr;
		can_transmit(&msg);

		_delay_ms(100);
	}
	if ((PIND & _BV(PD7)) && stat_switches.switches.tuerkontakt)
	{
		static can_message msg = {0x03, 0x00, 0x00, 0x01, 1, {0}};
		stat_switches.switches.tuerkontakt = 0;		
		msg.data[0] = stat_switches.bla;
		msg.addr_src = myaddr;
		can_transmit(&msg);

		_delay_ms(100);
	}
	if (!(PIND & _BV(PD7)) && stat_switches.switches.tuerkontakt == 0)
	{
		stat_switches.switches.tuerkontakt = 1;

		static can_message msg = {0x03, 0x00, 0x00, 0x01, 1, {0}};
		
		msg.data[0] = stat_switches.bla;
		msg.addr_src = myaddr;
		can_transmit(&msg);

		_delay_ms(100);
	}
}

