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

/*
	switch-parameter-matrix
	write/read/pin
*/

uint8_t helligkeitsstufen[] = { 1, 2 , 4 , 
																8, 16 , 24, 32, 
																48, 64 , 96, 128, 
																192, 255 };


/*
	nehme drei fuer Ringbuffersize an ... das kann sich auch aendern!
*/


uint16_t tickscounter=0;

// fuegt eine volle vortrags-anweisung zur queue hinzu
// return != 0 wenn queue voll ist
// prio ist QUEUE_A0->QUEUE_A8
static uint8_t add_queue_v(uint8_t prio,struct t_state_vortrag *state_vortrag)
{
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	for(;i<QUEUE_SIZE;i++){
		if ( queue[i]._state==QUEUE_FREE) {
			pos=i;
			break;
		}
	}
	if (pos==QUEUE_SIZE){
		return 1;
	}
	// wenn prio zu hoch ist, dann wird es irgendwann geloscht
	// da es nie ausgefuehrt wird
	// zumindest, solange noch platz ist und andere queue elemente
	// ausgefuehrt werden. wenn dies nicht mehr der fall ist, haben wir ein problem
	// XXX dummy-objekt einbauen das immer free ist und bloedsinn rausloescht

	// queue-objekt auf default setzen aka tue nichts! ;P
	queue[pos]._state=prio;
	for(i=0;i<O_LAMP_COUNT;i++){
		queue[pos].bright[i]=BRIGHT_NO_CHANGE;
	}
	for(i=0;i<O_SW_COUNT;i++){
		queue[pos].switch_relai[i]=A_SW_NOCHANGE;
	}

	// auf werte setzen die verlangt sind via state_vortrag

	queue[pos].bright[O_LAMP_TAFEL-O_LAMP_FIRST]=(*state_vortrag).bright_tafel;
	queue[pos].bright[O_LAMP_BEAMER-O_LAMP_FIRST]=(*state_vortrag).bright_beamer;
	queue[pos].bright[O_LAMP_SCHRANK-O_LAMP_FIRST]=(*state_vortrag).bright_schraenke;
	queue[pos].bright[O_LAMP_FLIPPER-O_LAMP_FIRST]=(*state_vortrag).bright_flipper;

	// XXX - hier das licht fuer die lampen auch ein und ausschalten
	queue[pos].switch_relai[O_SW02]=(*state_vortrag).onoff;

	return 0;	
}

static uint8_t add_queue_l(uint8_t prio,struct t_state_lounge *state_lounge)
{
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	for(;i<QUEUE_SIZE;i++){
		if ( queue[i]._state==QUEUE_FREE) {
			pos=i;
			break;
		}
	}
	if (pos==QUEUE_SIZE){
		return 1;
	}
	// wenn prio zu hoch ist, dann wird es irgendwann geloscht
	// da es nie ausgefuehrt wird
	// zumindest, solange noch platz ist und andere queue elemente
	// ausgefuehrt werden. wenn dies nicht mehr der fall ist, haben wir ein problem
	// XXX dummy-objekt einbauen das immer free ist und bloedsinn rausloescht

	// queue-objekt auf default setzen aka tue nichts! ;P
	queue[pos]._state=prio;
	for(i=0;i<O_LAMP_COUNT;i++){
		queue[pos].bright[i]=BRIGHT_NO_CHANGE;
	}
	for(i=0;i<O_SW_COUNT;i++){
		queue[pos].switch_relai[i]=A_SW_NOCHANGE;
	}

	// auf werte setzen die verlangt sind via state_vortrag

	queue[pos].bright[O_LAMP_LOUNGE-O_LAMP_FIRST]=(*state_lounge).bright_lounge;
	queue[pos].bright[O_LAMP_FREE-O_LAMP_FIRST]=(*state_lounge).bright_free;

	// XXX - hier das licht fuer die lampen auch ein und ausschalten
	queue[pos].switch_relai[O_SW02]=(*state_lounge).onoff;

	return 0;	
}

