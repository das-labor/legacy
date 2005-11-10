#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <getopt.h>

char *progname;

static char *optstring = "hvi:c:p::o";
struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "verbose", no_argument, NULL, 'v' },
  { "interval", required_argument, NULL, 'i' },
  { "count", required_argument, NULL, 'c' },
  { "port", required_argument, NULL, 'p' },
  { "out", required_argument, NULL, 'o' },
  { NULL, 0, NULL, 0 }
};

void help() 
{
	printf("\nUsage %s [OPTIONS] <CHANNEL-PLAN>\n", progname);
	printf("\n\
Options\n\n\
    -h, --help              display thos help and exit\n\
    -p, --port PORT         serial port (default: /dev/ttyS0)\n\
    -i, --interval MSEC    interval between two measurements in milliseconds\n\
    -c, --count NUMBER      number of iterations\n\
    -o, --out FILENAME      output file\n\n");


}

int main(int argc, char *argv[])
{
	char *port    = "/dev/ttyS0";
	char *outfile = NULL;
	int  verbose  = 0;
	int  interval = 500;
	int  optc;

	progname = argv[0];

	while ((optc=getopt_long(argc, argv, optstring, longopts, (int *)0))
		!= EOF) {
		switch (optc) {
			case 'v':
				verbose = 1;
				break;
			case 'i':
				interval = atoi(optarg);
				break;
			case 'p':
				port = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				help();
				exit(0);
			default:
				help();
				exit(1);
		}
	} // while

	// argv[optind];
}



