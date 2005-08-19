#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "inet.h"


int listen_socket;
net_client_t *net_clients_head = NULL;


#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

void net_init(int port)
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

int net_fdset(fd_set *set)
{
	net_client_t *client = net_clients_head;
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

net_client_t *net_new_connection(fd_set *set)
{
	net_client_t *client;

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
	client = (net_client_t *)malloc(sizeof(net_client_t));
	client->next  = net_clients_head;
	client->fd    = fd;
	client->missing_bytes = 0;
	client->error = 0;
	net_clients_head = client;

	return client;
}

void net_close_errors()
{
	net_client_t **client = &net_clients_head;

	while(*client) {
		if ( (*client)->error) {
			net_client_t *del = (*client);

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

net_client_t *net_client_activity(fd_set *set)
{
	net_client_t *client = net_clients_head;

	while(client) {
		if (!(client->error) && FD_ISSET(client->fd, set) ) {
			FD_CLR(client->fd, set);
			return client;
		}
		client = client->next;
	}

	return NULL;
}

/* nonblocking read on netwock socket -- returns msg if complete msg arrived */
rs232can_msg *net_get_nb(net_client_t *client)
{
	int ret; 

	// sanity
	debug_assert( !(client->error), 
			"net_get_nb() with error (%d)", client->error );

	// start new packet and read length?
	if (client->missing_bytes == 0) {
		if (read(client->fd, &(client->missing_bytes), 1) < 0)
			goto error;

		// check msg length
		if (client->missing_bytes > sizeof(client->msg)) {
			debug( 2, "Protocol error on fd %d (size=%d)", 
					client->fd, client->missing_bytes );
			client->error = 1;
			return NULL;
		};
			
		client->rcv_ptr = client->msg.data;
		client->msg.len = client->missing_bytes;

	};

	// read data
	ret = read(client->fd, client->rcv_ptr, client->missing_bytes);

	if ((ret < 0) && (errno == EAGAIN))
		return NULL;	// blocking IO: no data available

	if (ret <= 0)
		goto error;
		
	debug(10, "fd %d: recived %d bytes, %d missing",
			client->fd, ret, client->missing_bytes);

	client->missing_bytes -= ret;
	client->rcv_ptr       += ret;

	// message complete?
	if (client->missing_bytes == 0)
			return &(client->msg);

	return NULL;

error:
	debug_perror( 5, "Error readig fd %d (ret==%d)", client->fd, ret );
	client->error = 1;
	return NULL;

}

