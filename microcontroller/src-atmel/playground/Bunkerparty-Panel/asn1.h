
#include <inttypes.h>

typedef struct{
	uint16_t offset;
	uint16_t size;
}asn1_obj_t;

//get offset and size of an object inside of a constructed object.
//returns 0 on success.
uint8_t asn1_get(asn1_obj_t * child, asn1_obj_t * parent, uint8_t id);

//read data of an id in the given constructed object. returns size read or 0 on error.
uint16_t asn1_read(asn1_obj_t * parent, uint8_t id, uint8_t * buffer, uint16_t size);

//write data of an id in the given constructed object. returns size written or 0 on error.
uint16_t asn1_write(asn1_obj_t * parent, uint8_t id, uint8_t * buffer, uint16_t size);
