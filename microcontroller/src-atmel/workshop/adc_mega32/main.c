//Simples Beispiel zur Nutzung des ADCs im Mega32

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "util.h"

int main(){

	uart_init();
	sei();
	uart_putstr("test123\r\n");
	
/* ADMUX Register:
 * REFS1 REFS0 ADLAR MUX4-MUX0
 * 
 * 	REFS:
 *	0 0		Aref Pin
 *  0 1		AVcc
 *  1 0		Reserved
 *  1 1		Internal 2.56V Reference
 * 
 * MUX = 0-7: Single ended Input on ADC0-ADC7
*/
				//11000000
	ADMUX = 0xc0; //2.56V Reference, Channel 0

/* ADCSRA Register:
 * ADEN ADSC ADATE ADIF ADIE ADPS2-ADPS0
 */
	ADCSRA = (1<<ADEN) | 7;//ADC aktivieren, Takt/128

	while(1){
		//Puffer für Strings
		char buf[10];
		uint16_t result;

		ADCSRA |= (1<<ADIF);//Interrupt Flag zurücksetzen
		ADCSRA |= (1<<ADSC);//Einen Konvertierungsvorgang starten
		
		//warten bis Konvertierung fertig ist
		while( (ADCSRA & (1<<ADIF)) == 0  );
		
		//Wert aus ADCH und ADCL auslesen
		result = ADC;

		itoa(result, buf, 10); //Wert in dezimalen String wandeln

		uart_putstr(buf);//Auf dem UART ausgeben
		uart_putstr("\n\r");//Zeilenvorschub

		wait(100);//etwas warten, damit nicht zu viele Zeichen auf einmal kommen
	}
}
