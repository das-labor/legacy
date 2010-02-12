#include "../common/protocol.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

#ifndef AIRMUX_CLIENT_H
#define AIRMUX_CLIENT_H

typedef struct
{
	int sockfd;
	uint16_t port;
	char *host;
	struct addrinfo hints;
	struct addrinfo *res;
	fd_set rfds;
	fd_set wfds;
} airmux_context_t;

/* @description Initialize context with default values
 */
airmux_context_t *airmux_init();

/* @description Close the connection & context
 */
void airmux_end (airmux_context_t *);

/* @description Connect to a server.
 * @note if any of the values dhost or dport are NULL, default values are used.
 */
int airmux_connect (airmux_context_t *, char *in_dhost, char* in_dport);
uint16_t strtoui (char *in_c);
/* @description non-blocking function to retreive the receive buffer, returns the amount
 * of bytes received.
 */
size_t airmux_rx (airmux_context_t *, void*, size_t);
size_t airmux_tx (airmux_context_t *, void*, size_t);

#define E_SOCKNOTREADY -1

#endif /* AIRMUX_CLIENT_H */
