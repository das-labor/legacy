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
#include "pc_init.c"
#include "switch.c"
#include "bright.c"

struct t_busdata busdata = { { 0, 0, 0}, 0,HASNDATA};

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


/*
	nehme drei fuer Ringbuffersize an ... das kann sich auch aendern!
*/

static struct t_ringbuffer{
	uint16_t tastercounter; // mitzaehlen
	uint16_t clickstate[RINGBUFFERSIZE];
	uint16_t clickstate_free[RINGBUFFERSIZE];
  uint8_t clickpos;
}rb[]={
	{ 0, { 0,0,0}, {0,0,0}, 0},
	{ 0, { 0,0,0}, {0,0,0}, 0}
};

uint16_t tickscounter=0;


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
	/*
		andere interrupts aus!
	*/
	cli();
	/* 
		 etwa 0.000512 sekunden
	*/
	if (( tickscounter & (TICKS_A0) ) == 0 ){
	}
	/* 
		 etwa 0.0025 sekunden
	*/
	if (( tickscounter & (TICKS_A1) ) == 0 ){
	}
	/* 
		 etwa 0.0050 sekunden
	*/
	if (( tickscounter & (TICKS_A2) ) == 0 ){
		for(i=0;i<U_COUNT;i++){
			if ( rb[i].tastercounter > 0) rb[i].clickstate[rb[i].clickpos]++;
			else {
				rb[i].clickstate_free[rb[i].clickpos]++;
			}
			/*
				ring-buffer
			*/
			if ((rb[i].clickstate_free[rb[i].clickpos] > 0) && 
					(rb[i].clickstate[rb[i].clickpos] > 0) ){
				rb[i].clickpos++;
				rb[i].clickstate[rb[i].clickpos]=0;
				rb[i].clickstate_free[rb[i].clickpos]=0;
			}
			if (rb[i].clickpos > 2) ringbuffer_flush(&(rb[i]));
			
			/*
				check auf klick
				+ starte <was_auch_immer>-klickaktion / oder auch nicht aber immer
				+ reset des ringbuffers
			*/
			
			/*
				einmal kurz
			*/
			if ( (rb[i].clickstate[0] <= T_CLICKDELAY) && 
					 (rb[i].clickstate_free[1] > T_CLICKDELAY))
				{

				}
			
			
			/*
				einmal lang - und halten
			*/
			else if ( (rb[i].clickstate[0] > T_CLICKDELAY) )
				{

				}
			
			/*
				zwei mal schnell klicken
			*/
			else if ( (rb[i].clickstate[0] <= T_CLICKDELAY) &&
								(rb[i].clickstate_free[1] <= T_CLICKDELAY) &&
								(rb[i].clickstate[1] <= T_CLICKDELAY) && 
								(rb[i].clickstate_free[2] > T_CLICKDELAY) )
				{

				}
			/*
				druecken loslassen druecken halten
			*/
			else if ( (rb[i].clickstate[0] <= T_CLICKDELAY) &&
								(rb[i].clickstate_free[1] <= T_CLICKDELAY) &&
								(rb[i].clickstate[1] > T_CLICKDELAY))
				{

				}
			/*
				drei mal schnell klicken
			*/
			else if ( (rb[i].clickstate[0] <= T_CLICKDELAY) &&
								(rb[i].clickstate_free[1] <= T_CLICKDELAY) &&
								(rb[i].clickstate[1] <= T_CLICKDELAY) && 
								(rb[i].clickstate_free[2] <= T_CLICKDELAY) &&
								(rb[i].clickstate[2] <= T_CLICKDELAY) )
				{

				}
			/*
				druecken ->loslassen, druecken->loslassen, druecken->halten
			*/
			else if ( (rb[i].clickstate[0] <= T_CLICKDELAY) &&
								(rb[i].clickstate_free[1] <= T_CLICKDELAY) &&
								(rb[i].clickstate[1] <= T_CLICKDELAY) && 
								(rb[i].clickstate_free[2] <= T_CLICKDELAY) &&
								(rb[i].clickstate[2] > T_CLICKDELAY) )
				{

				}

			ringbuffer_flush(&(rb[i]));		
		
		}
	}

	/* 
		 etwa 0.01 sekunden
	*/
	if (( tickscounter & (TICKS_A3) ) == 0 ){
	}
	/* 
		 etwa 0.04 sekunden
	*/
	if (( tickscounter & (TICKS_A4) ) == 0 ){
	}

	/* 
		 etwa 0.125 sekunden
	*/
	if (( tickscounter & (TICKS_A5) ) == 0 ){
	}
	/* 
		 etwa 0.25 sekunden
	*/
	if (( tickscounter & (TICKS_A6) ) == 0 ){
	}
	/* 
		 etwa 0.5 sekunden
	*/
	if (( tickscounter & (TICKS_A7) ) == 0 ){
	}
	/* 
		 etwa 1 sekunde
	*/
	if (( tickscounter & (TICKS_A8) ) == 0 ){
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


	/*
		switch-parameter-matrix
		write/read/pin
	*/
	struct t_switch_parameter swp_matrix[] = { { PORTC, PORTC, PC3 }, // relai 00
																					 { PORTC, PORTC, PC2 }, // 01
																					 { PORTC, PORTC, PC1 }, // 02
																					 { PORTC, PORTC, PC0 }, // 03
																					 { PORTB, PORTB, PB5 }, // 04
																					 { PORTB, PORTB, PB4 }, // 05
																					 { PORTB, PORTB, PB0 }, // 06
																					 { PORTD, PORTD, PD7 }, // 07
																					 { PORTD, PORTD, PD0 }, // opto 00
																					 { PORTD, PORTD, PD1 }  // opto 01 
	};

	/*
		taster-parameter-matrix ... readonly schalter
	*/
		
	struct t_switch_parameter_ro tp_matrix[] = { {PINB , PINB7 }, // hauptschalter
																						 {PIND , PIND2 }, // Taster 1 int0
																						 {PIND , PIND4 }  // Taster 2 pcint20
	};

	/*
		lampen-parameter
	*/
	uint8_t lp_matrix[] = { OCR0A, // tafel
												OCR0B, // beamer
												OCR1A, // schraenke
												OCR1B, // flipper
												OCR2A, // free
												OCR2B  // lounge
	};

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


