#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/termios.h>
#include <sys/ioctl.h>

#define LTCSLEEP 500

/**
 * - Differential == 0
 *      Channel directly addresses a input channel 
 *
 * - Differential == 1
 *      Channel      0 1 2 3 4 5 6 7
 *         0         + -
 *         1         - +
 *         2             + -
 *         3             - +
 *         4                 + -
 *         5                 - +
 *         6                     + -
 *         7                     - +
 *
 */
typedef struct ltc1290_plan {
	int      differential;
	int      channel;
	int      bipolar;
	uint16_t value;
	double   dvalue;
} ltc1290_plan_t;

int ltcfd = 0;


int ltc1290_getlines()
{
	int lines;

	if(ioctl(ltcfd,TIOCMGET,&lines)!=-1)
		return lines;

	perror("Fehler bei TIOCMGET: ");
	return -1;
}

void ltc1290_setlines(int lines)
{
	if(ioctl(ltcfd,TIOCMSET,&lines)!=-1) 
		return;

	perror("Fehler bei TIOCMSET: ");
}

int ltc1290_open(char *port)
{
	if((ltcfd=open(port,O_RDWR|O_NOCTTY)) == -1) {
		perror("Fehler bei open: ");
		return -1;
	}

	ltc1290_csoff();
	return ltcfd;
}

void ltc1290_close()
{
	close(ltcfd);
}

void ltc1290_csoff()
{
	int lines;

	lines = ltc1290_getlines();
	lines |= TIOCM_DTR;                       // CLK on
	lines &= ~TIOCM_RTS;                      // DIN off
	ltc1290_setlines(lines);
	
	usleep(LTCSLEEP);
	
	lines = ltc1290_getlines();
	lines |= TIOCM_RTS;                       // DIN off
	ltc1290_setlines(lines);
}

int ltc1290_clk(int din) 
{
	int lines;
	int ret;

	usleep(LTCSLEEP);

	lines = ltc1290_getlines();
	lines &= ~(TIOCM_DTR | TIOCM_RTS);        // CLK off, DIN off
	if (din) 
			lines |= TIOCM_RTS;               // DIN on?
	ltc1290_setlines(lines);

	usleep(LTCSLEEP);

	lines = ltc1290_getlines();
	lines |= TIOCM_DTR;                      // CLK on
	ltc1290_setlines(lines);
	if (lines & TIOCM_CTS) 
			ret = 0;
	else
			ret = 1;

	return ret;
}

void ltc1290_measure(ltc1290_plan_t *plan)
{
	
}

