#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lib/can.h"
#define CAN_HANDLER_C
#include "netvar/netvar.h"

#include "lib-host/tcp_server.h"
#include "lib-host/can-encap.h"
#include "lib-host/debug.h"
#include "lib/can_message_converters.h"


tcp_server_t *netvar_server;

#define LINE_BUFFER_SIZE 256

typedef struct {
	char data[LINE_BUFFER_SIZE];
	unsigned int head;
	unsigned int tail;
} line_buffer_t;

typedef struct {
	line_buffer_t line_buffer;
	int fd;
	list_t registered_netvars;
} netvar_client_t;


char *read_line(line_buffer_t *buf, int fd) {
	char *d = buf->data;

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

	if (buf->tail == LINE_BUFFER_SIZE - 1) {
		//buffer is full and no newline yet - reset and return error
		buf->head = 0;
		buf->tail = 0;
		return (char *) 0xFFFFFFFF;
	}

	// read data
	i = read(fd, &d[buf->tail], LINE_BUFFER_SIZE - 1 - buf->tail);
	if (i == 0) return (char *) 0xFFFFFFFF; //eof = connection closed
	
	if (i < 0) {
		if ((errno == EAGAIN) || (errno == 0))
			return 0; //no data read -> no new strings
		else
			return (char *) 0xFFFFFFFF;
	};

	buf->tail += i;//set tail to new end of buffer
	goto begin; //and try finding new strings in the data received
}

int send_string (int fd, char *str) {
	int len = strlen(str);
	if (send(fd, str, len, MSG_NOSIGNAL) != len) {
		debug_perror(1, "Error writing fd %d", fd);
		return 1;//return error
	}
	return 0; //return success
}

void netvar_server_netvar_handler(netvar_desc *nd, void *ref) {
	netvar_client_t *client = (netvar_client_t *) ref; //ref is the client that regsitered this netvar
	char reply_string[256];

	sprintf(reply_string, "event: 0x%04X:0x%02X=%d\r\n", nd->idx, nd->sidx, nd->data[0]);
	send_string (client->fd, reply_string);
}

