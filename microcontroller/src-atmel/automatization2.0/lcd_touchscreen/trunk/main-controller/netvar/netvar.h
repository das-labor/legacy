
#include <stdint.h>

typedef struct {
	uint16_t idx;
	uint8_t sidx;
	uint8_t size;
	uint8_t *data;
	uint8_t flags;
	list_t *handlers; //pointer to list of handler_descriptor_t pointers
} netvar_desc;

typedef struct {
	void (*fkt)(void *);
	void *ref;
} handler_descriptor_t;


//register a netvar. returns the same handle if a netvar is registered multiple times.
netvar_desc *netvar_register(uint16_t idx, uint8_t sidx, uint8_t size);

//adds handler for incoming netvar, which will be called with the user supplied reference as argument.
void netvar_add_handler(netvar_desc *nd, void (*fkt)(netvar_desc *, void *), void *ref);

void netvar_write(netvar_desc *nd, void *data);

uint8_t netvar_read(netvar_desc *nd, void *data);

