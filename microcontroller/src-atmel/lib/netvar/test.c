
#include "netvar.h"


void var_test_handler(uint16_t id, uint16_t var, uint8_t flags){
	PORTC = var;
}


netvar_uint16_t var_test;



netvar_def listen_table[] = {
	{0x0100, 0, &var_test, var_test_handler },
};


