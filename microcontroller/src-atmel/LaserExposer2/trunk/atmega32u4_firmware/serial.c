//serial.c
#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"

#include "serial.h"
#include "string.h"
#include "buffer_handler.h"
#include "laser_control.h"
#include "usb_serial.h"

void serial_sendStringandValue(char PROGMEM *pstrv, int16_t value)
{
	char buf[6];
	send_Pstr(pstrv);
	itoa(value, &buf[0], 10);
	usb_serial_putchar(&buf[0]);
	send_Pstr(PSTR("\n\r"));
}

void serial_sendstatus()
{
	uint8_t temp;
	char buf[6];
	send_Pstr(PSTR("S=\n\r"));
	temp = laser_control_getmode();
	temp = temp | (exposer.status<<3);
	itoa(temp, &buf[0], 10);
	usb_serial_putchar(&buf[0]);
	send_Pstr(PSTR("\n\r"));
}

void serial_welcomemsg()
{
	send_Pstr(PSTR("\r\nLaserPCBExposer\n\r"
		"v=0.1\n\r"));
}

void serial_sendhelpmsg()
{
	send_Pstr(PSTR("\r\nLaserPCBExposer\n\r"
		"v=0.1\n\r"
		"commands:\n\r"
		"s?   - get status\n\r"
		"cl?  - get the current line no. of the carriage\n\r"
		"sc!  - send configuration\n\r"
		"h!   - print this help text\n\r"
		"dr!  - send RAW data\n\r"
		"dc!  - send compressed data\n\r"
		));
	
}

void serial_handledata()
{
	char buf[20];
	while(usb_serial_available())
	{
		memset(&buf[0], 0x00, sizeof(buf));
		serial_getString(&buf[0],sizeof(buf));
		if(!strcmp (buf,"s?"))		//status
		{
			uint8_t temp;
			temp = laser_control_getmode();
			temp = temp | (exposer.status<<3);
			serial_sendStringandValue(PSTR("S=\n\r"),temp);
		}
		else if(!strcmp (buf,"cl?"))	//current line
		{
			serial_sendStringandValue(PSTR("cl="),exposer.currentline);
		}
		else if(!strcmp (buf,"rc?"))	//recv configuration
		{
			//serial_recvconfiguration();	//TODO !
		}
		else if(!strcmp (buf,"sc!"))	//set configuration
		{
			serial_setconfiguration();
		}
		else if(!strcmp (buf,"h!"))	//help message
		{
			serial_sendhelpmsg();
		}
		else if(!strcmp (buf,"dr!"))	//recv raw data
		{
			serial_recvrawdata();
		}
		else if(!strcmp (buf,"dc!"))	//recv compressed data
		{
			serial_recvcompdata();
		}
		else if(!strcmp (buf,"ex!"))	//start exposing, host should wait for ex=0
		{
			serial_startexposing();
		}
		else
		{
			send_Pstr(PSTR("Unknown Command:"));
			uint8_t i=0;
			while (1) {
				if (!buf[i]) break;
				usb_serial_putchar(buf[i]);
				i++;
			}
			send_Pstr(PSTR("\n\r"));
		}
	}
}

void serial_setconfiguration()
{
	int16_t r,s;
	uint8_t *p;
	uint16_t datarecsize;
	uint16_t datarecnt=0;		//counts up to datarecsize
	serial_waituntilbuffercontains(2);
	exposer_config_t newconfig;
	r = usb_serial_getchar();
	s = usb_serial_getchar();
	
	datarecsize = (r<<8)+s;	//length of following data field, must exal buffer size set in config
	if(sizeof(exposer_config_t) != datarecsize)
	{
		send_Pstr(PSTR("ERR: buffersize missmatch!\n\r"));
		return;
	}
	p=(uint8_t *)&newconfig;
	while(datarecnt < datarecsize)
	{
		do{
			r = usb_serial_getchar();
		}
		while(r < 0);
		*(p++) = (uint8_t)r;
		datarecnt++;
	}
	if(!EXPOSER_IS_RUNNING)
	{
		exposer.prism_freq = newconfig.prism_freq;
		exposer.buffer_length = newconfig.buffer_length;
		exposer.backspeed = newconfig.backspeed;
		exposer.endline = newconfig.endline;
		exposer.fastforwardspeed = newconfig.fastforwardspeed;
		exposer.plotstartpos = newconfig.plotstartpos;
		exposer.linesperrotaryenctick = newconfig.linesperrotaryenctick;
		exposer.plotforwardspeed = newconfig.plotforwardspeed;
		
		if(!exposer.prism_freq || exposer.buffer_length != newconfig.buffer_length)
			exposer.status &=  ~EXPOSER_CONFIG;	//force recalibrate
			
		laser_control_setbufsize(exposer.buffer_length);
		send_Pstr(PSTR("OK\n\r"));
	}
	else
	{
		send_Pstr(PSTR("ERR:can't calibrate now, Exposer has to be on, but now running\n\r"));
	}
	
}

