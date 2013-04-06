#include <string.h>
#include <stdint.h>

#include "../config.h"

#ifdef AVR
	#include <stdlib.h>
	#include "../can/can.h"
#else
	#include <malloc.h>
	#include "../lib-host/can.h"
#endif

#include "../util_lib/list.h"


#include "netvar.h"

#define PORT_NETVAR 0x37

extern uint8_t myaddr;

typedef struct {
	uint16_t idx;
	uint8_t sidx;
	netvar_desc *nd;
} netvar_dir_element_t;

static netvar_dir_element_t netvar_dir[40];

static uint8_t netvar_dir_size;

static uint8_t find_netvar_position(uint16_t idx, uint8_t sidx) {
	uint8_t a, b, m;
	uint16_t m_idx;
	uint8_t m_sidx;

	if (netvar_dir_size == 0) return 0;

	a = 0;
	b = netvar_dir_size;

	//binary search
	while (a != b) {
		m = (a + b) / 2;
		m_idx = netvar_dir[m].idx;

		if (m_idx < idx) {
			a = m + 1;
		} else if (m_idx > idx) {
			b = m;
		} else {
			m_sidx = netvar_dir[m].sidx;
			if (m_sidx < sidx) {
				a = m + 1;
			} else if (m_sidx > sidx) {
				b = m;
			} else {
				a = m;
				break;
			}
		}
	}
	return a;
}

netvar_desc *get_netvar_by_idx(uint16_t idx, uint8_t sidx) {
	uint8_t i = find_netvar_position(idx, sidx);
	if ( (i < netvar_dir_size) && (netvar_dir[i].idx == idx) && (netvar_dir[i].sidx == sidx) ) {
		return netvar_dir[i].nd;
	} else {
		return 0;
	}
}

static void netvar_insert(uint8_t pos, uint16_t idx, uint8_t sidx, netvar_desc *nd) {
	uint8_t i;
	for (i = netvar_dir_size;i > pos; i--) {
		memcpy(&netvar_dir[i], &netvar_dir[i - 1], sizeof(netvar_dir_element_t));
	}
	netvar_dir_size ++;
	netvar_dir[pos].idx = idx;
	netvar_dir[pos].sidx = sidx;
	netvar_dir[pos].nd = nd;
}


/*
typedef struct {
	uint16_t idx;
	uint8_t sidx;
	uint8_t size;
	uint8_t *data;
	uint8_t flags;
	list_t *handlers; //pointer to list of handler_descriptor_t pointers
} netvar_desc;
*/

netvar_desc *new_netvar(uint16_t idx, uint8_t sidx, uint8_t size) {
	netvar_desc *nd = malloc(sizeof(netvar_desc));
	nd->idx = idx;
	nd->sidx = sidx;
	nd->size = size;
	if (size <= 5) {
		//normal netvar - data is malloced during lifetime
		nd->data = malloc(size);
	} else {
		//bulk netvar. the data is malloced on reception
		nd->data = 0;
	}
	nd->flags = 0;
	nd->handlers = 0;
	return nd;
}

//register a netvar. returns the same handle if a netvar is registered multiple times.
netvar_desc *netvar_register(uint16_t idx, uint8_t sidx, uint8_t size) {
	netvar_desc *nd;

	uint8_t i = find_netvar_position(idx, sidx);
	if ( (i < netvar_dir_size) && (netvar_dir[i].idx == idx) && (netvar_dir[i].sidx == sidx) ) {
		//netvar allready registered
		return netvar_dir[i].nd;
	} else {
		nd = new_netvar(idx, sidx, size);
		netvar_insert(i, idx, sidx, nd);
		return nd;
	}
}

//adds handler for incoming netvar, which will be called with the user supplied reference as argument.
void netvar_add_handler(netvar_desc *nd, void (*fkt)(netvar_desc *, void *), void *ref) {
	if (nd->handlers == 0) {
		//need to create handler list
		nd->handlers = new_list();
	}
	handler_descriptor_t *hd = malloc(sizeof(handler_descriptor_t));
	hd->fkt = fkt;
	hd->ref = ref;

	list_append(nd->handlers, hd);
}

