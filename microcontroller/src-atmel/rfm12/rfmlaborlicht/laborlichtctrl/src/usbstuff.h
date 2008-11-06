#include <stdint.h>
#include <stdlib.h>
#include "../../../rfm12usb/common/rfm12_buffer_size.h"

typedef struct{
	unsigned char len;			                       //length byte - number of bytes in buffer
	unsigned char type;			                       //type field for airlab
	unsigned char buffer[RFM12_BUFFER_SIZE];     //generic buffer
} radio_packetbuffer;

#ifndef USBSTUFF_H
int radio_tx(unsigned char len, unsigned char type, unsigned char *data);
void rlight_setcolor (uint8_t in_red, uint8_t in_green, uint8_t in_blue);
int usbstuff_init ();
#define USBSTUFF_H
#endif
