
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "canlib/spi.h"
#include "canlib/can.h"

#include "uart/uart.h"
#include "usbdrv/usbdrv.h"
#include "requests.h"


typedef enum { RS232CAN_RESET=0x00,
		RS232CAN_SETFILTER=0x10, RS232CAN_PKT=0x11, RS232CAN_SETMODE=0x12, RS232CAN_ERROR=0x13
} rs232can_cmd;

#define RS232CAN_MAXLENGTH 20

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;




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




/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

	//switch through requests
	//don't expect the cases to break, they may also return
	switch(rq->bRequest)
	{
		//host wants to write a raw packet to the rfm12
		case RFMUSB_RQ_RFM12_PUT:
			// initialize position index
			rfmusb_usbRxCnt = 0;

			// store the amount of data to be received
			rfmusb_usbRxLen = rq->wLength.bytes[0];

			// limit to buffer size
			if(rfmusb_usbRxLen > RFMUSB_USBRXBUFFER_SIZE)
				rfmusb_usbRxLen = RFMUSB_USBRXBUFFER_SIZE;

			//toggle status led
			LED_STATUS_TOGGLE;

			// tell driver to use usbFunctionWrite()
			return USB_NO_MSG;
			
		//send a single character
		//FIXME: please cleanup define name and location
		case USB_SENDCHAR:
			//copy data
			rfmusb_usbRxBuf[0] = rq->wIndex.bytes[0];

			//send
			rfm12_tx (1, 0, rfmusb_usbRxBuf);

			//toggle led
			LED_STATUS_TOGGLE;

			//use default return value
			break;			

		//host wants to read rfm12 packet data
		case RFMUSB_RQ_RFM12_GET:
			//if there is data to transmit
			if(rfmusb_usbTxLen)
			{
				uint8_t tmp;

				//set usb message pointer
				usbMsgPtr = (uchar *)rfmusb_usbTxBuf;

				//free buffer
				tmp = rfmusb_usbTxLen;
				rfmusb_usbTxLen = 0;

				//switch led
				LED_STATUS_TOGGLE;

				//tell the driver to send n bytes
				return tmp;
			}
			else
			{
				//use default return value
				break;
			}

		//host polls for notifications
		case RFMUSB_RQ_GETNOTIFICATION:
			//if there is any
			if(rfmusb_notifyBuf.len)
			{
				uint8_t tmp;

				//set usb message pointer
				usbMsgPtr = (uchar *)&rfmusb_notifyBuf;

				//free buffer
				tmp = rfmusb_notifyBuf.len;
				rfmusb_notifyBuf.len = 0;

				//tell driver to send n bytes
				return tmp;
			}

		//use default return value
		default:
			break;
	}

	return 0;   /* default for not implemented requests: return no data back to host */
}


//receive raw packet data from the host
//FIXME: to allow different modes, one should check the current mode and call appropriate subfunctions
uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;

	LED_STATUS_TOGGLE;

    //if this is the last incomplete chunk
    if(len > rfmusb_usbRxLen)
    {
		//limit to the amount we can store
        len = rfmusb_usbRxLen;
	}

	//copy data
	rfmusb_usbRxLen -= len;
    for(i = 0; i < len; i++)
    {
        rfmusb_usbRxBuf[rfmusb_usbRxCnt++] = data[i];
	}

    if(rfmusb_usbRxLen == 0)
    {
		//tx packet
		//FIXME: test if the transmit buffer is free and issue tx rate limit notification to host if not
		rfm12_tx(rfmusb_usbRxBuf[0], rfmusb_usbRxBuf[1], &rfmusb_usbRxBuf[2]);

		//return 1 if we have all data
		return 1;
	}
	else
	{
    	return 0;
	}
}














/*****************************************************************************
 * CAN to UART
 */
void write_buffer_to_uart(char* buf, uint8_t len)
{
	uint8_t i;

	for (i=0; i<len; i++) {
		uart_putc( *buf++);
	}
}

void write_can_message_to_uart(can_message * cmsg){
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8;//actual size of can message

	uart_putc(RS232CAN_PKT);  //command
	uart_putc(len);           //length
	
	write_buffer_to_uart((char*)cmsg, len); //data

	uart_putc(0x23);		// XXX CRC
}
/*****************************************************************************/



/*****************************************************************************
 * Receive a message from uart non blocking.
 * Returns Message or 0 if there is no complete message.
 */ 

typedef enum {STATE_START, STATE_LEN, STATE_PAYLOAD} canu_rcvstate_t;

rs232can_msg	canu_rcvpkt;
canu_rcvstate_t	canu_rcvstate = STATE_START;
unsigned char 	canu_rcvlen   = 0;


rs232can_msg * canu_get_nb(){
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
			if(canu_rcvlen--){
				*(uartpkt_data++) = c;
			} else {
				canu_rcvstate = STATE_START;
				//check CRC
				if(c == 0x23){ // XXX CRC
					return &canu_rcvpkt;
				}
			}
			break;
		}
	}

	return NULL;
}

/*****************************************************************************/


void process_cantun_msg(rs232can_msg *msg)
{
	can_message *cmsg;

	switch(msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			can_setmode(msg->data[0]);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get();                      //alocate buffer
			memcpy(cmsg, msg->data, sizeof(can_message)); //copy can message
			can_transmit(cmsg);                           //transmit it
			break;
	}
}

int main(){
	DDRB |= (1<<PB0); //LED-Pin to output

	uart_init();
	spi_init();
	can_init();

	sei();

	can_setmode(normal);

	while(1) {
		rs232can_msg  *rmsg;
		can_message *cmsg;


		rmsg = canu_get_nb();
		if (rmsg){
			PORTB ^= 0x01;
			process_cantun_msg(rmsg);
		}
		
		cmsg = can_get_nb();
		if (cmsg){
			PORTB ^= 0x01;
			write_can_message_to_uart(cmsg);
			can_free(cmsg);
		}
	}

	return 0;
}
