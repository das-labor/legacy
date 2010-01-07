
#include <stdint.h>
#include "can.h"

typedef struct{
	uint16_t var;
	uint8_t flags;
}netvar_uint16_t;

typedef struct{
	uint16_t id;
	uint8_t type;
	void * netvar;
	void * handler;
}netvar_def;

enum netvar_type_enum{
	netvar_type_uint16_t = 1,
};

extern netvar_def listen_table[];

void netvar_can_handler(can_message *rx_msg);


