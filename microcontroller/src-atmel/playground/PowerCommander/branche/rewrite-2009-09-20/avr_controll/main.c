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
#include "pc_init.i"
#include "switch.i"
#include "bright.i"

struct t_busdata busdata = { { 0, 0, 0}, 0,HASNDATA};

// aktuller zustand
struct t_state_vortrag vortrag_cur = { MAXHELL , MAXHELL , MAXHELL , MAXHELL , MACHDUNKEL, A_SW_ON }; // init sonst working
struct t_state_lounge lounge_cur = { MAXHELL , MAXHELL , MACHDUNKEL, A_SW_ON }; // init sonst working

// volle belaeuchtung - dimmrichtung dunkler
struct t_state_vortrag vortrag_default = { MAXHELL , MAXHELL , MAXHELL , MAXHELL, MACHDUNKEL,A_SW_ON };
struct t_state_lounge lounge_default = { MAXHELL , MAXHELL , MACHDUNKEL, A_SW_ON};

// tafel hell rest dunkel
struct t_state_vortrag vortrag_vortrag1 = { MAXHELL , MAXDUNKEL , MAXDUNKEL , MAXDUNKEL , MACHHELL,A_SW_ON };
// flipper hell rest dunkel
struct t_state_vortrag vortrag_vortrag2 = { MAXDUNKEL , MAXDUNKEL , MAXDUNKEL , MAXHELL , MACHHELL,A_SW_ON };

struct t_state_vortrag freak_cur = { MAXHELL , MAXHELL , MAXHELL , MAXHELL , MACHDUNKEL,A_SW_ON }; // init sonst working


