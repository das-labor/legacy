
#include <stdint.h>
#include <stdlib.h>
#include "netvar.h"
#include "netvar_io.h"

static void nv_handler(netvar_desc *nd, void *ref) {
	 netvar_output_8_t * nl = (netvar_output_8_t *) ref;
	 nl->set_output_func(nl->set_output_ref, nd->data[0]);
}

netvar_output_8_t * new_netvar_output_8(uint16_t           idx, 
                                        uint8_t            sidx, 
								        set_output_func_t  set_output_func,
								        void *             set_output_ref     )
{
	netvar_output_8_t * nl = malloc(sizeof(netvar_output_8_t));
	nl->nd = netvar_register(idx, sidx, 1);
	nl->set_output_func = set_output_func;
	nl->set_output_ref  = set_output_ref;
	
	netvar_add_handler(nl->nd, nv_handler, nl);
	
	return nl;
}
