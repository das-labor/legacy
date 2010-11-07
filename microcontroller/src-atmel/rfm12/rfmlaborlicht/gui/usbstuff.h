#include <stdint.h>
#include <stdlib.h>
#include <usb.h>
#include <string.h>
#include <stdio.h>

#if 0
#include "../../rfm12usb/common/rfm12_buffer_size.h"
#include "../common/protocol.h"
#include "usb_id.h"
#include "../../rfm12usb/common/requests.h"
#include "opendevice.h"
#endif

#include "../../rfm12usb/trunk/host/CDriver/RfmUsb.h"
#include "../common/protocol.h"

#ifndef USBSTUFF_H

#define DEFAULT_USB_TIMEOUT 1000
#define RADIO_TXBUFFER_HEADER_LEN 2

typedef struct{
	unsigned char len;			                       //length byte - number of bytes in buffer
	unsigned char type;			                       //type field for airlab
	unsigned char buffer[8];     //generic buffer
} radio_packetbuffer;

int radio_tx(unsigned char len, unsigned char type, unsigned char *data);
void rlight_setcolor (uint8_t in_red, uint8_t in_green, uint8_t in_blue);
int usbstuff_init ();

#define USBSTUFF_H
#endif
