
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

uint8_t myaddr;


#if laborhack
uint8_t laborcanbushack;
#endif

void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	can_message * txmsg;
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
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
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;

						txmsg = can_buffer_get();
						memcpy(txmsg, &msg, sizeof(can_message));
						can_transmit(txmsg);
						break;
				}
			}
			
		}
		if (rx_msg->port_dst == 0x37)
		{
			//printf("netvar received\r\n");
			netvar_received(rx_msg);
		}
		can_free(rx_msg);
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

void can_sendValuePacket(can_message *template_msg, uint8_t id, void* value, size_t length)
{
	can_message * txmsg;
	txmsg = can_buffer_get();
	memcpy(txmsg, template_msg, sizeof(can_message) - 8); //header
	txmsg->data[0] = id;
	memcpy(txmsg->data+1, value, max(length, 7));
	can_transmit(txmsg);
}

//TODO port, id
#define PORT_POWERMETER 0x06
void can_createDATAPACKET()
{
	static void* value_pointer[] =
	{
		(void *)&powermeter.powerdrawLastSecond.c1.P,
		(void *)&powermeter.powerdrawLastSecond.c2.P,
		(void *)&powermeter.powerdrawLastSecond.c3.P,
		(void *)&powermeter.powerdrawLastSecond.c1.S,
		(void *)&powermeter.powerdrawLastSecond.c2.S,
		(void *)&powermeter.powerdrawLastSecond.c3.S,
		(void *)&powermeter.powerdrawLastSecond.c1.Ueff,
		(void *)&powermeter.powerdrawLastSecond.c2.Ueff,
		(void *)&powermeter.powerdrawLastSecond.c3.Ueff,
		(void *)&powermeter.powerdrawLastSecond.c1.Ieff,
		(void *)&powermeter.powerdrawLastSecond.c2.Ieff,
		(void *)&powermeter.powerdrawLastSecond.c3.Ieff,
	}
	
	static can_message msg = {0, 0, PORT_POWERMETER, PORT_POWERMETER, 4, {}};
	can_message * txmsg;
	uint8_t id = 0, i;
	
	msg.addr_src = myaddr;
	msg.dlc = 4;
	msg.data[0] = (uint8_t)((powermeter.samplesPerSecondDone>>8)&0xff);   //counter (0=start message)
	//msg.data[1] = id;  //TODO
	msg.data[1] = (uint8_t)(powermeter.samplesPerSecondDone&0xff);
	//msg.data[2] = (uint8_t)((powermeter.adcsamples>>8)&0xff);
	//msg.data[3] = (uint8_t)(powermeter.adcsamples&0xff);
	//msg.data[4] = powermeter.ADCSamplesPerPeriod;
	msg.data[2] = (uint8_t)((powermeter.timercc1clks>>8)&0xff);
	msg.data[3] = (uint8_t)(powermeter.timercc1clks&0xff);
	powermeter.timercc1clks=0;
	powermeter.adcsamples=0;
	
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message));
	can_transmit(txmsg);
	
	id++;
	msg.dlc = 5;
	
	//send all data value ram locations,
	//assuming that _ALL_ values are 4 byte
	for(i = 0; i < (sizeof(value_pointer) / sizeof(void*)); i++)
	{
		can_sendValuePacket(&msg, id++, value_pointer[i], 4);
		#if sendwithDELAY
			_delay_ms(2);
		#endif
	}
	
	//powermeter_clearpowerdrawPerSecond();
}
