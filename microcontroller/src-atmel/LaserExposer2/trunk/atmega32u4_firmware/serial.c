//serial.c
#include "serial.h"
#include "string.h"

void serial_sendcurrentline(uint16_t line)
{
	char buf[6];
	send_Pstr(PSTR("cl=\n\r"));
		
	itoa(exposer.plotstartpos, &buf[0], 10);
	send_str(&buf);
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
	send_str(&buf);
	send_Pstr(PSTR("\n\r"));
}

void serial_welcomemsg()
{
	send_str(PSTR("\r\nLaserPCBExposer\n\r"
		"v=0.1\n\r"));
}

void serial_sendhelpmsg()
{
	send_str(PSTR("\r\nLaserPCBExposer\n\r"
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
	int16_t r;
	char buf[10];
	uint8_t stringsize;
	while(usb_serial_available())
	{
		if(!datarecsize)
		{
			stringsize = serial_getString(&buf[0],sizeof(buf));
			if(!strcmp (&buf,"s?"))		//status
			{
				serial_sendstatus();
			}
			if(!strcmp (&buf,"cl?"))	//current line
			{
				serial_sendcurrentline();
			}
			if(!strcmp (&buf,"rc?"))	//recv configuration
			{
				serial_recvconfiguration();
			}
			if(!strcmp (&buf,"sc!"))	//set configuration
			{
				serial_setconfiguration();
			}
			if(!strcmp (&buf,"h!"))	//help message
			{
				serial_sendhelpmsg();
			}
			if(!strcmp (&buf,"dr!"))	//recv raw data
			{
				serial_recvrawdata();
			}
			if(!strcmp (&buf,"dc!"))	//recv compressed data
			{
				serial_recvcompdata();
			}
		}
		else
		{
			if(datarec == 1)	//recv raw data
				serial_recvrawdata();
			elseif(datarec == 2)	//recv compressed data
				serial_recvcompdata();
		}
	}
}

uint8_t datarecsize;
uint8_t datareccnt;
uint8_t datarec;
void serial_recvrawdata()
{
	int16_t r,s;
	uint8_t *p;
	p = &exposer.buffers.bufferA[0];
	
	while(usb_serial_available() < 2)
		asm volatile ("nop");
		
	datarec = 1;
	
	if(!datareccnt)
	{
		r = usb_serial_getchar();
		s = usb_serial_getchar();

		datarecsize = (r<<8)+s;
		datareccnt  = 0;
		r = usb_serial_getchar();
		while((datarecnt < datarecsize) && (r > 0))
		{
			*p++ = (uint8_t)r;
			datarecnt++;
			r = usb_serial_getchar();
		}
	}
	else
	{
		p += datareccnt;
		r = usb_serial_getchar();
		while((datarecnt < datarecsize) && (r > 0))
		{
			*p++ = (uint8_t)r;
			datarecnt++;
			r = usb_serial_getchar();
		}
	}
	if(datarecnt == datarecsize)
			datarecsize = 0;
}

void serial_recvcompdata()
{
	int16_t r,s;
	uint8_t *p;
	
	p = &exposer.buffers.bufferA[0];
	
	while(usb_serial_available() < 2)
		asm volatile ("nop");
	
	datarec = 2;
	
	if(!datareccnt)
	{
		r = usb_serial_getchar();
		s = usb_serial_getchar();

		datarecsize = (r<<8)+s;
		datareccnt  = 0;
		r = usb_serial_getchar();
		while((datarecnt < datarecsize) && (r > 0))
		{
			if( ((uint8_t)r)&0x80)	//is MSB == 1
				*p++ = 
				
				
				
			datarecnt++;
			r = usb_serial_getchar();
		}
	}
	else
	{
		p += datareccnt;
		r = usb_serial_getchar();
		while((datarecnt < datarecsize) && (r > 0))
		{
			*p++ = (uint8_t)r;
			datarecnt++;
			r = usb_serial_getchar();
		}
	}
	if(datarecnt == datarecsize)
		datarecsize = 0;
}



uint8_t serial_getString(char *bufp, uint8_t bufsize)
{
	uint8_t cnt = 0;
	r = usb_serial_getchar();
	while(r == '\r' || r == '\n')
		r = usb_serial_getchar();
			
	for(uint8_t i=0; i < bufsize;i++)
	{
		if(r == -1)
			break;
			
		if (r == '\r' || r == '\n')
		{
			return cnt;
		}
		//UCASE
		if (r > '@' && r < '[')
			r+= ('a'-'A');
		*bufp++ = r;
		
		r = usb_serial_getchar();
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

void send_str(const char *s)
{
	while (*s) {
		usb_serial_putchar(*s++);
	}
}
