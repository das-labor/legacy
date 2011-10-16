
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/crc16.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "canlib/spi.h"
#include "canlib/can.h"

#include "uart/uart.h"


typedef enum
{
	RS232CAN_RESET=0x00,
	RS232CAN_SETFILTER=0x10,
	RS232CAN_PKT=0x11,
	RS232CAN_SETMODE=0x12,
	RS232CAN_ERROR=0x13,
	RS232CAN_NOTIFY_RESET=0x14,
	RS232CAN_PING_GATEWAY=0x15
} rs232can_cmd;

#define RS232CAN_MAXLENGTH 20

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;


/*****************************************************************************
 * CAN to UART
 */
uint16_t write_buffer_to_uart_and_crc(char* buf, uint8_t len) {
	uint8_t i;
	uint16_t crc = 0;

	for (i = 0; i < len; i++) {
		crc = _crc16_update(crc, *buf);
		uart_putc( *buf++);
	}
	
	return crc;
}

void write_can_message_to_uart(can_message * cmsg) {
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8;//actual size of can message
	uint16_t crc; 

	uart_putc(RS232CAN_PKT);  //command
	uart_putc(len);           //length
	
	crc = write_buffer_to_uart_and_crc((char*)cmsg, len); //data

	uart_putc(crc >> 8);
	uart_putc(crc & 0xFF);
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


rs232can_msg * canu_get_nb() {
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
			canu_rcvlen       = (unsigned char)c;
			if(canu_rcvlen > RS232CAN_MAXLENGTH)
			{
				canu_rcvstate = STATE_START;
				break;
			}
			canu_rcvstate     = STATE_PAYLOAD;
			canu_rcvpkt.len   = (unsigned char)c;
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


void process_cantun_msg(rs232can_msg *msg) {
	can_message *cmsg;

	switch (msg->cmd) {
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
		case RS232CAN_NOTIFY_RESET:
			break;
		case RS232CAN_PING_GATEWAY:
			uart_putc(RS232CAN_PING_GATEWAY);  // reply
		default:
			uart_putc(RS232CAN_ERROR);  //send error
			break;
	}
}


#define PORT_LEDS PORTD
#define DDR_LEDS DDRD
#define PIN_LEDCL PD5
#define PIN_LEDCK PD6
#define PIN_LEDD PD7


#define PORT_BUSPOWER PORTD
#define DDR_BUSPOWER DDRD
#define BIT_BUSPOWER PD4

void buspower_on() {
	DDR_BUSPOWER |= (1<<BIT_BUSPOWER);
	PORT_BUSPOWER |= (1<<BIT_BUSPOWER);
}

void led_init() {
	DDR_LEDS |= (1<<PIN_LEDD)|(1<<PIN_LEDCL)|(1<<PIN_LEDCK);
	PORT_LEDS |= (1<<PIN_LEDCL);
}

void led_set(unsigned int stat) {
	unsigned char x;
	for (x = 0; x < 16; x++) {
		if (stat & 0x01) {
			PORT_LEDS |= (1<<PIN_LEDD);
		} else {
			PORT_LEDS &= ~(1<<PIN_LEDD);
		}
		stat >>= 1;
		PORT_LEDS |= (1<<PIN_LEDCK);
		PORT_LEDS &= ~(1<<PIN_LEDCK);
	}
}

void adc_init() {
	DDRC = 0xCF;
	ADMUX = 0;
	ADCSRA = 0x07; //slowest adc clock
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADIF);
}


uint16_t leds, leds_old;

int main() {
	led_init();

	buspower_on();
	
	uart_init();
	spi_init();
	can_init();
	wdt_enable(WDTO_250MS);

	sei();

	uart_putc(RS232CAN_NOTIFY_RESET);  //notify host that we resetted

	can_setmode(normal);
	can_setled(0, 1);


	uint8_t r_count = 1, t_count = 1;
	

	while (1) {
		rs232can_msg  *rmsg;
		can_message *cmsg;
		
		wdt_reset();

		rmsg = canu_get_nb();
		if (rmsg) {
			r_count ++;
			process_cantun_msg(rmsg);
		}
		
		cmsg = can_get_nb();
		if (cmsg) {
			t_count ++;
			write_can_message_to_uart(cmsg);
			can_free(cmsg);
		}
		
		leds = (r_count << 8) | t_count;
		if (leds != leds_old) {
			leds_old = leds;
			led_set(leds);
		}
	}

	return 0;
}
