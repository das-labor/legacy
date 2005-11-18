#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/ioctl.h>

#include "ltc1290.h"

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

void ltc1290_measure(ltc1290_plan_t plan[], int count)
{
	int i = 0;
	int val;

	ltc1290_clk(plan[i].single);
	ltc1290_clk(plan[i].channel & 0x01);
	ltc1290_clk((plan[i].channel >> 2) & 0x01);
	ltc1290_clk((plan[i].channel >> 1) & 0x01);
	ltc1290_clk(plan[i].unipolar);
	ltc1290_clk(1);                     // MSB
	ltc1290_clk(1);                     // 12 Bit
	ltc1290_clk(0);                     // 12 Bit
	ltc1290_clk(0);                     // STUFF
	ltc1290_clk(1);                     // STUFF
	ltc1290_clk(0);                     // STUFF
	ltc1290_clk(1);                     // STUFF
	ltc1290_csoff();

	i++;
	while(i<count) {
		val = 0;
		val = (val<<1) + ltc1290_clk(plan[i].single);
		val = (val<<1) + ltc1290_clk(plan[i].channel & 0x01);
		val = (val<<1) + ltc1290_clk((plan[i].channel >> 2) & 0x01);
		val = (val<<1) + ltc1290_clk((plan[i].channel >> 1) & 0x01);
		val = (val<<1) + ltc1290_clk(plan[i].unipolar);
		val = (val<<1) + ltc1290_clk(1);                     // MSB
		val = (val<<1) + ltc1290_clk(1);                     // 12 Bit
		val = (val<<1) + ltc1290_clk(0);                     // 12 Bit
		val = (val<<1) + ltc1290_clk(0);                     // STUFF
		val = (val<<1) + ltc1290_clk(1);                     // STUFF
		val = (val<<1) + ltc1290_clk(0);                     // STUFF
		val = (val<<1) + ltc1290_clk(1);                     // STUFF

		plan[i-1].value  = val << 4;
		if (plan[i-1].unipolar) {
			plan[i-1].dvalue = (5.0 * (uint16_t)(val << 4)) / (1<<16);
		} else {
			plan[i-1].dvalue = (5.0 * ( int16_t)(val << 4)) / (1<<15);
		}


		i++;
	}

	val = 0;
	val = (val<<1) + ltc1290_clk(0);
	val = (val<<1) + ltc1290_clk(0);
	val = (val<<1) + ltc1290_clk(0);
	val = (val<<1) + ltc1290_clk(0);
	val = (val<<1) + ltc1290_clk(1);
	val = (val<<1) + ltc1290_clk(1);                     // MSB
	val = (val<<1) + ltc1290_clk(1);                     // 12 Bit
	val = (val<<1) + ltc1290_clk(0);                     // 12 Bit
	val = (val<<1) + ltc1290_clk(0);                     // STUFF
	val = (val<<1) + ltc1290_clk(1);                     // STUFF
	val = (val<<1) + ltc1290_clk(0);                     // STUFF
	val = (val<<1) + ltc1290_clk(1);                     // STUFF

	plan[i-1].value  = val << 4;
	if (plan[i-1].unipolar) {
		plan[i-1].dvalue = (5.0 * (uint16_t)(val << 4)) / (1<<16);
	} else {
		plan[i-1].dvalue = (5.0 * ( int16_t)(val << 4)) / (1<<15);
	}


	ltc1290_csoff();
}

