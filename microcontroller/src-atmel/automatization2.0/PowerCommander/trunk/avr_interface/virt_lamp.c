#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "../include/PowerCommander.h"
#include "i2c_funktionen.h"
#include "virt_lamp.h"


static void relais_control(void);

/*
* monitor lounge lamp status
*/

uint8_t lounge_lamp_status_1, lounge_lamp_status_2;

/*
* init lamp_matrix
*/

void init_lamp_control()
{
	lounge_lamp_status_1 = lounge_lamp_status_2 = 0;
	{
		can_message *msg = can_buffer_get();
		msg->addr_src = myaddr;
		msg->port_src = 2;
		msg->addr_dst = 0x60;
		msg->port_dst = 2;
		msg->dlc = 1;
		msg->data[0] = 2; // get lamp status
		can_transmit(msg);	// send packet to can_dimmer
	}
	{
		can_message *msg = can_buffer_get();
		msg->addr_src = myaddr;
		msg->port_src = 2;
		msg->addr_dst = 0x61;
		msg->port_dst = 2;
		msg->dlc = 1;
		msg->data[0] = 2; // get lamp status
		can_transmit(msg);	// send packet to can_dimmer
	}
}

void set_lounge_lamp_1(uint8_t val)
{
	lounge_lamp_status_1 = val;
	relais_control();
}

void set_lounge_lamp_2(uint8_t val)
{
	lounge_lamp_status_2 = val;
	relais_control();
}

static void output_set(uint8_t output, uint8_t enable)
{
	if (enable)
		outputdata.ports |= _BV(output);
	else
		outputdata.ports &= ~_BV(output);
}

/*
* enable or disable lamps
* room == 0: VORTRAG
*  index == 0: SWL_TAFEL
*  index == 1: SWL_BEAMER
*  index == 2: SWL_SCHRANK
*  index == 3: SWL_FLIPPER
*
* room == 1: LOUNGE
*  index == lamp to control
*
* room == 2: KUECHE
*  index == ignored
*
* enable > 0 : On else Off
*/

void set_lamp(uint8_t room, uint8_t index, uint8_t enable)
{
	if (index > 7)
		return;

	switch (room) {
		case ROOM_VORTRAG:
			switch (index) { // SWL_TAFEL
				case 0:
					output_set(SWL_TAFEL, enable);
					break;
				case 1: // SWL_BEAMER
					output_set(SWL_BEAMER, enable);
					break;
				case 2: // SWL_SCHRANK
					output_set(SWL_SCHRANK, enable);
					break;
				case 3: // SWL_FLIPPER
					output_set(SWL_FLIPPER, enable);
					break;
				case 4: // SWL_BEAMER - beamer steckdose
					if (enable == 1) // allow only power on
						output_set(SWA_BEAMER, enable);
					else if (enable == 0x23) // special power off - needed by can-ir
						output_set(SWA_BEAMER, 0);
					break;
			}
			relais_control();	// update relais status, will call twi_send()
			break;
		case ROOM_LOUNGE:
			/*{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 2;
				msg->addr_dst = 0x60;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 0; // switch lamp
				msg->data[1] = index;
				msg->data[2] = enable;
				can_transmit(msg);	// send packet to can_dimmer
			}
			{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 2;
				msg->addr_dst = 0x61;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 0; // switch lamp
				msg->data[1] = index;
				msg->data[2] = enable;
				can_transmit(msg);	// send packet to can_dimmer
			}*/
			break;
		case ROOM_KUECHE:
			output_set(SWL_KUECHE, enable);
			twi_send();
			break;
	}
}

void set_lamp_all(uint8_t room, uint8_t enable)
{
	switch (room) {
		case ROOM_VORTRAG:
			if (enable)
				outputdata.ports |= (1 << SWL_TAFEL) | (1 << SWL_BEAMER) | (1 << SWL_FLIPPER) | (1 << SWL_SCHRANK);
			else
				outputdata.ports &= ~((1 << SWL_TAFEL) | (1 << SWL_BEAMER) | (1 << SWL_FLIPPER) | (1 << SWL_SCHRANK));
			relais_control();	// update relais status, will call twi_send()
			break;
		case ROOM_LOUNGE:
			{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 0;
				msg->addr_dst = 0x60;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 3; // switch lamp all
				msg->data[1] = 0;
				msg->data[2] = enable;
				can_transmit(msg);	// send packet to can_dimmer
			}
			{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 0;
				msg->addr_dst = 0x61;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 3; // switch lamp all
				msg->data[1] = 0;
				msg->data[2] = enable;
				can_transmit(msg);	// send packet to can_dimmer
			}
			break;
		case ROOM_KUECHE:
			output_set(SWL_KUECHE, enable);
			twi_send();
			break;
	}
}

