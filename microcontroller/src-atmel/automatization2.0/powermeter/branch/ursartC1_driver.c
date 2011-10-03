#include <avr/io.h>
#include "ursartC1_driver.h"

void sendUSARTC1_putstr(char *str) {
	while(*str) {
	sendUSARTC1_char(*str++);
	}
}

void sendUSARTC1_putstr_P(PGM_P str) {
	char tmp;
	while((tmp = pgm_read_byte(str))) {
		sendUSARTC1_char(tmp);
		str++;
	}
}

void sendUSARTC1_char(char c){
		while (! (USARTC1.STATUS & USART_DREIF_bm)) {
      	// wait
  		}
		USARTC1.DATA = c;
}

void InitializeUSARTC1() {
   // Baud Rate variables
   // (3317, -4) = 9600 (at 32Mhz fper)
   // (3301, -5) = 19200
   // (2094, -7) = 115200
   int bsel = 3301;
   int bscale = -5;

   // Set Tx pin to an output
   PORTC.DIRSET = (1<<PIN7);

   // Set Baud Rate
   USARTC1.BAUDCTRLB = ((unsigned char)(bscale & 0x0F) << 4) | ((unsigned char)(bsel >> 8) & 0x0F);
   USARTC1.BAUDCTRLA = (unsigned char)(bsel & 0xFF);

   // Set Control Reg C
   // CMODE = 00 (Async USART)
   // PMODE = 00 (No Parity) (10 = Even, 11 = Odd)
   // SBMODE = 0 (1-Stop) (1 = 2-Stop Bits)
   // CHSIZE = 011 (8bit)
   USARTC1.CTRLC = (USARTC1.CTRLC & ~USART_CHSIZE_gm) | USART_CHSIZE_8BIT_gc;

   // Set Control Reg B
   // Enable Rx and Tx
   USARTC1.CTRLB = USART_TXEN_bm; // USART_RXEN_bm
} 
