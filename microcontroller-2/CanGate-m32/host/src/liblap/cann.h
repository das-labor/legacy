#ifndef CANN_H
#define CANN_H

/******************************************************************************
 * Encapsulated CAN messages over TCP/IP -- low level functions
 *
 *
 * Needs can-encap.* to encapsulate can packates
 *
 * HOST ONLY
 */
#include "liblap.h"

// for select
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*****************************************************************************
 * Structures
 */

typedef struct cann_conn {
	int			fd;
	struct sockaddr_in	inet_addr;
	struct cann_conn	*next;

	char			buf[16];
	char			*rcv_ptr;
	unsigned char		missing_bytes;
	int			error;
} cann_conn_t;

/*****************************************************************************
 * Global variables
 */
extern int listen_socket;
extern cann_conn_t *cann_conns_head;

/*****************************************************************************
 * Connection management
 */

/* open listening socket and initialize */
void cann_listen(int port);

/* open connect to cand */
cann_conn_t *cann_connect(char *server, int port);

/* set bits in fd_set and return highest FD */
int cann_fdset(fd_set *set);

/* check rset for activity in listenig socket and accept() */
cann_conn_t *cann_accept(fd_set *set);

/* close network connection */
void cann_close(cann_conn_t *client);

/* check rset for activity on existing client connections */
cann_conn_t *cann_activity(fd_set *set);


/*****************************************************************************
 * rcv
 */

// nonblocking read from netwock 
//   Returns 1 on succsess; 0 if there is no complete message and -1 on error.
int cann_get_nb(cann_conn_t *client, can_message_t *msg);

// blocking read on netwock socket 
//   Returns 1 on succsess; -1 on error.
int cann_get(cann_conn_t *client, can_message_t *msg);


/*****************************************************************************
 * transmit
 */

/* transmit and free message */
void cann_send(cann_conn_t *connt, can_message_t *msg);

#endif

