/*
 * PowerCommander - Ausführungs Kontroller
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "twi_slave/general.h"
#include "twi_slave/twi_slave.h"


#define ADRESSE 15 // I2C Adresse


int main (void)
{
    // Disable Analog Comparator (power save)
    ACSR = _BV(ACD);
	
	// Disable Analog to Digital converter (power save)
	ADCSRA = 0;

	DDRC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);      // relais ausgänge 1-4
    PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0));  // aus damit

    DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB0);                 // relais ausgänge 5-7  
    PORTB &= ~(_BV(PB5) | _BV(PB4) | _BV(PB0));             // aus damit

    DDRD |= _BV(PD7) | _BV(PD1) | _BV(PD0);                 // relais ausgang 8, 230V 1-2
    PORTD &= ~(_BV(PD7) | _BV(PD1) | _BV(PD0));             // aus


    DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);                 // pwm ausgänge | müssen die wirklich gesetzt werden?
    DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);                 // pwm ausgänge
    
    TCCR2A |= _BV(WGM20) | _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0); // FastPWM, Set OC0A on Compare Match, clear OCxx at BOTTOM, (inverting mode).
    TCCR2B |= _BV(WGM21) | _BV(CS22) | _BV(CS21) | _BV(CS20);                     // FastPWM bit 2, clk/1024
    
   	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0); // FastPWM, Set OC0A on Compare Match, clear OCxx at BOTTOM, (inverting mode).
  	TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10);                                 // FastPWM bit 2, clk/1024
  	
  	TCCR0A |= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0); // FastPWM, Set OC0A on Compare Match, clear OCxx at BOTTOM, (inverting mode).
    TCCR0B |= _BV(CS02) | _BV(CS00);                                 // FastPWM bit 2, clk/1024
  	
    TCNT2 = 0;     // pwm timer clear
    OCR2A = 0;   // pwm timer compare target
	OCR2B = 0;   // pwm timer compare target
  	
  	TCNT1 = 0;     // pwm timer clear
	OCR1A = 0;   // pwm timer compare target
    OCR1B = 0;   // pwm timer compare target
    
    TCNT0 = 0;     // pwm timer clear
    OCR0A = 0;   // pwm timer compare target
	OCR0B = 0;   // pwm timer compare target

// PCICR |= _BV(PCIE1);
// PCMSK2 |= _BV(PCINT18) | _BV(PCINT20);

/* TIMER0 OVF  TODO muss interrupt aktiviert werden?
{
    tick++; ?
    // wann ist es dimmen / tasten, zähler löschen
    if (i1/2 == )
    
    PORT |= 
    
    if (i2 / 2 == )
    PORT |= 
}
*/
 
/* PCINT2
{
    if (pcint18)
        i1++;
    if (pcint20)
        i2++;
}
*/


    uint8_t     tmp = 0;
    uint8_t     stat_haupt = 0; // status des Hauptschalters 0=ist aus; 1=ist an;<1 ist gesperrt
	uint8_t		byte[2];
	uint8_t		TWIS_ResonseType;

/*
** Clear any interrupt
*/
	cli();  //TODO  ist das noch aktuell?
	
	
/*
** Wait 1 second for POR
*/
	_delay_ms(500);


/*
** Start TWI Slave with address 15 and bitrate of 100000 Hz
*/
	TWIS_Init(ADRESSE, 100000);

	while (1)
	{
/*
** Check wether something is to do for the TWI slave interface
*/
		if (TWIS_ResonseRequired(&TWIS_ResonseType))
		{
			switch (TWIS_ResonseType)
			{
/*
** Slave is requests to read bytes from the master.
*/
                case TWIS_ReadBytes:
					byte[0] = TWIS_ReadAck();   // byte 1 lesen
    				byte[1] = TWIS_ReadNack();  // byte 2 lesen
    				TWIS_Stop();                // I2C stop
    				
					switch (byte[0])            // ziel suchen
					{
    					case 0:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTC |= _BV(PC3);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTC >> PC3) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTC &= ~_BV(PC3);
    					    }
    					    break;
                        case 1:
    					
    					    if (byte[1] == 0x01)
    					    {
    					        PORTC |= _BV(PC2);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTC >> PC2) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTC &= ~_BV(PC2);
    					    }
    					    break;
    					case 2:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTC |= _BV(PC1);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTC >> PC1) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTC &= ~_BV(PC1);
    					    }
    					    break;
    					case 3:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTC |= _BV(PC0);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTC >> PC0) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTC &= ~_BV(PC0);
    					    }
    					    break;
                        case 4:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTB |= _BV(PB5);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTB >> PB5) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTB &= ~_BV(PB5);
    					    }
    					    break;
    				    case 5:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTB |= _BV(PB4);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTB >> PB4) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTB &= ~_BV(PB4);
    					    }
    					    break;
    					case 6:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTB |= _BV(PB0);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTB >> PB0) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTB &= ~_BV(PB0);
    					    }
    					    break;
    					case 7:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTD |= _BV(PD7);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTD >> PD7) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTD &= ~_BV(PD7);
    					    }
    					    break;
    					case 8:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTD |= _BV(PD1);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTD >> PD1) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTD &= ~_BV(PD1);
    					    }
    					    break;
    					case 9:
    					    if (byte[1] == 0x01)
    					    {
    					        PORTD |= _BV(PD0);
    					    }
    					    else if (byte[1] == 0x02)
    					    {
    					        tmp = (PORTD >> PD0) & 1;
    					    }
    					    else if (byte[1] == 0x00)
    					    {
    					        PORTD &= ~_BV(PD0);
    					    }
                            break;
                        case 10:
    					    OCR0A = byte[1];
    					    break;
    					case 11:
    					    OCR0B = byte[1];
    					    break;
    					case 12:
    					    OCR1A = byte[1];
    					    break;
    					case 13:
    					    OCR1B = byte[1];
    					    break;
    					case 14:
    					    OCR2A = byte[1];
    					    break;
                        case 15:
    					    OCR2B = byte[1];
    					    break;
                        default:
                            break;
                    }
					break;
/*
** Slave is requested to send bytes to the master.
*/

    			case TWIS_WriteBytes:
    			    TWIS_Write(tmp);        // byte das in Leseoperation befüllt wurde schreiben
    			    TWIS_Stop();
    				break;
			}
		}
	    if ((!(PINB & _BV(PB7))) && stat_haupt == 0)   // Hauptschalter geht aus
	    {
            PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0)); //TODO Status merken?
            PORTB &= ~(_BV(PB5) | _BV(PB4) | _BV(PB0));
            PORTD &= ~(_BV(PD7) | _BV(PD1) | _BV(PD0));

/*		    OCR2A = 0;  
            OCR2B = 0;
            OCR1A = 0;
            OCR1B = 0;
            OCR0A = 0;
            OCR0B = 0; */
            stat_haupt = 1;
        }
        if ((PINB & _BV(PB7)) && stat_haupt == 1)   // Hauptschalter geht an
        {
            // PORTC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);
            // PORTB |= _BV(PC5) | _BV(PC4) | _BV(PB0);
            PORTD |= _BV(PD1) | _BV(PD0);
/*		    OCR2A = 0;
            OCR2B = 0;
            OCR1A = 0;
            OCR1B = 0;
            OCR0A = 0;
            OCR0B = 0; */
            stat_haupt = 0;
        }
//        if (nachtmodus) über can?!
        
    }
    return 0;
}

