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

#include "nl_flash.h"
#include "nl_firmware.h"

int v = 0;

void print_help (char *in_name)
{
	printf (
		"usage: %s <args> <address> <firmware-file>\n"
		"\t<address> may be specified in either hex or decimal notation. (\"0xA7\" or \"182\")\n"
		"\t<firmware-file> is the binary image to send to the target device\n"
		"\targs:\n"
		"\t\t -v\tprint debug info (to stderr)\n"
		, in_name
	);
}

int parse_address (char* in_arg)
{
	if (in_arg[0] < '0' || in_arg[0] > '9')
		return -1;

	if (strlen(in_arg) > 2 &&
		(in_arg[1] == 'x' || in_arg[1] == 'X'))
	{
		int out_addr = 0;
		sscanf (&in_arg[2], "%x", &out_addr);
		return out_addr;
	} else
	{
		return atoi (in_arg);
	}
}

int parse_args (nflash_ctx_t* in_c, int argc, char *argv[])
{
	int i, offset = 0, rv;

	if (argc < 2)
	{
		return -1;
	}
	
	for (i=0;i<argc-2;i++)
	{
		if (!strncmp (argv[i], "-v", 2))
			v = 1;

		offset++;
	}
	
	rv = parse_address (argv[offset++]);
	if (rv < 0)
		return rv;
	
	*((uint16_t *) in_c->address) = (uint16_t) rv;

	if (rv <= 0xff)
		in_c->addr_size = 1;
	else 
		in_c->addr_size = 2;
	
	if (nflash_read_binary (in_c, argv[offset]) <= 0)
		return - __LINE__;
	
	return 0;
}

int main (int argc, char* argv[])
{
	uint8_t buf[4096];
	int rv, i;
	nflash_ctx_t my_ctx;
	nflash_init_state (&my_ctx);
	
	if (parse_args(&my_ctx, argc, argv) < 0)
	{
		print_help(argv[0]);
		return -1;
	}
	
	if (v)
		my_ctx.mc = mmmuxd_init (MDBG_STDERR, NULL);
	else
		my_ctx.mc = mmmuxd_init (MDBG_NONE, NULL);

	if (my_ctx.mc == NULL)
	{
		printf ("can't init mmmux stuff.\n");
		return - __LINE__;
	}

	/* run timeout handler once to transmit EHLO packet */
	nflash_timeout_handler (&my_ctx);
	
	while (42)
	{
		rv = mmmux_receive (my_ctx.mc, buf, sizeof(buf));
		if (rv <= 0)
			return rv;
		
		if (nflash_handle_packet (&my_ctx, rv, buf) == NFLASH_DONE)
		{
			printf ("%s done.\n", argv[0]);
			return 0;
		}
		
		nflash_timeout_handler (&my_ctx);
	}
}
