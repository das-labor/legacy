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

#include "nl_firmware.h"

int nflash_read_binary (nflash_ctx_t *in_c, char* in_name)
{
	int fp, e, rv;
	struct stat s;
	size_t brem, off = 0;

	fp = open (in_name, O_RDONLY);
	e = errno;

	if (fp < 0)
	{
		printf ("can't read file '%s': %s\n",
			in_name, strerror(e));
		return - __LINE__;
	}

	rv = stat (in_name, &s);
	in_c->fw_buf = malloc (s.st_size + 1024); /* + 1024 to fill the page to the last byte */
	memset (in_c->fw_buf, 0xFF, s.st_size + 1024);
	brem = s.st_size;
	
	while (brem)
	{
		rv = read (fp, in_c->fw_buf + off, brem);
		e = errno;
		if (rv < 0)
		{
			printf ("error reading firmware file: %s\n", strerror(e));
			return - __LINE__;
		}

		if (!rv)
			break;
		
		off += rv;
		brem -= rv;
	}

	in_c->fw_size = s.st_size;
	dbg ("fw bytes: %02X%02X%02X%02X",
		((uint8_t *) in_c->fw_buf)[0],
		((uint8_t *) in_c->fw_buf)[1],
		((uint8_t *) in_c->fw_buf)[2],
		((uint8_t *) in_c->fw_buf)[3]
		);

	return in_c->fw_size;
}
