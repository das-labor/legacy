#ifndef CANN
#define CANN

/*****************************************************************************
 * can messages over TCP/IP low level functions
 *
 * needs rs232can.h to encapsulate can packates
 */
#include "can.h"
#include "can-encap.h"

// for select
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct cann_conn {
	int			fd;
	struct sockaddr_in	inet_addr;
	struct cann_conn	*next;

	rs232can_msg		msg;
	char			*rcv_ptr;
	int			missing_bytes;
	int			error;
};

typedef struct cann_conn cann_conn_t;

extern int listen_socket;
extern cann_conn_t *cann_conn_head;

/*****************************************************************************
 * connection management
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

/* nonblocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *cann_get_nb(cann_conn_t *client);

/* blocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *cann_get(cann_conn_t *client);

/*****************************************************************************
 * transmit
 */

void cann_tranmit(rs232can_msg *msg);

#endif

