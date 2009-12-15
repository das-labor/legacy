#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can.h"
#include "can_handler.h"
#include "lap.h"
#include "../twi_master/twi_master.h"
#include "../../include/PowerCommander.h"

#define SLAVE_ADDR 15


extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == 0x03))
		{
			PORTA |= _BV(PA4);
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
				case FKT_MGT_RESET:
					TCCR2 = 0;
					wdt_enable(0);
					while(1);
			
				case FKT_MGT_PING:

					msg.addr_src = 0x03;
					msg.addr_dst = rx_msg->addr_src;
					can_transmit(&msg);
					break;
				}
			}
			else if (rx_msg->port_dst == 1)
			{
				/*
					unterbinden, dass ueber can ein paar sachen umgelegt werden koennen
				*/
				if ( ( (rx_msg->data[0] == C_VIRT) &&	(rx_msg->data[1] == VIRT_POWER)) ||
					( (rx_msg->data[0] == C_SW) &&
					( (rx_msg->data[1] == SWA_HS) ||
					(rx_msg->data[1] == SWA_STECKDOSEN) ||
					(rx_msg->data[1] == SWA_KLO))))
				{
				}

				/*
					gehe davon aus, dass genau so viele daten die
					via can reingekommen sind auch wieder auf i2c raus sollen
								
					XXX - check mit rx_msg->cmd und data[i] was steht wirklich wo
				*/
				else
				{
					twi_send(rx_msg->data);
				}
			}
		}
	}
}

void twi_send(uint8_t *p)
{
	uint8_t i;
	if (!TWIM_Start(SLAVE_ADDR, TWIM_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		/*
			daten wirklich raus schreiben
		*/
		for (i = 0; i < 4; i++)
		{
			TWIM_Write(p[i]);
		}
		TWIM_Stop();
	}
}

void can_send(uint8_t *p)
{
	static can_message msg = {0x02, 0x00, 0x00, 0x01, 2, {0}};
	uint8_t i;
	for (i = 0; i < 3; i++)
		msg.data[i] = p[i];
	can_transmit(&msg);
}

