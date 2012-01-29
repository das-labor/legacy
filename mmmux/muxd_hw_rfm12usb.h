#include "muxd_config.h"

#if MMMUX_USE_RFM12USB == 1
#include "muxd.h"
#include "muxd_hw.h"
#include <usb.h>
#pragma once

#define RFMUSB_RQ_ECHO             0x00
#define RFMUSB_RQ_RFM12_PUT        0x01
#define RFMUSB_RQ_RFM12_GET        0x02
#define RFMUSB_RQ_GETNOTIFICATION  0x03
#define DEFAULT_USB_TIMEOUT        1000


/* additional userdata struct for the hw description */
typedef struct
{
	int vid;
	int pid;
	size_t rxlen;
	size_t txlen;
	usb_dev_handle *uhandle;
} rfm12usb_t;

int rfm12usb_find (mmmux_sctx_t *in_c);
#endif
