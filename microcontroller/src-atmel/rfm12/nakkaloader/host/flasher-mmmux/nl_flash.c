/*   NAKKALOADER - a bootloader for AVR microcontrollers & RFM12
 *   transceivers. (flasher utility)
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

#include "nl_flash.h"

void nflash_init_state (nflash_ctx_t *in_s)
{
	memset (in_s, 0x00, sizeof(nflash_ctx_t));
	in_s->state = INIT;
	in_s->fw_name = NULL;
	in_s->fw_buf = NULL;
}

int nflash_packet_match (nflash_ctx_t *in_c, size_t in_len, uint8_t *in_b)
{
	nflash_header_t *h = (nflash_header_t*) in_b;

	if (in_len < sizeof(nflash_header_t) + (in_c->addr_size - 1))
		return 0;
	
	if (h->type != NL_PACKETTYPE)
		return 0;
	
	if (in_c->addr_size == 1)
	{
		if (h->addr != *((uint16_t*) in_c->address))
		{
			dbg ("addr mismatch: %04X, %04X", h->addr, *((uint16_t*) in_c->address));
			return 0;
		}
	} else if (in_c->addr_size == 2)
	{
		uint16_t m_addr = *((uint16_t*) &in_b[sizeof(nflash_header_t) -1]);

		if (m_addr != *((uint16_t *) in_c->address))
			return 0;
	}

	return 1;
}

/* calculate the offset of the current page in the firmware buffer
 */
void *page_offset (nflash_ctx_t *in_c)
{
	return (void*) ((size_t) in_c->fw_buf + in_c->cc.pagesize * in_c->pagenum);
}

/* assemble the header and calculate the lenght argument accordingly. the length argument
 * should be the size of the payload (without nflash header).
 */
void nflash_header_set (nflash_header_t *out_h, nflash_ctx_t *in_c, size_t in_len, uint8_t in_cmd)
{
	out_h->len = in_len + sizeof(nflash_header_t) + in_c->addr_size - 1; /* minus 1 addr byte, minus 2 header bytes for rfm12 driver */
	out_h->type = NL_PACKETTYPE;
	out_h->cmd = in_cmd;
	out_h->addr = in_c->address[0];
	//printf ("sending %i bytes, header len = %i\n", in_len, out_h->len);
	//memcpy (&out_h->addr, in_c->address, in_c->addr_size);
}

/* send a page fill command
 */
void nflash_page_fill (nflash_ctx_t *in_c)
{
	uint8_t txbuf[4096];
	size_t txlen;
	//size_t end = in_c->bytes_confirmed + (in_c->cc.rxbufsize - sizeof(nflash_header_t) - sizeof(nl_flashcmd) + in_c->addr_size -1);
	size_t end = in_c->bytes_confirmed + (in_c->cc.rxbufsize - sizeof(nflash_header_t) - sizeof(nl_flashcmd) + in_c->addr_size -1);
	void *pp = &txbuf[sizeof(nflash_header_t) + in_c->addr_size - 1], *pfw;
	nl_flashcmd cmd;

	if (end > in_c->cc.pagesize)
		end = in_c->cc.pagesize;
	
	if (in_c->addr_size == 2)
		pp++;
	
	pfw = page_offset (in_c) + in_c->bytes_confirmed;

	txlen = sizeof(nl_flashcmd) + end - in_c->bytes_confirmed;
	cmd.pagenum = htole32 (in_c->pagenum);
	cmd.addr_start = htole16 (in_c->bytes_confirmed);
	cmd.addr_end = htole16 (end);
	in_c->bytes_sent = end;
	
	/* assemble packet header */
	nflash_header_set ((nflash_header_t*) txbuf, in_c, txlen, NLPROTO_PAGE_FILL);

	/* copy flash cmd struct */
	memcpy (pp, &cmd, sizeof(cmd));

	/* copy fw bytes to payload area */
	pp += sizeof (nl_flashcmd);
	memcpy (pp, pfw, end - in_c->bytes_confirmed);

	txlen += sizeof(nflash_header_t) + in_c->addr_size -1; /* raw packet length */
	mmmux_send (in_c->mc, txbuf, txlen);
	in_c->last_tx = time(NULL);
	in_c->state = AWAIT_CRC;

	dbg ("page_fill: page #%05i, %03iB confirmed, %03iB sent, txbuf %02X%02X%02X%02X%02X%02X%02X%02X\n",
		in_c->pagenum, in_c->bytes_confirmed, in_c->bytes_sent,
		((uint8_t *) txbuf)[0],
		((uint8_t *) txbuf)[1],
		((uint8_t *) txbuf)[2],
		((uint8_t *) txbuf)[3],
		((uint8_t *) txbuf)[4],
		((uint8_t *) txbuf)[5],
		((uint8_t *) txbuf)[6],
		((uint8_t *) txbuf)[7]
		);
}

void nflash_page_commit (nflash_ctx_t *in_c)
{
	uint8_t txbuf[32];
	uint32_t pagenum = htole32 (in_c->pagenum);
	uint8_t *p = &txbuf[sizeof(nflash_header_t)];
	
	memset (txbuf, 0x00, sizeof(txbuf));

	if (in_c->addr_size == 2)
		p++;

	memcpy (p, &in_c->pagenum, sizeof (pagenum));

	nflash_header_set ((nflash_header_t*) txbuf, in_c, sizeof(pagenum), NLPROTO_PAGE_COMMIT);
	mmmux_send (in_c->mc, txbuf, sizeof(nflash_header_t) + sizeof(pagenum) + (in_c->addr_size -1));
	in_c->last_tx = time(NULL);
	in_c->state = AWAIT_COMMIT;
}

