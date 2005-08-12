#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>


static int listen_socket;


void net_init(int port)
{
	struct sockaddr_in serv_addr;

	if ( (listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror( "Could not open listeing socket: ");
		exit(2);
	};

	/* bind address */
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	if (bind(listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		perror("Could not bind listening socket: ");
		exit(2); 
	} 
	
	/* specify queue */ 
	listen(listen_socket, 5); 
	
	/*
	for (;;) { 
		len = sizeof(client_addr);
		client_sockfd = accept(sockfd, &client_addr, &len);
		if (client_sockfd == -1) { perror(NULL);
		continue;
	} 
	*/
}

int net_fdset(fd_set *set)
{
	// socket for new connections
	FD_SET(listen_socket, set);

	// XXX
}


