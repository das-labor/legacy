#ifndef _RFMUSB_H
#define _RFMUSB_H

#include "rfmusb_err.h"


////////
// various buffer sizes
////////

//define the usb transmit buffer size 2 bytes larger than the rfm12 usb tx buffer
//(header overhead)
#define I2CUSB_USBCARDBUFFER_SIZE 256 + 2


////////
// cardstates
////////

#define I2CUSB_CARD_INSERTED	1
#define I2CUSB_CARD_NONE	0

#endif

