#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rf12.h"
#include "config.h"

#ifdef RF_USE_AVRX
	#include <avrx.h>
	#include <avrx-io.h>
	#include <avrx-signal.h>
#endif

uint16_t rf12_trans (uint16_t wert)
{
	unsigned short werti=0;
	unsigned char i;

	cbi(RF_PORT, CS);
	for (i=0; i<16; i++)
	{
		if (wert&32768)
			sbi(RF_PORT, SDI);
		else
			cbi(RF_PORT, SDI);
		werti<<=1;
		if (RF_PIN&(1<<SDO))
			werti|=1;
		sbi(RF_PORT, SCK);
		wert<<=1;
		_delay_us(0.3);
		cbi(RF_PORT, SCK);
	}
	sbi(RF_PORT, CS);
	return werti;
}

void rf12_init (void)
{
	unsigned char i;
	RF_DDR=(1<<SDI)|(1<<SCK)|(1<<CS);
	RF_PORT=(1<<CS);
	
	#ifdef RF_USE_AVRX
		AvrXDelay (&rftimer, 100);
	#else
		_delay_ms(100);
	#endif

	rf12_trans(0xC0E0);			// AVR CLK: 10MHz
	rf12_trans(0x80D7);			// Enable FIFO
	rf12_trans(0xC2AB);			// Data Filter: internal
	rf12_trans(0xCA81);			// Set FIFO mode
	rf12_trans(0xE000);			// disable wakeuptimer
	rf12_trans(0xC800);			// disable low duty cycle
	rf12_trans(0xC4F7);			// AFC settings: autotuning: -10kHz...+7,5kHz
}

void rf12_setbandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi)
{
	rf12_trans(0x9400|((bandwidth&7)<<5)|((gain&3)<<3)|(drssi&7));
}

void rf12_setfreq(unsigned short freq)
{	if (freq<96)				// 430,2400MHz
		freq=96;
	else if (freq>3903)			// 439,7575MHz
		freq=3903;
	rf12_trans(0xA000|freq);
}

void rf12_setbaud(unsigned short baud)
{
	if (baud<663)
		return;
	if (baud<5400)					// Baudrate= 344827,58621/(R+1)/(1+CS*7)
		rf12_trans(0xC680|((43104/baud)-1));
	else
		rf12_trans(0xC600|((344828UL/baud)-1));
}

void rf12_setpower(unsigned char power, unsigned char mod)
{	
	rf12_trans(0x9800|(power&7)|((mod&15)<<4));
}

/* @brief wait forever, (task-) blocking function.
 */
void rf12_ready(void)
{
	cbi(RF_PORT, CS);
	while (!(RF_PIN&(1<<SDO)))
	{
		#ifdef RF_USE_AVRX
			AvrXDelay (&rftimer, RF_WAITDELAY);
		#else
			asm volatile ("nop");
		#endif
	}
}

/* @brief wait a given time, return 1 if timeout hit, else 0
 * @note make sure to set the rfm12 into another state when a timeout is hit.
 */
uint8_t rf12_ready_timeout (uint8_t in_waitms)
{
	uint8_t i = 0;
	cbi(RF_PORT, CS);
	while (!(RF_PIN&(1<<SDO)) && i < in_waitms)
	{
		#ifdef RF_USE_AVRX
			AvrXDelay (&rftimer, RF_WAITDELAY);
		#else
			_delay_ms (RF_WAITDELAY);
		#endif
		i += RF_WAITDELAY;
	}
	if (i >= in_waitms) return 1;
	return 0;
}

void rf12_txdata(uint8_t data)
{	
	unsigned char i;

	rf12_trans(0x8238);			// TX on
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB82D);
	rf12_ready();
	rf12_trans(0xB8D4);
	
	rf12_ready();
	rf12_trans(0xB800|(data));
	PORTC ^= (_BV(PC0));
	
	rf12_ready();
	rf12_trans(0x8208);			// TX off
}

uint8_t rf12_rxbyte()
{
	uint8_t i;
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO
	rf12_ready();
	rf12_trans(0x0000);
	i = (rf12_trans(0xB000)); 
	rf12_trans(0x8208);			// RX off
	return i;
}

/* @note return value == amount of bytes actually received.
 */

uint8_t rf12_rxarray ( uint8_t *target, uint8_t maxlen )
{
	uint8_t i, timeoutcount = 0;
	
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO

	for (i=0;i<maxlen && timeoutcount == 0;i++)
	{
		timeoutcount = rf12_ready_timeout (RF_RXTIMEOUT);
		rf12_trans(0x0000);
		timeoutcount = rf12_ready_timeout (RF_RXTIMEOUT);
		*(target + i) = (uint8_t) ((rf12_trans(0xB000)) & 0x00ff);
	}

	rf12_trans(0x8208);			// RX off
	return i;
}
