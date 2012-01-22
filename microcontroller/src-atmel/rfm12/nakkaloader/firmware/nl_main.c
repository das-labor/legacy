#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include <util/crc16.h>

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

#if 0
static uint8_t nl_match_packet (uint8_t *in_packet)
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
#endif

static void nl_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_payload)
{
	rf_tx_buffer.buffer[0] = in_type;
//	rf_tx_buffer.buffer[0] = myaddress[0];
	rf_tx_buffer.buffer[1] = 0xff;

	#if NL_ADDRESSSIZE == 2
		rf_tx_buffer.buffer[2] = myaddress[1];
	#endif

	if (in_len)
		memcpy(&rf_tx_buffer.buffer[NL_ADDRESSSIZE+1], in_payload, in_len);

	rfm12_start_tx (NL_ADDRESSSIZE + 1 + in_len, NL_PACKETTYPE);
}

static void nl_boot_app ( void )
{
	#if (NL_VERBOSITY >= 100)
	nl_tx_packet (NLPROTO_BOOT, NL_ADDRESSSIZE, myaddress);
	rfm12_tick();
	#endif

	cli();

	//move interrupts back (also disables rfm12 int)
	GICR = (1 << IVCE);
	GICR = 0;

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

#if 0
	/* read address */
	/*for (i=0;i<NL_ADDRESSSIZE;i++)
	{
		myaddress[i] =
			eeprom_read_byte (
				(uint8_t *) (((uint8_t) i) + ((uint8_t) NL_ADDRESSPOS)));
	}*/

	myaddress[0] = 0xff;
#endif

	/* move interrupt vector table to bootloader section */

	GICR = (1<<IVCE);
	GICR = (1<<IVSEL);
	rfm12_init();
	sei();

//	for (i=0;i < NL_MAXFAILS || NL_MAXFAILS == 0;i++)
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

			mystate = rxbuf[0];
			i=0; /* somebody cares about us. reset error counter */
		}


		/* (re)transmit our configuration if master hasn't responded yet. */
		if ((i & 0x3fff) == 0)
		{
			switch (mystate)
			{
				case NLPROTO_SLAVE_CONFIG:
					nl_tx_packet (NLPROTO_SLAVE_CONFIG, sizeof(myconfig), (uint8_t *) &myconfig);
					break;

				case NLPROTO_MASTER_EHLO:
					nl_tx_packet (NLPROTO_MASTER_EHLO, 0, mypage);
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
					//crcsum = mycmd->addr_start;

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
		else if (i == 0x1ffff)
			nl_boot_app();

		i++;
		rfm12_tick();
	}
}
