#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "can/can.h"
#include "can/spi.h"
#include "can/lap.h"
#include "dimmer.h"

#define PORT_LEDS PORTD

unsigned char Station_id;

static inline void set_lampe(unsigned char lampe, unsigned char val)
{
	Bright[lampe] = 64-(val>>2);
}

#define LED_B1 0xF0
#define LED_B2 0xCC
#define LED_B3 0xAA
#define LED_B4 0xC0
#define LED_B5 0xCC
#define LED_B6 0x80

unsigned char FLAGS;
#define F_RCV_CAN 0x01
#define F_MODE_ON 0x02

#define LED_GREEN PD1

static inline void blink_leds() {
	static unsigned char rol;
	if (TIFR & _BV(OCF1A)) {
		TIFR = _BV(OCF1A); //clear flag
		if ((rol>>=1) == 0) rol = 0x80;
		if (FLAGS & F_RCV_CAN) {
			FLAGS &= ~ F_RCV_CAN;
			PORT_LEDS ^= (1<<LED_GREEN);
		} else {
			if (FLAGS & F_MODE_ON)
			{
				PORT_LEDS |= (1<<LED_GREEN);
			} else {
				if (rol & LED_B6) {
					PORT_LEDS |= (1<<LED_GREEN);
				} else {
					PORT_LEDS &= ~(1<<LED_GREEN);
				}
			}
		}
	}
}

static inline void relais_check() {
	unsigned char x, on = 0;
	for (x = 0; x < NUM_LAMPS; x++) {
		if (Bright[x] != 64) {
			on = 1;
		}
	}
	if (on) {
		FLAGS |= F_MODE_ON;
		PORTD |=  0x10;
	} else {
		FLAGS &= ~F_MODE_ON;
		PORTD &= ~0x10;
	}

}

void eventloop() {
	Tx_msg.addr_src = Station_id;
	while (1) {
		if (can_get_nb()) {
			FLAGS |= F_RCV_CAN;
			switch (Rx_msg.port_dst) {
			case PORT_MGT:
				// Management
				switch (((pdo_message*)&Rx_msg)->cmd) {
				case FKT_MGT_PING:
					{pdo_message *rmsg = (pdo_message *)&Tx_msg;
					rmsg->addr_dst = Rx_msg.addr_src;
					rmsg->port_dst = Rx_msg.port_src;
					rmsg->port_src = PORT_MGT;
					rmsg->cmd = FKT_MGT_PONG;
					rmsg->dlc = 1;
					can_transmit();
					break;}
				case FKT_MGT_RESET:
					asm volatile("rjmp __vectors");
					break;
				}
			case PORT_LAMPE:
				// Lampen steuern
				switch (((pdo_message*)&Rx_msg)->cmd) {
					case FKT_LAMPE_SET: {
						if (Rx_msg.dlc != 3) continue;
						unsigned char value;
						unsigned char lampe;
						lampe = ((pdo_message*)&Rx_msg)->data[0];
						value = ((pdo_message*)&Rx_msg)->data[1];
			
						Ramp.end_bright[lampe] = value;
						break;
					}
					case FKT_LAMPE_SETMASK: {
						unsigned char i;
						if (Rx_msg.dlc != NUM_LAMPS + 2) continue;
						for (i = 0; i < NUM_LAMPS; i++) {
							unsigned char value = ((pdo_message*)&Rx_msg)->data[i];
							set_lampe(i, value);
						}
						break;
					}
					case FKT_LAMPE_SETDELAY:{
						if (Rx_msg.dlc != 4) continue;
						union{
							unsigned int i;
							unsigned char c[2];
						}value;
						unsigned char lampe;
						lampe = ((pdo_message*)&Rx_msg)->data[0];
						value.c[1] = ((pdo_message*)&Rx_msg)->data[1];
						value.c[0] = ((pdo_message*)&Rx_msg)->data[2];
						Ramp.delay_rl[lampe] = value.i;
						Ramp.delay[lampe] = value.i;
						break;
					}
				}
			}
		}
		blink_leds();
		relais_check();
	}
}

static inline void timer1_init() {
	ICR1 = 8000;
	TCCR1B = 0x03 | _BV(CTC1); //CTC Mode, clk/64
	OCR1A = 0x6000;
	TCNT1 = 0;
}

int main() {
	DDRD = 0x73;
	DDRB = 0x0F;//rest will be set by spi_init
	PORTD = 0x12;
	
	timer1_init();
	
	Station_id = 0x35;
	
	spi_init();
	can_init();

	dimmer_init();

	sei();
	
	eventloop();
	return 0;
}

