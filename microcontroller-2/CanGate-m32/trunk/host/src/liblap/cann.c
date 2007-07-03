#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include "cann.h"


int listen_socket;
cann_conn_t *cann_conns_head = NULL;


#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


/*****************************************************************************
 * Utility functions -- not exportet
 */
struct in_addr *atoaddr(char *address) {
	struct hostent *host;
	static struct in_addr saddr;

	/* First try it as aaa.bbb.ccc.ddd. */
	saddr.s_addr = inet_addr(address);
	if (saddr.s_addr != -1) {
		return &saddr;
	}

	host = gethostbyname(address);
	if (host != NULL) {
		return (struct in_addr *) *host->h_addr_list;
	}
	return NULL;
}

/*****************************************************************************
 * Connection management
 */

/* open listening socket and initialize */
void cann_listen(int port)
{
	struct sockaddr_in serv_addr;
	int ret, flags;
	char one=1; 

	signal(SIGPIPE, SIG_IGN);

	ret = listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	debug_assert( ret >= 0, "Could not open listeing socket: ");

	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &one, 1);

	/* bind address */
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	ret = bind(listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	debug_assert( ret >= 0, "Could not bind listening socket"  );
	
	flags = fcntl( listen_socket, F_GETFL, 0 );
	fcntl( listen_socket, F_SETFL, flags | O_NDELAY );

	/* specify queue */ 
	listen(listen_socket, 5); 
	debug_assert( ret >= 0, "Could listen() listening socket"  );

}

/* open connect to cand */
cann_conn_t *cann_connect(char *server, int port)
{
	int ret;
	cann_conn_t *client;
	struct sockaddr_in addr;
	
	// initialize client struct
	client = (cann_conn_t *)malloc(sizeof(cann_conn_t));
	client->missing_bytes = 0;
	client->error = 0;

	// get socket
	ret = client->fd = socket(AF_INET, SOCK_STREAM, 0);
	debug_assert( ret >= 0, "Could not open socket: ");

	// connect
	memcpy( &(addr.sin_addr), atoaddr(server), sizeof(in_addr_t) );
	addr.sin_port = htons( port );
	addr.sin_family = AF_INET;

	if ( connect(client->fd, (struct sockaddr *)&addr, sizeof(addr)) <0 ) {
		debug_perror( 0, "Connect failed" );
		exit(1);
	}

	// set some options on socket
	fcntl(client->fd, F_SETFL, O_NONBLOCK);
	int flag = 1;
	setsockopt(client->fd,  
		   IPPROTO_TCP,     /* set option at TCP level */
		   TCP_NODELAY,     /* name of option */
		   (char *) &flag,  /* the cast is historical cruft */
	           sizeof(int));    /* length of option value */

	return client;
}

/* set bits in fd_set */
int cann_fdset(fd_set *set)
{
	cann_conn_t *client = cann_conns_head;
	int maxfd = listen_socket;

	// socket for new connections
	FD_SET(listen_socket, set);

	while(client) {
		if (!(client->error)) {
			FD_SET(client->fd, set);
			maxfd = max(maxfd, client->fd);
		}
		client = client->next;
	}

	return maxfd;
}

cann_conn_t *cann_accept(fd_set *set)
{
	cann_conn_t *client;

	// activity on listen_socket?
	if ( !FD_ISSET(listen_socket, set) ) 	
		return NULL;

	FD_CLR(listen_socket, set);

	// accept connection
	int len, fd;
	struct sockaddr_in remote;
	len = sizeof(struct sockaddr_in);
	fd = accept(listen_socket, (struct sockaddr*)&remote, &len);

	// set some options on socket
	fcntl(fd, F_SETFL, O_NONBLOCK);
	int flag = 1;
	setsockopt(fd,  
		   IPPROTO_TCP,     /* set option at TCP level */
		   TCP_NODELAY,     /* name of option */
		   (char *) &flag,  /* the cast is historical cruft */
	           sizeof(int));    /* length of option value */

	// initialize client struct
	client = (cann_conn_t *)malloc(sizeof(cann_conn_t));
	client->next  = cann_conns_head;
	client->fd    = fd;
	client->missing_bytes = 0;
	client->error = 0;
	cann_conns_head = client;

	return client;
}

