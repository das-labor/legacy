/* Utility functions for dumping stuff on the console.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */

#include <stdlib.h>
#include <stdint.h>

inline uint8_t sanitize_char (uint8_t in_char)
{
	if (in_char >= '0' && in_char <= '9') return in_char;
	if (in_char >= 'a' && in_char <= 'z') return in_char;
	if (in_char >= 'A' && in_char <= 'Z') return in_char;

	switch (in_char)
	{
		case '!':
		case '"':
		case '*':
		case '#':
		case '\\':
		case '/':
		case '%':
		case '\'':
		case '+':
		case '-':
		case '.':
		case ':':
		case '_':
		case ';':
		case '$':
		case ',':
		case '?':
		case '=':
		case '<':
		case '|':
		case '>':
		case '[':
		case ']':
		case '(':
		case ')':
		case '&':
		case '@':
		case ' ':
		case '^':
		case '}':
		case '{':
		case '\n':
		case '\r':
			return in_char;
		
		default:
			return ' ';
	}
}
/*
void dump_stdio (uint_fast16_t in_len, uint8_t *in_buf)
{
	uint_fast16_t i;
	uint_fast8_t dumpmode;

	dumpmode = conf_handler (CONF_DUMPMODE, NULL);

	for (i=0;i<in_len;i++)
	{
		if (dumpmode == CONF_DUMPMDOE_HEX)	
			printf ("%02X", buffer[BUF_IN][i]);
		else
			printf ("%c", sanitize_char(buffer[BUF_IN][i]));

		if (i % (conf_handler(CONF_TERMWIDTH, NULL) / 2) == 0) printf("\r\n");
	}
}
*/
