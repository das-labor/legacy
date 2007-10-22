#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <avr/eeprom.h>
#include <stdlib.h>
#include "rf12.h"     
#include "config.h"
#include "uart.h"

#include "avrx.h"
#include <util/delay.h>

void send(uint8_t i);
TimerControlBlock mytimer;
TimerControlBlock mytimer2;

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
}


void dump_registers (void)
{
	uint8_t i, k;
	uint16_t rxbuf;

	printf("\r\n");
	for (i=0;i<0xff;i++)
	{
		rxbuf = rf12_trans((i));
		if (rxbuf != 0x809c)
		{
			printf("%2x: %4x     ", i, rxbuf);
			if (k == 7)
			{
				printf("\r\n");
			}
			k = (k == 7) ? 0 : k+1;
		}
	}
	rxbuf = rf12_trans(0x0000 | i);
	printf("%i = %x\t", i, rxbuf);
	printf("\r\n");
	AvrXDelay(&mytimer2, 50);
}

AVRX_EXTERNTASK(rxtask);
AVRX_EXTERNTASK(txtask);

AVRX_GCC_TASKDEF (txtask, 100, 4)
{
	char txbuf[32] = "hallo, nakkatest.", inputchar;

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
	}
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
	}
}

int main(void)
{
	unsigned char i, tx=0x01, k=0;
	uint16_t rxbuf = 0x0000;
	char inputchar;

	AvrXSetKernelStack(0);

	MCUCR = 1<<SE;                   // Enable "sleep" mode (low power when idle)
	TCNT0 = TCNT0_INIT;              // Load overflow counter of timer0
	TCCR0 = TMC8_CK256; // Set Timer0 to CPUCLK/256
	TIMSK = 1<<TOIE0;                // Enable interrupt flag



	AvrXSetSemaphore(&EEPromMutex);
	inputchar = malloc(sizeof(char));

	DDRC = 0xff;
	DDRD &= ~(_BV(PD2));
	DDRB &= ~(_BV(PB0));
	PORTB |= _BV(PB0);

	uart_init();
	fdevopen(uart_putc, 0);
	uart_putstr("\r\n");
	uart_putstr("Hallo\r\n");
//	_delay_ms(19);

	PORTC = 0xff;

	AvrXRunTask(TCB(rxtask));
	AvrXRunTask(TCB(txtask));
	Epilog();

	for (;;)
	{
	}
}

void send(uint8_t i)
{
	uint8_t test[8] = { 'n', 'a', 'k', 'k', 'a', '\r', '\n' };
//	PORTC = i;
//	rf12_txdata(i,1);
//	rf12_txdata (i);
}

