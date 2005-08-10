#include "config.h"


#include <stdio.h>
#include <ezV24/ezV24.h>
#include <signal.h>

v24_port_t *Port = 0;

static void mySignalHandler ( int reason )
{
    v24ClosePort(Port);
    exit(99);
}

static void install_signalhandler ( void )
{
    signal(SIGINT,mySignalHandler);
    signal(SIGTERM,mySignalHandler);    
}



void uart_init(char *sport) {
	install_signalhandler();
	Port=v24OpenPort(sport, V24_NON_BLOCK);
	if ( Port==0 ){
        	fprintf(stderr, "error: could not open serial port %s\n", sport);
        	exit(1);
	}
	int rc = v24SetParameters(Port, V24_B57600, V24_8BIT, V24_NONE);
	if ( rc!=V24_E_OK )
	{
		fputs("error: setup of the port failed!\n",stderr);
		v24ClosePort(Port);
		exit(1);
	}
}

void uart_close(){
	v24ClosePort(Port);
}

void uart_putc(char c) {
	v24Putc(Port, c);
}


void uart_putstr(char *str) {
	while(*str) {
		uart_putc(*str++);
	}
}

unsigned char uart_getc_nb(char *c)
{
	int tmp = v24HaveData( Port );
	if(tmp>0){
		int i = v24Getc(Port);
		if(i<0){
			fputs("error: receive char failed!\n",stderr);
			v24ClosePort(Port);
			exit (1);
		}else{
			*c = (char) i;
			return 1;
		}
	}else if(tmp == 0){
		return 0;
	}else{
		fputs("error: have data failed!\n",stderr);
		v24ClosePort(Port);
		exit (1);
	}
}

char uart_getc(void)
{
	char c;
	while (! uart_getc_nb(&c) );
	return c;
}
