#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib-host/tcp_server.h"
#include "debug.h"

tcp_server_t * netvar_server;

#define LINE_BUFFER_SIZE 256

typedef struct {
	char data[LINE_BUFFER_SIZE];
	unsigned int head;
	unsigned int tail;
} line_buffer_t;

typedef struct {
	line_buffer_t line_buffer;

} netvar_client_t;


char * read_line(line_buffer_t * buf, int fd){
	char * d = buf->data;

	int h, t, i;
begin:
	h = buf->head;
	t = buf->tail;
	
	//find complete strings that are in the buffer allready
	for (i = h; i < t; i++) {
		if ((d[i] == '\r') || (d[i] == '\n')) {
			d[i] = 0;
			if (i == h) {
				//first charakter is newline: skip it
				h = i + 1;
				buf->head = h;
				continue;
			} else {
				//found newline terminated string
				buf->head = i+1; //set head to after this string
				return &d[h]; //return start of this string
			}
		}
	}
	
	if (buf->head != 0) {
		//shift all elements left in the buffer to 0
		for (h = 0, i = buf->head; i < buf->tail; i++, h++) {
			d[h] = d[i];
		}
		//adjust indexes
		buf->tail -= buf->head; 
		buf->head = 0;
	}

	if(buf->tail == LINE_BUFFER_SIZE - 1){
		//buffer is full and no newline yet - reset and return error
		buf->head = 0;
		buf->tail = 0;
		return (char *) 0xFFFFFFFF;
	}
	
	// read data
	i = read(fd, &d[buf->tail] , LINE_BUFFER_SIZE - 1 - buf->tail);
	if (i == 0) return (char *) 0xFFFFFFFF; //eof = connection closed
	
	if (i < 0) {
		if ((errno == EAGAIN) || (errno==0))
			return 0; //no data read -> no new strings
		else
			return (char *) 0xFFFFFFFF;
	};
	
	buf->tail += i;//set tail to new end of buffer
	goto begin; //and try finding new strings in the data received

}



// a nonzero return closes the connection. The user should clean up
// and free his ref structure in that case before returning.
static int receive_handler(int fd, void * ref) {
	netvar_client_t * client = (netvar_client_t *) ref;
	//printf("receive\n");
	
	char * line = read_line(&client->line_buffer, fd);
	
	if (line == (char *) 0xFFFFFFFF) {
		debug_perror(5, "Error readig fd %d", fd);
		return 1;//close connection
	}
	
	if (line) {
		printf("got: %s\n", line);
		if(strcmp(line, "close") == 0){
			return 1;
		}
	}
	
	return 0;
}


static void * accept_handler(int fd) {
	printf("accept\n");
	netvar_client_t * client = (netvar_client_t *) malloc(sizeof(netvar_client_t));
	client->line_buffer.head = 0;
	client->line_buffer.tail = 0;
	
	return client;
}



void init_netvar_server() {
	netvar_server = new_tcp_server( "2343", receive_handler, accept_handler);
}
