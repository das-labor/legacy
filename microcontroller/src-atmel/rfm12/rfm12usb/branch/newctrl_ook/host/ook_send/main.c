#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <muxd.h>
#include <endian.h>
#include <unistd.h>

#include "../../common/requests.h"

int main (int ac, char* av[])
{
	mmmux_sctx_t *mc;
	uint8_t txdata[64];
	uint8_t *ook_payload = txdata + sizeof(rfmusb_ook_t);
	rfmusb_ook_t *ook_header = txdata;

	mc = mmmuxd_init (MDBG_STDERR, NULL);
	if (mc == NULL)
	{
		printf ("can't init mmmux stuff\n");
		return - __LINE__;
	}

	ook_header->len   = 24;         /* 24 bits */
	ook_header->type  = OOK_2722;   /* 2722 power plug */
	ook_header->delay = htole16(500);        /* 500us between flanks */
	ook_header->count = 16;         /* repeat 16 times */
	
	ook_payload[0] = 0x41;
	ook_payload[1] = 0x45;
	ook_payload[2] = 0x51;

	printf ("sleeping\n");

	printf ("ctrl ook: %i\n", mode_ook);
	mmmux_ctrl (mc, mode_ook, NULL);
	sleep (1);
	
	printf ("txdata\n");
	mmmux_send (mc, txdata, sizeof(rfmusb_ook_t) + 3);
	sleep (1);
	
	printf ("ctrl normal\n");
	mmmux_ctrl (mc, mode_normal, NULL);
	
	return 0;
}
