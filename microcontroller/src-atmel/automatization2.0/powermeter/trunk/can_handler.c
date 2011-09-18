
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "can/can.h"
#include "can/lap.h"
#include "string.h"
#include "powermeter_driver.h"

#include "can_handler.h"

uint8_t myaddr;
#define sendwithDELAY 0
#define laborhack 1

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

//TODO port, id
#define PORT_POWERMETER 0x06
void can_createDATAPACKET()
{
	static can_message msg = {0, 0, PORT_POWERMETER, PORT_POWERMETER, 4, {}};
	can_message * txmsg;
	
	uint8_t id=0;
	
	msg.addr_src = myaddr;
	msg.dlc = 3;
	msg.data[0] = 0;   //counter (0=start message)
	msg.data[1] = id;  //TODO
	msg.data[2] = 48;

#if laborhack
	if(!laborcanbushack)
	{
	laborcanbushack =1;
#endif			
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message));
	can_transmit(txmsg);
	
	msg.dlc = 5;
	
	//transmit C1.P
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 1;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c1.P, 4);
	can_transmit(txmsg);
#if sendwithDELAY
	_delay_ms(2);
#endif
	//transmit C2.P
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 2;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c2.P, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C3.P
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 3;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c3.P, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C1.S
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 4;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c1.S, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C2.S
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 5;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c2.S, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C3.S
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 6;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c3.S, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C1.Ueff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 7;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c1.Ueff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C2.Ueff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 8;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c2.Ueff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
#if laborhack
	}else{
	laborcanbushack=0;
	msg.dlc = 5;
#endif
	//transmit C3.Ueff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 0x09;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c3.Ueff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C1.Ieff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 0x0a;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c1.Ieff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C2.Ieff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 0x0b;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c2.Ieff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
	//transmit C3.Ieff
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message) );//header
	*txmsg->data = 0x0c;
	memcpy(txmsg->data+1, (void *)&powermeter.powerdrawLastSecond.c3.Ieff, 4);
	can_transmit(txmsg);
#if sendwithDELAY
        _delay_ms(2);
#endif
#if laborhack
	}
#endif
	//powermeter_clearpowerdrawPerSecond();
	
}
