#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "config.h"
#include "canlib/spi.h"
#include "canlib/can.h"

#include "uart/uart.h"
#include "usbdrv/usbdrv.h"
#include "requests.h"


typedef enum {
	RS232CAN_RESET=0x00,
	RS232CAN_SETFILTER=0x10,
	RS232CAN_PKT=0x11,
	RS232CAN_SETMODE=0x12,
	RS232CAN_ERROR=0x13,
	RS232CAN_NOTIFY_TX_OVF=0x1F
} rs232can_cmd;

#define RS232CAN_MAXLENGTH 20

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;


/*
#define LED_STATUS_TOGGLE \
	PORTB ^= 0x01
*/

#define RFMUSB_USBTXBUFFER_SIZE 20
#define RFMUSB_USBRXBUFFER_SIZE 20
//size of the notify buffer
#define RFMUSB_NOTIFYBUFFER_SIZE 7

//and its header size
#define RFMUSB_NOTIFYBUFFER_OVERHEAD 1


////////
// notify buffer states
////////

#define USBRFM_NOTIFYBUF_FREE 0
#define USBRFM_NOTIFYBUF_OCCUPIED 1


////////
// notify buffer data structures
////////

typedef struct
{
	uint8_t notifyType;
	uchar data[RFMUSB_NOTIFYBUFFER_SIZE];
	uint8_t len;
} rfmusb_notifyPacket;


////////
// global variables
////////

//notify buffer
rfmusb_notifyPacket rfmusb_notifyBuf;

//buffer for transmitting usb data to the host
uint8_t rfmusb_usbTxLen = 0;
uint8_t rfmusb_usbTxBuf[RFMUSB_USBTXBUFFER_SIZE];

//buffer for receiving usb data from the host
uint8_t rfmusb_usbRxLen = 0, rfmusb_usbRxCnt = 0;
uint8_t rfmusb_usbRxBuf[RFMUSB_USBRXBUFFER_SIZE];





/*****************************************************************************
 * CAN to UART
 */

void can_message_encap(char *dest, can_message *cmsg) {
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8;//actual size of can message

	*dest++ = (RS232CAN_PKT);  //command
	*dest++ = (len);           //length

	uint8_t i;
	uint8_t *buf = (uint8_t *) cmsg;
	for (i = 0; i < len; i++) {
		*dest++ = (*buf++);
	}

	*dest++ = (0x23);		// XXX CRC
}

void put_overflow_message(char *dest) {
	*dest++ = (RS232CAN_NOTIFY_TX_OVF);  //command
	*dest++ = (0);           //length
	*dest++ = 0x23;
}

char usb_out_buf [32][16];

uint8_t usb_out_buf_head;
uint8_t usb_out_buf_tail;

void write_can_message_to_usb_buf(can_message *cmsg) {
	if (((usb_out_buf_head + 2) % 32) == usb_out_buf_tail) {

		put_overflow_message(usb_out_buf [usb_out_buf_head]);
		usb_out_buf_head = (usb_out_buf_head +1 ) % 32;

	} else if (((usb_out_buf_head + 1) % 32) != usb_out_buf_tail) {
		can_message_encap(usb_out_buf [usb_out_buf_head], cmsg);
		usb_out_buf_head = (usb_out_buf_head +1 ) % 32;
	}
}

uint8_t get_messages_from_usb_out_buf(uchar *buf, uint8_t max_size) {
	uint8_t len = 0;
	while (usb_out_buf_tail != usb_out_buf_head) {
		uint8_t this_len = usb_out_buf[usb_out_buf_tail][1] + 2;
		if ((len + this_len) > max_size) break;

		memcpy(buf, usb_out_buf[usb_out_buf_tail], this_len);
		buf += this_len;
		len += this_len;
		usb_out_buf_tail = (usb_out_buf_tail + 1) % 32;
	}

	return len;
}


/*****************************************************************************/


uchar usb_tx_buffer[128];

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *) data;

	//switch through requests
	//don't expect the cases to break, they may also return
	switch (rq->bRequest)
	{
		case 0x17:
		{
			uint8_t len = get_messages_from_usb_out_buf(usb_tx_buffer, 127);
			usbMsgPtr = usb_tx_buffer;
			return len;
		}

		//host wants to write a raw packet
		case 0x18:
			// initialize position index
			rfmusb_usbRxCnt = 0;

			// store the amount of data to be received
			rfmusb_usbRxLen = rq->wLength.bytes[0];

			// limit to buffer size
			if (rfmusb_usbRxLen > RFMUSB_USBRXBUFFER_SIZE)
				rfmusb_usbRxLen = RFMUSB_USBRXBUFFER_SIZE;

			//toggle status led
			//LED_STATUS_TOGGLE;

			// tell driver to use usbFunctionWrite()
			return USB_NO_MSG;

		default:
			break;
	}

	return 0;   /* default for not implemented requests: return no data back to host */
}


