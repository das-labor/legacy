#include <windows.h>
#include "config.h"

HANDLE serialPortHandle;
DCB serialConfig;

extern unsigned char color[3];

void uart_init(char *portName) {
	serialPortHandle = CreateFile(portName, 
							      GENERIC_READ | GENERIC_WRITE, 
							      0, 
							      NULL, 
							      OPEN_EXISTING, 
							      FILE_FLAG_NO_BUFFERING, 
							      NULL);
	
	GetCommState(serialPortHandle, &serialConfig);
	serialConfig.BaudRate = CBR_57600;
	serialConfig.Parity = NOPARITY;
	serialConfig.ByteSize = 8;
	serialConfig.StopBits = ONESTOPBIT;
	serialConfig.fOutxCtsFlow = FALSE;
	serialConfig.fOutxDsrFlow = FALSE;
	serialConfig.fRtsControl = RTS_CONTROL_ENABLE;
	serialConfig.fOutX = FALSE;
	serialConfig.fInX = FALSE;
	SetCommState(serialPortHandle, &serialConfig);
}


void put_c(const char inChar) {
	char buffer = inChar;
	DWORD charsWritten = 0;
	WriteFile(serialPortHandle, &buffer, sizeof(char), &charsWritten, NULL);
}

int put_str(char *inBuffer) {
	DWORD charsWritten = 0;
	WriteFile(serialPortHandle, inBuffer, strlen(inBuffer), &charsWritten, NULL);
	return charsWritten;
}

void uart_sendStopp() {
     put_c(UART_ESCAPE);
	 put_c(UART_SS);
}

void uart_sendDirectColor() {
    put_c(0x00);
    put_c(0x01);
    put_c(0x05);
    put_c(color[0]);
    put_c(color[1]);
    put_c(color[2]);
}

void uart_sendFadeColor() {
    put_c(0x00);
    put_c(0x01);
    put_c(0x04);
    put_c(color[0]);
    put_c(color[1]);
    put_c(color[2]);
}

void uart_delete() {
    put_c(0x00);
    put_c(0x01);
    put_c(0x02);
    put_c(color[0]);
    put_c(color[1]);
    put_c(color[2]);   
     
     
     
     uart_sendStopp();
     CloseHandle(serialPortHandle);
}


