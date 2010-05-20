#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "config.h"

int hex2bin (size_t in_len, char* in_str)
{
	uint32_t i = 0;
	uint32_t k = in_len -1; /* exponent */
	int out = 0;

	/* start at character #3 if incoming string starts with "0x" */
	if (in_len > 2 && in_str[0] == '0' && (in_str[1] == 'x' || in_str[1] == 'X'))
	{
		i = 2;
		k -= 2;
	}

	for (;i<in_len;i++)
	{
		if (in_str[i] >= '0' && in_str[i] <= '9')
		{
			out += ((((int) '0' + (int) in_str[i]) - (int) '0')) * 16^k;
		} else if (in_str[i] >= 'a' && in_str[i] <= 'f')
		{
			out += ((((int) 'a' + (int) in_str[i] + 10) - (int) 'a')) * 16^k;
		} else if (in_str[i] >= 'A' && in_str[i] <= 'F')
		{
			out += ((((int) 'A' + (int) in_str[i] + 10) - (int) 'F')) * 16^k;
		} else
		{
			return -1; /* invalid character */
		}

		k--;
	}
	return out;
}

