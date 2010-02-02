/* WARNING
 * Kruddelcode ahead.
 */
#include "../common/protocol.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <stdint.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include  <netdb.h>
#include  <errno.h>

#include "common.h"

static fd_set master;     // master file descriptor list
static fd_set read_fds;   // temp file descriptor list for select()
static int fdmax;         // maximum file descriptor number
static int listener;      // listening socket descriptor
rfmusb_dev_handle *udhandle; /* device handle for usb device */
void tcp_send_data (size_t in_len, uint8_t *in_buf);
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handle_clientdata (int in_fd)
{
	static uint8_t rxbuf[2048];
	static size_t nbytes = 0;
	tcp2air_t *rxpacket;

	if (nbytes = recv(in_fd, rxbuf, sizeof(rxbuf), 0) <= 0) 
	{
		// got error or connection closed by client
		if (nbytes == 0)
		{
			// connection closed
			printf("selectserver: socket %d hung up\n", in_fd);
		} else
		{
			perror("recv");
		}
		close(in_fd); // bye!
		FD_CLR(in_fd, &master); // remove from master set
	} else
	{
		printf("receive: %s\n", rxbuf);
		tcp_send_data (nbytes, rxbuf);

		if (nbytes < sizeof(tcp2air_t))
			return;

		rxpacket = decap_packet (rxbuf);
		rfmusb_txpacket (udhandle, rxpacket->data[0], rxpacket->len, rxpacket->data +1); /* XXX */
	}
}

void tcp_send_data (size_t in_len, uint8_t *in_buf)
{
	uint_fast32_t i;
	for (i=0;i<=fdmax;i++)
	{
		if (i == listener) /* skip listener socket */
			continue;


		if (!FD_ISSET(i, &master))
			continue;

		if (send(i, in_buf, in_len, 0) < in_len)
		{
			printf("could not send all data to client with fd %i\n", i);
		}
	}
}

int main(int argc, char *argv[])
{
	int newfd;         // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	uint8_t *rxbuf, *txbuf, *tmpbuf;
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];
	int yes=1;         // for setsockopt() SO_REUSEADDR, below
	int i, j, rv;
	struct addrinfo hints, *ai, *p;
	size_t txbuflen = 0, rxbuflen = 0;
	struct tcp2air_t *tmphdr;

	extern uint8_t opt_rfm12usb, opt_canusb, opt_dump, opt_debug, opt_foreground;
	extern char opt_port[], *progname;

	opt_debug = 0;
	opt_foreground = 0;
	opt_rfm12usb = 0;
	opt_canusb = 0;
	opt_dump = 0;
	progname = argv[0];

	if (parse_args (argc, argv) < 0)
		return -1 * __LINE__;
	
	/* alloc buffers */
	rxbuf = malloc (BUF_MAXLEN);
	txbuf = malloc (BUF_MAXLEN);
	tmpbuf = malloc (MAX(BUF_MAXLEN / 4, sizeof(rfmusb_packetbuffer)));

	if (txbuf == NULL || rxbuf == NULL || tmpbuf == NULL)
	{
		printf("Couldn't malloc() memory for my buffers.\n");
		return -1 * __LINE__;
	}
	
	if (opt_rfm12usb && rfmusb_connect (&udhandle))
	{

		printf("Error initializing USB device: ");
		printf("%s\n", strerror(errno));
		return -1 * __LINE__;
	}

	if (opt_debug && opt_rfm12usb)
		printf("USB device initialized\n");

	if (!(opt_canusb | opt_rfm12usb))
	{
		printf("neither CAN, nor rfm12 selected... what am i supposed to do?\r\n");
		print_usage();
		return -1 * __LINE__;
	}

	FD_ZERO(&master);     // clear the master and temp sets
	FD_ZERO(&read_fds);
	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, opt_port, &hints, &ai)) != 0)
	{
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}
	for (p=ai;p != NULL; p=p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
		{
			continue;
		}
		// lose the pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue;
		}
		break;
	}
	// if we got here, it means we didn't get bound
	if (p == NULL)
	{
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}
	freeaddrinfo(ai); // all done with this
	// listen
	if (listen(listener, 10) == -1)
	{
		perror("listen");
		exit(3);
	}
	// add the listener to the master set
	FD_SET(listener, &master);
	// keep track of the biggest file descriptor
	fdmax = listener; // so far, it's this one
	// main loop
	for(;;)
	{
		read_fds = master; // copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}
		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++)
		{
			nbytes = rfmusb_rxpacket (udhandle, (rfmusb_packetbuffer*) tmpbuf);
			if (PREDICT_ENCAPSIZE(nbytes + txbuflen) > BUF_MAXLEN)
			{
				printf("Warning: Dropping data.\n");
			} else
			{
				/* XXX hardcoded frequency & bandwidth for now - until there's a proper interface */
				tmphdr = prep_packet (DIR_RX, 433000000, 1000, 9600, nbytes,
						((rfmusb_packetbuffer *) tmpbuf)->buffer);
				tmpbuf = encap_packet (&nbytes, (tcp2air_t *) tmpbuf);
				txbuflen += nbytes;
			}

			if (FD_ISSET(i, &read_fds))
			{ // we got one!!
				if (i == listener)
				{
					
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener,(struct sockaddr *)&remoteaddr,
						&addrlen);
					if (newfd == -1)
					{
						perror("accept");
					} else
					{
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax)
						{     // keep track of the max
							fdmax = newfd;
						}
						printf("selectserver: new connection from %s on "
						"socket %d\n",
						inet_ntop(remoteaddr.ss_family,
						get_in_addr((struct sockaddr*)&remoteaddr),
						remoteIP, INET6_ADDRSTRLEN),
						newfd);
					}
				} else
				{
					handle_clientdata(i);
				} // END handle data from client
			} // END got new incoming connection
		} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!
	return 0;
}
