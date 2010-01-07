
#include "netvar.h"


#define LISTEN_TABLE_SIZE 1

uint16_t listen_table_search_id(uint16_t id){
	uint16_t x;
	for(x=0;x<LISTEN_TABLE_SIZE;x++){
		if(listen_table[x].id == id){
			return x;
		}
	}
	return 0xffff;
}



void netvar_can_handler(can_message *rx_msg){
	uint16_t idx;

	uint16_t id = rx_msg->data[0] | (rx_msg->data[1] << 8);

	idx = listen_table_search_id(id);
	if(idx == 0xffff) return;
	
	switch(listen_table[idx].type){
		case netvar_type_uint16_t:{
				uint16_t var = rx_msg->data[2] + (rx_msg->data[3] << 8);
				*((uint16_t *)listen_table[idx].netvar) = var;
				if(listen_table[idx].handler != 0){
					((void(*)(uint16_t, uint16_t, uint8_t)) listen_table[idx].handler)(id, var, 0);
				}
			}
			break;
	}
	


}

