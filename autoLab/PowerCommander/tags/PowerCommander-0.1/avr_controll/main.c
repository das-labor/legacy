/* -*- Mode: C; tab-width: 2 -*- */
/*
 * PowerCommander - Ausführungs Kontroller
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include "twi_slave/twi_slave.h"

#include "PowerCommander.h"


// aktuller zustand
struct t_state_vortrag vortrag_cur = { MAXHELL , MAXHELL , MAXHELL , MAXHELL , MACHDUNKEL }; // init sonst working
struct t_state_lounge lounge_cur = { MAXHELL , MAXHELL , MACHDUNKEL }; // init sonst working

// volle belaeuchtung - dimmrichtung dunkler
struct t_state_vortrag vortrag_default = { MAXHELL , MAXHELL , MAXHELL , MAXHELL, MACHDUNKEL };
struct t_state_lounge lounge_default = { MAXHELL , MAXHELL , MACHDUNKEL };

// tafel hell rest dunkel
struct t_state_vortrag vortrag_vortrag1 = { MAXHELL , MAXDUNKEL , MAXDUNKEL , MAXDUNKEL , MACHHELL };
// flipper hell rest dunkel
struct t_state_vortrag vortrag_vortrag2 = { MAXDUNKEL , MAXDUNKEL , MAXDUNKEL , MAXHELL , MACHHELL };

struct t_state_vortrag freak_cur = { MAXHELL , MAXHELL , MAXHELL , MAXHELL , MACHDUNKEL }; // init sonst working

uint8_t helligkeitsstufen[] = { 1, 2 , 4 , 
																8, 16 , 24, 32, 
																48, 64 , 96, 128, 
																192, 255 };

struct t_counter_status timing_counter = { 0,0,0,0,0 };

/*
	pro sekunde kommen 100 Events an (50Hz , eine halbwelle)

	wir brauchen also ranges fuer der schalter lange und wann 
	er kurz gedrueck wurde. Es handelt sich hierbei um die
	Anzahl der Events.

	x < 25 Events (1/4 sekunde) , dann kurz gedrueckt, 
	25 < x < 500 (1/4 - 5 sekunden) lange gedrueckt 
	500 < x < 700 tafel hell rest dunkel
	700 < x < 900 flipper hell, rest dunkel
	x> 900 freakshow!
*/


/*
	sequenzen: 
	0-50 = kurz drauf  (< 0.5 sekunden auf dem schalter)
	50-100 = lang drauf (0.5 < 1.0 sekunden auf dem schalter)
	100-900 = dimmen (1.0 < 9.0 sekunden auf dem schalter)
	900-1000 = vortragsbeleuchtung 1
	1000-1400 = vortragsbeleuchtung 2
	...
	
 */
uint16_t schaltinterval[] = { 0, 50, 100 ,900, 1000, 1400, 1700 };

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

#include "switch.c"
#include "bright.c"

/*
	diese funktion ist eine dummyfunktion 
	sie wird ueberall da in der Matrix verwendet, wo eigentlich 
	nichts passieren sollte - aka dauerhaft abschalten
*/

void dummy_bright_null(struct t_status *data)
{
	(*data).write_data = 0;
}


/*
	interrupt fuer schalter im vortrag
*/
void itr_schalter_vortrag_statisch()
{
	// licht kurz an
	if (timing_counter.tastercounter_vortrag > schaltinterval[0] &&
			 timing_counter.tastercounter_vortrag < schaltinterval[1]) {
		//		bright_vortrag_set(&vortrag_default);
		if ((PORTC >> PC1) & 1)
			PORTC &= ~_BV(PC1);
		else
			PORTC |= _BV(PC1);
	}
	// licht lange an
	if (timing_counter.tastercounter_vortrag > schaltinterval[1] &&
			 timing_counter.tastercounter_vortrag < schaltinterval[2]) {
		//		bright_vortrag_set(&vortrag_default);
		if ((PORTC >> PC1) & 1)
			PORTC &= ~_BV(PC1);
		else
			PORTC |= _BV(PC1);
	}
	if (timing_counter.tastercounter_vortrag > schaltinterval[2] &&
			timing_counter.tastercounter_vortrag < schaltinterval[3]) {
		if ( vortrag_cur.dimDirection == MACHDUNKEL ) {
			vortrag_cur.dimDirection = MACHHELL;
		} else {
			vortrag_cur.dimDirection = MACHDUNKEL;
		}
	}
	if (timing_counter.tastercounter_vortrag > schaltinterval[5] &&
			timing_counter.tastercounter_vortrag < schaltinterval[6]) {
		/*
			starting freakshow
		*/
		bright_vortrag_set(&freak_cur);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=0;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=0;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=0;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=0;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);
		freak_cur.bright_tafel=255;      freak_cur.bright_beamer=255;
		freak_cur.bright_schraenke=255;  freak_cur.bright_flipper=255;
		bright_vortrag_set(&freak_cur);
		_delay_ms(500);


		bright_vortrag_set(&vortrag_cur);
	}
}

