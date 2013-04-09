#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "can/can.h"
#include "can/spi.h"
#include "can/lap.h"
#include "dimmer.h"

#define PORT_LEDS PORTD

uint8_t Station_id;

static inline void set_lampe(uint8_t lampe, uint8_t val)
{
	Bright[lampe] = 64 - (val >> 2);
}

#define LED_B1 0xF0
#define LED_B2 0xCC
#define LED_B3 0xAA
#define LED_B4 0xC0
#define LED_B5 0xCC
#define LED_B6 0x80

uint8_t flags;
#define F_RCV_CAN 0x01
#define F_MODE_ON 0x02

#define LED_GREEN PD1

static inline void blink_leds() {
	static uint8_t rol;
	if (TIFR & _BV(OCF1A)) {
		TIFR = _BV(OCF1A); //clear flag
		if ((rol>>=1) == 0) rol = 0x80;
		if (flags & F_RCV_CAN) {
			flags &= ~ F_RCV_CAN;
			PORT_LEDS ^= (1 << LED_GREEN);
		} else {
			if (flags & F_MODE_ON)
			{
				PORT_LEDS |= (1 << LED_GREEN);
			} else {
				if (rol & LED_B6) {
					PORT_LEDS |= (1 << LED_GREEN);
				} else {
					PORT_LEDS &= ~(1 << LED_GREEN);
				}
			}
		}
	}
}

static inline void relais_check() {
	uint8_t x, on = 0;
	for (x = 0; x < NUM_LAMPS; x++) {
		if (Bright[x] != 64) {
			on = 1;
		}
	}
	if (on) {
		flags |= F_MODE_ON;
		PORTD |=  0x10;
	} else {
		flags &= ~F_MODE_ON;
		PORTD &= ~0x10;
	}
}

void eventloop() {
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	while (1) {
		can_message *rx_msg;
		if ((rx_msg = can_get_nb()) != 0) {
			flags |= F_RCV_CAN;
			switch (rx_msg->port_dst) {
				case PORT_MGT:
					// Management
					switch (rx_msg->data[0]) {
						case FKT_MGT_PING:
							msg.addr_src = Station_id;
							msg.addr_dst = rx_msg->addr_src;
							can_transmit(&msg);
							break;
						case FKT_MGT_RESET:
							asm volatile("rjmp __vectors");
							break;
					}
					break;
				case PORT_LAMPE:
					// Lampen steuern
					switch (rx_msg->data[0]) {
						case FKT_LAMPE_SET: {
							if (rx_msg->dlc != 3) continue;
							uint8_t value;
							uint8_t lampe;
							lampe = rx_msg->data[1];
							value = rx_msg->data[2];
							Ramp.end_bright[lampe] = value;
							break;
						}
						case FKT_LAMPE_SETMASK: {
							uint8_t i;
							if (rx_msg->dlc != NUM_LAMPS + 2) continue;
							for (i = 0; i < NUM_LAMPS; i++) {
								uint8_t value = rx_msg->data[i];
								set_lampe(i, value);
							}
							break;
						}
						case FKT_LAMPE_SETDELAY: {
							if (rx_msg->dlc != 4) continue;
							union {
								uint16_t i;
								uint8_t c[2];
							} value;
							uint8_t lampe;
							lampe = rx_msg->data[1];
							value.c[1] = rx_msg->data[2];
							value.c[0] = rx_msg->data[3];
							Ramp.delay_rl[lampe] = value.i;
							Ramp.delay[lampe] = value.i;
							break;
						}
					}
					break;
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

int main(void) {
	DDRD = 0x73; // 0111 0011
	DDRB = 0x0F; // 0000 1111
	PORTD = 0x12; // 0001 0010

	timer1_init();

	Station_id = 0x35;

	spi_init();
	can_init();

	dimmer_init();

	sei();

	eventloop();
	return 1;
}

