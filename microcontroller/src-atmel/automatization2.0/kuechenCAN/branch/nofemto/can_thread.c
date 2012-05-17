#include "can.h"
#include "lap.h"
#include <avr/wdt.h>

extern void can_user_cmd(can_message *rx_msg);
extern void can_sniffer(can_message *rx_msg);



void can_mgt(can_message *rx_msg)
{
  static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
  switch (rx_msg->data[0])
    {
    case FKT_MGT_RESET:
      TCCR2 = 0;
      wdt_enable(0);
      while(1);
	      
    case FKT_MGT_PING:
      msg.addr_src = rx_msg->addr_dst;
      msg.addr_dst = rx_msg->addr_src;
      can_transmit(&msg);
      break;
	      
    default:
      break;
    }
}


void can_handler()
{
  can_message *rx_msg;
  rx_msg = can_get_nb();
  if ( (rx_msg != 0))
    {
      if (rx_msg->addr_dst == CANADDR)
	{
	  if (rx_msg->port_dst == PORT_MGT)
	    {
	      can_mgt(rx_msg);
	    }
	  else
	    {
	      can_user_cmd(rx_msg);
	    }
	}
#ifdef CAN_SNIFFER
      else 
	{
	  can_sniffer(rx_msg);
	}
#endif
    }
}
