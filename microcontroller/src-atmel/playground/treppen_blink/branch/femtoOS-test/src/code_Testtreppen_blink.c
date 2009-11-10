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

void appBoot(void)
{ 
  devSwitchDRR |= _BV(DATA) | _BV(CLK);
  devSwitchPORT |= _BV(DATA) | _BV(CLK);
}

/*
  set as backgroundcolor
*/
static void makeBackground(element_t *point)
{
  point->red=0;
  point->green=0;
  point->blue=MAXCOLORVALUE;
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
      OUTPORT = ( ( (blue >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
      PORTD |= _BV(CLK);
      PORTD &= ~_BV(CLK);
    }
  for (k = 0; k < BITSPERLAMP; k++)
    {
      OUTPORT = ( ( (green >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
      PORTD |= _BV(CLK);
      PORTD &= ~_BV(CLK);
    }
  for (k = 0; k < BITSPERLAMP; k++)
    {
      OUTPORT = ( ( (red >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
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
  taskEnterGlobalCritical();

  for (i = 0; i < 5; i++)
    {
      PORTD |= _BV(DATA);
      PORTD &= ~_BV(DATA);
    }
  taskExitGlobalCritical();
}

#if (preTaskDefined(rundown))

void appLoop_rundown(void)
{ 
  Tuint08 i=0;
  element_t myelement={0,0,0};

  while (true)
  { 
    
    for(i=0;i<LAMPS;i++){
      makeBackground(&myelement);
      setWhite(&myelement,i);
      pushValue(myelement.red,myelement.green,myelement.blue);
    }
    taskDelayFromNow(1);
    updateLEDs();
    callnumber++;
    taskDelayFromNow(100);
  }
}

#endif