/*
	dynamisch ... die interrupts kommen weiter rein und laufen auch weiter
*/
inline void itr_schalter_vortrag_dynamisch()
{
	if (timing_counter.tastercounter_vortrag > schaltinterval[2] &&
			timing_counter.tastercounter_vortrag < schaltinterval[3]) {
		if ( vortrag_cur.dimDirection == MACHDUNKEL ){
			vortrag_cur.bright_tafel++;
			vortrag_cur.bright_beamer++;
			vortrag_cur.bright_flipper++;
			vortrag_cur.bright_schraenke++;

		}
		if ( vortrag_cur.dimDirection == MACHHELL ){
			vortrag_cur.bright_beamer--;
			vortrag_cur.bright_flipper--;
			vortrag_cur.bright_tafel--;
			vortrag_cur.bright_schraenke--;
		}
		if (vortrag_cur.bright_tafel == MAXHELL) { 
			vortrag_cur.dimDirection = MACHDUNKEL;
		} else {
			vortrag_cur.dimDirection = MACHHELL;
		}
																							 
		bright_vortrag_set(&vortrag_cur);
	}
	if (timing_counter.tastercounter_vortrag > schaltinterval[3] &&
			timing_counter.tastercounter_vortrag < schaltinterval[4])
		bright_vortrag_set(&vortrag_vortrag1);
	if (timing_counter.tastercounter_vortrag > schaltinterval[4] &&
			timing_counter.tastercounter_vortrag < schaltinterval[5])
		bright_vortrag_set(&vortrag_vortrag2);
	if (timing_counter.tastercounter_vortrag > schaltinterval[5] &&
			timing_counter.tastercounter_vortrag < schaltinterval[6])
		bright_vortrag_set(&vortrag_default);
	
}

/*
	interrupt fuer schalter im lounge
*/
inline void itr_schalter_lounge_statisch()
{
		if (timing_counter.tastercounter_lounge > schaltinterval[0] &&
			 timing_counter.tastercounter_lounge < schaltinterval[1]) {
		if ((PORTC >> PC0) & 1)
			PORTC &= ~_BV(PC0);
		else
			PORTC |= _BV(PC0);
	}
}

inline void itr_schalter_lounge_dynamisch()
{
	if (timing_counter.tastercounter_lounge > schaltinterval[2] &&
			timing_counter.tastercounter_lounge < schaltinterval[3]) {
		if ( lounge_cur.dimDirection == MACHDUNKEL ){
			lounge_cur.bright_lounge++;
			lounge_cur.bright_free++;
		}
		if ( lounge_cur.dimDirection == MACHHELL ){
			lounge_cur.bright_lounge--;
			lounge_cur.bright_free--;
		}
		if (lounge_cur.bright_lounge == MAXHELL) { 
			lounge_cur.dimDirection = MACHDUNKEL;
		} else {
			lounge_cur.dimDirection = MACHHELL;
		}
																							 
		bright_lounge_state_set(&lounge_cur);
	}

}

