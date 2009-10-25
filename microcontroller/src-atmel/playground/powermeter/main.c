/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>


#include "config.h"

int main(void)
{
	ADMUX |= _BV(REFS1) | _BV(REFS0); // Internal 2.56V Voltage Reference with external capacitor at AREF pin
	ADCSRA |= _BV(ADEN)| _BV(ADPS2) | _BV(ADPS1);	// enable adc, ADIE: enable int , ADSC: Start Conversion , clock / 64 (max clk) 250khz

	eia setup
http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial#Der_interne_ADC_im_AVR

	while (1)
	{
			// L1 Strom
			ADMUX &= ~(_BV(MUX2) | _BV(MUX1) |_BV(MUX0));
  while ( ADCSRA & (1<<ADSC) ) {
			send eia232
		// L1 Spannung
			ADMUX |= _BV(MUX0);
		// L2 Strom
			ADMUX &=  ~_BV(MUX0);
			ADMUX |= _BV(MUX1);
		// L1 Spannung
			ADMUX |= _BV(MUX1) | _BV(MUX0);
		// L3 Strom
			ADMUX &=  ~(_BV(MUX1) | _BV(MUX0));
			ADMUX |= _BV(MUX2);
		// L3 Spannung
			ADMUX |= _BV(MUX0);
	}
}


uint16_t ReadChannel(uint8_t mux)
{
  uint8_t i;
  uint16_t result;
 
  ADMUX = mux;                      // Kanal waehlen
  ADMUX |= (1<<REFS1) | (1<<REFS0); // interne Referenzspannung nutzen
 
  ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    // Frequenzvorteiler 
                               // setzen auf 8 (1) und ADC aktivieren (1)
 
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
  ADCSRA |= (1<<ADSC);              // eine ADC-Wandlung 
  while ( ADCSRA & (1<<ADSC) ) {
     ;     // auf Abschluss der Konvertierung warten 
  }
  result = ADCW;  // ADCW muss einmal gelesen werden,
                  // sonst wird Ergebnis der nächsten Wandlung
                  // nicht übernommen.
 
  /* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */
  result = 0; 
  for( i=0; i<4; i++ )
  {
    ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
    while ( ADCSRA & (1<<ADSC) ) {
      ;   // auf Abschluss der Konvertierung warten
    }
    result += ADCW;		    // Wandlungsergebnisse aufaddieren
  }
  ADCSRA &= ~(1<<ADEN);             // ADC deaktivieren (2)
 
  result /= 4;                     // Summe durch vier teilen = arithm. Mittelwert
 
  return result;
}

