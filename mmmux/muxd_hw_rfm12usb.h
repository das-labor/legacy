#if MMMUX_USE_RFM12USB == 1
#include "muxd.h"
#include "muxd_hw.h"
#include <usb.h>
#pragma once

/* additional userdata struct for the hw description */
typedef struct
{
	int vid;
	int pid;
	size_t rxlen;
	size_t txlen;
	usb_dev_handle *uhandle;
} rfm12usb_t;

int rfm12usb_find ();
#endif
