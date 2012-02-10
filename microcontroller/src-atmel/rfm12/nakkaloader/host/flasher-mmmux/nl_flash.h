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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <muxd.h>
#include <time.h>
#include <math.h>
#include <endian.h>

#pragma once

#include "crc.h"
#include "../../common/nl_protocol.h"

/* time (in seconds) before re-transmitting the last command */
#define NL_RESEND_DELAY 2

#define NFLASH_DONE 1337

typedef enum
{
	INIT,
	AWAIT_EHLO,
	AWAIT_CFG,
	FILL_PAGE,    /* in the middle of a page fill operation */
	AWAIT_CRC,    /* waiting for crc sum */
	AWAIT_COMMIT  /* waiting for commit ack */
} nflash_state_t;

typedef struct
{
	uint8_t address[2];
	size_t addr_size;
	char* fw_name;
	void* fw_buf;
	size_t fw_size;
	nflash_state_t state;
	uint32_t pagenum;        /* current page number */
	size_t bytes_sent;       /* bytes we sent for current page (= offset within page) */
	size_t bytes_confirmed;  /* amount of bytes correctly received on the client side */
	mmmux_sctx_t *mc;
	time_t last_tx;
	nl_config cc;
} nflash_ctx_t;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8_t len;  /* len field (rfm12 lib) */
	uint8_t type; /* type field (rfm12 lib) */
	uint8_t cmd;  /* nakkaflash command */
	uint8_t addr; /* address field  */
} nflash_header_t;
#pragma pack(pop)

/* init the state struct with default values */
void nflash_init_state (nflash_ctx_t *in_s);

/* returns 1 if given packet matches our context, 0 when not */
int nflash_packet_match (nflash_ctx_t *in_c, size_t in_len, uint8_t *in_b);

/* main packet handler routine / state machine
 */
int nflash_handle_packet (nflash_ctx_t *in_c, size_t in_len, uint8_t *in_b);

/* re-transmit commands if no packet has been received for some time.
 */
void nflash_timeout_handler (nflash_ctx_t *in_c);
