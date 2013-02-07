
#include <stdint.h>
#include <stdlib.h>
#include "netvar.h"
#include "netvar_lamp.h"

static void nv_handler(netvar_desc *nd, void *ref) {
	 netvar_lamp_t * nl = (netvar_lamp_t *) ref;
	 nl->set_lamp_func(nl->set_lamp_ref, nd->data[0]);
}

netvar_lamp_t * new_netvar_lamp(uint16_t         idx, 
                                uint8_t          sidx, 
								set_lamp_func_t  set_lamp_func,
								void *           set_lamp_ref   )
{
	netvar_lamp_t * nl = malloc(sizeof(netvar_lamp_t));
	nl->nd = netvar_register(idx, sidx, 1);
	nl->set_lamp_func = set_lamp_func;
	nl->set_lamp_ref  = set_lamp_ref;
	
	netvar_add_handler(nl->nd, nv_handler, nl);
	
	return nl;
}
