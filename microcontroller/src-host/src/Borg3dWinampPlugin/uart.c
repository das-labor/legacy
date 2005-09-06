#include <windows.h>
#include "config.h"
#include "pixel3d.h"



HANDLE serialPortHandle;
DCB serialConfig;

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

void uart_delete() {
     uart_sendStopp();
     CloseHandle(serialPortHandle);
}




void uartSendPixmap() {
	char i, j, k;
	put_c(UART_ESCAPE);
	put_c(UART_SOF);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < NUM_PLANES; j++) {
			for (k = 0; k  < PLANEBYTES; k++) {
				if (pixmap[i][j][k] == UART_ESCAPE)
					put_c(UART_ESCAPE);
				put_c(pixmap[i][j][k]);
			}
		}
	}
}
