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

#include "motion.h"

#define RGB_DATA _BV(PD6)
#define RGB_CLK  _BV(PD7)
#define MAXCOLORVALUE (0x0FFF)
#define RGB_PORT PORTD
#define RGB_DDR  DDRD
#define BITSPERLAMP 12
#define LAMPS 50


Tuint16 const gammatable[] PROGMEM = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 23, 25, 28, 30, 33, 36, 39, 43, 46, 49, 53, 57, 61, 64, 69, 73, 77, 82, 86, 91, 96, 101, 106, 111, 116, 122, 128, 133, 139, 145, 151, 157, 164, 170, 177, 184, 191, 197, 205, 212, 219, 227, 234, 242, 250, 258, 266, 274, 283, 291, 300, 309, 317, 326, 336, 345, 354, 364, 373, 383, 393, 403, 413, 423, 434, 444, 455, 466, 477, 488, 499, 510, 522, 533, 545, 556, 568, 580, 593, 605, 617, 630, 642, 655, 668, 681, 694, 708, 721, 735, 748, 762, 776, 790, 804, 818, 833, 847, 862, 877, 892, 907, 922, 937, 953, 968, 984, 1000, 1016, 1032, 1048, 1064, 1081, 1097, 1114, 1131, 1148, 1165, 1182, 1199, 1217, 1234, 1252, 1270, 1288, 1306, 1324, 1342, 1361, 1379, 1398, 1417, 1436, 1455, 1474, 1494, 1513, 1533, 1552, 1572, 1592, 1612, 1632, 1653, 1673, 1694, 1715, 1735, 1756, 1777, 1799, 1820, 1841, 1863, 1885, 1907, 1929, 1951, 1973, 1995, 2018, 2040, 2063, 2086, 2109, 2132, 2155, 2179, 2202, 2226, 2250, 2273, 2297, 2322, 2346, 2370, 2395, 2419, 2444, 2469, 2494, 2519, 2544, 2570, 2595, 2621, 2647, 2672, 2698, 2725, 2751, 2777, 2804, 2830, 2857, 2884, 2911, 2938, 2965, 2993, 3020, 3048, 3076, 3104, 3132, 3160, 3188, 3217, 3245, 3274, 3303, 3331, 3360, 3390, 3419, 3448, 3478, 3507, 3537, 3567, 3597, 3627, 3658, 3688, 3719, 3749, 3780, 3811, 3842, 3873, 3905, 3936, 3968, 3999, 4031, 4063, 4095};




typedef struct {
	Tuint16 red;
	Tuint16 green;
	Tuint16 blue;
} element_t;
Tuint16 callnumber=0;

Tuint08 blinkmode=8;



#define G_LED _BV(PD1)
#define R_LED _BV(PD3)
#define B_LED _BV(PD4)
#define PORT_LED PORTD
#define DDR_LED DDRD

#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0

/*
	XXX Powercommander.h
*/
#define C_VIRT (0x02)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)
#define O_VIRT01 (0x01)
#define O_VIRT02 (0x02)
#define VIRT_VORTRAG (O_VIRT01)
#define VIRT_VORTRAG_PWM (O_VIRT02)


void read_sensor();

Tuint08 rgbled_stat=B_LED;
  /* 
     fix me - this is the static message for powercommander
     we need the includes hier
  */
Tuint08 light=0;
Tuint08 mode=0;
Tuint08 lastlight=0;

void appBoot(void)
{ 
  spi_init();
  can_init();
  RGB_DDR |= RGB_DATA | RGB_CLK;
  RGB_PORT |= RGB_DATA | RGB_CLK;

  // RGB-LED als ausgang
  DDR_LED |= R_LED | G_LED | B_LED;
  // Taster als eingang
  DDRB &= ~(_BV(PB0) | _BV(PB1));

  // pullups aktivieren
  PORTB |= (_BV(PB0) | _BV(PB1));

}

/*
  set as backgroundcolor
*/
static void makeBlue(element_t *point)
{
  point->red=0;
  point->green=0;
  point->blue=MAXCOLORVALUE;
}

static void makeGreen(element_t *point)
{
  point->red=0;
  point->green=MAXCOLORVALUE;
  point->blue=0;
}

static void makeRed(element_t *point)
{
  point->red=MAXCOLORVALUE;
  point->green=0;
  point->blue=0;
}


/*
  set as backgroundcolor
*/
static void makeWhite(element_t *point)
{
  point->red=MAXCOLORVALUE;
  point->green=MAXCOLORVALUE;
  point->blue=MAXCOLORVALUE;
}

/*
  set as backgroundcolor
*/
static void makeOFF(element_t *point)
{
  point->red=0;
  point->green=0;
  point->blue=0;
}

