/**********************************************************
 * Copyright(C) 2007 Jochen Roessner <jochen@lugrot.de>
 * Copyright(C) 2007, 2008 Stefan Siegl <stesie@brokenpipe.de>
 *
 * @author      Benedikt K.
 * @author      Juergen Eckert
 * @author    Ulrich Radig (mail@ulrichradig.de) www.ulrichradig.de 
 * @date        04.06.2007  

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/

#include <stdlib.h>

#include "../config.h"
#include "../uip/uip.h"
#include "../spi.h"
#include "rfm12.h"
#include "../net/rfm12_raw_net.h"
#include "../crypto/encrypt-llh.h"
#include "../syslog/syslog.h"


rfm12_status_t rfm12_status;

static volatile rfm12_index_t rfm12_index;
static volatile rfm12_index_t rfm12_txlen;

#ifndef TEENSY_SUPPORT
uint8_t rfm12_bandwidth = 5;
uint8_t rfm12_gain = 1;
uint8_t rfm12_drssi = 4;
#endif

SIGNAL(RFM12_INT_SIGNAL)
{
  uint8_t byte;
  if ((rfm12_trans(0x0000) & 0x8000) == 0)
    return;

  switch (rfm12_status) 
    {
    case RFM12_RX:
      byte = rfm12_trans(0xB000) & 0x00FF;

      if(rfm12_index ? (rfm12_index < RFM12_BUFFER_LEN)
	 : (!_uip_buf_lock
#ifdef TEENSY_SUPPORT
	    && byte == 0	/* ignore packet if higher len byte set */
#endif
	    ))
	{
	  _uip_buf_lock = 8;
	  rfm12_buf[rfm12_index ++] = byte;
#ifdef HAVE_RFM12_RX_PIN
	  PIN_SET(RFM12_RX_PIN);
#endif
	}
      else
	{
	  if (rfm12_index)
	    uip_buf_unlock ();	/* we already locked, therefore unlock */

	  rfm12_trans(0x8208);
          rfm12_status = RFM12_OFF;
	  rfm12_rxstart();
#ifdef HAVE_RFM12_RX_PIN
	  PIN_CLEAR(RFM12_RX_PIN);
#endif
	  return;
	}

      if(rfm12_index > 2
	 && rfm12_index > (rfm12_buf[1] + 1
#ifndef TEENSY_SUPPORT
			   + (rfm12_buf[0] << 8)
#endif
			   ))
	{
	  rfm12_trans(0x8208);
	  rfm12_status = RFM12_NEW;
	}
      break;

    case RFM12_TX:
    case RFM12_TX_PREAMBLE_1:
    case RFM12_TX_PREAMBLE_2:
    case RFM12_TX_DATAEND:
    case RFM12_TX_SUFFIX_1:
    case RFM12_TX_SUFFIX_2:
      rfm12_trans (0xB8AA);
      rfm12_status ++;
      break;

    case RFM12_TX_PREFIX_1:
      rfm12_trans(0xB82D);
      rfm12_status ++;
      break;

    case RFM12_TX_PREFIX_2:
      rfm12_trans(0xB8D4);
      rfm12_status ++;
      break;

    case RFM12_TX_SIZE_HI:
#ifdef TEENSY_SUPPORT
      rfm12_trans(0xB800);
#else
      rfm12_trans(0xB800 | ((rfm12_txlen & 0xFF00) >> 8));
#endif
      rfm12_status ++;
      break;

    case RFM12_TX_SIZE_LO:
      rfm12_trans(0xB800 | (rfm12_txlen & 0xFF));
      rfm12_status ++;
      break;

    case RFM12_TX_DATA:
      rfm12_trans(0xB800 | rfm12_data[rfm12_index ++]);

      if(rfm12_index >= rfm12_txlen)
	rfm12_status = RFM12_TX_DATAEND;

      break;

    case RFM12_TX_END:
      rfm12_status = RFM12_OFF;
#ifdef HAVE_RFM12_TX_PIN
      PIN_CLEAR(RFM12_TX_PIN);
#endif
      rfm12_trans(0x8208);	/* TX off */
      uip_buf_unlock();
      rfm12_rxstart();
      //break;

    case RFM12_OFF:
    case RFM12_NEW:
      rfm12_trans(0x0000);	/* clear interrupt flags in RFM12 */
    }
    if(rfm12_status >= RFM12_TX)
      _uip_buf_lock = 8;
}


unsigned short
rfm12_trans(unsigned short wert)
{	
  unsigned short werti = 0;
	
  PIN_CLEAR(SPI_CS_RFM12);
	
  /* spi clock down */
  _SPCR0 |= _BV(SPR0);

  werti = (spi_send ((0xFF00 & wert) >> 8) << 8);
  werti += spi_send (0x00ff & wert);

  /* spi clock high */
  _SPCR0 &= ~_BV(SPR0);

  PIN_SET(SPI_CS_RFM12);
  return werti;
}


