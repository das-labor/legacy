//serial.c
#include "serial.h"
#include "string.h"

void serial_sendcurrentline(uint16_t line);

void serial_sendstatus();

void serial_welcomemsg();

void serial_sendhelpmsg();

void serial_handledata();


uint8_t datarecsize;
uint8_t datareccnt;
uint8_t datarec;
void serial_recvrawdata();

void serial_recvcompdata();

uint8_t serial_getString(char *bufp, uint8_t bufsize);

void send_Pstr(const char *s);

void send_str(const char *s);

