/*   MMMUX - a device access multiplexing library
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
#include "muxd.h"

int main (int argc, char* argv[])
{
	uint8_t buf[1024];
	int rv, i;
	mmmux_sctx_t *my_context;	
	
	printf ("connecting...\n");
	fflush (0);
	my_context = mmmuxd_init (MDBG_STDERR, NULL);

	if (my_context == NULL)
	{
		printf ("can't init mmmux stuff.\n");
		return - __LINE__;
	}

	printf ("context %p\n", my_context);
	
	while (42)
	{
		rv = mmmux_receive (my_context, buf, sizeof(buf));
		if (rv <= 0)
			return rv;
		
		printf ("\t");
		for (i=0;i<rv;i++)
		{
			printf ("%02X ", buf[i]);
			if (i > 0 && !(i % 16))
				printf ("\n\t");
		}
		printf ("\n");
	}
}
