#include <stdio.h>
#include <stdlib.h>

#include "lib-host/tcp_server.h"

tcp_server_t * netvar_server;


typedef struct {
	char data[256];
	int len;
} line_buffer_t;


char * read_line(line_buffer_t * buf, int fd){
}



typedef struct {
	line_buffer_t line_buffer;
} netvar_client_t;


static int receive_handler(int fd, void * ref) {
	printf("receive\n");
	return 0;
}


static void * accept_handler(int fd) {
	printf("accept\n");
	return 1;
}



void init_netvar_server() {
		netvar_server = new_tcp_server( "2343", receive_handler, accept_handler);


}