ISR(TIMER0_OVF_vect)
{
	/*
		andere interrupts aus!
	*/
	cli();
	/* 
		 alle 9.8 ticks sollte sollte was vom schalter kommen
		 also sagen wir, dass wenn sich nach 32ticks nichts am schalter
		 getan hat, dann wurde er los gelassen. Wir koennen den 
		 Counter fuer die eingaben auf null setzen
	*/
	if ((timing_counter.tickscounter & 0x001F) == 0) { // alle 32 ticks ... 0.032 sekunden
		if (timing_counter.tastercounter_vortrag != 0) {
			if (timing_counter.tastercounter_vortrag == timing_counter.tastercounter_vortrag_last) {
				/* 
					was soll passieren wenn der schlater losgelassen wurde
					in erster linie sicher ein Rest
				*/
				itr_schalter_vortrag_statisch();
				timing_counter.tastercounter_vortrag = 0;
				timing_counter.tastercounter_vortrag_last = 0;
			} else {

				itr_schalter_vortrag_dynamisch();
				/*
					und wir zaehlen natuerlich weiter
				*/
				timing_counter.tastercounter_vortrag_last = timing_counter.tastercounter_vortrag;
				/*
					der schalter wird noch gedrueckt. wir haben also einen 
					dynamischen Bereich
				*/
				//				itr_schalter_vortrag_dynamisch();
			}
		}
		if ( timing_counter.tastercounter_lounge != 0) {
			if ( timing_counter.tastercounter_lounge == timing_counter.tastercounter_lounge_last) {
				/* 
					 keine aenderung festgestellt folglich call to set fur lounge 
				*/
				itr_schalter_lounge_statisch();
				timing_counter.tastercounter_lounge = 0;
				timing_counter.tastercounter_lounge_last = 0;
			} else {
				itr_schalter_lounge_dynamisch();
				timing_counter.tastercounter_lounge_last = timing_counter.tastercounter_lounge;
			}
		}
	}

	if ( (timing_counter.tickscounter & 0x03FF) == 0) // alle 1024 ticks ... ca 1sec
		{}

	if ( (timing_counter.tickscounter & 0x14FF) == 0) // alle 5120 ticks ... ca 5sec
		{}

	/* 
		 ueberlaeufe sind ok!
	*/
	timing_counter.tickscounter++;
	/*
		und alle interrupts wieder auf go!
	*/
	sei();
}

ISR(PCINT2_vect)
{
	cli();
	timing_counter.tastercounter_vortrag++;
	sei();
}

ISR(INT0_vect)
{
	cli();
	timing_counter.tastercounter_lounge++;
	sei();
}

inline void init_commander()
{
	/* 
		 Disable Analog Comparator (power save)
	*/
	ACSR = _BV(ACD);

	/* 
		 Disable Analog to Digital converter (power save)
	*/
	ADCSRA = 0;

	DDRC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);      // relais ausgänge 1-4 küche licht, beamer, vortrag licht, lounge licht
	PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0));  // aus damit

	DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB0);                 // relais ausgänge 5-7 tischsteckdose lounge, geschaltete serverschrank steckdose,
	PORTB &= ~(_BV(PB4) | _BV(PB0));             						// aus damit
	PORTB |= _BV(PB5);	

	DDRD |= _BV(PD7) | _BV(PD1) | _BV(PD0);                 // relais ausgang 8, 230V 1-2
	PORTD &= ~(_BV(PD7) | _BV(PD1) | _BV(PD0));             // aus

	DDRD  &= ~(_BV(PD4) | _BV(PD2));	/* Pin PD2 und PD4 als Eingange für Taster blau und gelb */
	PORTD |= _BV(PD4) | _BV(PD2);			/* Pull Ups setzen */


	DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);                 // Pins mit pwm als ausgänge
	DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);                 // Pins mit pwm als ausgänge
  
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (inverting mode).
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);																								// clk/64

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
	
	EICRA |= _BV(ISC00);	// Trigger Interrupt on any logical change on pin pd2
	EIMSK |= _BV(INT0);								// Enable External Interrupt Request 0
	
	PCICR  |= _BV(PCIE2);							// Enable Pin Change Interrupt 2
	PCMSK2 |= _BV(PCINT20);						// Enable PCI18 (pin PD4) as Pin Change Interrupt
	
	TIMSK0 |= _BV(TOIE0);							// Enable Timer0 Overflow Interrupt

	/* 
		 nach dem flashen das licht einschalten
	*/
	/*
		Helligkeiten auf default
	*/
	bright_lounge_state_set(&lounge_default);
	bright_vortrag_set(&vortrag_default);
	/* 
		 und auch einschalten
	*/
	PORTC |= _BV(PC0);  // lounge
	PORTC |= _BV(PC1); // vortrag
}


