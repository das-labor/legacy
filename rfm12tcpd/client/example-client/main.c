#include "../airmux-client.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define BSIZE 1500

int main (int argc, char* argv[])
{
	char *dhost, *dport;
	uint_fast32_t i;
	size_t sz;
	fd_set rfds, wfds;
	airmux_context_t *my_ctx;
	uint8_t *rxbuf = NULL, *txbuf = NULL;
	time_t t;
	t = time(NULL);
	
	txbuf = malloc (sizeof(time_t));
	printf("Airmux example client\r\n");
	
	if (argc > 3)
	{
		printf("usage: %s [hostname] [port]\r\n", argv[0]);
		return -1;
	}

	if (argc >= 1)
	{
		dhost = argv[1];
	} else
	{
		dport = NULL;
	}
	
	if (argc >= 2)
	{
		dport = argv[2];
	} else
	{
		dport = NULL;
	}

	my_ctx = airmux_init();

	if (0 > airmux_connect(my_ctx, dhost, dport))
	{
		printf("Could not connect to remote side\r\n");
		return -1;
	}

	rxbuf = malloc (BSIZE);
	txbuf = malloc (BSIZE);
	
	while (42)
	{
		if (sz = airmux_rx (my_ctx, rxbuf, BSIZE))
		{
			printf("received:");
			for (i=0;i<sz;i++)
			{
				if (!(i % 32))
					printf("\r\n");
				printf("%02X", rxbuf[i]);
			}
			printf("\r\n");
		}
		if ((uint32_t) time(NULL) - (uint32_t) t > 10)
		{
			t = time(NULL);
			time((time_t *) txbuf);
			airmux_tx (my_ctx, txbuf, sizeof(time_t));
			printf("Sending %u\r\n", (uint32_t) t);
		}
	}
}
