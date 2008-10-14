/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */


int rc_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_data)
{
	uint8_t tmpbuf[512];
	if (in_len)
		memcpy (tmpbuf + 2, in_data, in_len);
	
	tmpbuf[0] = in_type;
	tmpbuf[1] = in_len;

	return rc_tx_cb (in_len + 2, tmpbuf);
}


