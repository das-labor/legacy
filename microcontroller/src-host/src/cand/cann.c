#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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
	int ret;
	char one=1; 

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
	client->state = CANN_LEN;
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
	fcntl(fd, F_SETFL, O_NONBLOCK);

	// initialize client struct
	client = (cann_conn_t *)malloc(sizeof(cann_conn_t));
	client->next  = cann_conns_head;
	client->fd    = fd;
	client->missing_bytes = 0;
	client->state = CANN_LEN;
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

			debug(2, "Closing fd %d", del->fd);
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
		if (!(client->error) && FD_ISSET(client->fd, set) ) {
			FD_CLR(client->fd, set);
			return client;
		}
		client = client->next;
	}

	return NULL;
}

/*****************************************************************************
 * rcv
 */

/* nonblocking read on netwok -- returns msg if complete msg arrived */
rs232can_msg *cann_get_nb(cann_conn_t *client)
{
	int ret; 
	unsigned char val; 
	static enum {STATE_START, STATE_LEN, STATE_PAYLOAD} state = STATE_START;

	// sanity
	debug_assert( !(client->error), 
			"cann_get_nb() with error %d on %d", 
			client->error, client->fd );

	// XXX das alles geht auch einfacher XXX
	if (client->state == CANN_LEN) {
		ret = read(client->fd, &val, 1);
		       
		if (ret < 0)
			goto error;

		if (ret == 0)
			return NULL;

		// check msg length
		if (val > sizeof(client->msg.data)) {
			debug( 2, "Protocol error on fd %d (size=%d)", 
					client->fd, client->missing_bytes );
			client->error = 1;
			return NULL;
		}

		if (val == 0)
			return NULL;

		debug(10, "Next packet on %d: length=%d", client->fd, val);
		client->msg.len        = val;
		client->missing_bytes  = val;
		client->rcv_ptr        = client->msg.data;
		client->state          = CANN_CMD;
	}

	if (client->state == CANN_CMD) {
		ret = read(client->fd, &(client->msg.cmd), 1);
		       
		if (ret < 0)
			goto error;

		if (ret == 0)
			return NULL;

		debug(10, "Next packet on %d: cmd=%d", client->fd, client->msg.cmd);
		client->state = CANN_PAYLOAD;
	}
	
	// read data
	ret = read(client->fd, client->rcv_ptr, client->missing_bytes);

	if ((ret < 0) && (errno == EAGAIN))
		return NULL;	// blocking IO: no data available

	if (ret <= 0)
		goto error;
		
	client->missing_bytes -= ret;
	client->rcv_ptr       += ret;

	debug(10, "fd %d: recived %d bytes, %d missing",
			client->fd, ret, client->missing_bytes);

	// message complete?
	if (client->missing_bytes == 0)
			return &(client->msg);

	return NULL;

error:
	debug_perror( 5, "Error readig fd %d (ret==%d)", client->fd, ret );
	client->error = 1;
	return NULL;
}

rs232can_msg *cann_get(cann_conn_t *client)
{
	/*
	int ret;
	int complete = 0;

	// sanity
	debug_assert( !(client->error), 
			"cann_get_nb() with error (%d)", client->error );

	while(1) {
		// XXX select XXX

		if (client->missing_bytes == 0) {
		}

		// read data
		ret = read(client->fd, client->rcv_ptr, client->missing_bytes);

		if ((ret < 0) && (errno == EAGAIN))
			continue;

		if (ret <= 0)
			goto error;
		
		client->missing_bytes -= ret;
		client->rcv_ptr       += ret;

		debug(10, "fd %d: recived %d bytes, %d missing",
			client->fd, ret, client->missing_bytes);

		// message complete?
		if (client->missing_bytes == 0)
			return &(client->msg);
	}

error:
	debug_perror( 5, "Error readig fd %d (ret==%d)", client->fd, ret );
	client->error = 1;
	return NULL;
	*/
	debug(0, "cann_get()..");
}


/*****************************************************************************
 * transmit
 */

/* transmit and free message */
void cann_transmit(cann_conn_t *conn, rs232can_msg *msg)
{
	int len;

	// sanity
	debug_assert( !(conn->error), 
			"cann_get_nb() with error (%d)", conn->error );

	if( write(conn->fd, &(msg->len), 1) != 1 ) 
		goto error;
	
	if( write(conn->fd, &(msg->cmd), 1) != 1 )
		goto error;

	if( write(conn->fd, msg->data, msg->len) != msg->len )
		goto error;

	return;
error:
	conn->error = 1;
	debug_perror( 5, "Error writing fd %d", conn->fd );
	return;

}

