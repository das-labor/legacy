#include "femtoos_code.h"
#include "can.h"
#include "lap.h"

extern Tuint08 blinkmode;
void can_handler()
{
  can_message *rx_msg;
  rx_msg = can_get_nb();
  if ((rx_msg != 0) && (rx_msg->addr_dst == CANADDR))
    {
      if (rx_msg->port_dst == PORT_MGT)
	{
	  can_mgt(rx_msg);
	}
      else if (rx_msg->port_dst == CANPORT)
	{
	  blinkmode = rx_msg->data[0];
	}
    }
}

#if (preTaskDefined(xcan))
void appLoop_xcan(void)
{
  while(true)
    {
      can_handler(); 
      taskDelayFromNow(100);
    }
}
#endif // preTaskDefined



