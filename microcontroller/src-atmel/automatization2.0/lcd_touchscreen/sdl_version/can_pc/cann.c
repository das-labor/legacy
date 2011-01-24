#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#define WINDOWS

#ifdef WINDOWS
	#include <winsock.h>
#else
	#include <sys/socket.h>
	#include <sys/select.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <netdb.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include "debug.h"

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

	#ifndef WINDOWS
		signal(SIGPIPE, SIG_IGN);
	#endif

	ret = listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	debug_assert( ret >= 0, "Could not open listeing socket: ");

	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &one, 1);

	/* bind address */
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	ret = bind(listen_socket, (struct sockaddr *)&serv_addr, sizeof(struct in_addr));
	debug_assert( ret >= 0, "Could not bind listening socket"  );
	
	#ifndef WINDOWS
		flags = fcntl( listen_socket, F_GETFL, 0 );
		fcntl( listen_socket, F_SETFL, flags | O_NDELAY );
	#endif

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
	client = malloc(sizeof(cann_conn_t));
	if (client == NULL)
	{
		printf("ERROR: Could not allocate client buffer!\n");
		free(client);
		exit(EXIT_FAILURE);
	}
	client->state = CANN_LEN;
	client->missing_bytes = 0;
	client->error = 0;

	#ifdef WINDOWS
		WSADATA wsaData;
    	int err;

	    err = WSAStartup(0x0202, &wsaData);
	    if (err != 0) {
	        /* Tell the user that we could not find a usable */
	        /* Winsock DLL.                                  */
	        printf("WSAStartup failed with error: %d\n", err);
	        return 1;
	    } 
	#endif

	// get socket
	ret = client->fd = socket(AF_INET, SOCK_STREAM, 0);
	debug_assert(ret >= 0, "Could not open socket: ");

	// connect
	memcpy(&(addr.sin_addr), atoaddr(server), sizeof(addr.sin_addr));
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	if (connect(client->fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		debug_perror(0, "Connect failed");
		free(client);
		exit(EXIT_FAILURE);
	}

	#ifdef WINDOWS
		u_long option = 1;
		ioctlsocket(client->fd, FIONBIO, &option);
	#else
		// set some options on socket
		fcntl(client->fd, F_SETFL, O_NONBLOCK);
	#endif
	
	int flag = 1;
	setsockopt(client->fd,  
			   IPPROTO_TCP,     /* set option at TCP level */
			   TCP_NODELAY,     /* name of option */
			   (char *) &flag,  /* the cast is historical cruft */
		        sizeof(int));    /* length of option value */
	
	debug(9, "connected to server, fd=%d\r\n", client->fd);

	return client;
}

