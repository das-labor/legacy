/* -*- Mode: C; tab-width: 2 -*- */
/*
 * Femto OS v 0.91 - Copyright (C) 2008-2009 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

/**
 * This file is solely for demonstration purposes.
 *
 * The Hello World example is made to get you started and this is the file
 * you want to play with. Do not use other examples to that end, that will
 * be a disappointing experience (or a steep learning curve)
 *
 * Explanation on the website, point your browser at:
 * http://www.femtoos.org/code_start.html
 */

/* This this the only include needed in your code .*/
//#include <util/delay.h>
#include "femtoos_code.h"
#include "spi.h"
#include "can.h"
#include "twi_master.h"
#include "i2ctempsens.h"
#include "kuechenCAN.h"

#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)

#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0

#define I2CTEMPCANPORT (0x10)
/*
	XXX Powercommander.h
*/
#define C_SW  (0x00)
#define C_PWM (0x01)
#define O_SW00 (0x00)
#define O_PWM05 (0x05)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)

#define PWM_KUECHE   (O_PWM05)
#define SWL_KUECHE      (O_SW00)

//0x02 0x05 0x02 0x00
//0x02 0x00 0x03 0x00
Tuint08 light=0;
Tuint08 lastlight=0;
Tuint08 rgbled_stat=0;
Tuint08 rgb_panic=0;
Tuint08 lastpanic=0;
Tuint08 panic=0;

// ALARM in hex
can_message panic_msg = {0x23, 0x00, ALARMCANPORT, 0x00, 0x06, {0x41,0x4C,0x41,0x52,0x4D,0x00}};
//can_message light_on_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x01,0x00}};
//can_message light_off_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x00,0x00}};


void appBoot(void)
{ 

	/*
	** Initiate TWI Master Interface with bitrate of 100000 Hz
	*/
//	if (!TWIM_Init(100000))
//	{
//		while (1);
//	}
#ifdef CAN_THREAD
  spi_init();
  can_init();
#endif

	rgbled_stat=R_LED;
	//Kuechenlicht
	DDRC |=  R_LED | G_LED | B_LED; // output led taster
	// led power on!
	PORTC &= ~(G_LED | R_LED);
	PORTC |= B_LED;
	// button
	DDRB &= ~_BV(PB1);      // in
	PORTB |= _BV(PB1);      // pullup on


	// XXX PC5 and PC4 are i2c for temp sense



	// Alarmbutton
	DDRD |= _BV(PD5) | _BV(PD6) | _BV(PD7);         // output


	// led power on!
	PORTD &= ~(_BV(PD5) | _BV(PD6) | _BV(PD7));     // off


	//Button
	DDRC &= ~_BV(PC0);      // in
	PORTC |= _BV(PC0);      // pullup on
#ifdef TWI_MTHREAD
  TWIM_Init();
#endif

}

/*
	set as backgroundcolor
*/

#ifdef TWI_MTHREAD
i2c_message commblock = {0,0,0,{0,0,0,0,0,0,0,0}};
void twi_mhandler_read(i2c_message *indata)
{
	Tuint08 i=0;
	can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x08, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	dstrl.dlc=indata->dlc+1;
	dstrl.data[0]=indata->process;
	for(i=0;i<indata->dlc;i++)
		{
			dstrl.data[i+1]=indata->data[i];
		}
	
	can_transmit(&dstrl);
}
// postwrite
void twi_mhandler_write(i2c_message *outdata)
{
	// not needed it is postwrite
}
void twi_mhandler_error(Tuint08 error,i2c_message *errdata)
{
	//can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x06, {error,errdata->data[0],errdata->data[1],errdata->data[2],0x11,0x11}};
	//can_transmit(&dstrl);
  
}
#endif // TWI_MTHREAD

void init_sensor()
{
	if(TWIM_Init()==0)
		{
			can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x06, {0x33,0x33,0x33,0x33,0x33,0x33}};
			can_transmit(&dstrl);
			
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=SOFTWARE_POR;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}
	
	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.data[0]=ACCESS_CONFIG;
	commblock.data[1]=I2CDEFAULTCONFIG; 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

}
/* data: 0x9e 0x01 0x02 0x22 0x8c 0x00  */
/* data: 0x9e 0x01 0x02 0xaa 0x8c 0x00  */
/* data: 0x8c 0x00 0x01 0x00 0x00 0x00  */
/* data: 0x9e 0x01 0x02 0x51 0x00 0x00  */
void read_sensor()
{

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=STOP_CONVERT;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=READ_TEMPERATURE;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.process=PROCESSI2CREAD;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

}