struct t_queue_object poweroff = { QUEUE_A1, 
																	 { BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
																	 { A_SW_OFF, // kuechenlicht
																		 A_SW_OFF, // beamer
																		 A_SW_OFF, // licht vortrag
																		 A_SW_OFF, // licht lounge
																		 A_SW_OFF, // tischsteckdose
																		 A_SW_OFF, // klo
																		 A_SW_OFF, // hauptschuetz
																		 A_SW_OFF, // frei
																		 A_SW_OFF, // licht+serverschrank-schuetz
																		 A_SW_OFF  // alle steckdosen (herd) schuetz
																	 }
};

struct t_queue_object sleepmode = { QUEUE_A1, 
																		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
																		{ A_SW_NOCHANGE, // kuechenlicht
																			A_SW_OFF,      // beamer
																			A_SW_NOCHANGE, // licht vortrag
																			A_SW_NOCHANGE, // licht lounge
																			A_SW_NOCHANGE, // tischsteckdose
																			A_SW_NOCHANGE, // klo
																			A_SW_OFF,      // hauptschuetz
																			A_SW_NOCHANGE, // frei
																			A_SW_OFF,      // licht+serverschrank-schuetz
																			A_SW_NOCHANGE  // alle steckdosen (herd) schuetz
																		}
};

struct t_queue_object poweron = { QUEUE_A1, 
																	{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
																	{ A_SW_NOCHANGE, // kuechenlicht
																		A_SW_NOCHANGE, // beamer
																		A_SW_NOCHANGE, // licht vortrag
																		A_SW_NOCHANGE, // licht lounge
																		A_SW_ON, // tischsteckdose
																		A_SW_ON, // klo
																		A_SW_ON, // hauptschuetz
																		A_SW_NOCHANGE, // frei
																		A_SW_ON, // licht+serverschrank-schuetz
																		A_SW_ON  // alle steckdosen (herd) schuetz
																	}
};

/*
	XXX nehme drei fuer Ringbuffersize an ... das kann sich auch aendern!
*/


uint16_t tickscounter=0;

#include "queue.i"

inline void ringbuffer_flush(struct t_ringbuffer *rb){
	uint8_t i=0;
	for(;i<RINGBUFFERSIZE;i++) 
		{
			(*rb).clickstate[i]=0;	
		(*rb).clickstate_free[i]=0;
		}
	(*rb).clickpos=0;
}


/*
	interrupt service routine
 */

/*
	multipexing / ticks / Timerueberlauf
*/
ISR(TIMER0_OVF_vect)
{
	uint8_t i;
	uint8_t taster_i;

	/*
		andere interrupts aus!
	*/
	cli();
	/* 
		 etwa 0.000512 sekunden
		 gibt es hier leider nicht
	*/
/* 	if (( tickscounter & (TICKS_A0) ) == TICKS_A0 ){ */
/* 		deque(QUEUE_A0); */
/* 	} */
	/* 
		 etwa 0.0025 sekunden
	*/
	if (( tickscounter & (TICKS_A1) ) == TICKS_A1 ){
		//		deque(QUEUE_A1);
	}
	/* 
		 etwa 0.0050 sekunden
	*/
	if (( tickscounter & (TICKS_A2) ) == TICKS_A2 ){
		// hier werden die clicks erfasst
#include "click_events.i"
		for(taster_i=0;taster_i<U_COUNT;taster_i++){
			/*
				check auf klick
				+ starte <was_auch_immer>-klickaktion / oder auch nicht aber immer
				+ reset des ringbuffers
			*/
			
			/*
				einmal kurz
			*/
			switch(taster_i) {
			case U_VORTRAG:
				{
					// die events die durch den vortrags-taster ausgeloesst werden
#include "events_vortrag.i"
				}
				break;
			case U_LOUNGE:
				{
					// die events die durch den lounge-taster ausgeloesst werden
#include "events_lounge.i"
				}
				break;
			default:
				break;
			}
		}
		deque(QUEUE_A2);
	}

	/* 
		 etwa 0.01 sekunden
	*/
	if (( tickscounter & (TICKS_A3) ) == TICKS_A3 ){
		deque(QUEUE_A3);
	}
	/* 
		 etwa 0.04 sekunden
	*/
	if (( tickscounter & (TICKS_A4) ) == TICKS_A4 ){
		/*
			check ob der Hauptschalter umgelegt wurde 
			muss ja nicht staendig passieren
		*/
		if ((!(PINB & _BV(PINB7))) == 1)   // Hauptschalter geht aus
		{ // TODO Status merken?
			//			add_queue(&poweroff);
		}
		else if ((PINB & _BV(PINB7))  == 0)   // Hauptschalter geht an
		{
			//add_queue(&poweron);
		}
		/*
			if (nachtmodus == on) 
			{
			  add_queue(&sleepmode);
      }
			else if (nachtmodus == off)
			{
			  add_queue(&poweron);
			}
		*/
		deque(QUEUE_A4);
	}

	/* 
		 etwa 0.125 sekunden
	*/
	if (( tickscounter & (TICKS_A5) ) == TICKS_A5 ){
		//		deque(QUEUE_A5);
	}
	/* 
		 etwa 0.25 sekunden
	*/
	if (( tickscounter & (TICKS_A6) ) == TICKS_A6 ){
		deque(QUEUE_A6);
	}
	/* 
		 etwa 0.5 sekunden
	*/
	if (( tickscounter & (TICKS_A7) ) == TICKS_A7 ){
		deque(QUEUE_A7);
	}
	/* 
		 etwa 1 sekunde
	*/
	if (( tickscounter & (TICKS_A8) ) == TICKS_A8 ){
		deque(QUEUE_A8);
	}
	/* 
		 etwa 5 sekunde - im moment nur die sequenzen
	*/
	if (( tickscounter & (TICKS_A9) ) == TICKS_A9 ){
		//		deque(QUEUE_A9);
	}

	/* 
		 ueberlaeufe sind ok!
	*/
	tickscounter++;

	sei();
}
/*
	taster in der lounge
*/
ISR(INT0_vect)
{
	cli();
	rb[U_LOUNGE].tastercounter++;
	sei();
}

/*
	taster im Vortragsraum
*/
ISR(PCINT2_vect)
{
	cli();
	rb[U_VORTRAG].tastercounter++;
	sei();
}

int main(void)
{
	uint8_t TWIS_ResonseType;

	init_modi();
	init_relais();
	init_timer();

#include "matrix_init.i"
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
	TWIS_Init(I2CADRESSE, 100000);

	sei();
	/*
		mainloop - die ist die kommunikation mit einem entferntem
		host
	*/
	while(1) 
		{
			if (TWIS_ResonseRequired(&TWIS_ResonseType))
				{
					switch (TWIS_ResonseType)
						{
							/*
							** Slave is requests to read bytes from the master.
							*/
						case TWIS_ReadBytes:
							{
								busdata.in_data[0] = TWIS_ReadAck();   // byte 1 lesen
								busdata.in_data[1] = TWIS_ReadAck();   // byte 2 lesen
								busdata.in_data[2] = TWIS_ReadNack();  // byte 3 lesen
								busdata.write_data = HASNDATA;
								TWIS_Stop();                // I2C stop
								/*
									die Anweisungen die ueber I2C kommen gehen an der queue vorbei
									d.h. die queue bleibt erhalten wenn eine i2c nachricht rein kommt
									XXX ... ist das gut oder schlecht?
								*/
								if (( busdata.in_data[0] >= (O_SW_FIRST)) && 
										( busdata.in_data[0] <= (O_SW_LAST)))	{
									switch_fkt(&(swp_matrix[busdata.in_data[0]]), &busdata);
								}
								if (( busdata.in_data[0] >= (O_LAMP_FIRST)) && 
										( busdata.in_data[0] <= (O_LAMP_LAST)))	{
									bright_fkt(lp_matrix[busdata.in_data[0]-(O_SW_COUNT)], &busdata);
								}
								if (( busdata.in_data[0] >= (O_TASTER_FIRST)) && 
										( busdata.in_data[0] <= (O_TASTER_LAST)))	{
									switch_ro_fkt(&(tp_matrix[busdata.in_data[0]-(O_SW_COUNT+O_LAMP_COUNT)]), &busdata);
								}
								
								if ( busdata.in_data[1] >= O_SW_COUNT ) break;
								
							}
							break;
						case TWIS_WriteBytes:
							{
								if(busdata.write_data == HASDATA){
									TWIS_Write(busdata.out_data);
								} else {
									TWIS_Write(0);
								}
								busdata.write_data = HASNDATA;
								TWIS_Stop();
							}
							break;
						default:
							break;
						}
				}
		}
}


