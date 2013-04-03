#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <muxd.h>
#include <endian.h>
#include <unistd.h>

#include "../../common/requests.h"

void print_help (int ac, char* av[])
{
	printf ("usage:\n  %s <housecode> <switch> <on|off>\n", av[0]);
	printf ("  housecode: housecode in binary - i.e. 10011\n");
	printf ("  switch:    switch code (A to E)\n");
	printf ("  on/off:    accepted values: on, off, 0, 1\n\n");
	printf ("alternate usage: %s disco\n", av[0]);
}

void ook_next (uint8_t **in_p)
{
	int i;
	uint8_t *dp = *in_p;
	uint16_t a = ((dp[1] << 8) | dp[2]) & 0x1ff0;

	/* increase switch code */
	switch (a)
	{
		case 0x0550: /* A */
			a = 0x1151;
		break;
		case 0x1150:
			a = 0x1451;
		break;
		case 0x1450:
			a = 0x1511;
		break;
		case 0x1510:
			a = 0x1541;
		break;
		case 0x1540: /* E -> A */
			a = 0x0550;
		break;

		default:
			printf ("WTF (%i)\n", __LINE__);
		break;
	}

	dp[2] = (dp[2] & 0x0f) | (a & 0x00f0);
	dp[1] = (dp[1] & 0xf0) | ((a >> 8) & 0x1f);

	if (a != 0x0550)
		return; /* don't increment housecode */
	
	printf ("\n");
	/* housecode++ */
	if ((dp[1] & 0xf0) == 0x10) /* dip switch #5 */
	{
		dp[1] = 0x45;
		dp[2] = 0x50 | (dp[2] & 0x0f);
		return;
	}

	dp[1] = 0x05;
	dp[2] = 0x51;
	
	for (i=0; i<2; i++)
	{
		uint8_t shift = i*4;
		uint8_t msk = (0x0f << shift);
		switch ((dp[0] & msk) >> shift) /* switches 1..4 */
		{
			case 0x00:
				dp[0] |= (0x01 << shift);
				return;

			case 0x01:
				dp[0] = (dp[0] & ~(msk)) | (0x04 << shift);
				return;

			case 0x04:
				dp[0] |= (0x01 << shift);
				return;

			case 0x05:
				dp[0] &= ~(msk);
				break; /* increment 1,2 */
		}
	}
}

int main (int ac, char* av[])
{
	mmmux_sctx_t *mc;
	static uint8_t txdata[64];
	static uint8_t *ook_payload;
	static uint8_t **pp;
	static rfmusb_ook_t *ook_header = (rfmusb_ook_t *) txdata;
	enum mymode_e {
		single,
		disco
	} mymode = single;
	int i;
	
	ook_payload = txdata + sizeof(rfmusb_ook_t);
	pp = &ook_payload;

	mc = mmmuxd_init (MDBG_NONE, NULL);
	if (mc == NULL)
	{
		printf ("can't init mmmux stuff\n");
		return - __LINE__;
	}

	ook_header->len   = 24;                  /* 24 bits */
	ook_header->type  = OOK_2722;            /* 2722 power plug */
	ook_header->delay = htole16(500);        /* 500us between flanks */
	ook_header->count = 16;                  /* repeat 16 times */
	

	mmmux_ctrl (mc, mode_ook, NULL);
	usleep (100000);

	if (ac < 2)
	{
		print_help (ac, av);
		return - __LINE__;
	}

	if (!strncasecmp (av[1], "disco", 5))
	{
		printf ("disco mode: %s\n", av[1]);
		mymode = disco;
	} else
	{
		mymode = single;
	}
	
	switch (mymode)
	{
		case disco:
			ook_payload[0] = 0x00;
			ook_payload[1] = 0x05;
			ook_payload[2] = 0x51;

			printf ("this program will run forever and switch on all powerplugs in the neighborhood ;-)\n");
			printf ("press CTRL-C to abort.\n");
			while (1)
			{
				printf ("sending code: %02X, %02X, %02X \t",
					ook_payload[0],
					ook_payload[1],
					ook_payload[2]
					);
				mmmux_send (mc, txdata, sizeof(rfmusb_ook_t) + 3);
				ook_next (pp);
				usleep (1750000);
			}
		break;
		case single:
		{
			uint8_t *txcode = txdata + sizeof (rfmusb_ook_t);
			int hc_tmp;
			int shift = 4;

			memset (txcode + sizeof(rfmusb_ook_t), 0x00, 3);

			if (ac != 4)
			{
				print_help (ac, av);
				return - __LINE__;
			}
			
			for (i=0; i<4; i+=2)
			{
				hc_tmp = (av[1][i] << 8) + av[1][i+1];
				switch (hc_tmp)
				{
					case ('0'<<8) + '0':
						txcode[0] += (0x05 << shift);
					break;
					case ('1'<<8) + '0':
						txcode[0] += (0x01 << shift);
					break;
					case ('0'<<8) + '1':
						txcode[0] += (0x04 << shift);
					break;
					case ('1'<<8) + '1':
						/* + 0 */
					break;
					default:
						printf ("can't parse txcode[0]\n");
						return -1;
				}
				shift -= 4;
			}

			/* bit 5 */
			if (av[1][4] == '0')
			{
				txcode[1] = 0x40;
			} else if (av[1][4] != '1')
			{
				printf ("can't parse housecode (bit 5)\n");
				return -1;
			}
			
			switch (av[2][0])
			{
				case 'a':
				case 'A':
					txcode[1] += 0x05;
					txcode[2]  = 0x50;
				break;
				case 'b':
				case 'B':
					txcode[1] += 0x11;
					txcode[2]  = 0x50;
				break;

				case 'c':
				case 'C':
					txcode[1] += 0x14;
					txcode[2]  = 0x50;
				break;
				case 'd':
				case 'D':
					txcode[1] += 0x15;
					txcode[2]  = 0x10;
				break;
				case 'e':
				case 'E':
					txcode[1] += 0x15;
					txcode[2]  = 0x40;
				break;
			}


			if (!strncasecmp (av[3], "on", 2) || av[3][0] == '1')
			{
				txcode[2] += 0x01;
			} else
			{
				txcode[2] += 0x04;
			}
			printf ("sending code: %02X, %02X, %02X\n",
				ook_payload[0],
				ook_payload[1],
				ook_payload[2]);

			mmmux_send (mc, txdata, sizeof(rfmusb_ook_t) + 3);
		}
		break;
	}
	
	usleep (100000);
	mmmux_ctrl (mc, mode_normal, NULL);
	
	return 0;
}
