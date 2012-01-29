#if MMMUX_USE_RFM12USB == 1
#include "muxd.h"
#pragma once

/* additional userdata struct for the hw description */
typedef struct
{
	int vid;
	int pid;
	usb_dev_handle *uhandle;
} rfm12usb_t;

int rfm12usb_find (void* in_ctx);
#endif
