#ifndef S_COMMON_H
#define S_COMMON_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

#include "../common/protocol.h"
#include "../common/encap.h"


/* absoloute maximum length for rx/tx buffers */
#define BUF_MAXLEN 16384

uint8_t
	opt_debug,
	opt_foreground,
	opt_rfm12usb,
	opt_canusb,     /* not yet implemented - any volounteers? */
	opt_dump;

char* progname;
char
	opt_port[6];

#include "rfmusb.h"
#include "args.h"
#ifndef MAX
	#define MAX(a,b) ((a > b) ? a : b)
#endif
#endif /* S_COMMON_H */
