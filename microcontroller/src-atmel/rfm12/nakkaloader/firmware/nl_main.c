/*   NAKKALOADER - a bootloader for AVR microcontrollers & RFM12
 *   transceivers. (firmware)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Copyright (C) 2012
 *       Soeren Heisrath (forename at surename dot org)
 *       Hans-Gert Dahmen (hansinator at das-labor dot org)
 */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include <util/crc16.h>
#include <util/delay.h>

#include "rfm12_config.h"
#include "rfm12.h"
#include "nl_config.h"
#include "../common/nl_protocol.h"



static void (*app_ptr)(void) = (void *)0x0000;
static uint8_t myaddress[NL_ADDRESSSIZE];
static nl_config myconfig = {SPM_PAGESIZE, RFM12_RX_BUFFER_SIZE, NL_VERSION};

void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;

	sreg = SREG;
	cli();

	page *= SPM_PAGESIZE;

	eeprom_busy_wait ();

	boot_page_erase (page);
	boot_spm_busy_wait ();      /* Wait until the memory is erased. */

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		boot_page_fill (page + i, *((uint16_t *) (buf + i)));
	}

	boot_page_write (page);     // Store buffer in flash page.
	boot_spm_busy_wait();       // Wait until the memory is written.

	/* re-enable flash */
	boot_rww_enable ();

	// Re-enable interrupts (if they were ever enabled).
	SREG = sreg;
}

static void nl_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_payload)
{
	#if 0
	/* this stuff doesn't seem to work, i can't see why; code looks allright... */
	rf_tx_buffer.buffer[0] = in_type;
	rf_tx_buffer.buffer[1] = myaddress[0];

	#if NL_ADDRESSSIZE == 2
		rf_tx_buffer.buffer[2] = myaddress[1];
	#endif

	if (in_len)
		memcpy(&rf_tx_buffer.buffer[NL_ADDRESSSIZE+1], in_payload, in_len);
	
	rfm12_start_tx (NL_ADDRESSSIZE + 2 + in_len, NL_PACKETTYPE);

	#else
	uint8_t txbuf[32];
	txbuf[0] = in_type;
	txbuf[1] = myaddress[0];
	#if NL_ADDRESSSIZE == 2
	txbuf[2] = myaddress[1];
	#endif
	memcpy (&txbuf[NL_ADDRESSSIZE+1], in_payload, in_len);

	rfm12_tx (NL_ADDRESSSIZE + 1 + in_len, NL_PACKETTYPE, txbuf);
	#endif
}

static void nl_boot_app ( void )
{
	#if (NL_VERBOSITY >= 100)
	nl_tx_packet (NLPROTO_BOOT, NL_ADDRESSSIZE, myaddress);
	rfm12_tick();
	#endif

	cli();

	//move interrupts back (also disables rfm12 int)
	#if MCU == atmega328p
	MCUCR = _BV(IVCE);
	MCUCR = 0x00;
	#else
	GICR = (1 << IVCE);
	GICR = 0;
	#endif

	app_ptr();
}

int main (void)
//int __attribute__ ((naked)) main (void)
{
	uint32_t i = 1;
	uint8_t k, mystate = NLPROTO_SLAVE_CONFIG;
	uint8_t *rxbuf;
	uint8_t mypage[SPM_PAGESIZE];
	uint32_t pagenum = 0;
	uint16_t crcsum = 0;
	nl_flashcmd *mycmd;

	#ifdef nl_bootloader_condition
	/* boot right into the application */
	if (!nl_bootloader_condition())
		nl_boot_app();
	#endif

	/* read address */
	for (i=0;i<NL_ADDRESSSIZE;i++)
	{
		myaddress[i] =
			eeprom_read_byte (
				(uint8_t *) (((uint8_t) i) + ((uint8_t) NL_ADDRESSPOS)));
	}

	/* move interrupt vector table to bootloader section */
	cli();

	#if MCU == atmega328p
	MCUCR = (1<<IVCE);
	MCUCR = (1<<IVSEL);
	#else
	GICR = (1<<IVCE);
	GICR = (1<<IVSEL);
	#endif
	rfm12_init();
	sei();

	while (2)
	{
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			rxbuf = rfm12_rx_buffer();

			if (rfm12_rx_type() != NL_PACKETTYPE)
			{
				rfm12_rx_clear();
				continue;
			}
			
			#if NL_ADDRESSSIZE == 2
			if (rxbuf[1] != myaddress[0] || rxbuf[2] != myaddress[1])
			#else
			if (rxbuf[1] != myaddress[0])
			#endif
			{
				rfm12_rx_clear();
				continue;
			}
			
			mystate = rxbuf[0];
			i=0; /* somebody cares about us. reset error counter */
		}


		/* (re)transmit our configuration if master hasn't responded yet. */
		if ((i & NL_RETRANSMIT_MASK) == 0)
		{
			switch (mystate)
			{
				/* do we really need the ehlo foo? we could just send our config right away.. */
				case NLPROTO_MASTER_EHLO: 
				case NLPROTO_SLAVE_CONFIG:
					nl_tx_packet (NLPROTO_SLAVE_CONFIG, sizeof(myconfig), (uint8_t *) &myconfig);
					break;

				/* master is ready to flash */
				case NLPROTO_PAGE_FILL:
					mycmd = (nl_flashcmd *) (rxbuf + NL_ADDRESSSIZE +1);
#if 1
					/* check boundaries */
					if (mycmd->addr_start + (mycmd->addr_end - mycmd->addr_start) > SPM_PAGESIZE)
					{
						#if NL_VERBOSITY > 1
						mypage[0] = (uint8_t) ((__LINE__ >> 8));
						mypage[1] = (uint8_t) (__LINE__);

						nl_tx_packet (NLPROTO_ERROR, 2, mypage);
						#elif NL_VERBOSITY > 0
						nl_tx_packet (NLPROTO_ERROR, 0, mypage);
						#endif

						nl_tx_packet (NLPROTO_ERROR, 0, mypage);
						break;
					}
#endif
					memcpy (mypage + mycmd->addr_start,
						rxbuf + NL_ADDRESSSIZE + 1 + sizeof(nl_flashcmd),
						mycmd->addr_end - mycmd->addr_start);

					for (k=mycmd->addr_start, crcsum = 0;k<mycmd->addr_end;k++)
						crcsum = _crc16_update (crcsum, *(mypage + k));
					
					mystate = NLPROTO_PAGE_CHKSUM;
					/* no break for retransmission purposes */

				case NLPROTO_PAGE_CHKSUM:
					nl_tx_packet (NLPROTO_PAGE_CHKSUM, 2, (uint8_t *) &crcsum);
					break;

				/* commit page write */
				case NLPROTO_PAGE_COMMIT:
					memcpy(&pagenum, rxbuf + NL_ADDRESSSIZE +1, sizeof(pagenum));
					boot_program_page (pagenum, mypage);
					mystate = NLPROTO_PAGE_COMMITED;
					/* intentionally no break statement here to retransmit the commit ack */

				case NLPROTO_PAGE_COMMITED:
					nl_tx_packet (NLPROTO_PAGE_COMMITED, sizeof(pagenum), (uint8_t *) &pagenum);
					break;

				case NLPROTO_BOOT:
					nl_boot_app();
					break;

			}

			rfm12_rx_clear();
		}
		else if (i == NL_MAXCOUNT)
			nl_boot_app();

		i++;
		rfm12_tick();
	}
}