static inline void deque(uint8_t inst_auto)
{
	struct t_busdata tmp_bus = { { 0, 0, 0 }, 0, 0 };

	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	
	// durchlaufen der queue bis wir den passenden Prozess gefunden haben
	for(;i<QUEUE_SIZE;i++){
		if(queue[i]._state == inst_auto) {
			pos = i;
			break;
		}
	}
	// wenn keiner zum aufruf passt, brechen wir ab
	if (pos == QUEUE_SIZE) return;
	// entferne alle objekte aus der queue die fuer
	// spaeter gedacht sind - sinn: aktuelle anforderung
	// ueberschreibt alte anforderung
	
	for(i=0;i<QUEUE_SIZE;i++){
		if(queue[i]._state > inst_auto) {
			queue[i]._state = QUEUE_FREE;
		}
	}
	// durchlaufe alle Lampen und aendere die Helligkeit
	// wie angefordert. wenn die Helligkeit BRIGHT_NO_CHANGE ist
	// dann mache keine Aenderung an der Helligkeit
	for (i=0;i<O_LAMP_COUNT;i++)
		{
			if ( queue[pos].bright[i] != BRIGHT_NO_CHANGE ) {
				tmp_bus.in_data[2]=queue[pos].bright[i];
				bright_fkt(lp_matrix[i],&tmp_bus);
			}
		}
	// durchlaufe alle switche/relais/optokoppler und setze sie
	// entweder auf an oder aus
	// wenn keiner der beiden zustaenge gefordert ist, bleibt 
	// alles wie es ist
	for (i=0;i<O_SW_COUNT;i++){
		if ( queue[pos].switch_relai[i] <= A_SW_ON ) switch_fkt(&(swp_matrix[i]),&tmp_bus);
	}
	// final markiere das Queue_objekt als abgearbeitet
	queue[pos]._state = QUEUE_FREE;
}

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
	*/
	if (( tickscounter & (TICKS_A0) ) == TICKS_A0 ){
		deque(QUEUE_A0);
	}
	/* 
		 etwa 0.0025 sekunden
	*/
	if (( tickscounter & (TICKS_A1) ) == TICKS_A1 ){
		deque(QUEUE_A1);
	}
	/* 
		 etwa 0.0050 sekunden
	*/
	if (( tickscounter & (TICKS_A2) ) == TICKS_A2 ){
		// hier werden die clicks erfasst
#include "click_events.c"
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
#include "events_vortrag.c"
				}
				break;
			case U_LOUNGE:
				{
					// die events die durch den lounge-taster ausgeloesst werden
#include "events_lounge.c"
				}
				break;
			default:
				break;
			}
		}
	}
	deque(QUEUE_A2);
		

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
		deque(QUEUE_A4);
	}

	/* 
		 etwa 0.125 sekunden
	*/
	if (( tickscounter & (TICKS_A5) ) == TICKS_A5 ){
		deque(QUEUE_A5);
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
	taster-parameter-matrix ... readonly schalter
*/

	tp_matrix[0].read_port=PINB;
	tp_matrix[0].pin=PINB7; // hauptschalter
	tp_matrix[1].read_port=PIND;
	tp_matrix[1].pin=PIND2; // Taster 1 int0
	tp_matrix[2].read_port=PIND;
	tp_matrix[2].pin=PIND4;  // Taster 2 pcint20

/*
	lampen-parameter
*/

 lp_matrix[0] = OCR0A; // tafel
 lp_matrix[0] = OCR0B; // beamer
 lp_matrix[0] = OCR1A; // schraenke
 lp_matrix[0] = OCR1B; // flipper
 lp_matrix[0] = OCR2A; // free
 lp_matrix[0] = OCR2B; // lounge

 swp_matrix[0].write_port = PORTC; 
 swp_matrix[0].read_port = PORTC;
 swp_matrix[0].pin = PC3; // relai 00 - kuechenlicht */

 swp_matrix[1].write_port = PORTC;
 swp_matrix[1].read_port = PORTC;
 swp_matrix[1].pin = PC2; // 01  - beamer */

 swp_matrix[2].write_port = PORTC;
 swp_matrix[2].read_port = PORTC;
 swp_matrix[2].pin = PC1; // 02  - licht vortrag */

 swp_matrix[3].write_port = PORTC;
 swp_matrix[3].read_port = PORTC;
 swp_matrix[3].pin = PC0; // 03  - licht lounge */

 swp_matrix[4].write_port = PORTB;
 swp_matrix[4].read_port = PORTB;
 swp_matrix[4].pin = PB5; // 04 - tischsteckdose*/

 swp_matrix[5].write_port = PORTB;
 swp_matrix[5].read_port = PORTB;
 swp_matrix[5].pin = PB4; // 05 - klo */

 swp_matrix[6].write_port = PORTB;
 swp_matrix[6].read_port = PORTB;
 swp_matrix[6].pin = PB0; // 06 - hauptschuetze */

 swp_matrix[7].write_port = PORTD;
 swp_matrix[7].read_port = PORTD;
 swp_matrix[7].pin = PD7; // 07 - frei */

 swp_matrix[8].write_port = PORTD;
 swp_matrix[8].read_port = PORTD;
 swp_matrix[8].pin = PD0; // opto 00 - licht+serverschrank-schuetz*/

 swp_matrix[9].write_port = PORTD;
 swp_matrix[9].read_port = PORTD;
 swp_matrix[9].pin = PD1; // opto 01 - alle steckdosen (herd) schuetz */

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