/*
  set as backgroundcolor
*/
static void makeGray(element_t *point)
{
  point->red=1024;
  point->green=1024;
  point->blue=1024;
}

/*
  push value to leds
*/
static void pushValue(Tuint16 red,Tuint16 green,Tuint16 blue)
{
  Tuint08 k;
  taskEnterGlobalCritical();
  for (k = 0 ; k< BITSPERLAMP; k++)
	{
		if ( ((blue >> (BITSPERLAMP-k-1)) & 1 ) ==1)
		{
			RGB_PORT |= RGB_DATA;
		} 
		else 
		{ 
			RGB_PORT &= ~RGB_DATA;
		}
		RGB_PORT |= RGB_CLK;
		RGB_PORT &= ~RGB_CLK;
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		if ( ((green >> (BITSPERLAMP-k-1)) & 1 ) ==1)
		{
			RGB_PORT |= RGB_DATA;
		} 
		else 
		{ 
			RGB_PORT &= ~RGB_DATA;
		}

		RGB_PORT |= RGB_CLK;
		RGB_PORT &= ~RGB_CLK;
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		if ( ((red >> (BITSPERLAMP-k-1)) & 1 ) == 1 )
		{
			RGB_PORT |= RGB_DATA;
		}
		else 
		{
			RGB_PORT &= ~RGB_DATA;
		}
		RGB_PORT |= RGB_CLK;
		RGB_PORT &= ~RGB_CLK;
	}
  taskExitGlobalCritical();
}

static void setWhite(element_t *point, Tuint16 pos)
{
  if (pos<25)
    switch (pos - (callnumber % 25)){
    case 2:
      point->red = MAXCOLORVALUE;
      point->green = MAXCOLORVALUE;
      point->blue= MAXCOLORVALUE;
      break;
    case 1:
      point->red = MAXCOLORVALUE;
      point->green = MAXCOLORVALUE;
      point->blue= MAXCOLORVALUE;
      break;
    case 0:
      point->red = MAXCOLORVALUE;
      point->green = MAXCOLORVALUE;
      point->blue= MAXCOLORVALUE;
      break;
    default:
      break;
  }
  if (pos>=25)
  switch (pos + (callnumber % 25)){
  case 49:
    point->red = MAXCOLORVALUE;
    point->green = MAXCOLORVALUE;
    point->blue= MAXCOLORVALUE;
    break;
  case 48:
    point->red = MAXCOLORVALUE;
    point->green = MAXCOLORVALUE;
    point->blue= MAXCOLORVALUE;
    break;
  case 47:
    point->red = MAXCOLORVALUE;
    point->green = MAXCOLORVALUE;
    point->blue= MAXCOLORVALUE;
    break;
  default:
    break;
  }

}

static void updateLEDs()
{
  Tuint08 i;
  //  taskEnterGlobalCritical();

  for (i = 0; i < 5; i++)
    {
      RGB_PORT |= RGB_DATA;
      RGB_PORT &= ~RGB_DATA;
    }
  //  taskExitGlobalCritical();
}

void can_user_cmd(can_message *rx_msg)
{
  if (rx_msg->port_dst == CANPORT)
    {
      blinkmode = rx_msg->data[0];
    }
	else if (rx_msg->port_dst == I2CTEMP)
		{
			read_sensor();
		}
}

#ifdef TWI_MTHREAD
i2c_message commblock = {0,0,0,{0,0,0,0,0,0,0,0}};
void twi_mhandler_read(i2c_message *indata)
{
	Tuint08 i=0;
	can_message dstrl = {0x25,0x00,0x10,0x10,0x08, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	dstrl.dlc=indata->dlc+1;
	dstrl.data[0]=indata->process;
	for (i=0; i < indata->dlc; i++)
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
	can_message dstrl = {0x25,0x00,0x10,0x10,0x06, {error,errdata->data[0],errdata->data[1],errdata->data[2],0x11,0x11}};
	can_transmit(&dstrl);
  
}

void init_sensor()
{
	if (TWIM_Init()==0)
	{
		can_message dstrl = {0x25,0x00,0x10,0x10,0x06, {0x33,0x33,0x33,0x33,0x33,0x33}};
		can_transmit(&dstrl);
	}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=SOFTWARE_POR;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}
	
	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.data[0]=ACCESS_CONFIG;
	commblock.data[1]=I2CDEFAULTCONFIG; 
	commblock.process=PROCESSI2CWRITE;

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while (commblock.process != PROCESSI2CNONE)
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

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=READ_TEMPERATURE;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.process=PROCESSI2CREAD;

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while (commblock.process != PROCESSI2CNONE)
	{
		taskDelayFromNow(10);
	}

}

