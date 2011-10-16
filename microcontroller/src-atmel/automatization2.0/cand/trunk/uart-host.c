#include "config.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

int uart_fd;


static void mySignalHandler ( int reason )
{
	// XXX
}

static void install_signalhandler ( void )
{
    signal(SIGINT,mySignalHandler);
    signal(SIGTERM,mySignalHandler);    
}


void uart_init(char *sport) {
	int rc;
	struct termios options;

	install_signalhandler();
	
	/**
	 * O_NOCTTY -- ttyS is not our controlling terminal: 
	 *   kernel, do not do fancy stuff! We're not getty, stupid!
	 *
	 * O_NDELAY -- don't block for DTR, we're not talking to a modem
	 */
	uart_fd = open(sport, O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart_fd == -1) {
        	debug_perror(0, "Error opening serial port %s", sport);
		exit(EXIT_FAILURE);
	}

	// set some options on socket
	fcntl(uart_fd, F_SETFL, O_NONBLOCK);

	// set serial options
	tcgetattr(uart_fd, &options);

//	#if defined(__WINDOWS__) | defined(__CYGWIN__)
		/*
		 * needed because cfsetspeed is not available on Windows. 
		 */
		cfsetispeed(&options, UART_BAUD_RATE);
		cfsetospeed(&options, UART_BAUD_RATE);
//	#else
		/*
		* Replaced the above with the following in the hope that it fixes the init
		* bug.
		*/
//		cfsetspeed(&options, UART_BAUD_RATE);
//	#endif

	options.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	options.c_cflag |= (CS8 | CLOCAL | CREAD);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	options.c_iflag &= ~(INPCK | PARMRK | BRKINT | INLCR | ICRNL | IUCLC | IXANY);
	options.c_iflag |= IGNBRK;

	options.c_oflag &= ~(OPOST | ONLCR);

	rc = tcsetattr(uart_fd, TCSANOW, &options);

	if (rc == -1) {
		debug_perror(0, "Error setting serial options");
		close(uart_fd);
		exit(EXIT_FAILURE);
	}
}

void uart_close() {
	close(uart_fd);
}

void uart_putc(char c) {
	ssize_t ret = write(uart_fd, &c, 1);
	if (ret != 1)
	{
		printf("uart_putc faild: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void uart_putstr(char *str) {
	while (*str) {
		uart_putc(*str++);
	}
}

unsigned char uart_getc_nb(char *c)
{
	int ret;

	ret = read(uart_fd, c, 1);
		       
	debug(3, "%d\n", c);
	if (ret<=0) return 0;

	return 1;
}

char uart_getc(void)
{
	char c;
	int ret;
	fd_set rset;

	FD_ZERO(&rset);
	FD_SET(uart_fd, &rset);

	ret = select(uart_fd + 1, &rset, (fd_set*)NULL, (fd_set*)NULL, NULL);
	debug_assert(ret >= 0, "uart-host.c: select failed");

	uart_getc_nb(&c);

	return c;
}