#ifdef CAN_THREAD
/*
  dies funktion wird aufgerufen wenn das Packet
  an unser devid ging, wir uns also dafuer
  interressieren
*/
void can_user_cmd(can_message *rx_msg)
{
  if(rx_msg->port_dst == I2CTEMPCANPORT)
		{
			read_sensor();
		}
}

#ifdef CAN_SNIFFER
/*
  wenn wir can_sniffer anschalten, dann bekommen wir 
  wirklich alle nachrichten mit
*/
void can_sniffer(can_message *rx_msg)
{
  // not needed
}
#endif // CAN_SNIFFER
#endif // CAN_THREAD


#if (preTaskDefined(mainthread))

void appLoop_mainthread(void)
{ 
	can_message dstpower = {0x23,0x02,LIGHTCANPORT,0x01,0x04, {0x00,0x00,0x00,0x00}};ß
	Tuint08 counter_0;
	Tuint08 clicked_0 = 0;
	Tuint08 held_0    = 0;
	Tuint08 last_held_0;

	while(true)
		{
			// lampe
			clicked_0=0;
			held_0=0;
			if(!(PINB & _BV(PB1))) 
				{
					counter_0++;
					if(counter_0 > HOLD_THRESHOLD)
						{
							held_0 = 1;
							counter_0 = HOLD_THRESHOLD;
						}
				} 
			else
				{
					if(counter_0 > CLICK_THRESHOLD)
						{
							if(counter_0 < HOLD_THRESHOLD)
								{
									clicked_0 = 1;
								}
						}
					counter_0 = 0;
				}
			
			if(clicked_0 == 1)
				{
					dstpower.data[0] = C_SW;
					dstpower.data[1] = SWL_KUECHE;
					dstpower.data[2] = F_SW_TOGGLE;
					if(rgbled_stat==R_LED)
						{
							rgbled_stat=G_LED;
						}
					else
						{
							rgbled_stat=R_LED;
						}
					can_transmit(&dstpower);
				}
		
			if(held_0)
				{
					dstpower.data[0] = C_PWM;
					dstpower.data[1] = PWM_KUECHE;
					dstpower.data[2] = F_PWM_MOD;
					can_transmit(&dstpower);
				}
			else if(last_held_0)
				{
					dstpower.data[0] = C_PWM;
					dstpower.data[1] = PWM_KUECHE;
					dstpower.data[2] = F_PWM_DIR;
					can_transmit(&dstpower);
				}
		
			last_held_0 = held_0;

			//panic
			if((PINC & _BV(PC0)) && (panic==0))
				{
					panic=1;
				}
			if(!(PINC & _BV(PC0)) && (panic==1))
				{
					panic=2;
				}
			if(panic==2)
				{
					can_transmit(&panic_msg);
					if(lastpanic == 0)
						{
							rgb_panic=1;
							lastpanic=1;
						}
					// you can send panic as offen as you want
					panic=0;
				}


			taskDelayFromNow(100);
		}

}

#endif

#if (preTaskDefined(alarmt))

void appLoop_alarmt(void)
{ 
	while(true)
		{
			if(lastpanic==1)
				{
					// 10 sec blinken
					taskDelayFromNow(10000);
					rgb_panic=0;
					lastpanic=0;
				}
			taskDelayFromNow(100);
			
		}

}

#endif


#if (preTaskDefined(alarm))

void appLoop_alarm(void)
{ 
	while(true)
		{
			if(lastpanic==1)
				{
					if(rgb_panic == 1)
						{
							rgb_panic=2;
						}
					taskDelayFromNow(500);
					if(rgb_panic == 2)
						{
							rgb_panic=1;
						}
				}
			taskDelayFromNow(100);
		}

}

#endif


#if (preTaskDefined(leds))

void appLoop_leds(void)
{ 
	while(true)
		{
			if((rgbled_stat & R_LED) !=0)
				{
					PORTC |= R_LED;
				}
			else
				{
					PORTC &= ~R_LED;
				}
			if((rgbled_stat & G_LED) !=0)
				{
					PORTC |= G_LED;
				}
			else
				{
					PORTC &= ~G_LED;
				}
			if( (rgbled_stat & B_LED) !=0)
				{
					PORTC |= B_LED;
				}
			else
				{
					PORTC &= ~B_LED;
				}

			if( rgb_panic == 1 )
				{
					PORTD |= _BV(PD5);
					PORTD |= _BV(PD6);
					PORTD |= _BV(PD7);
				}
			if( rgb_panic == 2 )
				{
					PORTD &= ~_BV(PD5);
					PORTD &= ~_BV(PD6);
					PORTD &= ~_BV(PD7);
				}
			
			taskDelayFromNow(100);
		}
}

#endif



