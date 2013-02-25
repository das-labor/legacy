#ifndef _CANN_H
#define _CANN_H

#ifdef __MINGW32__
	#define USE_WINSOCK
#endif

#ifdef USE_WINSOCK
	#include <winsock.h>
#else
	#include <sys/socket.h>
	#include <sys/select.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <sys/types.h>
	#include <netdb.h>
#endif

/******************************************************************************
 * Encapsulated CAN messages over TCP/IP -- low level functions
 *
 *
 * Needs can-encap.* to encapsulate can packates
 *
 * HOST ONLY
 */

#include "can.h"
#include "can-encap.h"

/*****************************************************************************
 * Structures
 */

typedef enum {CANN_LEN, CANN_CMD, CANN_PAYLOAD} cann_state;

typedef struct cann_conn {
	int			fd;
	struct sockaddr_in	inet_addr;
	struct cann_conn	*next;

	rs232can_msg		msg;
	char			*rcv_ptr;
	unsigned char		missing_bytes;
	cann_state		state;
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
void cann_listen(char *port);

/* open connect to cand */
cann_conn_t *cann_connect(char *server, char *port);

/* set bits in fd_set and return highest FD */
int cann_fdset(fd_set *set);

/* check rset for activity in listenig socket and accept() */
cann_conn_t *cann_accept(fd_set *set);

/* close network connection */
void cann_close(cann_conn_t *client);

/* check rset for activity on existing client connections */
cann_conn_t *cann_activity(fd_set *set);


/*****************************************************************************
 * Memory Management
 */
rs232can_msg *cann_buffer_get();
void cann_free(rs232can_msg *);
void cann_conn_free(cann_conn_t *conn);

/*****************************************************************************
 * rcv
 */

/* nonblocking read from netwock -- returns 0 if no complete msg arrived */
rs232can_msg *cann_get_nb(cann_conn_t *client);

/* blocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *cann_get(cann_conn_t *client);


/*****************************************************************************
 * transmit
 */

/* transmit and free message */
void cann_transmit(cann_conn_t *connt, rs232can_msg *msg);


void cann_dumpconn();
void cann_close_errors();

#endif
