/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>
#include <avr/io.h>

#define MACHDUNKEL 0
#define MACHHELL 1

// pwm ... daher hier invers
#define MAXHELL 20
#define MAXDUNKEL 250
#define BRIGHT_NO_CHANGE 255


#define HASDATA (0x01)
#define HASNDATA (0x00)

// der schalter generiert $Anzahl von events pro sekunde


// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#define I2CADRESSE 15 // I2C Adresse


/*
  userinterface Taster
 */
#define U_VORTRAG (0x00)
#define U_LOUNGE (0x01)
#define U_COUNT (0x02)
/* 
   Actionen als defines
*/
#define A_SW_OFF      (0x00)
#define A_SW_ON       (0x01)
#define A_SW_STATUS   (0x02)
#define A_SW_COUNT    (0x03)
#define A_SW_NOCHANGE (0x04)

// nur intern fuer queue notwendig
// daher nicht in der count
#define A_SW_NOCHANGE (0x04) 

#define A_BR_SET     (0x00)
#define A_BR_GET     (0x01)
#define A_BR_COUNT   (0x02)

/*
  Objekte als defines
*/
#define O_SW_FIRST (0x00)
// kuechenlicht
#define O_SW00 (0x00)
// beamer
#define O_SW01 (0x01)
// licht vortrag
#define O_SW02 (0x02)
// licht lounge
#define O_SW03 (0x03)
// tischsteckdose
#define O_SW04 (0x04)
// klo
#define O_SW05 (0x05)
// hauptschuetz
#define O_SW06 (0x06)
// frei
#define O_SW07 (0x07)
// licht+serverschrank-schuetz
#define O_SW08 (0x08)
// alle steckdosen (herd) schuetz
#define O_SW09 (0x09)
#define O_OPTO00 (O_SW08)
#define O_OPTO01 (O_SW09)
#define O_SW_LAST (0x09)

#define O_LAMP_FIRST   (0x0A)
#define O_LAMP_TAFEL   (0x0A)
#define O_LAMP_BEAMER  (0x0B)
#define O_LAMP_SCHRANK (0x0C)
#define O_LAMP_FLIPPER (0x0D)
#define O_LAMP_FREE    (0x0E)
#define O_LAMP_LOUNGE  (0x0F)
#define O_LAMP_LAST    (0x0F)

#define O_TASTER_FIRST (0x10)
#define O_TASTER_LOUNGE (0x10)
#define O_TASTER_VORTRAG (0x11)
#define O_TASTER_MAIN (0x12)
#define O_TASTER_LAST (0x12)

#define O_SW_COUNT (0x0A)
#define O_LAMP_COUNT (0x06)
#define O_TASTER_COUNT (0x03)
#define O_COUNT   (0x13)

// ist 0.005sekunden * DOUBLECLICK
#define T_CLICKDELAY 50
#define T_CLICKTHRESHOLD 5

// bei Timer/counter = 64*256 auf 16MHz
// etwa 0.000512 sekunden
// gibt es hier leider nicht ... ist aber auch nicht so das problem
// #define TICKS_A0 (0x0020)  
// etwa 0.002048
#define TICKS_A1 (0x0002)
// etwa 0.005120
#define TICKS_A2 (0x0005)
// etwa 0.01024
#define TICKS_A3 (0x000A)
// etwa 0.04096
#define TICKS_A4 (0x0028)
// etwa 0.125 sekunden
#define TICKS_A5 (0x007A)
// etwa 0.25 sekunden
#define TICKS_A6 (0x00F4)
// etwa 0.5 sekunden
#define TICKS_A7 (0x01E8)
// etwa 1.0 sekunden
#define TICKS_A8 (0x03D1)
// etwa 5.0 sekunden
#define TICKS_A9 (0x1313)


#define RINGBUFFERSIZE (3)
#define QUEUE_SIZE (35)

#define QUEUE_FREE (0x00)
#define QUEUE_A0   (0x01)
#define QUEUE_A1   (0x02)
#define QUEUE_A2   (0x03)
#define QUEUE_A3   (0x04)
#define QUEUE_A4   (0x05)
#define QUEUE_A5   (0x06)
#define QUEUE_A6   (0x07)
#define QUEUE_A7   (0x08)
#define QUEUE_A8   (0x09)
#define QUEUE_A9   (0x0A)

// struktur die daten enthaelt und das FLAG wenn daten
// auf den I2C geschrieben werden sollen
struct t_busdata {
	uint8_t in_data[3];
	uint8_t out_data;
	uint8_t write_data;
};

static struct t_switch_parameter{
	volatile uint8_t *write_port;
	volatile uint8_t *read_port;
	uint8_t pin;
}swp_matrix[]={
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 } 
};

volatile uint8_t* lp_matrix[] = { 0, 0, 0, 0, 0, 0 };

static struct t_ringbuffer{
	uint16_t tastercounter; // mitzaehlen
	uint16_t tastercounter_last; // was war der wert, als wir ihn zuletzt getestet haben
	uint8_t clickpos; 
	uint16_t clickstate[RINGBUFFERSIZE]; 
	uint16_t clickstate_free[RINGBUFFERSIZE];
}rb[]={
	{ 0, 0, 0, { 0,0,0}, {0,0,0}},
	{ 0, 0, 0, { 0,0,0}, {0,0,0}}
};

// den status nur fuer den Vortrag
struct t_state_vortrag {
  uint8_t bright_tafel;
  uint8_t bright_beamer;
  uint8_t bright_schraenke;
  uint8_t bright_flipper;
  uint8_t dimDirection;
  uint8_t onoff;
};

// den status nur fuer die lounge
struct t_state_lounge {
  uint8_t bright_lounge;
  uint8_t bright_free;
  uint8_t dimDirection;
  uint8_t onoff;
};

struct t_autoqueue_pos {
  uint8_t queuefill;
  uint8_t queuepos;
};

// unsere Queue lang genug um .... what ever!
// XXX dies muss angepasst werden wenn sich die Anzahl der Lampen/Switche aendert
static struct t_queue_object {
  uint8_t _state;
	uint8_t bright[O_LAMP_COUNT];
	uint8_t switch_relai[O_SW_COUNT];
}queue[]={
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}},
	{ QUEUE_FREE, 
		{ BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE, BRIGHT_NO_CHANGE }, 
		{A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE,A_SW_NOCHANGE}}
};



/*   { PORTC, PORTC, PC3 }, // relai 00 - kuechenlicht */
/*   { PORTC, PORTC, PC2 }, // 01  - beamer */
/*   { PORTC, PORTC, PC1 }, // 02  - licht vortrag */
/*   { PORTC, PORTC, PC0 }, // 03  - licht lounge */
/*   { PORTB, PORTB, PB5 }, // 04 - tischsteckdose*/
/*   { PORTB, PORTB, PB4 }, // 05 - klo */
/*   { PORTB, PORTB, PB0 }, // 06 - hauptschuetze */
/*   { PORTD, PORTD, PD7 }, // 07 - frei */
/*   { PORTD, PORTD, PD0 }, // opto 00 - licht+serverschrank-schuetz*/
/*   { PORTD, PORTD, PD1 }  // opto 01 - alle steckdosen (herd) schuetz */


static struct t_switch_parameter_ro {
  volatile uint8_t *read_port;
  uint8_t pin;
}tp_matrix[]={ 
	{ 0, 0},
	{ 0, 0},
	{ 0, 0}
};


#endif