int main (void)
{

	uint8_t stat_haupt = 0; // status des Hauptschalters 0=ist aus; 1=ist an;<1 ist gesperrt
	uint8_t	recv_buffer[3];
	uint8_t	TWIS_ResonseType;

	uint8_t hasharray[num_bright];  // ich benutze hier num_bright als die anzahl aller objekte
	struct t_status workparameter;

	void (*DoIt[num_bright][num_brightaction])(struct t_status *data);
	void (*switchDoIt[num_action])(struct t_status *data);

	init_commander();

	/* 
		 hier wird jedem Objekt und jeder Funktion ihre Aufgabe zugewiesen.
	*/
	/* 
		 switchmatrix aufbauen 
	*/
	switchDoIt[swoff]    = switch_off;
	switchDoIt[swon]     = switch_on;
	switchDoIt[swstatus] = switch_status;

	/*
		helligkeitsmatrix
	*/

	DoIt[tafel][brset]     = bright_tafel_set;     DoIt[tafel][1]     = dummy_bright_null; DoIt[tafel][2]     = dummy_bright_null;
	DoIt[beamer][brset]    = bright_beamer_set;    DoIt[beamer][1]    = dummy_bright_null; DoIt[beamer][2]    = dummy_bright_null;
	DoIt[schraenke][brset] = bright_schraenke_set; DoIt[schraenke][1] = dummy_bright_null; DoIt[schraenke][2] = dummy_bright_null;
	DoIt[flipper][brset]   = bright_flipper_set;   DoIt[flipper][1]   = dummy_bright_null; DoIt[flipper][2]   = dummy_bright_null;
	DoIt[free1][brset]     = bright_free_set;      DoIt[free1][1]     = dummy_bright_null; DoIt[free1][2]     = dummy_bright_null;
	DoIt[lounge][brset]    = bright_lounge_set;    DoIt[lounge][1]    = dummy_bright_null; DoIt[lounge][2]    = dummy_bright_null;


	/*
		host sendet und will damit objekt foo addressieren - er sendet 0 und will switch00
	*/
	hasharray[0] = tafel;
	hasharray[1] = beamer;
	hasharray[2] = schraenke;
	hasharray[3] = flipper;
	hasharray[4] = lounge;
	hasharray[5] = free1;

	/*
	** Clear any interrupt
	*/
	cli();
	
	
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
	sei();

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
					if(recv_buffer[0] >= (num_bright+num_switch)) break; 
					if(recv_buffer[1] >= MAX(num_action,num_brightaction)) break;

					workparameter.data = recv_buffer[2];
					workparameter.write_data = 0;
					if ( recv_buffer[0] < 10 ) { 
						/* 
							 wir brauchen keine zusaetzlichen daten
						*/
						workparameter.data = recv_buffer[0];
						switchDoIt[recv_buffer[1]](&workparameter);
					}
					else {
						DoIt[hasharray[recv_buffer[0]-num_switch]][recv_buffer[1]](&workparameter);
					}
					/*
					** Slave is requested to send bytes to the master.
					*/
				}
				break;
			case TWIS_WriteBytes:
				{
					// nur schreiben wenn auch wirklich daten da waren
					workparameter.write_data = 1;
					workparameter.data = 11;
					if (workparameter.write_data == 1)
						TWIS_Write(workparameter.data);        // byte das in Leseoperation befüllt wurde schreiben
					else
						TWIS_Write(0);
					TWIS_Stop();
				}
				break;
			default:
				break;
			}
		}
		if ((!(PINB & _BV(PINB7))) && stat_haupt == 1)   // Hauptschalter geht aus
		{ // TODO Status merken?
			PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0));	// Küche, Beamer, Blau, Gelb aus
			PORTB &= ~(_BV(PB5) | _BV(PB4) | _BV(PB0));	// Tisch steckdose, Küche aus
			PORTD &= ~(_BV(PD7) | _BV(PD1) | _BV(PD0));	// relais 8 aus, schütze aus

			/*
		 	OCR2A = 0;  
			OCR2B = 0;
			OCR1A = 0;
			OCR1B = 0;
			OCR0A = 0;
			OCR0B = 0; */
			stat_haupt = 0;
		}
		if ((PINB & _BV(PINB7)) && stat_haupt == 0)   // Hauptschalter geht an
		{
			// PORTC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);
			// PORTB |= _BV(PC5) | _BV(PC4) | _BV(PB0);
			PORTD |= _BV(PD1) | _BV(PD0); // Schütze an
			PORTB |= _BV(PB5) | _BV(PB4); // Tisch steckdose, Klo an
			
			/*
			OCR2A = 0;
			OCR2B = 0;
			OCR1A = 0;
			OCR1B = 0;
			OCR0A = 0;
			OCR0B = 0; */
			stat_haupt = 1;
		}
		//        if (nachtmodus) über can?!
		
	}
	return 0;
}

