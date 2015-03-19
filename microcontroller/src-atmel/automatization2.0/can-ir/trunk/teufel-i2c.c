#include <stdint.h>
#include "teufel-i2c.h"
#include "twi_master/twi_master.h"
#include "can_handler.h"

/* Nutzt die interne I²C Schnittstelle des Teufel Systems XXX um die 
 * Rückseitige Verstärkerbaugruppe anzusteuern. Das Frontpanel wird dabei ausser
 * Funktion gesetzt.
 */

/* TODO
 * use -/+ 12 V signal
 * bug in mute: front left is still on
 */

/*
 * pin1 schwarz: +12 V an -12 V aus
 * pin2 braun:   0V GND
 * pin3 rot:     +12V
 * pin4 orange: scl
 * pin5 gelb:   sda
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

#define DEFAULT_FRONT 47
#define DEFAULT_REAR 79
#define DEFAULT_SUB 62
#define DEFAULT_CENTER 70
#define DEFAULT_SIDE 0

static t_channel channels[NUM_TEUFEL_CHANNELS] = {
	{DEFAULT_REAR   , _CHANNEL(RR)  + _HL(LOW_NIBBLE)},
	{DEFAULT_SUB    , _CHANNEL(SUB) + _HL(LOW_NIBBLE)},
	{DEFAULT_REAR   , _CHANNEL(RL)  + _HL(LOW_NIBBLE)},
	{DEFAULT_CENTER , _CHANNEL(CEN) + _HL(LOW_NIBBLE)},
	{DEFAULT_FRONT  , _CHANNEL(FR)  + _HL(LOW_NIBBLE)},
	{DEFAULT_FRONT  , _CHANNEL(FL)  + _HL(LOW_NIBBLE)},
	{DEFAULT_SIDE   , _CHANNEL(SL)  + _HL(HIGH_NIBBLE)},
	{DEFAULT_SIDE   , _CHANNEL(SR)  + _HL(HIGH_NIBBLE)},
};

static const uint8_t defaultVolumes[NUM_TEUFEL_CHANNELS] = {
	DEFAULT_REAR,
	DEFAULT_SUB,
	DEFAULT_REAR,
	DEFAULT_CENTER,
	DEFAULT_FRONT,
	DEFAULT_FRONT,
	DEFAULT_SIDE,
	DEFAULT_SIDE,
};

/*
 * Der Wertebereich auf dem I2C Bus geht von 0x0 bis 0x79.
 * Dabei müssen aber Werte welche mit 0xXa -0xXf enden übersprungen werden.
 * Werte zwischen a bis f setzen die Lautstärke auf 0.
 * Wahrscheinlich ist das ein Artefakt der BCD 7 Segment Anzeige.
 *                       /
 *                     /
 *               _ _ /
 *              /|  |
 *            /  |  |
 *      _ _ /    |  |
 *     /|  |     |  |
 *   /  |  |     |  |
 * /    |__|     |__|
 *   10  6   10   6   10
 *
 */

static void writeChannel(t_channel *channel)
{
	uint8_t vol;
	if (channel->vol > 79)
		channel->vol = 79;
		
	vol = 121 - ((channel->vol / 10) * 16 + channel->vol % 10); // konvertierung von invertierem wert
	
	TWIM_Write(channel->id + (vol & 0x0f));
	TWIM_Write((channel->id ^_HL(LOW_NIBBLE)) + ((vol >> 4) & 0x0f));
}

static void writeAllChannels(void)
{
	for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i += 2) {
		if (TWIM_Start(((i < 6) ? ADDR_CHIP_1 : ADDR_CHIP_2) + TW_WRITE)) {
			writeChannel(&channels[i]);
			writeChannel(&channels[i + 1]);
		}
		else
			break;
	}
	TWIM_Stop();
	lap_send_teufel_status(channels);
}

void setAllChannels(uint8_t vol)
{
	for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++)
		channels[i].vol = vol;
	writeAllChannels();
}

void setDefaultAfterPoweron(void)
{
	for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++)
		channels[i].vol = defaultVolumes[i];
	writeAllChannels();
}

void setSingleChannel(uint8_t chanID, uint8_t vol)
{
	channels[chanID].vol = vol;
	if (TWIM_Start(((chanID < 6) ? ADDR_CHIP_1 : ADDR_CHIP_2) + TW_WRITE))
		writeChannel(&channels[chanID]);
	TWIM_Stop();
	lap_send_teufel_status(channels);
}

void incrementChannels(int8_t diff)
{
	for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++) {
		if ((((int16_t) channels[i].vol) + diff) < 0) {
			channels[i].vol = 0;
		} else if (((int16_t) channels[i].vol + diff) > 79) {
			channels[i].vol = 79;
		} else {
			channels[i].vol += diff;
		}
	}
	writeAllChannels();
}

void setMute(uint8_t muted)
{
	static uint8_t savedVolume[NUM_TEUFEL_CHANNELS] = {0xFF};
	if (muted) {
		for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++) {
			savedVolume[i] = channels[i].vol;
			channels[i].vol = 0;
		}
	} else if (savedVolume[0] != 0xFF) {
		for (uint8_t i = 0; i < NUM_TEUFEL_CHANNELS; i++) {
			channels[i].vol = savedVolume[i];
		}
		savedVolume[0] = 0xFF;
	}
	writeAllChannels();
}

static uint16_t TeufelOnCounter;
void TeufelPoweron(void)
{
	TeufelOnCounter = 1;
}

void TeufelPoweronTick(void)
{
	if (TeufelOnCounter) {
		TeufelOnCounter++;
		if (TeufelOnCounter > 10000) {
			TeufelOnCounter = 0;
			setDefaultAfterPoweron();
		}
	}
}
