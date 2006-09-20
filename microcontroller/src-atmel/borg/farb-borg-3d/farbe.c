#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdlib.h>


#include "config.h"
#include "uart.h"
#include "util.h"

#define MAX_X 3
#define MAX_Y 3
#define MAX_Z 5
#define COLORS 3

#define PORT_DATA PORTC	
#define PIN_DATA PINC
#define DDR_DATA DDRC
#define PORT_ADRESS_L PORTA
#define DDR_ADRESS_L  DDRA
#define PORT_ADRESS_H PORTB
#define DDR_ADRESS_H  DDRB

#define PORT_CONTROL PORTD
#define DDR_CONTROL DDRD
#define PORT_CONTROL_N_WE PORTD2
#define PORT_CONTROL_N_OE PORTD3
#define PORT_CONTROL_N_CS PORTD4

#define MAX_BRITH 64



//#define DEBUG_PROCLATCH

unsigned char pixmap[MAX_Z][MAX_Y][MAX_X][COLORS];

volatile unsigned char values[9];
volatile unsigned char masks[9];

extern volatile void writeRam();

/*
typedef struct {
	unsigned char value;
	unsigned char bitmask;
} BuildData;

BuildData buildLatch[9];


void sort() {
	unsigned char i, j, swap = 1;
	BuildData temp, *pbuildLatch0, *pbuildLatch1;
	for (i = 6; swap && i < 7; --i) { // unsigned ist schneller daher < 7 anstatt >= 0
		pbuildLatch0  = buildLatch;
		pbuildLatch1  = buildLatch + 1;
		swap = 0;
		for (j = 0; j <= i; ++j, ++pbuildLatch0, ++pbuildLatch1)
			if ((*pbuildLatch0).value > (*pbuildLatch1).value) {
				temp            = *pbuildLatch0; 
				*pbuildLatch0   = *pbuildLatch1; 
				*pbuildLatch1   = temp;
				swap = 1;
			}
	}
}
*/
/* needs 26 ticks more than bubblesort
void gnomesort() {
    unsigned char i = 0;
	BuildData temp;
    while (i < 8) {
        if (i == 0  ||  buildLatch[i - 1].value <= buildLatch[i].value) {
            i++;
        }
        else {
            temp              = buildLatch[i];
            buildLatch[i]     = buildLatch[i - 1];
            buildLatch[i - 1]  = temp;
			i--;
		}
    }
}
*/

/*
void procLatch() {
	unsigned char b, i = 0, j = 0, value, mask;
	sort();
	mask = 0xff;
	PORT_DATA = mask;
	
#ifdef DEBUG_PROCLATCH
	char help[6];
	for (b = 0; b < 9; b++) {
		uart_putstr("Value : ");
		itoa(values[b], help, 10);
		uart_putstr(help);
			uart_putstr(" \tBitmask : ");
		itoa(masks[b], help, 16);
		uart_putstr(help);
		uart_putstr("\r\n");
	}
#endif // DEBUG_PROCLATCH		
	
	//PORT_CONTROL &= ~(1 << PORT_CONTROL_N_WE);
	value = values[0];
	for (b = 0; b < MAX_BRITH; ++b) {
		while (value <= b) {
			mask  &= masks[i];
			PORT_DATA = mask;	
			value  = values[++i];
		}
		//PORT_CONTROL |=  (1 << PORT_CONTROL_N_WE);
		
#ifdef DEBUG_PROCLATCH
		itoa(PORT_DATA, help, 16);
		uart_putstr(help);
		if ((j & 1) == 1)
			uart_putc(' ');
		if (++j == 32) {
			j = 0;
			uart_putstr("\r\n"); 
		}
#endif // DEBUG_PROCLATCH 
	
		PORT_ADRESS++;
		
		//PORT_CONTROL &= ~(1 << PORT_CONTROL_N_WE);
		//wait(1);
	}
}
*/



void writeFrame() {
	unsigned char z, y, i, help;
	unsigned char * pix = (unsigned char *) pixmap;
	values[8] = 255;
	for (z = 0; z < MAX_Z; ++z) {
		i = 0;
		for (y = 0; y < MAX_Y * MAX_X * COLORS; ++y) {
			help      = *pix++;
			values[i] = help;
			if (++i == 8)  {
				writeRam();
				i = 0;
			}
		}
		writeRam();
	}
}

#define TEST_DATA_SIZE 250

