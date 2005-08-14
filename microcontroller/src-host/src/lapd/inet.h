#ifndef LAPDINET
#define LAPDINET

#include "can.h"
#include "rs232can.h"


struct net_client {
	int			fd;
	struct net_client	*next;
};

typedef struct net_client net_client_t;



/* open listening socket and initialize */
void net_init(int port);

/* set bits in fd_set and return highest FD */
int net_fdset(fd_set *set);

/* check rset for activity in listenig socket and accept() */
net_client_t *net_new_connection(fd_set *set);

/* check rset for activity on existing client connections */
net_client_t *net_client_activity(fd_set *set);

/* nonblocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *net_get_nb(net_client_t *client);

#endif
