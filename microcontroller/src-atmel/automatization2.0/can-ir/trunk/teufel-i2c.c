
#include <stdint.h>
#include "teufel-i2c.h"
#include "twi_master/twi_master.h"
#include "can/can.h"

/* TODO
 * setze sinvolles default nach labor start
 * erst nur globe lautstärke unterstützen
 *
 */

#define ADDR_CHIP_1 0x8c
#define ADDR_CHIP_2 0x88

enum {
	LOW_NIBBLE,
	HIGH_NIBBLE
} e_LH_NIBBLE;

#define _HL(mask) ((mask) << 4)
#define _CHANNEL(mask) ((mask) << 5)

enum {
	RR	= 3,
	SUB	= 5,
	RL	= 1,
	CEN	= 0,
	FR	= 2,
	FL	=	4,
	SR	=	5,
	SL	= 1,
} e_CHANNEL;

t_vol s_volume;

static void lap_send_msg(void) {
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = 0x10;
	tx_msg->port_src = 0x06;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = 8;
	tx_msg->data[0] = s_volume.rr;
	tx_msg->data[1] = s_volume.sub;
	tx_msg->data[2] = s_volume.rl;
	tx_msg->data[3] = s_volume.cen;
	tx_msg->data[4] = s_volume.fr;
	tx_msg->data[5] = s_volume.fl;
	tx_msg->data[6] = s_volume.sr;
	tx_msg->data[7] = s_volume.sl;
	can_transmit(tx_msg);
}

void setAllChannels(uint8_t vol)
{

}

void setSingleChannel(uint8_t channel, uint8_t vol)
{

}

void writeChannel(uint8_t channel, uint8_t vol)
{
	TWIM_Write(channel + _HL(LOW_NIBBLE) + (vol & 0x0f));
	TWIM_Write(channel + _HL(HIGH_NIBBLE) + ((vol >> 4) & 0x0f));
}

void setDefaultAfterPoweron(void) {
	if (TWIM_Start(ADDR_CHIP_1 + TW_WRITE))
	{
		writeChannel(_CHANNEL(RR), s_volume.rr);
		writeChannel(_CHANNEL(SUB), s_volume.sub);
	}
	if (TWIM_Start(ADDR_CHIP_1 + TW_WRITE))
	{
		writeChannel(_CHANNEL(RL), s_volume.rl);
		writeChannel(_CHANNEL(CEN), s_volume.cen);
	}
	if (TWIM_Start(ADDR_CHIP_1 + TW_WRITE))
	{
		writeChannel(_CHANNEL(FR), s_volume.fr);
		writeChannel(_CHANNEL(FL), s_volume.fl);
	}
	if (TWIM_Start(ADDR_CHIP_2 + TW_WRITE))
	{
		writeChannel(_CHANNEL(SR), s_volume.sr);
		writeChannel(_CHANNEL(SL), s_volume.sl);
	}
	TWIM_Stop();
}


static uint16_t TeufelOnCounter;
void TeufelPoweron(void)
{
	TeufelOnCounter = 1;
}

void TeufelPoweronTick(void)
{
	if (TeufelOnCounter)
		TeufelOnCounter++;
	if (TeufelOnCounter > 10000) {
		TeufelOnCounter = 0;
		setDefaultAfterPoweron();
	}
}