// a nonzero return closes the connection. The user should clean up
// and free his ref structure in that case before returning.
static int receive_handler(int fd, void *ref) {
	netvar_client_t *client = (netvar_client_t *) ref;
	char *line;

	do {
		line = read_line(&client->line_buffer, fd);

		if (line == (char *) 0xFFFFFFFF) {
			debug_perror(1, "Error readig fd %d", fd);
			goto network_error; //close connection
		}

		if (line) {
			debug(5, "Got netvar command: %s", line);

			char *strtok_ptr;
			char *cmd;
			char reply_string[256];
			cmd = strtok_r(line, " ", &strtok_ptr);

			if (strcmp(cmd, "set") == 0) {
				char *idx_str  = strtok_r(0, ":", &strtok_ptr);
				char *sidx_str = strtok_r(0, "=", &strtok_ptr);
				char *val_str  = strtok_r(0, " ", &strtok_ptr);

				if ((idx_str == 0) || (sidx_str == 0) || (val_str == 0)) goto argerror;

				int idx;
				int sidx;
				int val;

				int size = 1;//XXX need to support different sizes than 1 byte

				if (sscanf(idx_str,  "%i", &idx) != 1) goto argerror;
				if (sscanf(sidx_str, "%i", &sidx) != 1) goto argerror;
				if (sscanf(val_str,  "%i", &val) != 1) goto argerror;

				netvar_desc *nd = netvar_register(idx, sidx, size); //register or get the netvar
				netvar_write(nd, &val);

				sprintf(reply_string, "set: 0x%04X:0x%02X=%d\r\n", idx, sidx, val);
			} else if (strcmp(cmd, "get") == 0) {
				char *idx_str  = strtok_r(0, ":", &strtok_ptr);
				char *sidx_str = strtok_r(0, "=", &strtok_ptr);

				if ((idx_str == 0) || (sidx_str == 0)) goto argerror;

				int idx;
				int sidx;

				if (sscanf(idx_str,  "%i", &idx) != 1) goto argerror;
				if (sscanf(sidx_str, "%i", &sidx) != 1) goto argerror;

				netvar_desc *nd = get_netvar_by_idx(idx, sidx);
				if (nd) {
					sprintf(reply_string, "get: 0x%04X:0x%02X=%d\r\n", idx, sidx, nd->data[0]);
				} else {
					sprintf(reply_string, "get: 0x%04X:0x%02X=unregistered\r\n", idx, sidx);
				}
			} else if (strcmp(cmd, "register") == 0) {
				char *idx_str  = strtok_r(0, ":", &strtok_ptr);
				char *sidx_str = strtok_r(0, " ", &strtok_ptr);
				char *size_str = strtok_r(0, " ", &strtok_ptr);

				if ((idx_str == 0) || (sidx_str == 0)) goto argerror;

				int idx;
				int sidx;
				int size = 1;//1 byte is default if no size is given
				
				if (sscanf(idx_str,  "%i", &idx) != 1) goto argerror;
				if (sscanf(sidx_str, "%i", &sidx) != 1) goto argerror;
				if (size_str) {
					if (sscanf(size_str, "%i", &size) != 1) goto argerror;
				}

				netvar_desc *nd = netvar_register(idx, sidx, size); //register or get the netvar

				list_iterator_t it;
				list_foreach_begin(&it, &client->registered_netvars);

				void *e;
				uint8_t allready_registered = 0;
				while ((e = list_foreach(&it, &client->registered_netvars))) {
					if ((netvar_desc *) e == nd) {
						//we allready regsitered this netvar
						allready_registered = 1;
						break;
					}
				}

				if (allready_registered){
					sprintf(reply_string, "register: 0x%04X:0x%02X %d allready registered\r\n", idx, sidx, nd->size);
				} else {
					netvar_add_handler(nd, netvar_server_netvar_handler, ref);//add our handler to this netvar with this client as reference
					list_append(&client->registered_netvars, nd); //remember that we regsitered this netvar

					sprintf(reply_string, "register: 0x%04X:0x%02X %d\r\n", idx, sidx, nd->size);
				}
			} else if (strcmp(cmd, "close") == 0) {
				goto close_connection;
			} else {
				sprintf(reply_string, "unknwon command\r\n");
			}

			if (send_string(fd, reply_string) != 0) goto network_error;


			/*

			*/
		}
	} while (line);
	return 0;

argerror:
	if (send_string(fd, "error: wrong argument(s)\r\n") != 0) goto network_error;
	return 0;
	
network_error:
close_connection:
	{
		list_iterator_t it;
		void *e;

		list_foreach_begin(&it, &client->registered_netvars);
		while ((e = list_foreach(&it, &client->registered_netvars))) {
			//remove all registered netvar handlers
			netvar_desc *nd = (netvar_desc *) e;
			debug(8, "remove handler for %X:%X", nd->idx, nd->sidx);
			netvar_remove_handler(nd, netvar_server_netvar_handler, ref);
		}
		uninit_list(&client->registered_netvars); //free the list

		free(client); //free our structure

		return 1; //close connection on network error
	}
}


static void *accept_handler(int fd) {
	debug(5, "Netvar connection accept");
	netvar_client_t *client = (netvar_client_t *) malloc(sizeof(netvar_client_t));
	client->line_buffer.head = 0;
	client->line_buffer.tail = 0;
	client->fd = fd;
	init_list(&client->registered_netvars);
	return client;
}


void deinit_netvar_server(tcp_server_t *tcp_server) {
	// XXX clean netvar
	tcp_server_close_all_connections(tcp_server);
}


void init_netvar_server() {
	netvar_server = new_tcp_server("2343", receive_handler, accept_handler);
}


void netvar_can_handler(rs232can_msg *msg) {
	can_message_raw raw_msg;
	can_message can_msg;

	//convert to regular can message
	can_message_raw_from_rs232can_msg(&raw_msg, msg);
	can_message_from_can_message_raw(&can_msg, &raw_msg);

	if (can_msg.port_src == 0x37) {
		netvar_received(&can_msg);
	}
}

