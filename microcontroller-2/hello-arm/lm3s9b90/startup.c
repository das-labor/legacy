/* startup.c */
/*
    This file is part of the OpenARMWare.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RAM_START 0x20000000
#define RAM_SIZE (96*1024)

int main(void);
void uart0_isr(void);

/* the following are defined by the linker */
extern char _text;
extern char _text_end;
extern char _data;
extern char _data_end;
extern char _bss;
extern char _bss_end;

typedef void(*isr_fpt)(void);

static void fault_isr(void){
	for(;;){
	}
}

static void default_isr(void){
	for(;;){
	}
}


static void nmi_isr(void){
	for(;;){
	}
}

void reset_isr(void){
	memcpy(&_data, &_text_end, &_data_end - &_data);
	memset(&_bss, 0, &_bss_end - &_bss);
	main();
}

isr_fpt isr_vector[] __attribute__ ((section(".isr_vectors"))) = {
		(isr_fpt)(RAM_START+RAM_SIZE-4),
		reset_isr,   /* Reset */
		nmi_isr,     /* Non-Maskable Interrupt (NMI) */
		fault_isr,   /* Hard Fault */
		default_isr, /* Memory Management */
		fault_isr, /* Bus Fault */
		fault_isr, /* Usage Fault */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		default_isr, /* SVCall */
		default_isr, /* Debug Monitor */
		NULL,         /* Reserved */
		default_isr, /* PendSV */
		default_isr, /* SysTick */
		default_isr, /* GPIO Port A */
		default_isr, /* GPIO Port B */
		default_isr, /* GPIO Port C */
		default_isr, /* GPIO Port D */
		default_isr, /* GPIO Port E */
		uart0_isr, /* UART0 */
		default_isr, /* UART1 */
		default_isr, /* SSI0 */
		default_isr, /* I2C0 */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		NULL,         /* Reserved */
		default_isr, /* ADC0 Sequence 0 */
		default_isr, /* ADC0 Sequence 1 */
		default_isr, /* ADC0 Sequence 2 */
		default_isr, /* ADC0 Sequence 3 */
		default_isr, /* Watchdog Timers 0 and 1 */
		default_isr, /* Timer 0A */
		default_isr, /* Timer 0B */
		default_isr, /* Timer 1A */
		default_isr, /* Timer 1B */
		default_isr, /* Timer 2A */
		default_isr, /* Timer 2B */
		default_isr, /* Analog Comparator 0 */
		default_isr, /* Analog Comparator 1 */
		default_isr, /* Analog Comparator 2 */
		default_isr, /* System Control */
		default_isr, /* Flash Memory Control */
		default_isr, /* GPIO Port F */
		default_isr, /* GPIO Port G */
		default_isr, /* GPIO Port H */
		default_isr, /* UART2 */
		default_isr, /* SSI1 */
		default_isr, /* Timer 3A */
		default_isr, /* Timer 3B */
		default_isr, /* I2C1 */
		NULL,         /* Reserved */
		default_isr, /* CAN0 */
		default_isr, /* CAN1 */
		NULL,         /* Reserved */
		default_isr, /* Ethernet Controller */
		default_isr, /* Hibernation Module */
		default_isr, /* USB */
		NULL,         /* Reserved */
		default_isr, /* µDMA Software */
		default_isr, /* µDMA Error */
		default_isr, /* ADC1 Sequence 0 */
		default_isr, /* ADC1 Sequence 1 */
		default_isr, /* ADC1 Sequence 2 */
		default_isr, /* ADC1 Sequence 3 */
		default_isr, /* I2S0 */
		default_isr, /* EPI */
		default_isr, /* GPIO Port J */
		NULL,         /* Reserved */
};



