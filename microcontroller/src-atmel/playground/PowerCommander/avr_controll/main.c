/*
 * PowerCommander - Ausführungs Kontroller
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "twi_slave/twi_slave.h"


#define ADRESSE 15 // I2C Adresse

// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

// was willst du von dem Relai / Optokoppler
enum action { 
  swon = 0,   // einschalten
  swoff,    // ausschalten
  swstatus, // status
  num_action // ist immer der letzte eintrag
};

// was soll mit der lampe passieren?
enum brightaction {
  brset = 0,   // helligkeit setzen
  num_brightaction // ist immer der letzte eintrag
};

// welches relai willst du ansteuern?

enum relais {
  switch00 = 0,
  switch01,
  switch02,
  switch03,
  switch04,
  switch05,
  switch06,
  switch07,
  num_switch // muss immer der letzte sein
};

// welchen optokoppler willst du?
enum optokop {
  optokopp00 = num_switch,
  optokopp01,
  num_optokopp // muss immer der letzte sein
};


// welche lampe willst du veraendern?
enum bright {
  tafel=num_optokopp,
  beamer,
  schraenke,
  flipper,
  free1,
  lounge,
  num_bright // ist immer der letzte eintrag
};

// struktur die daten enthaelt und das FLAG wenn daten
// auf den I2C geschrieben werden sollen
struct t_status {
  uint8_t data;
  uint8_t write_data;
};

/*
  fuer jedes objekt gibt es funktionen der form:
  unit8_t NAME_ACTION(struct t_status * data)
  
  der name ist erstmal nicht wichtig, auch die Action nicht wirklich
  sie soll hier nur das lesen vereinfachen

  wenn du also ein neues objekt hinzufuegen willst, dann muss 
  erstmal nur die Funktion diesem schema entsprechen.

  der Rueckgabe wert kann, muss aber nicht ueberprueft werden.
  dies ist im Moment nicht der Fall.

  in jeder funktion muss entschieden werden, ob sie Informationen 
  zurueck gibt. wenn

  (*data).write_data = 0;
  
  verwendet wird, werden keine daten zurueck gegeben. wird

  (*data).write_data = 1;
  
  verwendet, wird das auf den Bus gegeben, was sich in 

  (*data).data; 

  befindet.
  
  Daten auf denen gearbeitet werden soll, stehen beim start der Funktion 
  in (*data).data;

  auf diese Art lassen sich auch recht einfach neue Aktionen erstellen
  die ihre eigene Funktion haben. und auch ein Byte daten aus dem I2C 
  annehmen

*/