void nflash_timeout_handler (nflash_ctx_t *in_c)
{
	uint8_t cmd = 0x00;
	size_t txlen = 0;
	uint8_t txbuf[64];

	if (in_c->last_tx > time(NULL) - NL_RESEND_DELAY)
		return;
	
	memset (txbuf, 0x00, sizeof(txbuf));
	
	switch (in_c->state)
	{
		case INIT:
		case AWAIT_CFG:
			/* eh dude, you still haven't told me your config (or i haven't received it). */
			in_c->state = AWAIT_CFG;
			cmd = NLPROTO_SLAVE_CONFIG;
		break;

		case FILL_PAGE:
			/* client didn't get our fill command */
			nflash_page_fill (in_c);
		break;

		case AWAIT_CRC:
			/* sorry, didn't hear you. what was the checksum again..? */
			cmd = NLPROTO_PAGE_CHKSUM;
		break;

		case AWAIT_COMMIT:
			/* COMMIT I SAID! are you dumb or deaf, huh? */
			nflash_page_commit (in_c);
			return;
	}
	
	if (cmd == 0x00)
		return;

	nflash_header_set ((nflash_header_t*) txbuf, in_c, txlen, cmd);
	mmmux_send (in_c->mc, txbuf, sizeof(nflash_header_t) + txlen + (in_c->addr_size -1));
	in_c->last_tx = time(NULL);
}

/* this function parses the given crc buffer and when successful updates the bytes_confirmed counter
 * and returns 1. if the crc sums didn't match, 0 is returned.
 */
int nflash_crc_verify (nflash_ctx_t *in_c, size_t in_len, uint8_t* in_b)
{
	uint16_t crc_local, crc_remote;
	
	if (in_len < sizeof (nflash_header_t) + sizeof (crc_local) + in_c->addr_size - 1)
		return 0;
	
	memcpy (&crc_remote, in_b + sizeof(nflash_header_t) + in_c->addr_size -1, sizeof (crc_local));
	crc_local = calc_crc (page_offset(in_c) + in_c->bytes_confirmed, in_c->bytes_sent - in_c->bytes_confirmed);
	
	if (crc_local != crc_remote)
	{
		printf ("crc mismatch: local = %04X, remote = %04X\n", crc_local, crc_remote);
		return 0;
	}
	in_c->bytes_confirmed = in_c->bytes_sent;
	return 1;
}

int nflash_handle_packet (nflash_ctx_t *in_c, size_t in_len, uint8_t *in_b)
{
	nflash_header_t *h = (nflash_header_t*) in_b;
	uint8_t cmd = 0x00, txbuf[4096];
	size_t txlen = 0;
	memset (txbuf, 0x00, sizeof(txbuf));

	if (!nflash_packet_match (in_c, in_len, in_b))
		return 0;
	
	switch (in_c->state)
	{
		case INIT:
			cmd = NLPROTO_SLAVE_CONFIG;
			in_c->state = AWAIT_CFG;
		break;
		case AWAIT_CFG:
		{
			nl_config *tc;

			if (h->cmd != NLPROTO_SLAVE_CONFIG)
			{
				printf ("error: expected slave cfg, but got %02X\n", h->cmd);
				break;
			}

			if (in_len < sizeof (nl_config) + sizeof (nflash_header_t) + in_c->addr_size -1)
				break;

			tc = (nl_config*) &in_b[sizeof(nflash_header_t) + in_c->addr_size-1];
			in_c->cc.pagesize = le16toh(tc->pagesize);
			in_c->cc.rxbufsize = MIN(28,tc->rxbufsize);
			in_c->cc.version = tc->version;

			printf ("got client config: PSZ = %u, RXSZ = %u, V = %u\n",
				in_c->cc.pagesize, in_c->cc.rxbufsize, in_c->cc.version);
			
			in_c->state = FILL_PAGE;
			/* fallthrough */
		}
		case AWAIT_CRC:
		case FILL_PAGE:
			if (in_c->bytes_sent != 0 && h->cmd != NLPROTO_PAGE_CHKSUM)
			{
				printf ("error: expected crcsum packet, but received %02X\n", h->cmd);
				/* send fill command again */
			} else if (h->cmd == NLPROTO_PAGE_CHKSUM)
			{
				nflash_crc_verify (in_c, in_len, in_b);
			}

			if (in_c->bytes_confirmed != in_c->cc.pagesize)
			{
				nflash_page_fill (in_c);
				break;
			}
			
			/* all ok, send commit command */
			nflash_page_commit (in_c);
		break;

		case AWAIT_COMMIT:
			if (h->cmd != NLPROTO_PAGE_COMMITED)
			{
				printf ("error: expected PAGE_COMMITED but received %02X\n", h->cmd);
				/* send commit again */
				nflash_page_commit (in_c);
				return 0;
			}
			
			printf ("page %i successfully flashed\n", in_c->pagenum);

			/* reset counters */
			in_c->bytes_sent = 0;
			in_c->bytes_confirmed = 0;
			in_c->pagenum++;

			if (in_c->pagenum == 1 + ceil(in_c->fw_size / in_c->cc.pagesize))
			{
				cmd = NLPROTO_BOOT;
				break;
			}
			
			/* push the next page */
			in_c->state = FILL_PAGE;
			nflash_page_fill (in_c);
			return 0;
	}

	if (cmd == 0x00)
		return 0;
	
	nflash_header_set ((nflash_header_t*) txbuf, in_c, txlen, cmd);
	mmmux_send (in_c->mc, txbuf, sizeof(nflash_header_t) + txlen + (in_c->addr_size -1));
	in_c->last_tx = time(NULL);

	if (cmd == NLPROTO_BOOT)
		return NFLASH_DONE; /* all done */
	
	return 0;
}
