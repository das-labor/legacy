/* This is the firmware for the RFMUSB device (USB handling functions).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Copyright (C) 2013 Soeren Heisrath (forename at surename dot org)
 */
#include "usbstuff.h"

static uint8_t usb_rx_cnt = 0;                    /* usb rx c(o)unt */
static uint8_t usb_rx_target_cnt = 0;             /* amount of bytes to be received */
static uint8_t rfm12usb_mode = MODE_IDLE;         /* current mode */
static uint8_t usb_buf_state = BUFSTATE_IDLE;     /* state of the usb buffer */
static uint8_t usb_buf[RFMUSB_USB_BUF_SIZE];      /* buffer for data from/to host */
static uint8_t usb_buf_len = 0;                   /* fill state of usb buffer */

ISR (TIMER0_COMPA_vect)
{
	TCNT0 = 0;
	usbPoll();
}

void usbstuff_init ()
{
	DEBUG_LED(1);
	usbInit();
	usbDeviceDisconnect();
	_delay_ms (250);
	_delay_ms (250);
	usbDeviceConnect();
	
	TCNT0 = 0;
	TCCR0B = (_BV(CS02) | _BV(CS00)); /* clk/1024 */
	OCR0A = 190;                      /* approx. 9.8ms */
	TIMSK0 = _BV(OCIE0A);             /* */


	DEBUG_LED(0);
}

/* handle host requests - mostly like the old handler
 */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

	USB_LED(1);

	switch(rq->bRequest)
	{
		/* host -> rfm12 */
		case RFMUSB_RQ_OOK_SEND:    /* ook */
		case RFMUSB_RQ_RADIO_PUT:   /* data packet */
			usb_rx_cnt = 0;
			usb_rx_target_cnt = rq->wLength.bytes[0];
			/* note: this will terminate any ongoing receiption */
			if (rq->bRequest == RFMUSB_RQ_RADIO_PUT)
			{
				rfm12usb_mode = MODE_TX;
			} else
			{
				rfm12usb_mode = MODE_OOK_TX;
			}

			if(usb_rx_target_cnt > RFMUSB_USB_BUF_SIZE)
				usb_rx_target_cnt = RFMUSB_USB_BUF_SIZE;

			/* use usbFunctionWrite() */
			return USB_NO_MSG;

		/* rfm12 -> host (polling) */
		case RFMUSB_RQ_RADIO_GET:
			if (usb_buf_state == BUFSTATE_RX)
			{
				uint8_t tmp;
				
				usbMsgPtr = (usbMsgPtr_t) usb_buf;

				tmp = usb_buf_len;
				usb_buf_len = 0;
				usb_buf_state = BUFSTATE_IDLE;

				/* send tmp bytes to host */
				USB_LED(0);
				return tmp;
			}
		break;

		case RFMUSB_RQ_NOTIFY_GET:
			/* TODO: debug printing */
			break;

		/* rfm12 configuration */
		case RFMUSB_RQ_RADIO_CFG:
			if (rq->wIndex.bytes[0] >= NUM_LIVECTRL_CMDS)
				break;

			rfm12_livectrl (rq->wIndex.bytes[0], rq->wValue.word);
			break;
		

		default:
			break;
	}

	USB_LED(0);
	return 0;   /* default for not implemented requests: return no data back to host */

}

/* host -> rfm12 transmission */
uchar usbFunctionWrite(uchar *data, uchar len)
{
	uint8_t i;

	if (usb_rx_target_cnt < len)
		len = usb_rx_target_cnt;
	
	usb_rx_target_cnt -= len;

	for (i=0; i<RFMUSB_USB_BUF_SIZE; i++)
	{
		usb_buf[usb_rx_cnt++] = data[i];
	}

	if (usb_rx_target_cnt > 0)
		return 0; /* we haven't got all data of the current packet */
	
	/* fire! */
	TX_LED(1);
	
	if (rfm12usb_mode == MODE_TX)
	{
		rfm12_tx (usb_buf[0], usb_buf[1], &usb_buf[2]);
	} else if (rfm12usb_mode == MODE_OOK_TX)
	{
		ook_queue_msg (usb_buf);
	}
	usb_buf_state = BUFSTATE_IDLE;
	rfm12usb_mode = MODE_IDLE;

	TX_LED(0);
	USB_LED(0);

	return 1;
}

/* rfm12 -> host, polling mode */
void handle_rx ()
{
	uint8_t len;

	if (rfm12_rx_status() != STATUS_COMPLETE)
		return;
	
	if (usb_buf_state != BUFSTATE_IDLE)
		return;
	
	RX_LED(1);
	rfm12usb_mode = MODE_RX;

	len = rfm12_rx_len();

#if RFM12_RX_BUFFER_SIZE > RFMUSB_USB_BUF_SIZE
	/* bounds checking (only makes sense when usb buffer is smaller than that 
	 * of the rfm12lib)
	 */
	if (len > RFMUSB_USB_BUF_SIZE)
		len = RFMUSB_USB_BUF_SIZE;
#endif

	memcpy (&usb_buf[2], (uint8_t *) rfm12_rx_buffer(), len);
	usb_buf[0] = len;
	usb_buf[1] = rfm12_rx_type();
	usb_buf_len = len + 2;
	
	/* let usbFunctionSetup() know we've got data */
	usb_buf_state = BUFSTATE_RX;

	rfm12_rx_clear();

	RX_LED(0);
}
