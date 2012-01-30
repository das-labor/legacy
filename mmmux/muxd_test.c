#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "muxd.h"

int main (int argc, char* argv[])
{
	uint8_t buf[1024];
	int rv, i;
	mmmux_sctx_t *my_context;	

	my_context = mmmuxd_init (1, NULL);

	printf ("context %p\n", my_context);
	
	while (42)
	{
		rv = mmmux_receive (my_context, buf, sizeof(buf));
		if (rv <= 0)
			return rv;

		for (i=0;i<rv;i++)
		{
			printf ("%02X ", buf[i]);
			if (i > 0 && !(i % 16))
				printf ("\n");
		}
		printf ("\n\n");
	}
}
