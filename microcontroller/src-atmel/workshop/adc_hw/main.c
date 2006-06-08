//Beispiel für die Nutzung eines Digitalen IOs als ADC
//
//Schaltung:
//         220 Ohm         100k
//  PC0 ---RRRRR----+------RRRRR----O  Input
//                  |
//                 --- 100n
//                 ---
//                  |
//                  |
//                 --- 
//



#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "util.h"

int main(){

	uart_init();
	sei();
	uart_putstr("test123\r\n");
		
	PORTC = 0;

	while(1){
		//Puffer für Strings
		char buf[10];
		uint16_t counter;
	
		//Kondensator entladen
		DDRC = 1;
		wait(10);
		DDRC = 0;

		//Zähler zurücksetzen
		counter = 0;

		//Warten bis der Kondensator bis zur Schwelle des Eingangs
		//aufgeladen ist, dabie den Zähler erhöhen
		while( (PINC & 1) == 0){
			counter++;
			if(counter == 65535) break;
		}

		utoa(counter, buf, 10); //Wert in dezimalen String wandeln

		uart_putstr(buf);//Auf dem UART ausgeben
		uart_putstr("\n\r");//Zeilenvorschub

		wait(100);//etwas warten, damit nicht zu viele Zeichen auf einmal kommen
	}
}
