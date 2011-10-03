#ifndef serial_h__
#define serial_h__
//serial.c
#include "serial.h"


void serial_sendcurrentline(uint16_t line);

void serial_sendstatus();

void serial_welcomemsg();

void serial_sendhelpmsg();

void serial_handledata();

void serial_recvrawdata();

void serial_recvcompdata();

uint8_t serial_getString(char *bufp, uint8_t bufsize);

void serial_setconfiguration();

void serial_startexposing();

void send_Pstr(const char *s);

void serial_waituntilbuffercontains(uint8_t cnt);

#endif
