#ifndef _NETVAR_H
#define _NETVAR_H

#include <stdint.h>
#include "../util_lib/list.h"

typedef struct {
	uint16_t idx;
	uint8_t sidx;
	uint8_t size;
	uint8_t *data;
	uint8_t flags;
	list_t *handlers; //pointer to list of handler_descriptor_t pointers
} netvar_desc;

typedef struct {
	void (*fkt)(netvar_desc *, void *);
	void *ref;
} handler_descriptor_t;


//register a netvar. returns the same handle if a netvar is registered multiple times.
netvar_desc *netvar_register(uint16_t idx, uint8_t sidx, uint8_t size);

//adds handler for incoming netvar, which will be called with the user supplied reference as argument.
void netvar_add_handler(netvar_desc *nd, void (*fkt)(netvar_desc *, void *), void *ref);

//user api to write to previously registered netvars
void netvar_write(netvar_desc *nd, void *data);

//write to unregistered netvars. This is for elements, that only transmit
//on specific netvars, but are not interested in receiving data on this netvar.
void unregistered_netvar_write (uint16_t idx, uint8_t sidx, uint8_t size, void * data);

//user api to read from previously registered netvars
uint8_t netvar_read(netvar_desc *nd, void *data);

#ifdef CAN_HANDLER_C
	//called when can message on netvar port is received
	void netvar_received(can_message * msg);

	//called when netvar on bulk port is received
	void netvar_bulk_received(can_message * msg);
#endif

void netvar_handle_events();

#endif // _NETVAR_H
