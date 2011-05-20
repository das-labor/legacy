
#include <string.h>

#include "../config.h"

#ifdef AVR
	#include <stdlib.h>
	#include "../can/can.h"
#else
	#include "../can_pc/can.h"
	#include <malloc.h>
#endif

#include "../util_lib/list.h"


#include "netvar.h"

#define PORT_NETVAR 0x37

extern uint8_t myaddr;

typedef struct{
	uint16_t idx;
	uint8_t sidx;
	 netvar_desc * nd;
}netvar_dir_element_t;

netvar_dir_element_t netvar_dir[40];

uint8_t netvar_dir_size;

uint8_t find_netvar_position(uint16_t idx, uint8_t sidx){
	uint8_t a, b, m;
	uint16_t m_idx;
	uint8_t m_sidx;
	
	if(netvar_dir_size == 0) return 0;

	a = 0;
	b = netvar_dir_size - 1;
	
	//binary search
	while(a != b){
		m = (a + b) / 2;
		m_idx = netvar_dir[m].idx;
		
		if(m_idx < idx){
			a = m + 1;
		}else if(m_idx > idx){
			b = m;
		}else{
			m_sidx = netvar_dir[m].sidx;
			if(m_sidx < sidx){
				a = m + 1;
			}else if(m_sidx > sidx){
				b = m;
			}else{
				a = m;
				break;
			}
		}
	}
	
	return a;
}

netvar_desc * get_netvar_by_idx(uint16_t idx, uint8_t sidx){
	uint8_t i = find_netvar_position(idx, sidx);
	if((netvar_dir[i].idx == idx) && (netvar_dir[i].sidx == sidx) ){
		return netvar_dir[i].nd;
	}else{
		return 0;
	}
}

