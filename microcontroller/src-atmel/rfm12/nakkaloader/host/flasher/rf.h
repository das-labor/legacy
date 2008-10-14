/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */


/* @description Formats a given chunk of data accordingly and hands it over to the
 * transmitting function. */
int rc_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_data);