/*
* set lamps brightness
* room == 0: VORTRAG
*  index == 0: SWL_TAFEL
*  index == 1: SWL_BEAMER
*  index == 2: SWL_SCHRANK
*  index == 3: SWL_FLIPPER
*
* room == 1: LOUNGE
*  index == lamp to control
*
* room == 2: KUECHE
*  index == ignored
*
* value: new brightness value
*/

void set_bright(uint8_t room, uint8_t index, uint8_t value)
{
	if (index > 7)
		return;

	switch (room) {
		case ROOM_VORTRAG:
			if (index < 4) {
				outputdata.pwmval[index] = value;
				twi_send();	// push outputdata
			}
			break;
		case ROOM_LOUNGE:
			/*{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 0;
				msg->addr_dst = 0x60;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 1; // set brightness lamp
				msg->data[1] = index;
				msg->data[2] = value;
				can_transmit(msg);	// send packet to can_dimmer
			}
			{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 0;
				msg->addr_dst = 0x61;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 1; // set brightness lamp
				msg->data[1] = index;
				msg->data[2] = value;
				can_transmit(msg);	// send packet to can_dimmer
			}*/
			break;
		case ROOM_KUECHE:
			outputdata.pwmval[PWM_KUECHE] = value;	// PWM_KUECHE is 5
			twi_send();	// push outputdata
			break;
	}
}

void set_bright_all(uint8_t room, uint8_t value)
{
	switch (room) {
		case ROOM_VORTRAG:
			outputdata.pwmval[PWM_TAFEL] = value;
			outputdata.pwmval[PWM_BEAMER] = value;
			outputdata.pwmval[PWM_SCHRANK] = value;
			outputdata.pwmval[PWM_FLIPPER] = value;
			twi_send();	// push outputdata
			break;
		case ROOM_LOUNGE:
			/*{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 2;
				msg->addr_dst = 0x60;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 4; // set brightness lamp all
				msg->data[1] = 0;
				msg->data[2] = value;
				can_transmit(msg);	// send packet to can_dimmer
			}
			{
				can_message *msg = can_buffer_get();
				msg->addr_src = myaddr;
				msg->port_src = 2;
				msg->addr_dst = 0x61;
				msg->port_dst = 2;
				msg->dlc = 3;
				msg->data[0] = 4; // set brightness lamp all
				msg->data[1] = 0;
				msg->data[2] = value;
				can_transmit(msg);	// send packet to can_dimmer
			}*/
			break;
		case ROOM_KUECHE:
			outputdata.pwmval[PWM_KUECHE] = value;	// PWM_KUECHE is 5
			twi_send();	// push outputdata
			break;
	}
}


/*
* Return status of all lamp channels
*/
uint8_t get_channel_status(void)
{
	uint8_t tmp = 0;
	if (outputdata.ports & _BV(SWL_TAFEL))
		tmp |= _BV(0x00);
	if (outputdata.ports & _BV(SWL_BEAMER))
		tmp |= _BV(0x01);
	if (outputdata.ports & _BV(SWL_SCHRANK))
		tmp |= _BV(0x02);
	if (outputdata.ports & _BV(SWL_FLIPPER))
		tmp |= _BV(0x03);
	if (outputdata.ports & _BV(SWL_KUECHE))
		tmp |= _BV(0x04);
	if (outputdata.ports & _BV(SWL_VORTRAG))
		tmp |= _BV(0x05);
	if (outputdata.ports & _BV(SWL_LOUNGE))
		tmp |= _BV(0x06);
	return tmp;
}

/*
* Return brightness of lamp channel <index>
*  index == 0: PWM_TAFEL
*  index == 1: PWM_BEAMER
*  index == 2: PWM_SCHRANK
*  index == 3: PWM_FLIPPER
*  index == 5: PWM_KUECHE
*/
uint8_t get_channel_brightness(uint8_t index)
{
	if (index > PWM_CHAN - 1)
		return 0;

	return outputdata.pwmval[index];
}

uint8_t get_output_status(void)
{
	uint8_t tmp = 0;
	if (outputdata.ports & _BV(SWA_HS))
		tmp |= _BV(0x00);
	if (outputdata.ports & _BV(SWA_BEAMER))
		tmp |= _BV(0x01);
	return tmp;
}

/*
* Disable relais that are not used at the moment
* PWM values are ignored
* only touch relais of ROOM_VORTRAG, ROOM_LOUNGE
*/

static void relais_control(void)
{
	if (lounge_lamp_status_1 || lounge_lamp_status_2) // one or more lamps in lounge are on
		outputdata.ports |= _BV(SWL_LOUNGE);
	else
		outputdata.ports &= ~_BV(SWL_LOUNGE);

	if (outputdata.ports & (_BV(SWL_TAFEL) | _BV(SWL_BEAMER) | _BV(SWL_SCHRANK) | _BV(SWL_FLIPPER))) // one ore more lamps in vortrag raum are on
		outputdata.ports |= _BV(SWL_VORTRAG);
	else
		outputdata.ports &= ~_BV(SWL_VORTRAG);

	twi_send();	// push outputdata
}
