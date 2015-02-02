#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "lab-irkit.h"
#include "can/can.h"
#include "can/spi.h"
#include "can/lap.h"
#include "uart/uart.h"
#include "beamer_rs232.h"
#include "twi_master/twi_master.h"
#include "teufel-i2c.h"

static volatile uint8_t tickscounter;

ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

static uint8_t myaddr;

static uint8_t beamer_power;

//message handler
static void can_handler(void)
{
	can_message *rx_msg;

	//get next canmessage in rx_msg that is destined for us
	if ((rx_msg = can_get_nb())) {
		if (rx_msg->addr_dst == myaddr) {
			//handle management functions
			if (rx_msg->port_dst == PORT_MGT) {
				switch (rx_msg->data[0]) {
					case FKT_MGT_RESET:
					wdt_enable(WDTO_15MS);
					while (1);
						case FKT_MGT_PING: {
							can_message *tx_msg = can_buffer_get();
							tx_msg->port_src = PORT_MGT;
							tx_msg->port_dst = PORT_MGT;
							tx_msg->addr_src = myaddr;
							tx_msg->addr_dst = rx_msg->addr_src;
							tx_msg->dlc = 1;
							tx_msg->data[0] = FKT_MGT_PONG;
							can_transmit(tx_msg);
						}
						break;
							default:
						break;
					}
				}
				//handle ir commands
				else if (rx_msg->port_dst == PORT_REMOTE) { // 0x21
					//switch the remote device type
					switch (rx_msg->data[0]) {
						//0 was teufel ir
						//this is a message for the acer beamer
						case 1:
							PORTD |= _BV(PD7); // Enable debug LED
							rs232_send_command(rx_msg->data[1]);
							break;
						// 2 was beamer IR
						case 3:
							setAllChannels(rx_msg->data[1]);
							TeufelSendCANPacket();
							break;
						case 4:
							setSingleChannel(rx_msg->data[1], rx_msg->data[2]);
							TeufelSendCANPacket();
							break;
						case 5:
							setIncrementChannels(rx_msg->data[2]);
							TeufelSendCANPacket();
							break;
						default:
							break;
					}
				}
			}
			/* Nach Hauptschütz an nachricht mit verzögerung default werte an teufel system senden
			 * Wenn Beamer Strom hat lamp status (auflösung / laufzeit / src) pollen
			 * beim labor abschalten beamer abschalten, nachlaufen lassen, strom trennen
			 */
			if (rx_msg->addr_src == 0x02) { // get powercommander status
				static uint8_t status = 0;
				switch (rx_msg->port_src) {
					case 0x02:
						if ((rx_msg->data[1] & 0x01) && !(status & _BV(1))) { // Hauptschütz an
							TeufelPoweron();
							status |= _BV(1);
						}
						else if (!(rx_msg->data[1] & 0x01) && status & _BV(1)) {
							status &= ~_BV(1);
						}
						if (rx_msg->data[1] & 0x02) { // Beamer Schütz an
							beamer_power = 1;
						}
						else if (!(rx_msg->data[1] & 0x02)) { // Beamer Schütz aus
							beamer_power = 0;
						}
						break;
					/*case 0x03:
						if (rx_msg->data[0] & 0x01 && rx_msg->data[1] == 0) { // Hauptschalter an
							status |= _BV(0);
						} else {
							status &= ~_BV(0);
						}
						break;*/
				}
			}
		can_free(rx_msg);
	}
}

//system initialization
static void init(void)
{
	// disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	// debug LED output
	DDRD |= _BV(PD7);

	// initialize ir subsystem
//	ir_init();

	TCCR0 = _BV(CS02) | _BV(CS00); // clk / 256
	TIMSK = _BV(TOIE0);

	// initialize spi port
	spi_init();

	// initialize uart
	uart_init();

	// initialize twi
	if (!TWIM_Init())
		while (1);

	myaddr = eeprom_read_byte(0x00);
	//initialize can communication
	can_init();

	// turn on interrupts
	sei();
}


int main(void)
{
	// system initialization
	init();

	setDefaultAfterPoweron(); // teufel

	// the main loop continuously handles can messages
	while (1) {
		can_handler();
		rs232_receive_handler();
		if (tickscounter > 10) {
			tickscounter = 0;
			TeufelPoweronTick();
			if (beamer_power)
				poll_beamer_state();
		}
		PORTD &= ~_BV(PD7); // Disable debug LED
	}
}
