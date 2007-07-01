#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rf12.h"
#include "config.h"

unsigned short rf12_trans(unsigned short wert)
{	unsigned short werti=0;
	unsigned char i;

	cbi(RF_PORT, CS);
	for (i=0; i<16; i++)
	{	if (wert&32768)
			sbi(RF_PORT, SDI);
		else
			cbi(RF_PORT, SDI);
		werti<<=1;
		if (RF_PIN&(1<<SDO))
			werti|=1;
		sbi(RF_PORT, SCK);
		wert<<=1;
		_delay_us(0.3);
//		_delay_ms(1);
		cbi(RF_PORT, SCK);
	}
	sbi(RF_PORT, CS);
	return werti;
}

void rf12_init(void)
{
	unsigned char i;
	RF_DDR=(1<<SDI)|(1<<SCK)|(1<<CS);
	RF_PORT=(1<<CS);

//	RFM_DDR_NIRQ &= ~(RFM_PIN_NIRQ);
//	RFM_PORT_NIRQ |= RFM_PIN_NIRQ;

	for (i=0; i<10; i++)
		_delay_ms(10);			// wait until POR done

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

void rf12_ready(void)
{	cbi(RF_PORT, CS);
	while (!(RF_PIN&(1<<SDO))); // wait until FIFO ready
}
/* @brief nop around for at max ARG cycles. returns 1 on success
 */
uint8_t rf12_ready_wait(uint16_t in_cycles)
{
	uint16_t passedcycles = 0;
	cbi(RF_PORT, CS);
	while (!(RF_PIN&(1<<SDO)) && passedcycles < in_cycles) passedcycles++;
	if (passedcycles == in_cycles) return 0x00;
	return 0x01;
}


unsigned int crcUpdate(unsigned int crc, unsigned char serialData)
{
        unsigned int tmp;
        unsigned char j;

	tmp = serialData << 8;
        for (j=0; j<8; j++)
	{
                if((crc^tmp) & 0x8000)
			crc = (crc<<1) ^ 0x1021;
                else
			crc = crc << 1;
                tmp = tmp << 1;
        }
	return crc;
}

void rf12_txarray (unsigned char *data, unsigned char number)
{	
	unsigned char i;
	unsigned int crc;
	rf12_trans(0x8238);			// TX on

	rf12_ready();
	rf12_trans(0xB8AA);
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
	rf12_trans(0xB800 | number);

	PORTC ^= (_BV(PC1));
	crc = crcUpdate(0, number);
	for (i=0; i<number; i++)
	{	
		rf12_ready();
		rf12_trans(0xB800 | data[i]);
		crc = crcUpdate(crc, data[i]);
		printf(".");
	}
//	rf12_ready();
//	rf12_trans(0xB800 | (crc & 0x00FF));
	printf("sending crc: %4x\r\n", crc);
//	rf12_ready();
//	rf12_trans(0xB800 | (crc >> 8));
	rf12_ready();
	rf12_trans(0xB800 | '\r');
	rf12_ready();
	rf12_trans(0xB800 | '\n');
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();

	rf12_trans(0x8208);			// TX off
}


void rf12_txdata(uint8_t data)
{	
	unsigned char i;
	uint8_t crc = 0;

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
	
	crc = crcUpdate (0, 8);
	rf12_ready();
	rf12_trans(0xB808);
	rf12_ready();
	rf12_trans(0xB800|(data));
	crc = crcUpdate (0, data);
	PORTC ^= (_BV(PC0));
//	rf12_ready();
//	rf12_trans(0xB800|(data));
	for (i=0;i<7;i++)
	{
		rf12_ready();
		rf12_trans(0xB800|('a' + i));
		crc = crcUpdate (0, ('a' + i));
	}
	rf12_trans(0xB800 | (crc & 0x00FF));
	rf12_ready();
	rf12_trans(0xB800 | (crc >> 8));
	printf("\r\ncrc: %4x\r\n", crc);
	/*
	rf12_ready();
	rf12_trans(0xB800|(data));
	rf12_ready();
	rf12_trans(0xB800|(data));
	*/
	
	rf12_ready();
	rf12_trans(0x8208);			// TX off
}

uint8_t rf12_rxdata()
{
	uint8_t i;
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO
	rf12_ready();
	i = (rf12_trans(0xB000)); 
#ifdef RFM_DEBUG
	printf("RFM12\treceived char: '%c'\r\n", i);
#endif
	rf12_trans(0x8208);			// RX off
	return i;
}

unsigned char rf12_rxarray(unsigned char *data)
{	
	unsigned char i, number;
	unsigned int crc, crc_chk;
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO
	
	rf12_ready();
	number = rf12_trans(0xB000) & 0x00FF;
	crc_chk = crcUpdate(0, number);

	for (i=0; i<number; i++)
	{
		rf12_ready();
		data[i] = (unsigned char) (rf12_trans(0xB000) & 0x00FF);
		crc_chk = crcUpdate(crc_chk, data[i]);
	}

	rf12_ready();
	crc = rf12_trans(0xB000) & 0x00FF;
	rf12_ready();
	crc |=  rf12_trans(0xB000) << 8;

	rf12_trans(0x8208);			// RX off

	if (crc != crc_chk)
		number = 0;

	return number;
}

/* @note return values > 0xff == data received
 */

uint16_t rf12_rxdata_nb(uint16_t in_waitcycles)
{
	uint8_t i;
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO
	if (!rf12_ready_wait(in_waitcycles))
	{
//		rf12_trans(0x8208);			// RX off
		return 0x0000;
	}

	i = (rf12_trans(0xB000)); 
#ifdef RFM_DEBUG
	printf("RFM12\treceived char: '%c'\r\n", i);
#endif
	rf12_trans(0x8208);			// RX off
	return (0xff00 | i);
}
