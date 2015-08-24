#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can_handler.h"
#include "can/can.h"
#include "can/lap.h"
#include "beamer_rs232.h"
#include "audio_matrix.h"

static uint8_t myaddr;

//message handler
void can_handler(void)
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
						beamer_send_command(rx_msg->data[1]);
						break;
					// 2 was beamer IR
					case 3:
						setAllChannels(rx_msg->data[1]);
						break;
					case 4:
						setSingleChannel(rx_msg->data[1], rx_msg->data[2]);
						break;
					case 5:
						incrementChannels(rx_msg->data[2]);
						break;
					case 6:
						setMute(rx_msg->data[1]);
						break;
					case 7:
						beamer_start_shutdown();
						break;
					case 8:
						am_set_output(rx_msg->data[1], rx_msg->data[2]);
						break;
					case 9:
						am_set_gain(rx_msg->data[1], rx_msg->data[2]);
						break;
					case 10:
						am_set_output(rx_msg->data[1], 0);
						am_set_output(rx_msg->data[1], 1);
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
		else if (rx_msg->addr_src == 0x02 && rx_msg->port_src == 0x02 && rx_msg->addr_dst == 0x00 && rx_msg->port_dst == 0x00) { // get powercommander status
			static uint8_t status = 0;
			if ((rx_msg->data[1] & _BV(0)) && (!(status & _BV(0)))) { // Hauptschütz an
				TeufelPoweron();
				am_init_delay();
				status |= _BV(0);
			}
			else if ((!(rx_msg->data[1] & _BV(0))) && (status & _BV(0))) { // Hauptschütz aus
				beamer_start_shutdown();
				am_set_power_led(0);
				status &= ~_BV(0);
			}

			if ((rx_msg->data[1] & _BV(1)) && (!(status & _BV(1)))) { // Beamer Schütz an
				set_status_beamer_power(1);
				status |= _BV(1);
			}
			else if ((!(rx_msg->data[1] & _BV(1))) && (status & _BV(1))) { // Beamer Schütz aus
				set_status_beamer_power(0);
				status &= ~_BV(1);
			}
		}
		can_free(rx_msg);
	}
}

void lap_get_status(void)
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = myaddr;
	tx_msg->port_src = 0x00;
	tx_msg->addr_dst = 0x02;
	tx_msg->port_dst = 0x02;
	tx_msg->dlc = 1;
	tx_msg->data[0] = 0x02; // send status
	can_transmit(tx_msg);
}

void lap_switch_beamer_relais(uint8_t status)
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = myaddr;
	tx_msg->port_src = 0x00;
	tx_msg->addr_dst = 0x02;
	tx_msg->port_dst = 0x01;
	tx_msg->dlc = 3;
	tx_msg->data[0] = 0x00; // C_SW
	tx_msg->data[1] = 0x01; // SWA_BEAMER
	tx_msg->data[2] = status ? 0x01 : 0x23; // AUS - special value
	can_transmit(tx_msg);
}

// XXX provide usefull feedback
void lap_send_beamer_status(uint8_t type, uint8_t len, uint16_t data)
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = myaddr;
	tx_msg->port_src = 0x05;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = len;
	tx_msg->data[0] = type;
	tx_msg->data[1] = (uint8_t) data & 0xff;
	tx_msg->data[2] = (uint8_t) (data >> 8) & 0xff;
	can_transmit(tx_msg);
}

void lap_send_teufel_status(t_channel channels[])
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = myaddr;
	tx_msg->port_src = 0x06;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = NUM_TEUFEL_CHANNELS;
	for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++)
		tx_msg->data[i] = channels[i].vol;
	can_transmit(tx_msg);
}

void lap_send_matrix_status(uint8_t (*matrix)(uint8_t))
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = myaddr;
	tx_msg->port_src = 0x07;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = 4;
	for (uint8_t i = 0; i < 4; i++)
		tx_msg->data[i] = (*matrix)(i);
	can_transmit(tx_msg);
}

static const uint8_t EE_lap_addr EEMEM = EEPROM_LAP_ADDR;

void can_read_addr(void)
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}
