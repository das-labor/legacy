#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <curses.h>



#define NOP     0x80
#define CLK_ON  0x01
#define CLK_OFF 0x02
#define DIN_ON  0x04
#define DIN_OFF 0x08
#define SPL     0x10
#define PRINT   0x20

int seq_cs[] = {
	CLK_ON + DIN_OFF,		// ~CS
	DIN_ON,				// ~CS
	0
};

int seq_mess[] = { 
	CLK_OFF + DIN_ON,		// SGL
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,		// ODD
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,     	// SELECT1
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,      // SELECT0
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,       // UNI
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,       // MSB-First
	CLK_ON + SPL,
	CLK_OFF + DIN_ON,       // WL1
	CLK_ON + SPL,
	CLK_OFF + DIN_OFF,      // WL0
	CLK_ON + SPL,
	CLK_OFF,				// t_conv (0)
	CLK_ON + SPL,
	CLK_OFF,				// t_conv (1)
	CLK_ON + SPL,
	CLK_OFF,				// t_conv (2)
	CLK_ON + SPL,
	CLK_OFF,				// t_conv (3)
	CLK_ON + SPL + PRINT,
	CLK_ON + DIN_OFF,			// ~CS
	DIN_ON,					// ~CS
	0
};






// Non Blocking Keyboard
static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard()
{
	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}

close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}


int kbhit()
{
	char ch;
	int nread;

	if(peek_character != -1)
		return 1;
	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0,&ch,1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_settings);

	if(nread == 1) {
		peek_character = ch;
		return 1;
	}
	return 0;
}

int readch()
{
	char ch;

	if(peek_character != -1) {
		ch = peek_character;
		peek_character = -1;
		return ch;
	}
	read(0,&ch,1);
	return ch;
}

// MESS

int mess_fd;
int mess_debug = 0;

void mess_open(char *port)
{
	if((mess_fd=open(port,O_RDWR|O_NOCTTY)) != -1)
		return;

	perror("Konnte den Port nicht oeffnen: ");
	return;
}

int mess_getlines()
{
	int lines;

	if(ioctl(mess_fd,TIOCMGET,&lines)!=-1)
		return lines;

	perror("Fehler bei TIOCMGET: ");
	return -1;
}

void mess_setlines(int lines)
{
	if(ioctl(mess_fd,TIOCMSET,&lines)!=-1) 
		return;

	perror("Fehler bei TIOCMSET: ");
}


void mess_dumplines(int lines)
{
	if (!mess_debug)
		return;

	printf("> ");
	if (lines & TIOCM_LE)
		printf("LE ");
	else
		printf("le ");

	if (lines & TIOCM_DTR)
		printf("DTR ");
	else
		printf("dtr ");

	if (lines & TIOCM_RTS)
		printf("RTS ");
	else
		printf("rts ");

	if (lines & TIOCM_ST)
		printf("ST ");
	else
		printf("st ");

	if (lines & TIOCM_SR)
		printf("SR ");
	else
		printf("sr ");

	if (lines & TIOCM_CTS)
		printf("CTS ");
	else
		printf("cts ");

	if (lines & TIOCM_CD)
		printf("CD ");
	else
		printf("cd ");

	if (lines & TIOCM_RI)
		printf("RI ");
	else
		printf("ri ");

	if (lines & TIOCM_DSR)
		printf("DSR ");
	else
		printf("dsr ");

	printf("<\n");
}

void run_seq(int *cmd)
{
	int lines;
	int val = 0;

	while(*cmd) {
		lines = mess_getlines();
		mess_dumplines(lines);
		if (*cmd & CLK_ON) 
			lines |= TIOCM_DTR;
		if (*cmd & CLK_OFF) 
			lines &= ~TIOCM_DTR;
		if (*cmd & DIN_ON) 
			lines |= TIOCM_RTS;
		if (*cmd & DIN_OFF) 
			lines &= ~TIOCM_RTS;
		if (*cmd & SPL) {
			val <<= 1;
			if (!(lines&TIOCM_CTS))
				val++;
		};
		if (*cmd & PRINT) {
			printf("\t%3.3f\n", (5.0*val)/((1<<12)-1));
			fflush(stdout);
			val = 0;
		};
		mess_setlines(lines);

		cmd++;
		if (*cmd) usleep(50);
	}
}


int main()
{
	int lines;
	int quit=0;

	mess_open("/dev/ttyUSB0");

	run_seq(seq_cs);

	while(!quit) {
		run_seq(seq_mess);
		usleep(100000);
	}
}
