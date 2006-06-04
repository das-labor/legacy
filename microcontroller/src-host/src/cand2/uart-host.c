#include "config.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

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
	if ( uart_fd == -1 ){
        	debug_perror("Error opening serial port %s", sport);
	}

	// set some options on socket
	fcntl(uart_fd, F_SETFL, O_NONBLOCK);

	// set serial options
	tcgetattr(uart_fd, &options);

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	options.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
	options.c_cflag |= (CS8 | CLOCAL | CREAD);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	options.c_iflag &= ~(INPCK | PARMRK | BRKINT | INLCR | ICRNL | IUCLC | IXANY);
	options.c_iflag |= IGNBRK;

	options.c_oflag &= ~(OPOST | ONLCR);

	rc = tcsetattr(uart_fd, TCSANOW, &options);

	if (rc == -1) {
		debug_perror(0, "Error setting serial options");
		close(uart_fd);
	}
}

void uart_close(){
	close(uart_fd);
}

void uart_putc(char c) {
	write(uart_fd, &c, 1);
}

void uart_putstr(char *str) {
	while(*str) {
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
	debug_assert( ret >= 0, "uart-host.c: select failed" );

	uart_getc_nb(&c);

	return c;
}
