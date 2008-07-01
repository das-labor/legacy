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
#define LED_BIT             6

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <string.h>

#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "requests.h"       /* The custom request numbers we use */

#include "rfm12.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

uint8_t buflen;
uint8_t buffer[20];

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;
static uchar    dataBuffer[4];  /* buffer must stay valid when usbFunctionSetup returns */

    if(rq->bRequest == CUSTOM_RQ_ECHO){ /* echo -- used for reliability tests */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 4;
    }else if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
        if(rq->wValue.bytes[0] & 1){    /* set LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        }else{                          /* clear LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    }else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
        usbMsgPtr = buffer;         /* tell the driver which data to return */
        return buflen;                       /* tell the driver to send n bytes */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
	rfm12_init();
    
	usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
 	i = 0;
	while(--i){             /* fake USB disconnect for > 250 ms */
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();
    LED_PORT_DDR |= _BV(LED_BIT);   /* make the LED bit an output */
    sei();

	for(;;){                /* main event loop */
        usbPoll();
		rfm12_tick();
		
		if (rfm12_rx_status() == STATUS_COMPLETE){
			uint8_t * bufcontents;
			bufcontents = rfm12_rx_buffer();

			buflen = rfm12_rx_len();
			if (buflen>20) buflen = 20;
			memcpy (buffer, bufcontents, buflen);
			
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
