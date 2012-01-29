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

#define dbg(f, a...) printf ("% 20s:% 22s:% 5i:\t" f "\n", __FILE__, __func__, __LINE__, ##a)
#define err(f, a...) fprintf (stderr, "% 20s:% 22s:% 5i:\t" f "\n", __FILE__, __func__, __LINE__, ##a)
#ifndef MAX
	#define MAX(a,b) ((a>b)?a:b)
#endif

static int v = 0;

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
	int pfds_hw[2];
} mmmux_sctx_t;
