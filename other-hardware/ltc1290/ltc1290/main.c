#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include "ltc1290.h"

char *progname;

static char *optstring = "hvi:c:p:o:";
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
    -i, --interval MSEC     interval between two measurements in milliseconds\n\
    -c, --count NUMBER      number of iterations\n\
    -o, --out FILENAME      output file\n\n\
Channel-Plan:\n\n\
    Multiple channel specs:\n\
    <Channel>[u/b]          channel number followed by u (unipolar) or\n\
	                                               b (bipolar)\n\
Example:\n\n\
    ltc1290 1u 2u 3b 4u  -- measure channel 1, 2 and 4 unipolar, channel 3 bipolar.\n\
    ltc1290 12b 34u      -- measure difference between chan 1 and 2 in bipolar mode;\n\
                            difference between 3 and 4 in unipolar mode.\n\n");

}

int parse_chanspec(char *str, ltc1290_plan_t *plan)
{
	if (!strncmp("12", str, 2)) {
		plan->channel = 0;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("21", str,2)) {
		plan->channel = 1;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("34", str,2)) {
		plan->channel = 2;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("43", str,2)) {
		plan->channel = 3;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("56", str,2)) {
		plan->channel = 4;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("65", str,2)) {
		plan->channel = 5;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("78", str,2)) {
		plan->channel = 6;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("87", str,2)) {
		plan->channel = 7;
		plan->single = 0;
		str += 2;
	} else if (!strncmp("1", str, 1)) {
		plan->channel = 0;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("2", str, 1)) {
		plan->channel = 1;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("3", str, 1)) {
		plan->channel = 2;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("4", str, 1)) {
		plan->channel = 3;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("5", str, 1)) {
		plan->channel = 4;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("6", str, 1)) {
		plan->channel = 5;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("7", str, 1)) {
		plan->channel = 6;
		plan->single = 1;
		str += 1;
	} else if (!strncmp("8", str, 1)) {
		plan->channel = 7;
		plan->single = 1;
		str += 1;
	} else {
		fprintf(stderr, "Could not parse channel spec.: %s\n", str);
		return -1;
	}

	if ((*str == 0) || !strcmp("b", str)) {
		plan->unipolar = 0;
	} else if (!strcmp("u", str)) {
		plan->unipolar = 1;
	} else {
		fprintf(stderr, "Could not parse channel spec.: %s (uni/bipolar?)\n", str);
		return -1;
	}

	return 0;
}	

void print_values(ltc1290_plan_t *plan, int numchan)
{
	int i;
	for(i=0; i<numchan; i++) {
		printf("% 01.3f \t", plan[i].dvalue);
	}

	printf("\n");
	fflush(stdout);
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
	int numchan, i;
	ltc1290_plan_t plan[10];

	numchan = argc - optind;
	for(i=0; i<numchan; i++) {
		if (parse_chanspec(argv[optind+i], &plan[i]))
			return 1;
	}

	printf( "numchan: %d\n", numchan );

	ltc1290_open(port);
	usleep(1000 * interval);
	ltc1290_measure(plan, numchan);
	usleep(1000 * interval);
	while(1) {
		ltc1290_measure(plan, numchan);
		print_values(plan, numchan);

		usleep(1000 * interval);
	}
	ltc1290_close(port);
}