void procLatch_Test() {
	/*
	buildLatch[0].bitmask = ~(1 << 0); 
	buildLatch[0].value   = 2;
	buildLatch[1].bitmask = ~(1 << 1); 
	buildLatch[1].value   = 1;
	buildLatch[2].bitmask = ~(1 << 2); 
	buildLatch[2].value   = 0;
	buildLatch[3].bitmask = ~(1 << 3); 
	buildLatch[3].value   = 3;
	buildLatch[4].bitmask = ~(1 << 4); 
	buildLatch[4].value   = 4;
	buildLatch[5].bitmask = ~(1 << 5); 
	buildLatch[5].value   = 63;
	buildLatch[6].bitmask = ~(1 << 6); 
	buildLatch[6].value   = 0;
	buildLatch[7].bitmask = ~(1 << 7); 
	buildLatch[7].value   = 12;
	// end marker  
	buildLatch[8].value = 255;
	*/
	values[0] = 2;
	values[1] = 1;
	values[2] = 0;
	values[3] = 3;
	values[4] = 4;
	values[5] = 63;
	values[6] = 0;
	values[7] = 12;
	values[8] = 255;
	
	// in the memory should be written
	// bbb9 b8b0 a0a0 a0a0 a0a0 a0a0 2020 2020 
	// 2020 2020 2020 2020 2020 2020 2020 2020
	// 2020 2020 2020 2020 2020 2020 2020 2020 
	// 2020 2020 2020 2020 2020 2020 2020 2000

}


void writeRamTest() {
	unsigned char * pix = (unsigned char *) pixmap, i;
	for (i = 0; i < 100; i++)
			pix[i] = i;
}


int main() {
	//unsigned char test[TEST_DATA_SIZE], i;
	unsigned char i, j = 0, help[8];
	unsigned int time0, time1;
	uart_init();

	uart_putstr("INIT\r\n");
	PORT_DATA     = 0;
	DDR_DATA      = 0xff;  // data output
	PORT_ADRESS_L = 0;
	DDR_ADRESS_L  = 0xff; 

	PORT_ADRESS_H = 0;
	DDR_ADRESS_H  = 0xff; 
	
	// Timer 2 zur Preformance Messung initialisieren
	TCCR1B = 1;	// kein precale also 16 Mhz

	DDR_CONTROL  |= (1 << PORT_CONTROL_N_OE);
	DDR_CONTROL  |= (1 << PORT_CONTROL_N_WE);
	DDR_CONTROL  |= (1 << PORT_CONTROL_N_CS);
	
	PORT_CONTROL |= (1 << PORT_CONTROL_N_OE);
	PORT_CONTROL |= (1 << PORT_CONTROL_N_CS);
	/*
	uart_putstr("begin writeRam()\r\n");
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_CS);
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_WE);
	
	procLatch_Test();	// Testdaten intitialisieren
	time0 = TCNT1;
	writeRam();
	time1 = TCNT1;

	uart_putstr("end writeRam()  ticks : ");
	itoa(time1 -= time0, help, 10);
	uart_putstr(help);
	uart_putstr(" = ");
	itoa(time1 >> 4, help, 10);
	uart_putstr(help);
	uart_putstr("E-06 s\r\n");
	*/
	uart_putstr("begin writeFrame()\r\n");
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_CS);
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_WE);
	
	writeRamTest();	// Testdaten intitialisieren
	time0 = TCNT1;
	writeFrame();
	time1 = TCNT1;

	uart_putstr("end writeRam()  ticks : ");
	itoa(time1 -= time0, help, 10);
	uart_putstr(help);
	uart_putstr(" = ");
	itoa(time1 >> 4, help, 10);
	uart_putstr(help);
	uart_putstr("E-06 s\r\n");


	DDR_DATA      = 0; // Dataport auf Eingang stellen
	PORT_CONTROL |=  (1 << PORT_CONTROL_N_WE);
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_CS);
	PORT_CONTROL &= ~(1 << PORT_CONTROL_N_OE);
	unsigned int adr = 0;
	for (i = 0; i < TEST_DATA_SIZE; i++) {
		itoa(PIN_DATA, help, 16);
		PORT_CONTROL |=  (1 << PORT_CONTROL_N_OE);
		adr++;
		PORT_ADRESS_H = adr >> 8;
		PORT_ADRESS_L = adr;
		
		//PORT_CONTROL |=  (1 << PORT_CONTROL_N_CS);
		//PORT_CONTROL &= ~(1 << PORT_CONTROL_N_CS);
		PORT_CONTROL &= ~(1 << PORT_CONTROL_N_OE);
		uart_putstr(help);
		if ((j & 1) == 1)
			uart_putc(' ');
		if (++j == 32) {
			j = 0;
			uart_putstr("\r\n"); 
		}
		wait(1);
	}
	
	//uart_hexdump(test, TEST_DATA_SIZE);
	while (1);
	return 0;
}
