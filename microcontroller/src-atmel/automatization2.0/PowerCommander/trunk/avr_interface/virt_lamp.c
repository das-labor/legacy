
#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "i2c_funktionen.h"
#include "virt_lamp.h"

#define ROOM_VORTRAG 0
#define ROOM_LOUNGE 1
#define ROOM_KUECHE 2

static void relais_control(void);

/*
* monitor lamp status and brightness
*/

static struct t_lamp_control {
	uint8_t pwmval[7];
	uint8_t enabled;
} lamp_matrix[4];


/*
* init lamp_matrix
*/

void init_lamp_control()
{
	uint8_t i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<7;j++)
		{
			lamp_matrix[i].pwmval[j] = 0;
		}
		lamp_matrix[i].enabled = 0;
	}
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
	if(room > 2)
		return;
	if(index > 7)
		return;

	if(enable)
		lamp_matrix[room].enabled |= 1<<index;
	else
		lamp_matrix[room].enabled &= ~(1<<index);

	if(room == ROOM_VORTRAG)
	{
		if(index == 0){ /* SWL_TAFEL */
			if(enable)
				outputdata.ports |= (1<<SWL_TAFEL);
			else
				outputdata.ports &= ~(1<<SWL_TAFEL);
		}
		else if(index == 1){ /* SWL_BEAMER */
			if(enable)
				outputdata.ports |= (1<<SWL_BEAMER);
			else
				outputdata.ports &= ~(1<<SWL_BEAMER);
		}
		else if(index == 2){ /* SWL_SCHRANK */
			if(enable)
				outputdata.ports |= (1<<SWL_SCHRANK);
			else
				outputdata.ports &= ~(1<<SWL_SCHRANK);
		}
		else if(index == 3){ /* SWL_FLIPPER */
			if(enable)
				outputdata.ports |= (1<<SWL_FLIPPER);
			else
				outputdata.ports &= ~(1<<SWL_FLIPPER);
		}

	}

	else if(room == ROOM_LOUNGE)
	{
		static can_message msg = {0x03, 0x60, 0x02, 0x02, 3, {0}};
		msg.data[0] = 0; /* switch lamp */
		msg.data[1] = index;
		msg.data[2] = enable;
		msg.addr_src = myaddr;
		can_transmit(&msg);	/* send packet to can_dimmer */
		msg.addr_dst = 0x61;
		can_transmit(&msg);	/* send packet to can_dimmer */
	}
	else if(room == ROOM_KUECHE)
	{
		if(enable)
			outputdata.ports |= (1<<SWA_KUECHE);
		else
			outputdata.ports &= ~(1<<SWA_KUECHE);
	}


	relais_control();	/* update relais status */
}

void set_lamp_all(uint8_t room, uint8_t enable)
{
	if(room > 2)
		return;

	if(room == ROOM_VORTRAG)
	{
		if(enable)
		{
			lamp_matrix[room].enabled |= 15;
			outputdata.ports |= (1<<SWL_TAFEL)|(1<<SWL_BEAMER)|(1<<SWL_FLIPPER)|(1<<SWL_SCHRANK);
		}
		else
		{
			lamp_matrix[room].enabled &= ~15;
			outputdata.ports &= ~((1<<SWL_TAFEL)|(1<<SWL_BEAMER)|(1<<SWL_FLIPPER)|(1<<SWL_SCHRANK));
		}

	}
	else if(room == ROOM_LOUNGE)
	{
		if(enable)
			lamp_matrix[room].enabled |= 31;
		else
			lamp_matrix[room].enabled &= ~31;

		static can_message msg = {0x03, 0x60, 0x02, 0x02, 3, {0}};
		msg.data[0] = 3; /* switch lamp all */
		msg.data[1] = 0;
		msg.data[2] = enable;
		msg.addr_src = myaddr;
		can_transmit(&msg);	/* send packet to can_dimmer */
		msg.addr_dst = 0x61;
		can_transmit(&msg);	/* send packet to can_dimmer */
	}
	else if(room == ROOM_KUECHE)
	{
		lamp_matrix[room].enabled = enable;
		if(enable)
			outputdata.ports |= (1<<SWA_KUECHE);
		else
			outputdata.ports &= ~(1<<SWA_KUECHE);
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
	if(room > 2)
		return;
	if(index > 7)
		return;

	lamp_matrix[room].pwmval[index] = value;

	if(room == ROOM_VORTRAG)
	{
		if(index < 4)
			outputdata.pwmval[index] = value;
	}
	else if(room == ROOM_LOUNGE)
	{
		static can_message msg = {0x03, 0x60, 0x02, 0x02, 3, {0}};
		msg.data[0] = 1; /* set brightness lamp */
		msg.data[1] = index;
		msg.data[2] = value;
		msg.addr_src = myaddr;
		can_transmit(&msg);	/* send packet to can_dimmer */
		msg.addr_dst = 0x61;
		can_transmit(&msg);	/* send packet to can_dimmer */
	}
	else if(room == ROOM_KUECHE)
	{
		outputdata.pwmval[PWM_KUECHE] = value;	/* PWM_KUECHE is 5 */
	}

	relais_control();	/* update relais status, will call twi_send() */
}

void set_bright_all(uint8_t room, uint8_t value)
{
	if(room > 2)
		return;

	if(room == ROOM_VORTRAG)
	{
		outputdata.pwmval[PWM_TAFEL] = value;
		outputdata.pwmval[PWM_BEAMER] = value;
		outputdata.pwmval[PWM_SCHRANK] = value;
		outputdata.pwmval[PWM_FLIPPER] = value;
	}
	else if(room == ROOM_LOUNGE)
	{
		static can_message msg = {0x03, 0x60, 0x02, 0x02, 3, {0}};
		msg.data[0] = 4; /* set brightness lamp all */
		msg.data[1] = 0;
		msg.data[2] = value;
		msg.addr_src = myaddr;
		can_transmit(&msg);	/* send packet to can_dimmer */
		msg.addr_dst = 0x61;
		can_transmit(&msg);	/* send packet to can_dimmer */
	}
	else if(room == ROOM_KUECHE)
	{
		outputdata.pwmval[PWM_KUECHE] = value;	/* PWM_KUECHE is 5 */
	}

	relais_control();	/* update relais status, will call twi_send() */
}

/*
* Disable relais that are not used at the moment
* PWM values are ignored
* only touch relais of ROOM_VORTRAG, ROOM_KUECHE, ROOM_LOUNGE
*/

static void relais_control() {

	if(lamp_matrix[ROOM_VORTRAG].enabled)	/* one ore more are on */
		outputdata.ports |= SWA_VORTRAG;
	else
		outputdata.ports &= ~SWA_VORTRAG;

	if(lamp_matrix[ROOM_KUECHE].enabled)	/* one ore more are on */
		outputdata.ports |= SWA_KUECHE;
	else
		outputdata.ports &= ~SWA_KUECHE;

	if(lamp_matrix[ROOM_LOUNGE].enabled)	/* one ore more are on */
		outputdata.ports |= SWA_LOUNGE;
	else
		outputdata.ports &= ~SWA_LOUNGE;

	twi_send();	/* push outputdata */

}


