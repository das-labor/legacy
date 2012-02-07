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

uint8_t nf_parse_args (int in_argc, char **in_argv, nf_flashconfig_t *out_conf)
{
	uint_fast16_t i;
	uint_fast16_t argmask = 0x00;

	for (i=0;i<in_argc;i++)
	{
		if (in_argv[i][0] == '-')
		{
			switch (in_argv[i][1])
			{
				case 'a': /* target address */
					if (in_argc <= ++i)
					{
						printf("No address given.\n");
						return (-1 * __LINE__); /* no argument given */
					}

					// if (sscanf(in_argv[i], "0x%2x", out_conf->dstaddr) != 1) return (-1 * __LINE__);
					out_conf->dstaddr = 0;

					argmask |= 0x01;
				break;
				case 'f': /* source file */
	printf ("L: %i\n", __LINE__);
					if (in_argc <= ++i)
					{
						printf("No filename given.\n");
						return (-1 * __LINE__); /* no argument given */
					}

					out_conf->fname_len = strlen((char *)((unsigned int)in_argv[i] * sizeof(char)));
	printf ("L: %i\n", __LINE__);
					out_conf->fname = malloc(out_conf->fname_len);
					strncpy (out_conf->fname, in_argv[i], out_conf->fname_len);
			//printf("aaa %i, %s", out_conf->fname_len,  out_conf->fname);
	printf ("L: %i\n", __LINE__);

					argmask |= 0x03;
				break;
				case 'v': /* verbosity level */
					out_conf->verbosity = strlen(in_argv[i] - 1);
				break;
				case '-': /* end argument parser */
					return 1;
				case 'h':
					return 0;
				default:
//					printf ("Argument not understood: %c\n", argv[i][1]);
					return (-1 * __LINE__);
			}
		}
	}
	if (argmask != 0x0003)
	{
		printf("One or more required arguments not set.\n");
		return 0;
	}
	return 1;
}