uint8_t usb_got_packet;

//receive raw packet data from the host
//FIXME: to allow different modes, one should check the current mode and call appropriate subfunctions
uchar usbFunctionWrite(uchar *data, uchar len)
{
	uchar i;

	//LED_STATUS_TOGGLE;

	//if this is the last incomplete chunk
	if (len > rfmusb_usbRxLen)
	{
		//limit to the amount we can store
		len = rfmusb_usbRxLen;
	}

	//copy data
	rfmusb_usbRxLen -= len;
	for (i = 0; i < len; i++)
	{
		rfmusb_usbRxBuf[rfmusb_usbRxCnt++] = data[i];
	}

	if (rfmusb_usbRxLen == 0)
	{
		//tx packet
		//FIXME: test if the transmit buffer is free and issue tx rate limit notification to host if not
		//rfm12_tx(rfmusb_usbRxBuf[0], rfmusb_usbRxBuf[1], &rfmusb_usbRxBuf[2]);

		usb_got_packet = 1;

		//return 1 if we have all data
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
 * Receive a message from uart non blocking.
 * Returns Message or 0 if there is no complete message.
 */ 

typedef enum {STATE_START, STATE_LEN, STATE_PAYLOAD} canu_rcvstate_t;

static rs232can_msg	canu_rcvpkt;
static canu_rcvstate_t	canu_rcvstate = STATE_START;
static unsigned char	canu_rcvlen   = 0;


static rs232can_msg *canu_get_nb(void) {
	static char *uartpkt_data;
	char c;

	while (uart_getc_nb(&c)) {
		#ifdef DEBUG
		printf("canu_get_nb received: %02x\n", c);
		#endif
		switch (canu_rcvstate) {
		case STATE_START:
			if (c) {
				canu_rcvstate = STATE_LEN;
				canu_rcvpkt.cmd = c;
			}
			break;
		case STATE_LEN:
			canu_rcvstate     = STATE_PAYLOAD;
			canu_rcvlen       = (unsigned char)c;
			canu_rcvpkt.len   = c;
			uartpkt_data      = &canu_rcvpkt.data[0];
			break;
		case STATE_PAYLOAD:
			if (canu_rcvlen--) {
				*(uartpkt_data++) = c;
			} else {
				canu_rcvstate = STATE_START;
				//check CRC
				if (c == 0x23) { // XXX CRC
					return &canu_rcvpkt;
				}
			}
			break;
		}
	}

	return NULL;
}

/*****************************************************************************/


static void process_cantun_msg(rs232can_msg *msg)
{
	can_message *cmsg;

	switch (msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			mcp_setmode(msg->data[0]);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get();                      //alocate buffer
			memcpy(cmsg, msg->data, sizeof(can_message)); //copy can message
			can_transmit(cmsg);                           //transmit it
			break;
	}
}


static void usb_init(void)
{
	uchar i;

	/* RESET status: all port bits are inputs without pull-up.
	* That's the way we need D+ and D-. Therefore we don't need any
	* additional hardware initialization.
	*/
	usbInit();

	usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
	i = 0;
	while (--i)
	{             /* fake USB disconnect for > 250 ms */
		//wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();
}

int main(void) {
	DDR_LED |= (1<<BIT_LED); //LED-Pin to output
	PORT_LED |= (1<<BIT_LED); //LED on during init

	usb_init();


#ifdef WANT_UART
	uart_init();
#endif

	spi_init();
	can_init();

	sei();

	PORT_LED &= ~(1<<BIT_LED); //LED off after init

	uint16_t led_count = 0;

	while (1) {
		can_message *cmsg;

		usbPoll();

#ifdef WANT_UART
		rs232can_msg *rmsg;
		rmsg = canu_get_nb();
		if (rmsg) {
			process_cantun_msg(rmsg);
		}
#endif

		if (usb_got_packet) {
			usb_got_packet = 0;
			led_count = 2000;
			process_cantun_msg((rs232can_msg *) rfmusb_usbRxBuf);
		}

		cmsg = can_get_nb();
		if (cmsg) {
			led_count = 2000;

			write_can_message_to_usb_buf(cmsg);
#ifdef WANT_UART
			write_can_message_to_uart(cmsg);
#endif
			can_free(cmsg);
		}

		if (led_count) {
			PORT_LED |= (1<<BIT_LED); //LED on
			led_count--;
		} else {
			PORT_LED &= ~(1<<BIT_LED); //LED off
		}
	}
}
