/*   CGI statusmonitor - sending live status updates
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
 *   Copyright (C) 2012 Soeren Heisrath (forename at surename dot org)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <muxd.h>

#include "../../common/protocol.h"

const char* cmd_to_str (ldc_header_t *in_hdr)
{
	switch (in_hdr->ltype)
	{
		case LDC_CMD_LIGHT_GET:
			return (const char*) "light_get";

		case LDC_CMD_SPEED_GET:
			return (const char*) "speed_get";
		
		case LDC_CMD_TARGET_SPEED_GET:
			return (const char*) "target_speed_get";

		default:
			return (const char*) "unknown_command";
	}
}

int main (int argc, char* argv[])
{
	uint8_t buf[1024];
	int rv, i;
	mmmux_sctx_t *my_context;	
	
	my_context = mmmuxd_init (MDBG_NONE, NULL);
	memset (buf, 0x00, sizeof(buf));

	if (my_context == NULL)
	{
		printf ("HTTP/1.0 500 ERROR\r\n\r\ncan't init mmmux stuff.\n");
		return - __LINE__;
	}

	
	printf ("HTTP/1.0 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Expires: -1\r\nPragma: no-cache\r\n\r\n");
	
	fflush (0);

	while (42)
	{
		ldc_header_t *lh;
		rfm12_header_t *rh;
		rv = mmmux_receive (my_context, buf, sizeof(buf));
		if (rv <= 0)
			return rv;
		
		rh = (rfm12_header_t *) buf;
		lh = (ldc_header_t *) (buf + sizeof(rfm12_header_t));

		if (rh->type != LDC_TYPE)
		{
			/* not a locodoco packet */
			printf ("\"non-ldc\":\"1\",");
		} else
		{
			/* ldc command */
			printf ("{\"cmd\":\"%s\",", cmd_to_str(lh));
			printf ("\"addr\":\"%u\",", lh->addr);
		}

		printf ("\"debuginfo\":{\"rawdata\":\"");
		for (i=0;i<rv;i++)
		{
			printf ("%02X ", buf[i]);
		}
		printf ("\"} };\r\n");
		fflush(0);
	}
}
