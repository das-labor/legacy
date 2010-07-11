#ifndef _RFMUSB_H
#define _RFMUSB_H

#include "rfmusb_err.h"

//for rfm12 tx/rx buffer size
#include "rfm12_config.h"


////////
// various buffer sizes
////////

//define the usb transmit buffer size 2 bytes larger than the rfm12 usb tx buffer
//(header overhead)
#define RFMUSB_USBTXBUFFER_SIZE RFM12_RX_BUFFER_SIZE + 2

//define the usb receive buffer size 2 bytes larger than the rfm12 usb tx buffer
//(header overhead)
#define RFMUSB_USBRXBUFFER_SIZE RFM12_TX_BUFFER_SIZE + 2

//size of the notify buffer
#define RFMUSB_NOTIFYBUFFER_SIZE 7

//and its header size
#define RFMUSB_NOTIFYBUFFER_OVERHEAD 1


////////
// notify buffer states
////////

#define USBRFM_NOTIFYBUF_FREE 0
#define USBRFM_NOTIFYBUF_OCCUPIED 1


////////
// notify buffer data structures
////////

typedef struct
{
	uint8_t notifyType;
	uchar data[RFMUSB_NOTIFYBUFFER_SIZE];
	uint8_t len;
} rfmusb_notifyPacket;

#endif