//removes handler from netvar by value
void netvar_remove_handler (netvar_desc *nd, void (*fkt)(netvar_desc *, void *), void *ref) {
	if (nd->handlers) {
		list_iterator_t it;
		list_foreach_begin(&it, nd->handlers);
		handler_descriptor_t *hd;
		while (((hd = list_foreach(&it, nd->handlers)) != 0)) {
			if ((hd->fkt == fkt) && (hd->ref == ref)){
				list_remove(nd->handlers, hd);
				free(hd);
				break;
			}
		}
	}
}

//calls handlers on nd if any
static void nd_call_handlers(netvar_desc *nd) {
	if (nd->handlers) {
		//printf("calling handlers for %d, %X\r\n", i, nd->handlers);
		list_iterator_t it;
		list_foreach_begin(&it, nd->handlers);
		handler_descriptor_t *hd;
		while (((hd = list_foreach(&it, nd->handlers)) != 0)) {
			//printf("calling %X, ref=%X\r\n", hd->fkt, hd->ref );
			hd->fkt(nd, hd->ref);
		}
	}
}

typedef struct {
	uint8_t data[8];
} netvar_data_store_t;

//double buffered nd table.
//one is used to store the events we are working on, while the other one is filled with the new events.
//the event data for the new events is therewhile stored in the single buffered event_data_table,
//as we don't need the event data anymore since it has been copied to all netvars before
//the new events are processed.
static netvar_desc          *event_nd_table[2][32];

static netvar_data_store_t   event_data_table[32];

//selects which of the two nd buffer stores the new events
static uint8_t in_buffer_select;

//new event index for stroing events.
static uint8_t event_table_index;

//buffer event in event table being filled and buffer data
//to data table
static void netvar_store_event(netvar_desc *nd, void *data) {
	event_nd_table[in_buffer_select][event_table_index] = nd;
	memcpy(event_data_table[event_table_index].data, data, 5);
	event_table_index++;
}


#define IN_FIFO_SIZE 32

uint8_t in_fifo_head, in_fifo_tail;

netvar_data_store_t in_fifo_data[IN_FIFO_SIZE];
netvar_desc *in_fifo_nd[IN_FIFO_SIZE];



//called to store incoming netvar data from CAN for netvars
//that we have registered 
void in_fifo_store(netvar_desc *nd, void *data) {
	in_fifo_nd[in_fifo_head] = nd;
	memcpy(in_fifo_data[in_fifo_head].data, data, 5);
	in_fifo_head++;
	if (in_fifo_head == IN_FIFO_SIZE) {
		in_fifo_head = 0;
	}
}



uint8_t bulk_state;
#define BS_IDLE    0
#define BS_RECEIVE 1

uint8_t       akt_bulk_addr;
netvar_desc  *akt_bulk_nd;
uint16_t      akt_bulk_size;
uint16_t      akt_bulk_received;

uint8_t *bulk_data;


//called from can handler when netvar on bulk port is received
void netvar_bulk_received(can_message *msg) {
	if (bulk_state == BS_RECEIVE) {
		if (msg->addr_src == akt_bulk_addr) {
			akt_bulk_received += 8;
			memcpy(bulk_data, msg->data, 8);
			if (akt_bulk_received >= akt_bulk_size) {
				akt_bulk_nd->data = bulk_data; //pass malloced buffer to handlers in nd data field
				nd_call_handlers(akt_bulk_nd); //call the handlers - the data is only valid during the handler
				free(bulk_data);               //free the data buffer again. handler must make local copy if it still needs the data.
				akt_bulk_nd->data = 0;
				bulk_state = BS_IDLE;
			}
		}
	}
}

//called from netvar received if nd indicates bulk netvar.
//set the bulk receiver up to receive the data from that address
static void netvar_bulk_begin(netvar_desc *nd, uint8_t src_addr, uint16_t size) {
	if (bulk_state == BS_RECEIVE) {
		//drop ongoing transmission if any
		free(bulk_data);
		akt_bulk_nd->data = 0;
	}

	akt_bulk_nd = nd;
	akt_bulk_addr = src_addr;
	akt_bulk_size = size;
	akt_bulk_received = 0;
	bulk_state = BS_RECEIVE;
	bulk_data = malloc((size + 7) & ~ 7);
}

