#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>

#include "rfm12_config.h"
#include "rfm12.h"
#include "nl_config.h"
#include "nl_protocol.h"

/* simple for-loop to "wrap" around an error-prone section */
// #define NL_ERRORWRAP for (i=0;i < NL_MAXFAILS || NL_MAXFAILS == 0;i++)

void (*app_ptr)(void) = (void *)0x0000;
uint8_t myaddress[NL_ADDRESSSIZE];

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

uint8_t nl_match_packet (uint8_t *in_packet)
{
	#if NL_ADDRESSSIZE == 1
	if ((*(in_packet + 1) & NL_ADDRESSMASK) != (myaddress[0] & NL_ADDRESSMASK))
		return 0;
	#elif NL_ADDRESSSIZE == 2
	if (*((uint16_t *) (in_packet + 1)) & (NL_ADDRESSMASK) != 
		(*((uint16_t *) &myaddress) & (NL_ADDRESSMASK)))
			return 0;
	#endif
	return 1;
}

void nl_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_payload)
{
	uint8_t txpacket[NL_ADDRESSSIZE + 1 + sizeof(nl_config)];
	uint8_t i = NL_ADDRESSSIZE + 1, k = 0;

	txpacket[1] = myaddress[0];

	#if NL_ADDRESSSIZE == 2
	txpacket[2] = myaddress[1];
	#endif

	txpacket[0] = in_type;

	if (in_len)
	{
		for (;k<in_len;k++)
			txpacket[i] = *(in_payload + k);
		i++;
	}
	
	rfm12_tx (i + NL_ADDRESSSIZE + 1, NL_PACKETTYPE, txpacket);
}

void nl_boot_app ( void )
{
	#if (NL_VERBOSITY >= 1)
	nl_tx_packet (NLPROTO_BOOT, 0, mypage);
	rfm12_tick();
	#endif
	
	cli();
	app_ptr();
}

int main (void)
{
	uint16_t i;
	uint8_t k, mystate = 0x00;
	uint8_t *rxbuf;
	uint8_t mypage[SPM_PAGESIZE];
	nl_config myconfig;
	nl_flashcmd mycmd;
	
	/* fill config variables */
	myconfig.pagesize = SPM_PAGESIZE;
	myconfig.rxbufsize = RFM12_RX_BUFFER_SIZE;
	myconfig.version = NL_VERSION;


	/* read address */
	for (i=0;i<NL_ADDRESSSIZE;i++)
	{
		myaddress[i] = eeprom_read_byte ((uint8_t *) i + NL_ADDRESSPOS);
	}

	rfm12_init();
	sei();

	for (i=0;i < NL_MAXFAILS || NL_MAXFAILS == 0;i++)
	{
		/* (re)transmit our configuration if master hasn't responded yet. */
		if (i & 0x000F && mystate == 0)
			nl_tx_packet (NLPROTO_SLAVE_CONFIG, sizeof(myconfig), (uint8_t *) &myconfig);

		rfm12_tick();
		
		if (rfm12_rx_status() != STATUS_COMPLETE)
			continue;
			
		rxbuf = rfm12_rx_buffer();

		if (rfm12_rx_type() != NL_PACKETTYPE
			|| !nl_match_packet (rxbuf))
		{
			rfm12_rx_clear();
			continue;
		}
			
		i=0; /* somebody cares about us. reset error counter */

		switch (rxbuf[NL_ADDRESSSIZE])
		{
			/* they're out there... */
			case NLPROTO_MASTER_EHLO:
			{
				rfm12_rx_clear();
				mystate = 1;
			}
			break;

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

					#if NL_VERBOSITY > 1
					mypage[0] = (uint8_t) ((__LINE__ >> 8));
					mypage[1] = (uint8_t) (__LINE__);

					nl_tx_packet (NLPROTO_ERROR, 2, mypage);
					#elif NL_VERBOSITY > 0
					nl_tx_packet (NLPROTO_ERROR, 0, mypage);
					#endif
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
				
				rfm12_rx_clear();
				
				k = NL_ADDRESSSIZE + 1;

				pagenum = (uint32_t) rxbuf[k++] << 24;
				pagenum += (uint32_t) rxbuf[k++] << 16;
				pagenum += (uint32_t) rxbuf[k++] << 8;
				pagenum += (uint32_t) (rxbuf[k++]);

				boot_program_page (pagenum, mypage);
				break;
			}
			break;
			
			/* jump into application */
			case NLPROTO_BOOT:
			{
				rfm12_rx_clear();

				nl_boot_app();
			}
			break;

			/* default case: return error */
			default:
			{
				rfm12_rx_clear();

				#if NL_VERBOSITY > 1
				mypage[0] = (uint8_t) ((__LINE__ >> 8));
				mypage[1] = (uint8_t) (__LINE__);

				nl_tx_packet (NLPROTO_ERROR, 2, mypage);
				#elif NL_VERBOSITY > 0
				nl_tx_packet (NLPROTO_ERROR, 0, (uint8_t *) 0x0000);
				#endif
			}
			break;
		}
	}

	nl_boot_app();	
}