void netvar_insert(uint8_t pos, uint16_t idx, uint8_t sidx, netvar_desc * nd){
	uint8_t i;
	for(i = netvar_dir_size;i > pos; i--){
		memcpy(&netvar_dir[i], &netvar_dir[i-1], sizeof(netvar_dir_element_t));
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

netvar_desc * new_netvar(uint16_t idx, uint8_t sidx, uint8_t size ){
	netvar_desc * nd = malloc(sizeof(netvar_desc));
	nd->idx = idx;
	nd->sidx = sidx;
	nd->size = size;
	nd->data = malloc(size);
	nd->flags = 0;
	nd->handlers = 0;
	return nd;
}

//register a netvar. returns the same handle if a netvar is registered multiple times.
netvar_desc * netvar_register(uint16_t idx, uint8_t sidx, uint8_t size){
	netvar_desc * nd;

	uint8_t i = find_netvar_position(idx, sidx);
	if((netvar_dir[i].idx == idx) && (netvar_dir[i].sidx == sidx) ){
		//netvar allready registered
		return netvar_dir[i].nd;
	}else{
		nd = new_netvar(idx, sidx, size);
		netvar_insert(i, idx, sidx, nd);
		return nd;
	}


}

//adds handler for incoming netvar, which will be called with the user supplied reference as argument.
void netvar_add_handler(netvar_desc * nd, void (*fkt)(netvar_desc *, void *), void * ref){
	if(nd->handlers == 0){
		//need to create handler list
		nd->handlers = new_list();
	}
	handler_descriptor_t * hd = malloc(sizeof(handler_descriptor_t));
	hd->fkt = fkt;
	hd->ref = ref;
	
	list_append(nd->handlers, hd);	
}


typedef struct{
	uint8_t data[8];
}netvar_data_store_t;

//double buffered nd table.
//one is used to store the events we are working on, while the other one is filled with the new events.
//the event data for the new events is therewhile stored in the single buffered event_data_table,
//as we don't need the event data anymore since it has been copied to all netvars before
//the new events are processed.
netvar_desc *          event_nd_table[2][32];

netvar_data_store_t   event_data_table[32];

//selects which of the two nd buffer stores the new events
uint8_t in_buffer_select;

//new event index for stroing events.
uint8_t event_table_index;


void netvar_store_event(netvar_desc * nd, void * data){
	event_nd_table[in_buffer_select][event_table_index] = nd;
	memcpy(event_data_table[event_table_index].data, data, 5);
	event_table_index++;
}


#define IN_FIFO_SIZE 32

uint8_t in_fifo_head, in_fifo_tail;

netvar_data_store_t in_fifo_data[IN_FIFO_SIZE];
netvar_desc * in_fifo_nd[IN_FIFO_SIZE];




void in_fifo_store(netvar_desc * nd, void * data){
	in_fifo_nd[in_fifo_head] = nd;
	memcpy(in_fifo_data[in_fifo_head].data, data, 5);
	in_fifo_head++;
	if(in_fifo_head == IN_FIFO_SIZE){
		in_fifo_head = 0;
	}
}


//called when can message on netvar port is received
void netvar_received(can_message * msg){
	uint16_t idx;
	uint8_t sidx;
	idx = msg->data[0] | ((uint16_t)(msg->data[1])<<8);
	sidx = msg->data[2];
	
	//find the netvar
	netvar_desc * nd;
	nd = get_netvar_by_idx(idx,sidx);
	
	if(nd){
		//found
		in_fifo_store(nd, &msg->data[3]);
	}
}


void netvar_write(netvar_desc * nd, void * data){
	netvar_store_event(nd, data);
}

uint8_t netvar_read(netvar_desc * nd, void * data){
	memcpy(data, nd->data, nd->size);
	return 0;
}

void netvar_transmit(netvar_desc * nd){
	static can_message msg = {0, 0, PORT_NETVAR, PORT_NETVAR, 1, {0,0,0,0,0,0,0,0}};
	uint16_t idx;
	uint8_t sidx;
	
	msg.addr_src = myaddr;
	msg.addr_dst = 0;
	
	idx = nd->idx;
	sidx = nd->sidx;
	
	msg.data[0] = idx;
	msg.data[1] = idx>>8;
	msg.data[2] = sidx;
	memcpy (&msg.data[3], nd->data, 5);
	msg.dlc = nd->size + 3;
	can_transmit(&msg);
}


extern void lap_button_nv_handler(void);

void netvar_handle_events(){
	uint8_t i;
	uint8_t event_table_size = event_table_index;
	event_table_index = 0;
	
	//swap buffers
	in_buffer_select ^= 1;
	
	//update data and transmit can message for each event
	for (i = 0; i < event_table_size; i++ ) {
		netvar_desc * nd = event_nd_table[in_buffer_select ^ 1][i];
		memcpy(nd->data, event_data_table[i].data, nd->size); //update netvar data from event data
		netvar_transmit(nd);
	}
	
	
	//call event handlers if any
	for (i = 0; i < event_table_size; i++ ) {
		netvar_desc * nd = event_nd_table[in_buffer_select ^ 1][i];
		if(nd->handlers){
			//printf("calling handlers for %d, %X\r\n", i, nd->handlers);
			list_iterator_t it;
			list_foreach_begin(&it, nd->handlers);
			handler_descriptor_t * hd;
			while(((hd = list_foreach(&it, nd->handlers)) != 0)){
				//printf("calling %X, ref=%X\r\n", hd->fkt, hd->ref );
				hd->fkt(nd, hd->ref);
			}
		}
	}
	
	//handle in fifo
	while(in_fifo_tail != in_fifo_head){
		netvar_desc * nd = in_fifo_nd[in_fifo_tail];
		memcpy(nd->data, in_fifo_data[in_fifo_tail].data, nd->size); //update netvar data from event data
		
		if(nd->handlers){
			//printf("calling handlers for %d, %X\r\n", i, nd->handlers);
			list_iterator_t it;
			list_foreach_begin(&it, nd->handlers);
			handler_descriptor_t * hd;
			while(((hd = list_foreach(&it, nd->handlers)) != 0)){
				//printf("calling %X, ref=%X\r\n", hd->fkt, hd->ref );
				hd->fkt(nd, hd->ref);
			}
		}
	
		in_fifo_tail++;
		if(in_fifo_tail == IN_FIFO_SIZE){
			in_fifo_tail = 0;
		}
	}
	
	
}