void cann_close_errors()
{
	cann_conn_t **client = &cann_conns_head;

	while(*client) {
		if ( (*client)->error) {
			cann_conn_t *del = (*client);

			*client = del->next;
			client = &(del->next);

			debug(2, "===> Closing fd %d", del->fd);
			close(del->fd);
			free(del);

		} else {
			client = &((*client)->next);
		}
	}
}

void cann_close(cann_conn_t *conn)
{
	// close all connections?
	if (!conn) {
		conn = cann_conns_head;
		while(conn) {
			cann_conn_t *oldconn = conn;
			
			shutdown(conn->fd, SHUT_RDWR);
			close(conn->fd);
			conn = conn->next;
			free(oldconn);
		}
	}
}


cann_conn_t *cann_activity(fd_set *set)
{
	cann_conn_t *client = cann_conns_head;

	while(client) {
		if (FD_ISSET(client->fd, set) && !client->error) {
			FD_CLR(client->fd, set);
			return client;
		}
		client = client->next;
	}

	return NULL;
}


void cann_dumpconn()
{
	cann_conn_t *client = cann_conns_head;

	while(client) {
		debug(9, "CANN connection: fd=%d error=%d", client->fd, client->error);
		client = client->next;
	}
}



/*****************************************************************************
 * rcv
 */

// Nonblocking read from netwock 
//   Returns 1 on succsess, 0 if there is no complete message and -1 on error.
int cann_get_nb(cann_conn_t *client, can_message_t *msg)
{
	int     ret; 
	uint8_t c; 

	// sanity
	debug_assert( !(client->error), 
			"cann_get_nb() with error %d on %d", 
			client->error, client->fd );

	while((ret=read(client->fd, &c, 1)) == 1) { // recieved 1 byte
		if (client->missing_bytes == 0) {
			if (c >= 16) {
				debug( 2, "Protocol error on fd %d (size=%d)", 
				       client->fd, c );
				client->error = 1;
				return -1;
			}
			debug(10, "Next packet on %d: length=%d", client->fd, c);
			client->rcv_ptr = client->buf;
			client->missing_bytes = c;
			continue;
		}

		*(client->rcv_ptr) = c;
		client->rcv_ptr++;
		client->missing_bytes--;

		if (client->missing_bytes == 0) {
			switch (client->buf[0]) {
			case 0: // NOP
				break;
			case 1:
				memcpy(msg, client->buf+1, sizeof(can_message_t));
				return 1;
			default:
				debug( 2, "Protocol error on fd %d (cmd=%d)", 
				       client->fd, client->buf[0]);
				client->error = 1;
				return -1;
			}
		}
	}

	if (ret == 0) {
		debug_perror( 5, "Error readig fd %d (ret==%d)", client->fd, ret );
		client->error = 1;
		return -1;
	} else if (ret < 0) {
		if ((errno == EAGAIN) || (errno==0)) 
			return 0;
	}
}

// blocking read on netwock socket 
//   Returns 1 on succsess; -1 on error.
int cann_get(cann_conn_t *client, can_message_t *msg)
{
	int ret;
	fd_set rset;

	for(;;) {
		FD_ZERO(&rset);
		FD_SET(client->fd, &rset);

		ret = select(client->fd + 1, &rset, (fd_set*)NULL, (fd_set*)NULL, NULL);
		debug_assert( ret >= 0, "cann_get: select failed" );

		ret = cann_get_nb(client, msg);
		if (ret == 0) 
				continue;

		return ret;
	}
}


/*****************************************************************************
 * send
 */

/* transmit message */
void cann_send(cann_conn_t *conn, can_message_t *msg)
{
	uint8_t len;
	uint8_t cmd = 1;

	// sanity
	if (conn->error) {
		debug(5, "cann_transmit: not transmiting on errorous connection fd=%d", conn->fd);
		return;
	}

	len = 1 + 4 + 1 + msg->dlc;   // len = cmd + id[4] + dlc + data 

	if( send(conn->fd, &len, 1, MSG_NOSIGNAL) != 1 ) 
		goto error;

	if( send(conn->fd, &cmd, 1, MSG_NOSIGNAL) != 1 )
		goto error;

	if(send(conn->fd, msg, len-1, MSG_NOSIGNAL) != len-1 )
		goto error;

	return;
error:
	conn->error = 1;
	debug_perror( 5, "Error sending fd %d", conn->fd );
	return;

}

