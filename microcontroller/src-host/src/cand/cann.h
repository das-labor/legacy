#ifndef CANN
#define CANN

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "can.h"
#include "rs232can.h"

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

/* open listening socket and initialize */
void cann_init(int port);

/* set bits in fd_set and return highest FD */
int cann_fdset(fd_set *set);

/* check rset for activity in listenig socket and accept() */
cann_conn_t *cann_new_connection(fd_set *set);

/* close network connection */
void cann_close_connection(cann_conn_t *client);

/* check rset for activity on existing client connections */
cann_conn_t *cann_activity(fd_set *set);

/* nonblocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *cann_get_nb(cann_conn_t *client);

#endif