#else

void init_sensor()
{
	// do nothing, twim is not defined
}

void read_sensor()
{
	// do nothing, twim is not defined
}

#endif // TWI_MTHREAD

#if (preTaskDefined(rgbled))
void appLoop_rgbled(void)
{
  while (true)
  {
		if ((rgbled_stat & R_LED) !=0)
		{
			PORT_LED |= R_LED;
		}
		else
		{
			PORT_LED &= ~R_LED;
		}
		if ((rgbled_stat & G_LED) !=0)
		{
			PORT_LED |= G_LED;
		}
		else
		{
			PORT_LED &= ~G_LED;
		}
		if ((rgbled_stat & B_LED) !=0)
		{
			PORT_LED |= B_LED;
		}
		else
		{
			PORT_LED &= ~B_LED;
		}
		taskDelayFromNow(100);
  }
}
#endif

#if (preTaskDefined(taster))
void appLoop_taster(void)
{
  can_message dstpower = {0x25,0x02,0x00,0x01,0x04, {0x00,0x00,0x00,0x00}};
  Tuint08 counter_0 = 0;
  Tuint08 clicked_0 = 0;
  Tuint08 held_0    = 0;
  Tuint08 last_held_0 = 0;
  
  while (true)
  {
		clicked_0=0;
		held_0=0;
		if (!(PINB & _BV(PB0))) 
		{
			counter_0++;
			if (counter_0 > HOLD_THRESHOLD)
			{
				held_0 = 1;
				counter_0 = HOLD_THRESHOLD;
			}
		} 
		else
		{
			if (counter_0 > CLICK_THRESHOLD)
			{
				if (counter_0 < HOLD_THRESHOLD)
				{
					clicked_0 = 1;
				}
			}
			counter_0 = 0;
		}
		
		if (clicked_0 == 1)
		{
			dstpower.data[0] = C_VIRT;
			dstpower.data[1] = VIRT_VORTRAG;
			dstpower.data[2] = F_SW_TOGGLE;
			if (rgbled_stat==R_LED)
			{
				rgbled_stat=G_LED;
			}
			else
			{
				rgbled_stat=R_LED;
			}
			can_transmit(&dstpower);
		}
		
		if (held_0)
		{
			dstpower.data[0] = C_VIRT;
			dstpower.data[1] = VIRT_VORTRAG_PWM;
			dstpower.data[2] = F_PWM_MOD;
			can_transmit(&dstpower);
		}
		else if (last_held_0)
		{
			dstpower.data[0] = C_VIRT;
			dstpower.data[1] = VIRT_VORTRAG_PWM;
			dstpower.data[2] = F_PWM_DIR;
			can_transmit(&dstpower);
		}
		
		last_held_0 = held_0;

    if ((PINB & _BV(PB1)) && (mode==0))
		{
			mode=1;
		}
    if (!(PINB & _BV(PB1)) && (mode==1))
		{
			mode=2;
		}
    if (mode==2)
		{
			if (blinkmode < 8)
			{
				blinkmode++;
			}
			else 
			{
				blinkmode = 0;
			}
			mode = 0;
		}
    taskDelayFromNow(100);
  }
}
#endif

#if (preTaskDefined(rundown))

