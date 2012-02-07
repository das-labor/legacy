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

#include "muxd.h"
#include <unistd.h>

#pragma once
void mmmux_sigh_cleanup (int in_s);
int mmmux_server_sock_task (mmmux_sctx_t *c);
void mmmux_server_drop_privs (mmmux_sctx_t *in_c);
int mmmux_server_connect (mmmux_sctx_t *in_c);
void mmmux_server_disconnect (mmmux_sctx_t *in_c, int in_fd);
int mmmux_server_handle_data (mmmux_sctx_t *in_c);
int mmmux_server_create_socket (mmmux_sctx_t *in_c);
