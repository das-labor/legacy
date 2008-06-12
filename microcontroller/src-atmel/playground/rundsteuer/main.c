#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define UART_BAUD_RATE 38400
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_CPU)/((UART_BAUD_RATE)*16L)-1)

unsigned char buf[1502];
volatile unsigned int count;

#define PORT_LA PORTC
#define DDR_LA DDRC
#define PIN_LA PINC

void uart_init() {
	PORTD |= 0x01;				//Pullup an RXD an

	UCSRB |= (1<<TXEN);			//UART TX einschalten
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);		//Asynchron 8N1

	UCSRB |= ( 1 << RXEN );			//Uart RX einschalten

	UBRRH=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
	UBRRL=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU));
	
	// activate rx IRQ
	UCSRB |= (1 << RXCIE);
}

void uart_putc(char c) {
	while (!(UCSRA & (1<<UDRE))); /* warten bis Senden moeglich                   */
	UDR = c;                      /* schreibt das Zeichen x auf die Schnittstelle */
}

void printbin(unsigned char b){
	unsigned char msk = 0x80;
	while(msk){
		uart_putc((msk & b)?'1':'0');
		msk>>=1;
	}
}

SIGNAL(SIG_UART_RECV) {
	unsigned char * ptr = buf;
	printf("\t");
	printbin(*ptr);
	ptr++;
	printf("\n\r");
	
	while(count--){
		printf("%d:\t", *(unsigned int *)ptr);
		ptr+=2;
		printbin(*ptr);
		ptr++;
		printf("\n\r");
	}
	
	while(1);
}

void init(){
	unsigned int x;
	DDR_LA = 0x0;
	PORT_LA = 0xFF;
	
	for(x=0;x<1000;x++){
		buf[x]=0;
	}
	count = 0;

	/* 	TCCR1B: ICNC1 ICES1   -   WGM13 WGM12 CS12 CS11 CS10
		CS12 CS11 CS10
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
		 1    1    0       T1 falling
		 1    1    1       T1 rising
	*/
	
	TCCR1B = 5; //clk/1024	
}


int main(){
	unsigned char tmp1=0, tmp2=0;
	unsigned char * p = buf;
	
	unsigned int ms=0;

	init();
	uart_init();
	fdevopen(&uart_putc,0);
	printf("ready.\n\r");
	sei();
	
	while(1){
		if(TCNT1 >= 16){
			TCNT1 = 0;
			ms++;
		}

		tmp1 = PINC & 0x01;
		if((tmp1 != tmp2) && (ms > 30) ){
			tmp2 = tmp1;
			printf("%d: %d\r\n", !tmp1, ms);
			ms = 0;
		}
	
	}
}