void serial_getconfiguration()
{
	uint8_t *p;
	exposer_config_t curconfig;
	p=&curconfig;
	curconfig.prism_freq =exposer.prism_freq;
	curconfig.buffer_length =exposer.buffer_length;
	curconfig.backspeed =exposer.backspeed;
	curconfig.endline =exposer.endline;
	curconfig.fastforwardspeed= exposer.fastforwardspeed;
	curconfig.plotstartpos =exposer.plotstartpos;
	curconfig.linesperrotaryenctick =exposer.linesperrotaryenctick;
	curconfig.plotforwardspeed = exposer.plotforwardspeed;
	for(uint8_t i=0;i < sizeof(exposer_config_t);i++)
	{
		usb_serial_putchar(*(p++));
	}
	send_Pstr(PSTR("OK\n\r"));
}

void serial_startexposing()
{
	if(EXPOSER_BUFF_CHANGED)
	{
		if(exposer.bufferinuse == BUFFERA_IN_USE)
		{
			laser_control_setaddr(&exposer.bufferB.buffer[0]);
			exposer.bufferinuse = BUFFERB_IN_USE;
		}
		else
		{
			laser_control_setaddr(&exposer.bufferA.buffer[0]);
			exposer.bufferinuse = BUFFERA_IN_USE;
		}
		exposer.status &=~ EXPOSER_BUFF_HAS_CHANGED;
	}
	exposer.status |=EXPOSER_USB_GO;
	send_Pstr(PSTR("OK\n\r"));
}

void serial_recvrawdata()
{
	uint16_t r,s;
	uint8_t *p;
	uint16_t datarecsize;
	uint16_t datarecnt=0;		//counts up to datarecsize
	if(exposer.bufferinuse)
		p = &exposer.bufferA.buffer[0];	//point to buffer
	else
		p = &exposer.bufferB.buffer[0];
	
	serial_waituntilbuffercontains(2);

	r = usb_serial_getchar();
	s = usb_serial_getchar();

	datarecsize = (r<<8)+s;	//length of following data field, must exal buffer size set in config
	if(exposer.buffer_length != datarecsize)
	{
		send_Pstr(PSTR("ERR: buffersize missmatch!\n\r"));
		return;
	}
	while(datarecnt < datarecsize)
	{
		do{
			r = usb_serial_getchar();
		}
		while(r < 0);
		*(p++) = (uint8_t)r;
		datarecnt++;
	}
	exposer.status |= EXPOSER_BUFF_HAS_CHANGED;
	send_Pstr(PSTR("OK\n\r"));
}

void serial_waituntilbuffercontains(uint8_t cnt)
{
	while(usb_serial_available() < cnt)
		asm volatile ("nop");
}


void serial_recvcompdata()
{
	uint16_t r,s;
	exposer_buffer_t *p;
	uint16_t datarecsize;
	uint16_t datarecnt;		//counts up to datarecsize
	if(exposer.bufferinuse)
		p = &exposer.bufferA;	//point to buffer
	else
		p = &exposer.bufferB;
		
	serial_waituntilbuffercontains(2);
	
	r = usb_serial_getchar();
	s = usb_serial_getchar();
	datarecsize = (r<<8)+s;	//length of following data field
	
	resetbufferpointer(p);
	
	while(datarecnt < datarecsize)
	{
		do{
			r = usb_serial_getchar();
		}
		while(r < 0);
		do{
			s = usb_serial_getchar();
		}
		while(s < 0);
		if(r & 0x80)
			append_n_one_bits(p, ((r&0x7f)<<8)+s);
		else
			append_n_zero_bits(p, ((r&0x7f)<<8)+s);
		
		datarecnt+=2;
	}
	exposer.status |= EXPOSER_BUFF_HAS_CHANGED;
	send_Pstr(PSTR("OK\n\r"));
}



uint8_t serial_getString(char *bufp, uint8_t bufsize)
{
	int16_t r;
	do
	{
		r = usb_serial_getchar();
	}while(r == 0x0A || r == 0x0D);	// \n or \r
		
	uint8_t i=0;
	while(i < bufsize)
	{		
		do{
			r = usb_serial_getchar();
		}
		while(r < 0);
		
		if (r == 0x0A || r == 0x0D)	// \n or \r
		{
			return i;
		}
		//LCASE
		if (r > '@' && r < '[')
			r+= ('a'-'A');
		
		*bufp = (uint8_t)(r&0xff);
		bufp++;
		if (r == '?' || r == '!')	// \n or \r
		{
			return i;
		}

		i++;
	}
	return bufsize;
}

// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
//
void send_Pstr(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}