//called when can message on netvar port is received
void netvar_received(can_message *msg) {
	uint16_t idx;
	uint8_t sidx;
	idx = msg->data[0] | ((uint16_t)(msg->data[1]) << 8);
	sidx = msg->data[2];

	//find the netvar
	netvar_desc *nd;
	nd = get_netvar_by_idx(idx, sidx);

	if (nd) {
		//found
		
		//up to 5 bytes are transferred in one packet.
		//if the netvar description says it is larger than 5 bytes
		//then it is a bulk netvar.
		if (nd->size > 5) {
			uint16_t size = (((uint16_t)msg->data[4]) << 8) | msg->data[3];
			netvar_bulk_begin(nd, msg->addr_src, size);
		} else {
			in_fifo_store(nd, &msg->data[3]);
		}
	} else {
		//not found
		#ifdef NETVAR_AUTO_REGISTER
			//register it automatically
			netvar_register(idx, sidx, msg->dlc - 3);
		#endif
	}
}

//user api to write to previously registered netvars
void netvar_write(netvar_desc *nd, void *data) {
	netvar_store_event(nd, data);
}

//user api to read from previously registered netvars
uint8_t netvar_read(netvar_desc *nd, void *data) {
	memcpy(data, nd->data, nd->size);
	return 0;
}

static void netvar_transmit(netvar_desc *nd) {
	can_message *msg = can_buffer_get();
	uint16_t idx;
	uint8_t sidx;

	msg->addr_src = myaddr;
	msg->addr_dst = 0;
	msg->port_src = PORT_NETVAR;
	msg->port_dst = PORT_NETVAR;

	idx = nd->idx;
	sidx = nd->sidx;

	msg->data[0] = idx;
	msg->data[1] = idx >> 8;
	msg->data[2] = sidx;
	memcpy (&msg->data[3], nd->data, 5);
	msg->dlc = nd->size + 3;
	can_transmit(msg);
}

//write to unregistered netvars. This is for elements, that only transmit
//on specific netvars, but are not interested in receiving data on this netvar.
void unregistered_netvar_write (uint16_t idx, uint8_t sidx, uint8_t size, void *data) {
	//search if this netvar is also registered on this device.
	netvar_desc *nd = get_netvar_by_idx (idx, sidx);

	if (nd) {
		//the netvar is registered. default to normal wite function.
		netvar_write (nd, data);
	} else {
		//it is unregistered. transmit it to can immediately
		netvar_desc  nds = {idx, sidx, size, data, 0, 0}; //temp netvar descriptor
		netvar_transmit (&nds);
	}
}

void netvar_handle_events() {
	uint8_t i;
	uint8_t event_table_size = event_table_index;
	event_table_index = 0;

	//swap buffers
	in_buffer_select ^= 1;

	//update data and transmit can message for each event
	for (i = 0; i < event_table_size; i++ ) {
		netvar_desc *nd = event_nd_table[in_buffer_select ^ 1][i];
		memcpy(nd->data, event_data_table[i].data, nd->size); //update netvar data from event data
		netvar_transmit(nd);
	}


	//call event handlers if any
	for (i = 0; i < event_table_size; i++ ) {
		netvar_desc *nd = event_nd_table[in_buffer_select ^ 1][i];
		nd_call_handlers(nd);
	}

	//handle in fifo
	while (in_fifo_tail != in_fifo_head) {
		netvar_desc *nd = in_fifo_nd[in_fifo_tail];
		memcpy(nd->data, in_fifo_data[in_fifo_tail].data, nd->size); //update netvar data from event data

		nd_call_handlers(nd);

		in_fifo_tail++;
		if (in_fifo_tail == IN_FIFO_SIZE) {
			in_fifo_tail = 0;
		}
	}
}