/* set bits in fd_set */
int cann_fdset(fd_set *set)
{
	cann_conn_t *client = cann_conns_head;
	int maxfd = listen_socket;

	// socket for new connections
	FD_SET(listen_socket, set);

	while (client) {
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

	#ifndef WINDOWS
		// set some options on socket
		fcntl(fd, F_SETFL, O_NONBLOCK);
	#endif

	int flag = 1;
	setsockopt(fd,  
		   IPPROTO_TCP,     /* set option at TCP level */
		   TCP_NODELAY,     /* name of option */
		   (char *) &flag,  /* the cast is historical cruft */
	           sizeof(int));    /* length of option value */

	// initialize client struct
	client = (cann_conn_t *)malloc(sizeof(cann_conn_t));
	if (client == NULL)
	{
		printf("ERROR: Could not allocate client buffer!\n");
		exit(EXIT_FAILURE);
	}
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

	while (*client) {
		if ( (*client)->error) {
			cann_conn_t *del = (*client);

			*client = del->next;
			//client = &(del->next);

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
		printf("trying to close socket");
		conn = cann_conns_head;
		while (conn) {
			cann_conn_t *oldconn = conn;
			printf("close socket");
			#ifndef WINDOWS
				shutdown(conn->fd, SHUT_RDWR);
			#endif
			close(conn->fd);
			conn = conn->next;
			free(oldconn);
		}
	}
}


cann_conn_t *cann_activity(fd_set *set)
{
	cann_conn_t *client = cann_conns_head;

	while (client) {
		if (FD_ISSET(client->fd, set) && !client->error) {
			FD_CLR(client->fd, set);
			return client;
		}
		client = client->next;
	}

	return NULL;
}


/*****************************************************************************
 * Memory Management
 */

rs232can_msg *cann_buffer_get()
{
	rs232can_msg *cmsg = malloc(sizeof(rs232can_msg));
	if (cmsg == NULL)
	{
		printf("ERROR: cann_buffer_get malloc fail!\n");
		exit(EXIT_FAILURE);
	}
	else
		return cmsg;
}

void cann_free(rs232can_msg *rmsg)
{
	free(rmsg);
}


void cann_dumpconn()
{
	cann_conn_t *client = cann_conns_head;

	while (client) {
		debug(9, "CANN connection: fd=%d error=%d", client->fd, client->error);
		client = client->next;
	}
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
		ret = recv(client->fd, &val, 1, 0);
		       
		if (ret == 0) goto eof;
		if (ret < 0) goto error;

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
		ret = recv(client->fd, &(client->msg.cmd), 1, 0);
		       
		if (ret == 0) goto eof;
		if (ret < 0) goto error;

		debug(10, "Next packet on %d: cmd=%d", client->fd, client->msg.cmd);
		client->state = CANN_PAYLOAD;
	}

	// read data
	ret = recv(client->fd, client->rcv_ptr, client->missing_bytes, 0);

	if (ret == 0) goto eof;
	if (ret < 0) goto error;
		
	client->missing_bytes -= ret;
	client->rcv_ptr       += ret;

	debug(10, "fd %d: recived %d bytes, %d missing",
			client->fd, ret, client->missing_bytes);

	// message complete?
	if (client->missing_bytes == 0) {
		rs232can_msg *rmsg = malloc(sizeof(rs232can_msg));

		if (rmsg == NULL)
		{
			printf("ERROR: Could not allocate rmsg buffer!\n");
			exit(EXIT_FAILURE);
		}

		memcpy(rmsg, &(client->msg), sizeof(rs232can_msg));
		client->state = CANN_LEN;
		return rmsg;
	}

	return NULL;

error:
	if ((errno == EAGAIN) || (errno==0)) 
		return NULL;

eof:
	debug_perror( 5, "Error readig fd %d (ret==%d)", client->fd, ret );
	client->error = 1;
	return NULL;
}

rs232can_msg *cann_get(cann_conn_t *client)
{
	int ret;
	fd_set rset;
	rs232can_msg *rmsg;

	for (;;) {
		FD_ZERO(&rset);
		FD_SET(client->fd, &rset);

		ret = select(client->fd + 1, &rset, (fd_set*)NULL, (fd_set*)NULL, NULL);
		debug_assert( ret >= 0, "cann_get: select failed" );

		rmsg = cann_get_nb(client);
		if (rmsg)
			return rmsg;
	}
}


/*****************************************************************************
 * transmit
 */

/* transmit and free message */
void cann_transmit(cann_conn_t *conn, rs232can_msg *msg)
{
	// sanity
	if (conn->error) {
		debug(5, "cann_transmit: not transmiting on errorous connection fd=%d", conn->fd);
		return;
	}
	
	debug(9, "Transmitting message fd=%d", conn->fd);
	
	

	#ifndef WINDOWS
		int flags = MSG_NOSIGNAL;
	#else
		int flags = 0;
	#endif

		if (send(conn->fd, &(msg->len), 1, flags) != 1) 
			goto error;
		
		if (send(conn->fd, &(msg->cmd), 1, flags) != 1)
			goto error;
	
		if (send(conn->fd, msg->data, msg->len, flags) != msg->len)
			goto error;
	
	return;
error:
	conn->error = 1;
	debug_perror(5, "Error sending fd %d", conn->fd);
	return;

}

