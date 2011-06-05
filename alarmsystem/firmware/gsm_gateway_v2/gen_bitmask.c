#include <stdint.h>
#include <stdio.h>

uint8_t u_log_2 (uint32_t in_i)
{
	uint8_t out_l = 0;
	uint32_t tmp = in_i;

	while (tmp >>= 1)
		out_l++;
	
	return out_l;
}

void print_unit_macro (uint8_t in_us, uint8_t in_lim, char* in_name)
{
	// printf ("/* us = %8u, lim = %8u */\n", in_us, in_lim);

	uint32_t us_per_t = 1, lim;
	uint8_t i  = in_us, s, k;
	
	for (i = in_us; i > 0 ; i--)
		us_per_t *= 10;
	
	lim = (0x02 << in_lim);
	s = u_log_2(us_per_t) + 1;
	k = u_log_2(lim);

	if (k > s)
	{
		// s = u_log_2(lim - us_per_t);

		if (k - s > 15)
			printf ("\t#define _CRON_%s_CIRCA(a) (0)\n", in_name);
		else
			printf ("\t#define _CRON_%s_CIRCA(a) (a >> %u)\n", in_name, k - s);
		
		return;
	}
	
	if (s-k == 0)
	{
		printf ("\t#define _CRON_%s_CIRCA(a) (a)\n", in_name);
		return;
	}
	
	/* ... us_per_t > lim  ... */ 

	if (s - k > 15)
		printf ("\t#define _CRON_%s_CIRCA(a) (-1)\n", in_name);
	else
		printf ("\t#define _CRON_%s_CIRCA(a) (a << %u)\n", in_name, (s - k));
	
	

	return;

}

int main (int ac, char* av[])
{
	uint8_t i;
	uint32_t msk = 0x02;

	printf("/* this file is automatically generated by %s */\n", av[0]);

	for (i=0;i<31;i++)
	{
		/* SHIFT MASK*/
		printf("#%sif CRON_US_PER_TICK < 0x%08X\n", (i == 0x0) ? "" : "el", 0x02 << i);
		print_unit_macro (0, i, "US");
		print_unit_macro (3, i, "MS");
		print_unit_macro (6, i, "S");
		//print_unit_macro (54, i, "M");
		msk <<= 1;
	}
	printf("#endif\n");

}
