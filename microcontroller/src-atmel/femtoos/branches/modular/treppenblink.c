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

#ifdef CAN_THREAD
#include "spi.h"
#include "can.h"
#endif // CAN_THREAD

#ifdef TWI_MTHREAD
#include "twi_master.h"
#endif //TWI_MTHREAD

#define DATA PD6
#define CLK  PD7
#define MAXCOLORVALUE (0x0FFF)
#define OUTPORT PORTD
#define BITSPERLAMP 12
#define LAMPS 50


Tuint08 blinkmode=0;


/* 
   diese Funktion wird beim starten des 
   AVRs genau einmal ausgefuehrt
*/
void appBoot(void)
{ 
  /* 
     wenn in der Config CAN_THREAD definiert ist, 
     dann initialisiere ihn 
  */
#ifdef CAN_THREAD
  spi_init();
  can_init();
#endif
  /*
    wenn TWI_MTHREAD in der config aktiv ist,
    dann initialisiere ihn hier
  */
#ifdef TWI_MTHREAD
  TWIM_Init();
#endif
}

#ifdef CAN_THREAD
/*
  dies funktion wird aufgerufen wenn das Packet
  an unser devid ging, wir uns also dafuer
  interressieren
*/
void can_user_cmd(can_message *rx_msg)
{
  blinkmode = rx_msg->data[0];
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

#ifdef TWI_MTHREAD
i2c_message commblock = {0,0,0,{0,0,0,0,0,0,0,0}};
void twi_mhandler_read(i2c_message *data)
{
  
}
void twi_mhandler_write(i2c_message *data)
{
  
}
void twi_mhandler_error(Tuint08 error,i2c_message *data)
{
  
}
#endif // TWI_MTHREAD



#if (preTaskDefined(mainthread))

void appLoop_mainthread(void)
{ 
  while (true)
  { 
    taskDelayFromNow(1000);
  }
}

#endif



