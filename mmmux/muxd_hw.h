#include "muxd.h"

#pragma once
#if (MMMUX_USE_RFM12USB == 1)
//#include "muxd_hw_rfm12usb.h"
#endif

/* hardware init function */
typedef void (*mmmux_hw_init_f)  (void*);

/* hardware close/shutdown/deinit function */
typedef void (*mmmux_hw_close_f) (void*);

/* transmit & receive functions */
typedef ssize_t  (*mmmux_hw_tx_f)    (void*, size_t, void*);
typedef ssize_t  (*mmmux_hw_rx_f)    (void*, size_t, void*);

/* hardware description struct
 */
typedef struct
{
	char name[256];         /* human-readable name */
	mmmux_hw_init_f init;   /* init function */
	mmmux_hw_tx_f tx;       /* transmit function */
	mmmux_hw_rx_f rx;       /* receive function */
	mmmux_hw_close_f close; /* close function */
	void *udata;            /* extra data */
	void *next;
	size_t txcount;
	size_t rxcount;
} mmmux_hw_t;

int mmmux_hw_init (void);
int mmmux_hw_add (mmmux_hw_t *in_h);
void mmmux_hw_remove (mmmux_hw_t *in_h);
