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

typedef struct{
	uint8_t type;
	uint8_t len;
	uint8_t data[30];
}airlab_message_t;

typedef struct{
	airlab_message_t messages[8];
	uint8_t in;
	uint8_t out;
}airlab_fifo_t;


airlab_message_t * fifo_get(airlab_fifo_t * f){
	if(f->out == f->in) return 0;
	airlab_message_t * p;
	p = &f->messages[f->out];
	f->out = (f->out+1) % 8;
	return p;
}

airlab_message_t * fifo_put(airlab_fifo_t * f){
	if(f->out - f->in == 2) return 0;
	airlab_message_t * p;
	p = &f->messages[f->in];
	f->in = (f->in+1) % 8;
	return p;
}


airlab_fifo_t rx_fifo;



usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;
    if(rq->bRequest == CUSTOM_RQ_PUT_DATA){
        if(rq->wValue.bytes[0] & 1){    /* set LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        }else{                          /* clear LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    }else if(rq->bRequest == CUSTOM_RQ_GET_DATA){
        airlab_message_t * m;
		m = fifo_get(&rx_fifo);
		if(m){
			usbMsgPtr = (void*) m;  /* tell the driver which data to return */
        	return m->len + 2; /* tell the driver to send n bytes */
		}else{
			return 0;
		}
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
			airlab_message_t * m;
			uint8_t buflen;

			m = fifo_put(&rx_fifo);
			if(m){
				buflen = rfm12_rx_len();
				if(buflen > 30) buflen = 30;
				
				memcpy(m->data, rfm12_rx_buffer(), buflen);
				m->len = buflen;
				m->type = rfm12_rx_type();
			}
			
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
