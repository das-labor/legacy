/*   CGI locodoco controller - send commands to locos
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
#include <string.h>
#include <muxd.h>

#include "../../common/protocol.h"

static uint8_t txpacket[1024];
static size_t txlen = sizeof(ldc_header_t) + sizeof(rfm12_header_t);

/* find 1st occurance of needle or return NULL */
char *str_search (char* in_h, const char* in_n)
{
	size_t len_n = strlen (in_n), len_h = strlen(in_h);
	char *p = in_h;
	int i, k=0;

	for (i=0; i<len_h; i++, p++)
	{
		if (*p != in_n[k])
		{
			k = 0;
			continue;
		}

		k++;
		if (k == len_n)
			return p - len_n + 1;
	}
	
	return NULL;
}

/* find 1st number in string and return it */
int parse_atoi (char* in_str)
{
	char *p = in_str;
	uint_fast8_t intfound = 0;
	int i, res = 0, mul = 1;
	size_t len;

	len = strlen(in_str);

	for (i=0;i<len;i++, p++)
	{
		if ((*p <= ' ' || *p > 'z') && !intfound)
		{
			return -1;
		}
		
		if (!(*p >= '0' && *p <= '9') && intfound)
		{
			break;
		}

		if (*p >= '0' && *p <= '9')
		{
			intfound = 1;
		}
	}
	p--;

	while (*p >= '0' && *p <= '9')
	{
		res += mul * (*p - '0');
		mul *= 10;
		p--;
	}

	return res;
}

int match_cmd (char *in_s)
{
	if (str_search(in_s, "stop") == in_s)
		return LDC_CMD_STOP;

	if (str_search(in_s, "resume") == in_s)
		return LDC_CMD_RESUME;
	
	if (str_search(in_s, "coupler_get") == in_s)
		return LDC_CMD_COUPLER_GET;

	if (str_search(in_s, "coupler_set") == in_s)
		return LDC_CMD_COUPLER_SET;

	if (str_search(in_s, "target_speed_set") == in_s)
		return LDC_CMD_TARGET_SPEED_SET;

	if (str_search(in_s, "target_speed_get") == in_s)
		return LDC_CMD_TARGET_SPEED_GET;

	if (str_search(in_s, "speed_get") == in_s)
		return LDC_CMD_SPEED_GET;

	if (str_search(in_s, "speed_set") == in_s)
		return LDC_CMD_SPEED_SET;

	if (str_search(in_s, "reboot") == in_s)
		return LDC_CMD_REBOOT;

	return -1;
}

/* parse the query string */
int parse_qstr ()
{
	char q[4096];
	char *qs = getenv ("QUERY_STRING");
	char *t;
	ldc_header_t *hdr = (ldc_header_t*) (txpacket + sizeof(rfm12_header_t));
	
	if (q == NULL)
	{
		return -1;
	}
	strncpy (q, qs, sizeof(q));
	
	/* target address */
	t = str_search (q, "target=");
	if (t != NULL)
	{
		int t_int;
		t_int = parse_atoi(t);

		if (t_int < 0)
			hdr->addr = (ldc_addr_t) -1;
		else
			hdr->addr = (ldc_addr_t) t_int;
	}

	/* command */
	t = str_search (q, "cmd=");
	if (t != NULL && strlen(t) > 4)
	{
		int c = match_cmd (t+4);

		if (c < 0)
			return -1;

		hdr->ltype = (uint8_t) c;
	} else /* this one's critical - bail out if we can't find it */
	{
		return -1;
	}

	/* additional payload: integer */
	t = str_search (q, "u16val=");
	if (t != NULL)
	{
		int v = parse_atoi(t + 7);

		*((uint16_t*) (txpacket + sizeof(ldc_header_t) + sizeof(rfm12_header_t))) = (uint16_t) v;
		txlen += 2;
		printf ("u16v = %i\n", v);
	}
	
	return 0;
}

int main (int argc, char* argv[])
{
	int rv, i;
	mmmux_sctx_t *my_context;	
	
	my_context = mmmuxd_init (MDBG_STDERR, NULL);
	memset (txpacket, 0x00, sizeof(txpacket));

	if (my_context == NULL)
	{
		printf ("HTTP/1.0 500 ERROR\r\n\r\ncan't init mmmux stuff.\n");
		return - __LINE__;
	}
	
	printf ("HTTP/1.0 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Expires: -1\r\nPragma: no-cache\r\n\r\n");
	
	fflush (0);
	
	if (parse_qstr() >= 0)
	{
		ldc_header_t *lh = (ldc_header_t *) (txpacket + sizeof(rfm12_header_t));
		rfm12_header_t *rh = (rfm12_header_t *) txpacket;

		rh->len = txlen - sizeof(rfm12_header_t);
		rh->type = LDC_TYPE;

		rv = mmmux_send (my_context, txpacket, txlen);
		if (rv <= 0)
		{
			printf ("ERROR sending command: %i\r\n", rv);
			return rv;
		}
		printf ("OK, cmd = %02X, dst = %04X<br />\r\n", lh->ltype, lh->addr);

		for (i=0;i<txlen;i++)
			printf ("%02X ", txpacket[i]);

		printf ("\r\n");
		return 0;
	}

	printf ("ERROR parsing arguments: '%s'\r\n", getenv("QUERY_STRING"));
	return -1;
}