void appLoop_rundown(void)
{ 
  Tuint08 i=0;
  Tuint08 start_r = 0;
  Tuint08 start_b = LAMPS;
  Tuint08 start_g = 25;
  Tuint16 count_r = 50 * 256;
  Tuint16 count_g = 50 * 256;
  Tuint16 count_b = 50 * 256;
  
  element_t myelement={0,0,0};
  //taskDelayFromNow(1000);
	init_sensor();
	while (true) {
		while (blinkmode == 0) {
			for (i = 0; i < LAMPS; i++) {
				makeBlue(&myelement);
				setWhite(&myelement, i);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			callnumber++;
			taskDelayFromNow(100);
		} 
		while (blinkmode == 1) {
			// call it stop
			taskDelayFromNow(100);
		}
		while (blinkmode == 2) {
			for (i = 0; i < LAMPS; i++) {
				makeWhite(&myelement);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(100);
		}
    while (blinkmode == 3) {
			for (i = 0; i < LAMPS; i++) {
				makeGray(&myelement);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(100);
		}
		while(blinkmode == 4) {
			for(i = 0; i < LAMPS; i++) {
				makeOFF(&myelement);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(100);
		}
		while (blinkmode == 5) {
      for (i = 0; i < LAMPS; i++) {
				makeGreen(&myelement);
				setWhite(&myelement, i);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			callnumber++;
			taskDelayFromNow(100);
		} 
		while (blinkmode == 6) {
			for (i = 0; i < LAMPS; i++) {
				makeRed(&myelement);
				setWhite(&myelement, i);
				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			callnumber++;
			taskDelayFromNow(100);
		} 

		while (blinkmode == 7) {
			for (i = 0; i < LAMPS; i++) {
				makeRed(&myelement);
				pushValue(myelement.red,myelement.green,myelement.blue);
			}
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(20);

      for (i = 0; i < LAMPS; i++) {
				makeGreen(&myelement);
				pushValue(myelement.red, myelement.green, myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(20);

      for (i = 0; i < LAMPS; i++) {
				makeBlue(&myelement);
				pushValue(myelement.red,myelement.green,myelement.blue);
			}
			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(20);
		}
		while (blinkmode == 8) {
    	Tuint16 lightblob[] = {0x03f, 0x009f, 0x00ff, 0x01ff, 0x03ff, 0x06ff, 0x0fff, 0x0fff, 0x06ff, 0x3ff, 0x01ff, 0x00ff, 0x009f, 0x003f};
    	#define lb_len 14

    	for (i = 0; i < LAMPS; i++) {
				myelement.red = 0x00;
				myelement.green = 0x00;
				myelement.blue = 0x00;

    		if ( (uint8_t)((LAMPS + i - start_r)%LAMPS) < lb_len)
					myelement.red = lightblob[(LAMPS + i - start_r)%LAMPS];

    		if ( (uint8_t)((LAMPS + i - start_g)%LAMPS) < lb_len)
					myelement.green = lightblob[(LAMPS + i - start_g)%LAMPS];

    		if ( (uint8_t)((LAMPS + i - start_b)%LAMPS) < lb_len)
					myelement.blue = lightblob[(LAMPS + i - start_b)%LAMPS];

				pushValue(myelement.red, myelement.green, myelement.blue);
			}
			
			count_r += 200;
			count_b += 150;
			count_g -= 175;

			count_r = (count_r + (LAMPS*256))%(LAMPS*256);
			count_b = (count_b + (LAMPS*256))%(LAMPS*256);
			count_g = (count_g + (LAMPS*256))%(LAMPS*256);
			
			start_r = count_r >> 8;
			start_b = count_b >> 8;
			start_g = count_g >> 8;

			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(70);
    }
    // zufällige farben an zufälligen stellen einfügen
    //while (blinkmode == 9) {
    //}
    // angereter pixel mit nachschwingen in eigenfrequenz
    //while (blinkmode == 10) {
    //}
  }
}

#endif




volatile static uint16_t adcval[2];
volatile static uint8_t mtd_state = MTD_STATE_ENABLED;
volatile static uint8_t val_new = 0;


Tuint16 filter (Tuint08 in_idx, Tuint16 in_val)
{
	static int32_t fval[2];
	static Tuint08 init_done = 0;
	int32_t delta[2];

	if (init_done != 0x01)
	{
		fval[in_idx] = in_val * 65536;
		init_done |= (0x01 << in_idx);
		return 0x00;
	}
	delta[in_idx] = ((in_val * 65536) - fval[in_idx]);
	fval[in_idx] += delta[in_idx] / 1024;
	return (abs(delta[in_idx] / 65536));
}


Tuint16 motion_check()
{
	Tuint08 i;
	Tuint16 tmp;

	
	for (i = 0; i < 2; i++)
	{
		if (!(val_new & (i + 1)))
			continue;
		
		val_new &= ~(i + 1);
		tmp = filter (i, adcval[i]);
		if (tmp > 28)
			return tmp;
	}
	return 0;
}


#if (preTaskDefined(motiond))

void appLoop_motiond(void)
{
  can_message bla = {0x25,0x00,0x00,0x00,0x01, {0x00}};
  Tuint16 foo;
	while (true)
	{
		motiond_tick();
		foo = motion_check();
		if (foo)
		{
			bla.data[0] = foo;
			can_transmit(&bla);
		}
		taskDelayFromNow(100);
	}
}
#endif

void motiond_tick()
{
	static Tuint08 mystate = 0;
	
	switch  (mystate)
	{
		case 0x00:
			ADMUX = (_BV(REFS0) | _BV(REFS1) | 0x02); // int. 2.56V ref, left adjust result, channel 1 adc2
			ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x01:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			val_new |= 0x01;
			adcval[0] = ADC;
			/* intentional fallthrough - reset (and don't increment) counter */
		default:
			mystate = 0;
			return;
	}
	mystate++;
}

Tuint16 get_adc (Tuint08 in_num)
{
	return adcval[in_num];
}

