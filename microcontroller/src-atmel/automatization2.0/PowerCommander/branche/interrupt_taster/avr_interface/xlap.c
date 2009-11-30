/* -*- Mode: C; tab-width: 2 -*- */
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
#include "queue.h"
#include "PowerCommander.h"

#define SLAVE 15

uint8_t myaddr;

static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};

TimerControlBlock   i2cResponseTimer;             // Declare the control blocks needed for timers


void process_mgt_msg()
{
	/*
		watchdog ausschalten um hardware-reset zu enforcen
		aka der chip bekommt keinen watchdog mehr und
		macht selbst reset
	*/
	switch (rx_msg.data[0])
	{
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
			break;
		case FKT_MGT_PING:
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
		default:
			break;
	}
}


/*
	ein task der nur daten vom i2c-slave abholt - es ist jetzt 
	nicht eingebaut, dass unterschiedlich viele Daten abgeholt
	werden, sondern es wird immer die gesamte Arraygroesse
	aus edm i2c-bus gezogen
*/
AVRX_GCC_TASKDEF(i2ccom_in, 50, 3)
{
	MessageControlBlock *p;
	uint8_t i = 0; // keep compiler happy
	static t_canMessage_out can_outdata;
	
	while (1)
	{
		/*
			Es ist notwendig daten von i2c zu lesen
		*/
		p = AvrXWaitMessage(&i2cQueue_in);
		AvrXDelay(&i2cResponseTimer, 10); // 10ms auf den ctrl warten bevor wir lesen
		if (!TWIM_Start (SLAVE, TWIM_READ))
		{
			TWIM_Stop();
			for (i = 0; i < CAN_OUTDATACOUNT; i++)
				{
					can_outdata.outdata[i] = 0xff;
				}
		}
		else
		{
			/*
				die ersten n-1 Daten lesen wir via
				ReadAck
			*/
			for (i = 0; i < (I2C_INDATACOUNT - 1); i++)
			{
				((t_i2cMessage_in*)p)->indata[i] = TWIM_ReadAck();
			}
			/*
				die letzte via ReadNack
			*/
			((t_i2cMessage_in*)p)->indata[i] = TWIM_ReadNack();
			/*
				i2c aus
			*/
			TWIM_Stop();

		/*
			daten wurden gelesen und wandern in die can_send_queue
			wir gehen dabei davon aus, dass die can-daten genau 
			so gross sind wie die i2c-daten, resp es genau so viele sind
		*/

			for (i = 0; i < CAN_OUTDATACOUNT; i++)
			{
				can_outdata.outdata[i] = ((t_i2cMessage_in*)p)->indata[i];
			}
		}	
		AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
		AvrXWaitMessageAck(&can_outdata.mcb);

		AvrXAckMessage(p);
				 
	}
}

/*
	ein Task der Daten via i2c an einen i2c-slave sendet
	dieser Task wird aktiv, wenn es in der i2cQueue_out
	ein Datenobjekt gibt
*/
AVRX_GCC_TASKDEF(i2ccom_out, 50, 3)
{
	MessageControlBlock *p;
	uint8_t i = 0; // keep compiler happy
	static t_i2cMessage_in i2c_indata;
	
	while (1)
	{
		/*
			wir warten auf ein objekt in der queue
		*/
		p = AvrXWaitMessage(&i2cQueue_out); 
		/*
			wenn es da versuchen wir die daten an den ctrl zu senden
		*/
		if (!TWIM_Start(SLAVE, TWIM_WRITE))
		{
			TWIM_Stop();
		}
		else
		{
			/*
				daten wirklich raus schreiben
			*/
			for (i = 0; i < I2C_OUTDATACOUNT; i++)
			{
				TWIM_Write(((t_i2cMessage_out*)p)->outdata[i]);
			}
			TWIM_Stop();
		}

		/*
			wann sollen wirklich daten gelesen werden - erstmal 
			immer
		*/


		/*
			wir schmeissen daten in die i2cQueue_in
			und warten darauf, dass sie von dort entfernt
			werden - aka via can wo anders hin gesendet werden oder so
		*/
		if (((((t_i2cMessage_out*)p)->outdata[0] == C_SW) && (((t_i2cMessage_out*)p)->outdata[2] == F_SW_STATUS)) ||
		   ((((t_i2cMessage_out*)p)->outdata[0] == C_PWM) && (((t_i2cMessage_out*)p)->outdata[2] == F_PWM_GET)))
		{
			AvrXSendMessage(&i2cQueue_in, &i2c_indata.mcb);
			AvrXWaitMessageAck(&i2c_indata.mcb);
		}
		// else eben nicht
				
		// final dann selber fertig sagen
		AvrXAckMessage(p);
	}
}

/*
	task der alle nachrichten die via can rein kommen
	behandelt. Primaer ist das hier wohl das 
	Abarbeiten der mgt-funktionen und das weiterleiten
	der can-nachrichten an den i2c 
	XXX - hier ist der punkt wo man ntp einbauen sollte
*/
AVRX_GCC_TASKDEF(cancom_in, 50, 3)
{
	static t_i2cMessage_out i2c_outdata;
	uint8_t i = 0;
	while (1)
	{
		can_get(); //get next canmessage in rx_msg
		if (rx_msg.addr_dst == myaddr)
		{
			switch (rx_msg.port_dst)
			{
				case PORT_MGT:
				{
					process_mgt_msg();
				}
				break;
				case PORT_POWERCOMMANDER:
				{
					/*
						unterbinden, dass ueber can ein paar sachen umgelegt werden koennen
					*/
					if ( ( (rx_msg.data[0] == C_VIRT) &&	(rx_msg.data[1] == VIRT_POWER)) ||
							( (rx_msg.data[0] == C_SW) &&
								( (rx_msg.data[1] == SWA_HS) ||
									(rx_msg.data[1] == SWA_STECKDOSEN) ||
									(rx_msg.data[1] == SWA_KLO))))
						break;

					/*
						gehe davon aus, dass genau so viele daten die
						via can reingekommen sind auch wieder auf i2c raus sollen
									
						XXX - check mit rx_msg->cmd und data[i] was steht wirklich wo
					*/
					for (i = 0; i < I2C_OUTDATACOUNT; i++)
					{
						i2c_outdata.outdata[i] = rx_msg.data[i];
					}
					/*
						in die queue rein
					*/
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					/*
						warten bis sie fertig ist
					*/
					//					AvrXWaitMessageAck(&i2c_outdata.mcb);
								
				}
				break;
				default:
				break;
			}
		}
	}
}


AVRX_GCC_TASKDEF(cancom_out, 50, 3)
{
	MessageControlBlock *p;
	uint8_t i = 0; // keep compiler happy
	while (1)
	{
		p = AvrXWaitMessage(&canQueue_out);
		for (i = 0; i < CAN_OUTDATACOUNT; i++)
		{
				msg.data[i] = ((t_canMessage_out*)p)->outdata[i];
		}
		msg.port_dst = PORT_POWERCOMMANDER;
		msg.addr_dst = rx_msg.addr_src;
		msg.port_src = PORT_POWERCOMMANDER;
		can_put(&msg);
		AvrXAckMessage(p);
	}
}
	

void xlap_init()
{
	myaddr = eeprom_read_byte(0x00);
	msg.addr_src = myaddr;
	spi_init();
	can_init();
}