uint8_t switch00_off(struct t_status * data)
{
  PORTC &= ~_BV(PC3);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch00_on(struct t_status * data)
{
  PORTC |= _BV(PC3);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch00_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC3) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t switch01_off(struct t_status * data)
{
  PORTC &= ~_BV(PC2);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch01_on(struct t_status * data)
{
  PORTC |= _BV(PC2);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch01_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC2) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch02_off(struct t_status * data)
{
  PORTC &= ~_BV(PC1);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch02_on(struct t_status * data)
{
  PORTC |= _BV(PC1);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch02_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC1) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch03_off(struct t_status * data)
{
  PORTC &= ~_BV(PC0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch03_on(struct t_status * data)
{
  PORTC |= _BV(PC0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch03_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC0) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch04_off(struct t_status * data)
{
  PORTB &= ~_BV(PB5);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch04_on(struct t_status * data)
{
  PORTB |= _BV(PB5);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch04_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB5) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch05_off(struct t_status * data)
{
  PORTB &= ~_BV(PB4);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch05_on(struct t_status * data)
{
  PORTB |= _BV(PB4);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch05_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB4) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t switch06_off(struct t_status * data)
{
  PORTB &= ~_BV(PB0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch06_on(struct t_status * data)
{
  PORTB |= _BV(PB0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch06_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB0) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch07_off(struct t_status * data)
{
  PORTD &= ~_BV(PD7);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch07_on(struct t_status * data)
{
  PORTD |= _BV(PD7);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch07_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD7) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t opto00_off(struct t_status * data)
{
  PORTD &= ~_BV(PD0);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto00_on(struct t_status * data)
{
  PORTD |= _BV(PD0);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto00_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD0) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t opto01_off(struct t_status * data)
{
  PORTD &= ~_BV(PD1);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto01_on(struct t_status * data)
{
  PORTD |= _BV(PD1);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto01_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD1) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t bright_tafel_set(struct t_status *data)
{
  OCR0A = (*data).data;
  (*data).write_data=0;
  return 0;
}

uint8_t bright_beamer_set(struct t_status *data)
{
  OCR0B = (*data).data;
  (*data).write_data=0;
  return 0;
}

uint8_t bright_schraenke_set(struct t_status *data)
{
  OCR1A = (*data).data;
  (*data).write_data=0;
  return 0;
}

uint8_t bright_flipper_set(struct t_status *data)
{
  OCR1B = (*data).data;
  (*data).write_data=0;
  return 0;
}

uint8_t bright_free_set(struct t_status *data)
{
  OCR2A = (*data).data;
  (*data).write_data=0;
  return 0;
}


uint8_t bright_lounge_set(struct t_status *data)
{
  OCR2B = (*data).data;
  (*data).write_data=0;
  return 0;
}

// diese funktion ist eine dummyfunktion 
// sie wird ueberall da in der Matrix verwendet, wo eigentlich 
// nichts passieren sollte - aka dauerhaft abschalten
uint8_t dummy_switch_null(struct t_status *data)
{
  (*data).write_data=0;
  return 0;
}

uint8_t dummy_opto_null(struct t_status *data)
{
  (*data).write_data=0;
  return 0;
}

uint8_t dummy_bright_null(struct t_status *data)
{
  (*data).write_data=0;
  return 0;
}

// funktionsmatrix fuer switch-controlle - passt nur wenn die als "muss letztes sein" 
// immer da ist und ganz hinten steht... sonst mact es hier bum

//uint8_t (*DoIt[num_bright][3])(struct t_status *data);


void reset_commander()
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
    
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (inverting mode).
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);												// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0); // FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS11);                                 // FastPWM bit 2, clk/64

	TCCR0A |= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0); // FastPWM, Set OC0X on Compare Match, clear OC0x at BOTTOM, (inverting mode).
	TCCR0B |= _BV(CS01) | _BV(CS00);                                                           // clk/64

	TCNT2 = 0;   // pwm timer clear
	OCR2A = 0;   // pwm timer compare target
	OCR2B = 0;   // pwm timer compare target
  	
  	TCNT1 = 0;   // pwm timer clear
	OCR1A = 0;   // pwm timer compare target
	OCR1B = 0;   // pwm timer compare target
	
	TCNT0 = 0;   // pwm timer clear
	OCR0A = 0;   // pwm timer compare target
	OCR0B = 0;   // pwm timer compare target
	// PCICR |= _BV(PCIE1);
	// PCMSK2 |= _BV(PCINT18) | _BV(PCINT20);

}


int main (void)
{

	uint8_t stat_haupt = 0; // status des Hauptschalters 0=ist aus; 1=ist an;<1 ist gesperrt
	uint8_t	recv_buffer[3];
	uint8_t	TWIS_ResonseType;
	
	uint8_t hasharray[num_bright];  // ich benutze hier num_bright als die anzahl aller objekte
	struct t_status workparameter;

	uint8_t (*DoIt[num_bright][MAX(num_action,num_brightaction)])(struct t_status *data);

	reset_commander();

	/* 
	   hier wird jedem Objekt und jeder Funktion ihre Aufgabe zugewiesen.
	*/
	// switchmatrix aufbauen 
	DoIt[switch00][swoff]    = switch00_off; DoIt[switch00][swon] = switch00_on; DoIt[switch00][swstatus] = switch00_status;
	DoIt[switch01][swoff]    = switch01_off; DoIt[switch01][swon] = switch01_on; DoIt[switch01][swstatus] = switch01_status;
	DoIt[switch02][swoff]    = switch02_off; DoIt[switch02][swon] = switch02_on; DoIt[switch02][swstatus] = switch02_status;
	DoIt[switch03][swoff]    = switch03_off; DoIt[switch03][swon] = switch03_on; DoIt[switch03][swstatus] = switch03_status;
	DoIt[switch04][swoff]    = switch04_off; DoIt[switch04][swon] = switch04_on; DoIt[switch04][swstatus] = switch04_status;
	DoIt[switch05][swoff]    = switch05_off; DoIt[switch05][swon] = switch05_on; DoIt[switch05][swstatus] = switch05_status;
	DoIt[switch06][swoff]    = switch06_off; DoIt[switch06][swon] = switch06_on; DoIt[switch06][swstatus] = switch06_status;
	DoIt[switch07][swoff]    = switch07_off; DoIt[switch07][swon] = switch07_on; DoIt[switch07][swstatus] = switch07_status;

	// optokopplermatrix
	DoIt[optokopp00][swoff]    = opto00_off; DoIt[optokopp00][swon] = opto00_on; DoIt[optokopp00][swstatus] = opto00_status;
	DoIt[optokopp01][swoff]    = opto01_off; DoIt[optokopp01][swon] = opto01_on; DoIt[optokopp01][swstatus] = opto01_status;

	/*
	  Die Anzahl der Aktionen fuer Lampen und Relais/Switche ist leider nicht identisch. Daher ist es 
	  notwendig an den fehlenden Punkten in der Matrix trotzdem eine Funktion zuzuweisen.
	 */
	// helligkeitsmatrix
	DoIt[tafel][brset]     = bright_tafel_set;     DoIt[tafel][1]     = dummy_bright_null; DoIt[tafel][2]     = dummy_bright_null;
	DoIt[beamer][brset]    = bright_beamer_set;    DoIt[beamer][1]    = dummy_bright_null; DoIt[beamer][2]    = dummy_bright_null;
	DoIt[schraenke][brset] = bright_schraenke_set; DoIt[schraenke][1] = dummy_bright_null; DoIt[schraenke][2] = dummy_bright_null;
	DoIt[flipper][brset]   = bright_flipper_set;   DoIt[flipper][1]   = dummy_bright_null; DoIt[flipper][2]   = dummy_bright_null;
	DoIt[free1][brset]     = bright_free_set;      DoIt[free1][1]     = dummy_bright_null; DoIt[free1][2]     = dummy_bright_null;
	DoIt[lounge][brset]    = bright_lounge_set;    DoIt[lounge][1]    = dummy_bright_null; DoIt[lounge][2]    = dummy_bright_null;


	// host sendet und will damit objekt foo addressieren - er sendet 0 und will switch00
	hasharray[0] = switch00;
	hasharray[1] = switch01;
	hasharray[2] = switch02;
	hasharray[3] = switch03;
	hasharray[4] = switch04;
	hasharray[5] = switch05;
	hasharray[6] = switch06;
	hasharray[7] = switch07;
	hasharray[8] = optokopp00;
	hasharray[9] = optokopp01;
	hasharray[10] = tafel;
	hasharray[11] = beamer;
	hasharray[12] = schraenke;
	hasharray[13] = flipper;
	hasharray[14] = lounge;
	hasharray[15] = free1;



/* ISR(TIMER0_OVF)  TODO muss interrupt aktiviert werden?
{
    tick++; ?
    // wann ist es dimmen / tasten, zähler löschen
    if (i1/2 == )
    
    PORT |= 
    
    if (i2 / 2 == )
    PORT |= 
}
*/
 
/* ISR(PCINT2)
{
    if (pcint18)
        i1++;
    if (pcint20)
        i2++;
}
*/



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


	/*
	  mainloop
	*/

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
				{
					recv_buffer[0] = TWIS_ReadAck();   // byte 1 lesen
					recv_buffer[1] = TWIS_ReadAck();   // byte 2 lesen
					recv_buffer[2] = TWIS_ReadNack();  // byte 3 lesen
					TWIS_Stop();                // I2C stop
					// Range checken
					if(recv_buffer[0] >= num_bright) break; // nochmal num_bright ist anzahl aller Objekte
					if(recv_buffer[1] >= MAX(num_action,num_brightaction)) break;

					workparameter.data=recv_buffer[2];
					workparameter.write_data=0;
					DoIt[hasharray[recv_buffer[0]]][recv_buffer[1]](&workparameter);
	/*
	** Slave is requested to send bytes to the master.
	*/
				}
				break;
	    			case TWIS_WriteBytes:
				{
				  // nur schreiben wenn auch wirklich daten da waren
				  if (workparameter.write_data==1)
				    TWIS_Write(workparameter.data);        // byte das in Leseoperation befüllt wurde schreiben
				  TWIS_Stop();
				}
				break;
				default:
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

