#include "args.h"

void print_usage ()
{
	printf("Usage: %s <options>\r\n", progname);
	printf(
		"\t-v    Dump data\r\n"
		"\t-f    Run in foreground\r\n"
		"\t-r    Use rfm12usb device\r\n"
		"\t-c    Use canusb device\r\n"
	);
}

int parse_args (int argc, char* argv[])
{
	int i;
	uint8_t tmp;

	for (i=1;i<argc;i++)
	{
		if (argv[i][0] != '-')
		{
			printf ("Argument #%i not understood.\r\n", i);
			continue;
		}

		switch (argv[i][1])
		{
			case 'v':
				opt_dump = 1;
			break;
			case 'f':
				opt_foreground = 1;
			break;
			case 'r':
				opt_rfm12usb = 1;
			break;
			case 'c':
				opt_canusb = 1;
			break;
			default:
				printf ("Argument #%i not understood.\r\n", i);
			case 'h':
				print_usage();
				return -1;
			break;
		}
	}
	return 11;
}

