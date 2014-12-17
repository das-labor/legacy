
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
	HIGH_NIBBLE,
	LOW_NIBBLE,
} e_LH_NIBBLE;

#define _HL(mask) ((mask) << 4)
#define _CHANNEL(mask) ((mask) << 5)

enum {
	RR	= 3,
	SUB	= 5,
	RL	= 1,
	CEN	= 0,
	FR	= 2,
	FL	= 4,
	SL	= 5,
	SR	= 1,
} e_CHANNEL;

#define DEFAULT_VOL 60

t_channel channels[8] = {
	{DEFAULT_VOL, _CHANNEL(RR) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(SUB) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(RL) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(CEN) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(FR) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(FL) + _HL(LOW_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(SL) + _HL(HIGH_NIBBLE)},
	{DEFAULT_VOL, _CHANNEL(SR) + _HL(HIGH_NIBBLE)},
};

static void lap_send_msg(void) {
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = 0x10;
	tx_msg->port_src = 0x06;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = 8;
	for (uint8_t i = 0; i < 8; i++)
		tx_msg->data[i] = channels[i].vol;
	can_transmit(tx_msg);
}

void setAllChannels(uint8_t vol)
{

}

static void writeChannel(t_channel *channel)
{
	uint8_t vol = 121 - channel->vol;
	TWIM_Write(channel->id + (vol & 0x0f));
	TWIM_Write((channel->id ^ _HL(LOW_NIBBLE)) + ((vol >> 4) & 0x0f));
}

void setSingleChannel(uint8_t chanID, uint8_t vol)
{
	channels[chanID].vol = vol;
	if (TWIM_Start(((chanID < 6) ? ADDR_CHIP_1 : ADDR_CHIP_2) + TW_WRITE)) {
		writeChannel(&channels[chanID]);
	}
	TWIM_Stop();
}

void setDefaultAfterPoweron(void) {
	for (uint8_t i = 0; i < 8; i += 2) {
		if (TWIM_Start(((i < 6) ? ADDR_CHIP_1 : ADDR_CHIP_2) + TW_WRITE)) {
			writeChannel(&channels[i]);
			writeChannel(&channels[i + 1]);
		}
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