void
rfm12_init(void)
{
  uint8_t i;

  for (i=0; i<10; i++)
    _delay_ms(10);		/* wait until POR done */

  rfm12_trans(0xC0E0);		/* AVR CLK: 10MHz */
  rfm12_trans(0x80D7);		/* Enable FIFO */
  rfm12_trans(0xC2AB);		/* Data Filter: internal */
  rfm12_trans(0xCA81);		/* Set FIFO mode */
  rfm12_trans(0xE000);		/* disable wakeuptimer */
  rfm12_trans(0xC800);		/* disable low duty cycle */
  rfm12_trans(0xC4F7);		/* AFC settings: autotuning: -10kHz...+7,5kHz */
  rfm12_trans(0x0000);
  
  rfm12_status = RFM12_OFF;
#ifdef HAVE_RFM12_TX_PIN
  DDR_CONFIG_OUT(RFM12_TX_PIN);
#endif
#ifdef HAVE_RFM12_RX_PIN
  DDR_CONFIG_OUT(RFM12_RX_PIN);
#endif

  rfm12_int_enable ();
}

/* Prologue/epilogue macros, disabling/enabling interrupts. 
   Be careful, these are not well suited to be used as if-blocks. */
#define rfm12_prologue()			\
  uint8_t sreg = SREG; cli();
#define rfm12_epilogue()			\
  SREG = sreg;


#ifndef TEENSY_SUPPORT
void
rfm12_setbandwidth(uint8_t bandwidth, uint8_t gain, uint8_t drssi)
{
  rfm12_bandwidth = bandwidth;
  rfm12_gain = gain;
  rfm12_drssi = drssi;

  rfm12_prologue ();
  rfm12_trans (0x9400 | ((bandwidth & 7) << 5)|((gain & 3) << 3) | (drssi & 7));
  rfm12_epilogue ();
}


void
rfm12_setfreq(unsigned short freq)
{	
  if (freq < 96)		/* 430,2400MHz */
    freq = 96;

  else if (freq > 3903)		/* 439,7575MHz */
    freq = 3903;

  rfm12_prologue ();
  rfm12_trans (0xA000 | freq);
  rfm12_epilogue ();
}


void
rfm12_setbaud(unsigned short baud)
{
  if (baud < 7)
    return;

  rfm12_prologue ();

  /* Baudrate = 344827,58621 / (R + 1) / (1 + CS * 7) */
  if (baud < 54)
    rfm12_trans(0xC680 | ((43104 / baud / 100) - 1));
  else
    rfm12_trans(0xC600 | ((344828UL / baud / 100) - 1));

  rfm12_epilogue ();
}


void
rfm12_setpower(uint8_t power, uint8_t mod)
{	
  rfm12_prologue ();
  rfm12_trans(0x9800|(power&7)|((mod&15)<<4));
  rfm12_epilogue ();
}
#endif


uint8_t
rfm12_rxstart(void)
{
  if(rfm12_status != RFM12_OFF){
    return(1);			/* rfm12 is not free for RX or now in RX */
  }

  rfm12_prologue ();

  rfm12_trans(0x82C8);		/* RX on */
  rfm12_trans(0xCA81);		/* set FIFO mode */
  rfm12_trans(0xCA83);		/* enable FIFO */

  rfm12_epilogue ();

  rfm12_index = 0;
  rfm12_status = RFM12_RX;

  return(0);
}


rfm12_index_t
rfm12_rxfinish(void)
{
  if(rfm12_status != RFM12_NEW)
    return (0);			/* no new Packet */

#ifdef HAVE_RFM12_RX_PIN
  PIN_CLEAR(RFM12_RX_PIN);
#endif

  rfm12_index_t len = rfm12_buf[1];
#ifndef TEENSY_SUPPORT
  len += rfm12_buf[0] << 8;
#endif
  
  rfm12_status = RFM12_OFF;

#ifdef RFM12_RAW_SUPPORT
  if (!rfm12_raw_conn->rport)
#endif
  {
#ifdef SKIPJACK_SUPPORT
    rfm12_decrypt (&len);
#endif
  }
  
  if (!len) {
    uip_buf_unlock ();
    rfm12_rxstart ();		/* rfm12_decrypt destroyed the packet. */
  }
  
  return(len);			/* receive size */
}


void
rfm12_txstart(rfm12_index_t size)
{
  if(rfm12_status > RFM12_RX
     || (rfm12_status == RFM12_RX && rfm12_index > 0)) {
    return;			/* rx or tx in action or
				   new packet left in buffer */
  }

  rfm12_status = RFM12_TX;

#ifdef HAVE_RFM12_TX_PIN
  PIN_SET(RFM12_TX_PIN);
#endif

  rfm12_index = 0;

#ifdef RFM12_RAW_SUPPORT
  if (!rfm12_raw_conn->rport)
#endif
  {
#ifdef SKIPJACK_SUPPORT
    rfm12_encrypt (rfm12_data, &size);

    if (!size){
      rfm12_status = RFM12_OFF;
      uip_buf_unlock ();
      rfm12_rxstart ();		/* destroy the packet and restart rx */
      return;
    }
#endif
  }
  rfm12_txlen = size;

  rfm12_prologue ();
  rfm12_trans(0x8238);		/* TX on */
  rfm12_epilogue ();

  /* Force interrupts active no matter what.

     If we're forwarding a packet from say Ethernet, uip_buf_unlock won't
     unlock since there's an active RFM12 transfer, but it'd leave
     the RFM12 interrupt disabled as well.*/
  rfm12_int_enable ();

  return;
}


uint16_t
rfm12_get_status (void)
{
  uint16_t r;

  rfm12_prologue ();
  r = rfm12_trans(0x0000);	/* read status word */
  rfm12_epilogue ();

  return r;
}
