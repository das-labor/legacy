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
#include <string.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>

#pragma once
#include "muxd_config.h"

#define SOCKNAME_DEFAULT "/tmp/mmmuxd.sock"

#ifndef MIN
	#define MIN(a,b) ((a>b)?b:a)
#endif

extern int v;
#define dbg(f, a...) dprintf (v, "% 20s:% 22s:% 5i:\t" f "\n", __FILE__, __func__, __LINE__, ##a)
#define err(f, a...) fprintf (stderr, "% 20s:% 22s:% 5i:\t" f "\n", __FILE__, __func__, __LINE__, ##a)
#ifndef MAX
	#define MAX(a,b) ((a>b)?a:b)
#endif

#define MDBG_STDOUT -1
#define MDBG_NONE    0
#define MDBG_STDERR -2
#define MDBG_FILE   -3


typedef enum
{
	fs,
	tcp,
	hw
} mmmux_socktype_t;

typedef enum
{
	client,
	server
} mmmux_mode_t;

#pragma pack(push)
#pragma pack(1)
typedef enum
{
	data = 0,
	management = 1
} mmmux_packettype_t;

typedef struct 
{
	uint8_t version;
	mmmux_packettype_t type;
} mmmux_hdr_t;
#pragma pack(pop)

typedef struct
{
	int listenfd;
	int debugfd;
	mmmux_socktype_t type;
	mmmux_mode_t mode;
	struct sockaddr_un sl;
	struct sockaddr_un sr;
	/* read, write, master fd sets */
	fd_set fds_read;
	fd_set fds_write;
	fd_set fds_master;
	int nfds;
	int nclients;
	char sockname[256];
	char chdir[256];
	int pfds_sock[2];
	int fds_hw[256];
} mmmux_sctx_t;

mmmux_sctx_t* mmmuxd_init (int in_debug, char* in_sockname);
