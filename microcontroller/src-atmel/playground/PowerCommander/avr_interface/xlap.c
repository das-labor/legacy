
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include <string.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"
#include "twi_master/twi_master.h"

#define SLAVE 15

uint8_t myaddr;
TimerControlBlock switchtimer;
static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	
void process_mgt_msg()
{
	switch (rx_msg.data[0])
	{
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
	}
}

AVRX_GCC_TASKDEF(laptask, 50, 3)
{
    uint8_t tmp;
    while (1)
    {
    can_get();			//get next canmessage in rx_msg
    if (rx_msg.addr_dst == myaddr)
    {
        if (rx_msg.port_dst == PORT_MGT)
        {
            process_mgt_msg();
            } else
            if (rx_msg.port_dst == 0x01)
            {
                if (!TWIM_Start(SLAVE, TWIM_WRITE))
                {
                    TWIM_Stop();
                }
                else
                {
                    TWIM_Write(rx_msg.data[0]);
                    TWIM_Write(rx_msg.data[1]);
                    TWIM_Write(rx_msg.data[2]);                        
                    TWIM_Stop();
                    
                    if (rx_msg.data[0] == 0x00 && rx_msg.data[1] == 0x02)
                    {
                        _delay_ms(10);
                        if (!TWIM_Start (SLAVE, TWIM_READ))
                        {
                            TWIM_Stop();
                        }
                        else
                        {
                            tmp = TWIM_ReadNack();
                            TWIM_Stop();
                        }
                        msg.port_dst = 0x01;
                        msg.addr_dst = rx_msg.addr_src;
                        msg.data[0] = rx_msg.data[tmp];
                        can_put(&msg);
                    }
                    if (rx_msg.data[0] == 0x0a && rx_msg.data[1] == 0x01)
                    {
                    	_delay_ms(10);
                        if (!TWIM_Start (SLAVE, TWIM_READ))
                        {
                            TWIM_Stop();
                        }
                        else
                        {
                            tmp = TWIM_ReadNack();
                            TWIM_Stop();
                        }
                        msg.port_dst = 0x00;
                        msg.addr_dst = rx_msg.addr_src;
                        msg.data[0] = rx_msg.data[tmp];
                        can_put(&msg);
                    }
                }
            }
        }
    }
};

void xlap_init() {
	myaddr = eeprom_read_byte(0x00);
	msg.addr_src = myaddr;
	spi_init();
	can_init();
}

