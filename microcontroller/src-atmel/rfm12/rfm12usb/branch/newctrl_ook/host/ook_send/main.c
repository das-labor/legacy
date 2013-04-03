#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <muxd.h>

#include "../../common/requests.h"

int main (int ac, char* av[])
{
	mmmux_sctx_t *mc;

	mc = mmmuxd_init (MDBG_STDERR, NULL);
	if (mc == NULL)
	{
		printf ("can't init mmmux stuff\n");
		return - __LINE__;
	}
	
	return 0;
}
