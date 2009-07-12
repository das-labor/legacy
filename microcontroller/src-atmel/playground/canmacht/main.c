
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
//#include "xcan.h"
//#include "xlap.h"
#include "stuff.h"
#include "menu.h"
#include "grafiklcd.h"
#include "../../lib/rfmxx-avrx/rf12.h"



AVRX_SIGINT(SIG_OVERFLOW0)
{
	IntProlog();			// Save interrupted context, switch stacks
	TCNT0 = TCNT0_INIT;		// Reload the timer counter
	AvrXTimerHandler();		// Process Timer queue
	Epilog();			// Restore context of next running task
}

AVRX_EXTERNTASK(rxtask);
AVRX_EXTERNTASK(txtask);

AVRX_GCC_TASKDEF (txtask, 100, 4)
{
	char txbuf[32] = "hallo, nakkatest.", inputchar;
/*
	while (10)
	{
		if (uart_getc_nb(&inputchar))
                {
                        switch (inputchar)
                        {
                                case 's':
//                                      rxbuf = rf12_trans(0x0000);
//                                      printf("status register: %4x\r\n", rxbuf);
                                        dump_registers();
                                break;
                                case 't':
                                        uart_putc(inputchar);
					rf12_txdata(txbuf, strlen(txbuf));
                                break;
                        }
                }
		AvrXDelay (&mytimer2, 100); 
	} */
}

AVRX_GCC_TASKDEF (rxtask, 100, 4)
{
	uint8_t moo = 0, i;
	char tmp[16];
	
	rf12_init();
	rf12_setfreq(RF12FREQ(434.0));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);	// 200kHz Bandbreite, -6dB Verstärkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);		// 19200 baud
	rf12_setpower(0, 6);		// 1mW Ausgangangsleistung, 120kHz Frequenzshift

/*
	while (92)
	{
		moo = rf12_rxarray (&tmp, 10);
		if (moo > 0)
		{
			for (i=0;i<moo;i++)
				printf("%c", tmp[i]);
			moo = 0;
		}
		AvrXDelay (&mytimer, 1); 
	} */
}

int main(void)
{
	AvrXSetKernelStack(0);
	MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
	TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
	TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
	TIMSK = 1<<TOIE0;		// Enable interrupt flag


	PORTB = 0x0f; // Tastenpin pullups
	DDRB  = 0x00; // 
	//InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	//xlap_init();
	dispInit(); // Display initialisierung

//	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(rxtask));
	AvrXRunTask(TCB(txtask));
	AvrXRunTask(TCB(stuff));
	AvrXRunTask(TCB(menu));

	/* Needed for EEPROM access in monitor */
	//AvrXSetSemaphore(&EEPromMutex);


	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
}
