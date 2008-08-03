/* Name: main.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: main.c 602 2008-05-13 15:15:13Z cs $
 */

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
We assume that an LED is connected to port B bit 0. If you connect it to a
different port or bit, change the macros below:
*/
#define LED_PORT_DDR        DDRD
#define LED_PORT_OUTPUT     PORTD
#define LED_BIT_RED         6
#define LED_BIT_GREEN		7

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <string.h>

#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "requests.h"       /* The custom request numbers we use */
//#include "../common/console.h"

#include "rfm12.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

uint8_t usbtxlen = 0;
uint8_t txbuf[32];


usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

	if (rq->bRequest == CUSTOM_RQ_PUT_DATA)
	{
		switch (rq->wValue.bytes[0])
		{
			default://case USB_SENDCHAR: /* send a single character */
				txbuf[0] = rq->wIndex.bytes[0];
				rfm12_tx (1, 0, &txbuf);
				//rfm12_tx (sizeof(foobar), 0, &foobar);
				LED_PORT_OUTPUT ^= _BV(LED_BIT_RED);
			break;
		}

		#if 0
		if (rq->wValue.bytes[0] & 1)
		{
			LED_PORT_OUTPUT |= _BV(LED_BIT_RED);
		} else
		{                          /* clear LED */
			LED_PORT_OUTPUT &= ~_BV(LED_BIT_RED);
		}
		#endif
	} else if (rq->bRequest == CUSTOM_RQ_GET_DATA)
	{
		//LED_PORT_OUTPUT ^= _BV(LED_BIT);
		if (usbtxlen)
		{
			uint8_t tmp;
			tmp = usbtxlen;
			LED_PORT_OUTPUT ^= _BV(LED_BIT_RED);
			usbtxlen = 0;
			return tmp; /* tell the driver to send n bytes */
		} else
		{
			return 0;
		}
	}
	return 0;   /* default for not implemented requests: return no data back to host */
}


/* ------------------------------------------------------------------------- */

void init()
{
	uchar i;

	/* RESET status: all port bits are inputs without pull-up.
	* That's the way we need D+ and D-. Therefore we don't need any
	* additional hardware initialization.
	*/
	usbInit();

	usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
 	i = 0;
	while(--i)
	{             /* fake USB disconnect for > 250 ms */
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();

	//init rfm12
	rfm12_init();

	LED_PORT_DDR |= _BV(LED_BIT_RED) | _BV(LED_BIT_GREEN);   /* make the LED bit an output */
}


int main(void)
{
	//main initialization
	init();

	//enable ints
	sei();

	//power led on
	LED_PORT_OUTPUT |= _BV(LED_BIT_RED);

	while (42)
	{
		rfm12_tick();
		usbPoll();

		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			uint8_t buflen;

			LED_PORT_OUTPUT ^= _BV(LED_BIT_GREEN);

			buflen = rfm12_rx_len();
			usbMsgPtr = (void *) rfm12_rx_buffer();
			usbtxlen = buflen;
			/*
			m = fifo_put(&rx_fifo);
			if(m)
			{

				buflen = rfm12_rx_len();
				if (buflen > 30) buflen = 30;

				memcpy(m->data, rfm12_rx_buffer(), buflen);
				m->len = buflen;
				m->type = rfm12_rx_type();
			}
			*/
			rfm12_rx_clear();
			usbSetInterrupt(0, 0);  /* NULL message on interrupt socket */
		}

    }
    return 0;
}


void foo(){

	while(1){
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			//uart_putstr ("new packet:\r\n");

			//bufcontents = rfm12_rx_buffer();

			// dump buffer contents to uart
			//for (i=0;i<rfm12_rx_len();i++)
			{
				//uart_putc ( bufcontents[i] );
			}

			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();

		}


//		if (!(PINB & (_BV(PB0))) && (ticker % 3000 == 0))
//		{
//			uart_putc ('#');
//			rfm12_tx (sizeof(tv), 0, tv);
//		}

		rfm12_tick();
	}

}


/* ------------------------------------------------------------------------- */
