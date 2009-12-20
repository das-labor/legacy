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

#define DATA PD6
#define CLK  PD7
#define MAXCOLORVALUE (0x0FFF)
#define OUTPORT PORTD
#define BITSPERLAMP 12
#define LAMPS 50


typedef struct {
	Tuint16 red;
	Tuint16 green;
	Tuint16 blue;
} element_t;
Tuint16 callnumber=0;

Tuint08 blinkmode=0;

#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)

#define RGBCANPORT 0x10
Tuint08 rgbled_stat=0;


void appBoot(void)
{ 
  spi_init();
  can_init();
  DDRD |= _BV(DATA) | _BV(CLK);
  PORTD |= _BV(DATA) | _BV(CLK);

  // RGB-LED als ausgang
  DDRC |= _BV(PC1) | _BV(PC2) | _BV(PC3);
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
      if( ((blue >> (BITSPERLAMP-k-1)) & 1 ) ==1)
	{
	  OUTPORT |= _BV(DATA);
	} 
      else 
	{ 
	  OUTPORT &= ~_BV(DATA);
	}
      PORTD |= _BV(CLK);
      PORTD &= ~_BV(CLK);
    }
  for (k = 0; k < BITSPERLAMP; k++)
    {
      if( ((green >> (BITSPERLAMP-k-1)) & 1 ) ==1)
	{
	  OUTPORT |= _BV(DATA);
	} 
      else 
	{ 
	  OUTPORT &= ~_BV(DATA);
	}
      
      PORTD |= _BV(CLK);
      PORTD &= ~_BV(CLK);
    }
  for (k = 0; k < BITSPERLAMP; k++)
    {
      if ( ((red >> (BITSPERLAMP-k-1)) & 1 ) == 1 )
	{
	  OUTPORT |= _BV(DATA);
	}
      else 
	{
	  OUTPORT &= ~_BV(DATA);
	}
      PORTD |= _BV(CLK);
      PORTD &= ~_BV(CLK);
    }
  taskExitGlobalCritical();

}

static void setWhite(element_t *point, Tuint16 pos)
{
  if(pos<25)
    switch(pos - (callnumber % 25)){
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
  if(pos>=25)
  switch(pos + (callnumber % 25)){
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
      PORTD |= _BV(DATA);
      PORTD &= ~_BV(DATA);
    }
  //  taskExitGlobalCritical();
}

void can_user_cmd(can_message *rx_msg)
{
  if (rx_msg->port_dst == CANPORT)
    {
      blinkmode = rx_msg->data[0];
    }
  else if (rx_msg->port_dst == RGBCANPORT)
    {
      rgbled_stat=rx_msg->data[0];
    }
}


#if (preTaskDefined(rgbled))
void appLoop_rgbled(void)
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
      taskDelayFromNow(100);
    }
}
#endif

#if (preTaskDefined(taster))
void appLoop_taster(void)
{
  static Tuint08 light=0;
  static Tuint08 mode=0;
  static Tuint08 lastlight=0;
  /* 
     fix me - this is the static message for powercommander
     we need the includes hier
  */
  static can_message msg = {0x00, 0x00, 0x02, 0x01, 0x04, {0x02,0x01,0x03,0x00}};
  
  while(true)
    {
      if((PORTB & _BV(PB0)) != 0 && (light==0))
	{
	  light=1;
	}
      if((!(PORTB & _BV(PB0)) != 0) && (light==1))
	{
	  light=2;
	}
      if(light==2)
	{
	  can_transmit(&msg);
	  if(lastlight == 0)
	    {
	      rgbled_stat=G_LED;
	      lastlight=1;
	    }
	  else 
	    {
	      rgbled_stat=R_LED;
	      lastlight=0;
	    }
	  light=0;
	}

      if((PORTB & _BV(PB1)) != 0 && (mode==0))
	{
	  mode=1;
	}
      if((!(PORTB & _BV(PB0)) != 0) && (mode==1))
	{
	  mode=2;
	}
      if(mode==2)
	{
	  if(blinkmode <= 6)
	    {
	      blinkmode++;
	    }
	  else 
	    {
	      blinkmode=0;
	    }
	  mode=0;
	}

      taskDelayFromNow(100);
    }
}
#endif

#if (preTaskDefined(rundown))

void appLoop_rundown(void)
{ 
  Tuint08 i=0;
  element_t myelement={0,0,0};
  taskDelayFromNow(1000);
  while (true)
  { 
    while(blinkmode == 0){
      for(i=0;i<LAMPS;i++){
	makeBlue(&myelement);
	setWhite(&myelement,i);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      callnumber++;
      taskDelayFromNow(100);
    } 
    while(blinkmode == 1){
      // call it stop
      taskDelayFromNow(100);
    }
    while(blinkmode == 2){
      for(i=0;i<LAMPS;i++){
	makeWhite(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(100);
    }
    while(blinkmode == 3){
      for(i=0;i<LAMPS;i++){
	makeGray(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(100);
    }
    while(blinkmode == 4){
      for(i=0;i<LAMPS;i++){
	makeOFF(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(100);
    }
    while(blinkmode == 5){
      for(i=0;i<LAMPS;i++){
	makeGreen(&myelement);
	setWhite(&myelement,i);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      callnumber++;
      taskDelayFromNow(100);
    } 
    while(blinkmode == 6){
      for(i=0;i<LAMPS;i++){
	makeRed(&myelement);
	setWhite(&myelement,i);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      callnumber++;
      taskDelayFromNow(100);
    } 

    while(blinkmode == 7){
      for(i=0;i<LAMPS;i++){
	makeRed(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(20);

      for(i=0;i<LAMPS;i++){
	makeGreen(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(20);

      for(i=0;i<LAMPS;i++){
	makeBlue(&myelement);
	pushValue(myelement.red,myelement.green,myelement.blue);
      }
      taskDelayFromNow(1);
      updateLEDs();
      taskDelayFromNow(20);
    } 
    
  }
}

#endif



