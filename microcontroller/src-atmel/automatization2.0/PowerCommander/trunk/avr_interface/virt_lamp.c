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
	/*{
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
	}*/
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
	if (room > 2)
		return;
	if (index > 7)
		return;

	if (room == ROOM_VORTRAG)
	{
		if (index == 0) { /* SWL_TAFEL */
			if (enable)
				outputdata.ports |= (1<<SWL_TAFEL);
			else
				outputdata.ports &= ~(1<<SWL_TAFEL);
		}
		else if (index == 1) { /* SWL_BEAMER */
			if (enable)
				outputdata.ports |= (1<<SWL_BEAMER);
			else
				outputdata.ports &= ~(1<<SWL_BEAMER);
		}
		else if (index == 2) { /* SWL_SCHRANK */
			if (enable)
				outputdata.ports |= (1<<SWL_SCHRANK);
			else
				outputdata.ports &= ~(1<<SWL_SCHRANK);
		}
		else if (index == 3){ /* SWL_FLIPPER */
			if (enable)
				outputdata.ports |= (1<<SWL_FLIPPER);
			else
				outputdata.ports &= ~(1<<SWL_FLIPPER);
		}
		else if (index == 4){ /* SWL_FLIPPER */
			if (enable)
				outputdata.ports |= (1<<SWA_BEAMER);
			else
				outputdata.ports &= ~(1<<SWA_BEAMER);
		}
	}

	else if (room == ROOM_LOUNGE)
	{
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
	}
	else if (room == ROOM_KUECHE)
	{
		if (enable)
			outputdata.ports |= (1<<SWL_KUECHE);
		else
			outputdata.ports &= ~(1<<SWL_KUECHE);
	}

	relais_control();	/* update relays status */
}

void set_lamp_all(uint8_t room, uint8_t enable)
{
	if (room > 2)
		return;

	if (room == ROOM_VORTRAG)
	{
		if (enable)
		{
			outputdata.ports |= (1<<SWL_TAFEL)|(1<<SWL_BEAMER)|(1<<SWL_FLIPPER)|(1<<SWL_SCHRANK);
		}
		else
		{
			outputdata.ports &= ~((1<<SWL_TAFEL)|(1<<SWL_BEAMER)|(1<<SWL_FLIPPER)|(1<<SWL_SCHRANK));
		}

	}
	else if (room == ROOM_LOUNGE)
	{
		{
			can_message *msg = can_buffer_get();
			msg->addr_src = myaddr;
			msg->port_src = 2;
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
			msg->port_src = 2;
			msg->addr_dst = 0x61;
			msg->port_dst = 2;
			msg->dlc = 3;
			msg->data[0] = 3; // switch lamp all
			msg->data[1] = 0;
			msg->data[2] = enable;
			can_transmit(msg);	// send packet to can_dimmer
		}
	}
	else if (room == ROOM_KUECHE)
	{
		if (enable)
			outputdata.ports |= (1<<SWL_KUECHE);
		else
			outputdata.ports &= ~(1<<SWL_KUECHE);
	}

	relais_control();	/* update relais status, will call twi_send() */
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
	if (room > 2)
		return;
	if (index > 7)
		return;

	if (room == ROOM_VORTRAG)
	{
		if (index < 4)
			outputdata.pwmval[index] = value;
	}
	else if(room == ROOM_LOUNGE)
	{
		/*{
			can_message *msg = can_buffer_get();
			msg->addr_src = myaddr;
			msg->port_src = 2;
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
			msg->port_src = 2;
			msg->addr_dst = 0x61;
			msg->port_dst = 2;
			msg->dlc = 3;
			msg->data[0] = 1; // set brightness lamp
			msg->data[1] = index;
			msg->data[2] = value;
			can_transmit(msg);	// send packet to can_dimmer
		}*/
	}
	else if (room == ROOM_KUECHE)
	{
		outputdata.pwmval[PWM_KUECHE] = value;	/* PWM_KUECHE is 5 */
	}

	relais_control();	/* update relais status, will call twi_send() */
}

void set_bright_all(uint8_t room, uint8_t value)
{
	if (room > 2)
		return;

	if (room == ROOM_VORTRAG)
	{
		outputdata.pwmval[PWM_TAFEL] = value;
		outputdata.pwmval[PWM_BEAMER] = value;
		outputdata.pwmval[PWM_SCHRANK] = value;
		outputdata.pwmval[PWM_FLIPPER] = value;
	}
	else if (room == ROOM_LOUNGE)
	{
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
	}
	else if (room == ROOM_KUECHE)
	{
		outputdata.pwmval[PWM_KUECHE] = value;	/* PWM_KUECHE is 5 */
	}

	relais_control();	/* update relais status, will call twi_send() */
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

/*
* Disable relais that are not used at the moment
* PWM values are ignored
* only touch relais of ROOM_VORTRAG, ROOM_KUECHE, ROOM_LOUNGE
*/

static void relais_control()
{
	if (lounge_lamp_status_1 || lounge_lamp_status_2) // one or more lamps in lounge are on
		outputdata.ports |= _BV(SWL_LOUNGE);
	else
		outputdata.ports &= ~_BV(SWL_LOUNGE);

	if (outputdata.ports & _BV(SWL_TAFEL) || outputdata.ports & _BV(SWL_BEAMER) || outputdata.ports & _BV(SWL_SCHRANK) || outputdata.ports & _BV(SWL_FLIPPER))	// one ore more are on
		outputdata.ports |= _BV(SWL_VORTRAG);
	else
		outputdata.ports &= ~_BV(SWL_VORTRAG);


	twi_send();	// push outputdata
}


