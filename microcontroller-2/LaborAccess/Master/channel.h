
#include "avrx.h"
#include <inttypes.h>


/***** Interface with using task *****/

//read from channel, size actually read is returned.
uint8_t channel_read(uint8_t num, uint8_t * buffer, uint8_t size);
uint8_t channel_write(uint8_t num, uint8_t * buffer, uint8_t size);


/****** Interface with receiving task *****/
void packet_received(uint8_t id, uint8_t * data, uint16_t size);
