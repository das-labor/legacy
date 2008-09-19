#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

#include "rfm12_config.h"
#include "rfm12.h"
#include "nl_config.h"
#include "nl_protocol.h"

#ifndef MAX
	#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/* simple for-loop to "wrap" around an error-prone section */
// #define NL_ERRORWRAP for (i=0;i < NL_MAXFAILS || NL_MAXFAILS == 0;i++)

void (*app_ptr)(void) = (void *)0x0000;

void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;

	sreg = SREG;
	cli();

	eeprom_busy_wait ();

	boot_page_erase (page);
	boot_spm_busy_wait ();      /* Wait until the memory is erased. */

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little-endian word.

		uint16_t w = *buf++;
		w += (*buf++) << 8;

		boot_page_fill (page + i, w);
	}

	boot_page_write (page);     // Store buffer in flash page.
	boot_spm_busy_wait();       // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable ();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
	sei();
}

uint8_t nl_match_packet (uint8_t *in_packet, uint8_t *in_address, uint8_t *in_mask)
{
	uint8_t k = 0;

	for (;k<NL_ADDRESSSIZE;k++)
		if ((*(in_packet + k) & in_mask[k]) != (in_mask[k] & in_address[k])) k = 0xff;
	
	return (k == 0xff) ? 0 : 1;
}

int main (void)
{
	uint16_t i;
	uint8_t k, mystate = 0x00;
	uint8_t *rxbuf;
	uint8_t txpacket[MAX((NL_ADDRESSSIZE + 4), sizeof(nl_config))];
	uint8_t myaddress[NL_ADDRESSSIZE], msk[NL_ADDRESSSIZE];
	uint8_t mypage[SPM_PAGESIZE];
	nl_config myconfig;
	nl_flashcmd mycmd;

	/* read address */
	for (i=NL_ADDRESSPOS;i<NL_ADDRESSPOS + NL_ADDRESSSIZE;i++)
	{
		myaddress[i] = eeprom_read_byte ((uint8_t *) i);
		txpacket[i] = myaddress[i];
		msk[i] = (NL_ADDRESSMASK >> (8*i));
	}
	
	txpacket[NL_ADDRESSSIZE] = NLPROTO_WAIT;

	rfm12_init();
	sei();

	/*
	 * BOOT WAIT SEQUENCE
	 *
	 */
	for (i=0;i<NL_BOOTDELAY * 10;i++)
	{
		/* add current counter value to request */
		txpacket[NL_ADDRESSSIZE + 2] = (i >> 8);
		txpacket[NL_ADDRESSSIZE + 3] = (i & 0xff);
		
		rfm12_tick();

		/* ask if somebody is out there... */
		rfm12_tx (sizeof(txpacket), NL_PACKETTYPE, txpacket);
		
		/* give the host time to respond */
		_delay_ms(10);

		if (rfm12_rx_status() != STATUS_COMPLETE)
			continue;

		rxbuf = rfm12_rx_buffer();
		
		/* check if packet is for us */
		if (rfm12_rx_type() == NL_PACKETTYPE
				&& nl_match_packet (rxbuf, (uint8_t *) &myaddress, (uint8_t *) &msk))
		{
			/* response from master - exit the loop */
			if (rxbuf[NL_ADDRESSSIZE] == NLPROTO_MASTER_EHLO)
			{
				rfm12_rx_clear();
				break;
			}
		}
		rfm12_rx_clear(); /* free the packet buffer for the next one */
	}
	
	/* no response - jump into application */
	if (++i >= NL_BOOTDELAY)
	{
		cli();
		app_ptr();
	}
	/*
	 * CONFIGURATION HANDSHAKE
	 *
	 */
	k = NL_ADDRESSSIZE;

	txpacket[k++] = NLPROTO_SLAVE_CONFIG;
	myconfig.pagesize = SPM_PAGESIZE;
	myconfig.rxbufsize = RFM12_RX_BUFFER_SIZE;
	myconfig.version = NL_VERSION;
	memcpy (txpacket + k, &myconfig, sizeof(myconfig));

	for (i=0;i < NL_MAXFAILS || NL_MAXFAILS == 0;i++)
	{
		/* (re)transmit our configuration if master hasn't responded yet. */
		if (i & 0x10 && mystate == 0)
			rfm12_tx (sizeof(txpacket), NL_PACKETTYPE, txpacket);

		rfm12_tick();
		
		if (rfm12_rx_status() != STATUS_COMPLETE)
			continue;
			
		rxbuf = rfm12_rx_buffer();

		if (rfm12_rx_type() != NL_PACKETTYPE
			|| !nl_match_packet (rxbuf, (uint8_t *) &myaddress, (uint8_t *) &msk))
			continue;
			
		i=0; /* somebody cares about us. reset error counter */

		switch (rxbuf[NL_ADDRESSSIZE])
		{
			/* master is ready to flash */
			case NLPROTO_PAGE_FILL:
			{

				mystate = (mystate == 0) ? 1 : mystate;

				k = NL_ADDRESSSIZE + 1;
				memcpy (&mycmd, rxbuf + k, sizeof(nl_flashcmd));
				
				/* check boundaries */
				if (mycmd.addr_start + (mycmd.addr_end - mycmd.addr_start) > SPM_PAGESIZE)
				{
					rfm12_rx_clear();

					txpacket[k++] = NLPROTO_ERROR;
					txpacket[k++] = (uint8_t) ((__LINE__ >> 8));
					txpacket[k++] = (uint8_t) (__LINE__);
					rfm12_tx (k, NL_PACKETTYPE, txpacket);
					break;
				}
				memcpy (&mypage + mycmd.addr_start, rxbuf,
					mycmd.addr_end - mycmd.addr_start);
				rfm12_rx_clear();
				break; /* TODO: return checksum to master */	
			}
			break;

			/* commit page write */
			case NLPROTO_PAGE_COMMIT:
			{
				uint32_t pagenum = 0;
				
				k = NL_ADDRESSSIZE + 1;

				pagenum = (uint32_t) rxbuf[k++] << 24;
				pagenum += (uint32_t) rxbuf[k++] << 16;
				pagenum += (uint32_t) rxbuf[k++] << 8;
				pagenum += (uint32_t) (rxbuf[k++]);
				rfm12_rx_clear();

				boot_program_page (pagenum, mypage);
				break;
			}
			break;
			
			/* jump into application */
			case NLPROTO_BOOT:
			{
				rfm12_rx_clear();

				#if (NL_VERBOSITY >= 1)
				k = NL_ADDRESSSIZE;

				txpacket[k++] = NLPROTO_BOOT;

				rfm12_tx (k, NL_PACKETTYPE, txpacket);
				rfm12_tick();
				_delay_ms(100);
				#endif

				cli();
				app_ptr();
			}
			break;

			/* default case: return error */
			default:
			{
				rfm12_rx_clear();

				for (k = 0;k<NL_ADDRESSSIZE;k++)
					txpacket[k] = myaddress[k];
				
				txpacket[k++] = NLPROTO_ERROR;
				txpacket[k++] = (uint8_t) ((__LINE__ >> 8));
				txpacket[k++] = (uint8_t) (__LINE__);
				rfm12_tx (k, NL_PACKETTYPE, txpacket);
			}
			break;
		}
	}
	return 0;
}
