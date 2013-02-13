/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>
#include <avr/io.h>

#define TWI_ADDRESS 42 // I2C Adresse

// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

// Zuordnung der Ausgänge.
// Die Zahlen sind indexes in die Port-Tabelle, welche mit den
// O_SWXX_PORT und O_SWXX_PIN werten unten gefüllt wird.

// switch lampe - das sind die Relais bzw. Opto relais, die die Lampen mit 230V verbinden
#define SWL_TAFEL       ( 8)
#define SWL_BEAMER      ( 9)
#define SWL_SCHRANK     (10)
#define SWL_FLIPPER     (11)
#define SWL_LOUNGE      ( 3)
#define SWL_KUECHE      ( 0)
#define SWL_VORTRAG     ( 2)

// switch andere
// beamer anaus
#define SWA_BEAMER      ( 1)
// Tischsteckdose Lounge
#define SWA_STECKDOSEN  ( 4)
// Toilette Lich + Steckdosen
#define SWA_KLO         ( 5)
// Hauptschuetz
#define SWA_HS          ( 6)
// freier schalter
#define SWA_FREE        ( 7)


// Ports - Addressen
#define O_SW00_PORT (&(PORTC))
#define O_SW01_PORT (&(PORTC))
#define O_SW02_PORT (&(PORTC))
#define O_SW03_PORT (&(PORTC))
#define O_SW04_PORT (&(PORTB))
#define O_SW05_PORT (&(PORTB))
#define O_SW06_PORT (&(PORTB))
#define O_SW07_PORT (&(PORTD))
#define O_SW08_PORT (&(PORTD))
#define O_SW09_PORT (&(PORTD))
#define O_SW0A_PORT (&(PORTD))
#define O_SW0B_PORT (&(PORTD))

// PINs - Bitmaske
#define O_SW00_PIN (_BV(PC3)) //SWL_KUECHE
#define O_SW01_PIN (_BV(PC2)) //SWA_BEAMER
#define O_SW02_PIN (_BV(PC1)) //SWL_VORTRAG
#define O_SW03_PIN (_BV(PC0)) //SWL_LOUNGE
#define O_SW04_PIN (_BV(PB5)) //SWA_STECKDOSEN
#define O_SW05_PIN (_BV(PB4)) //SWA_KLO
#define O_SW06_PIN (_BV(PB0)) //SWA_HS
#define O_SW07_PIN (_BV(PD7)) //SWA_FREE
#define O_SW08_PIN (_BV(PD0)) //SWL_TAFEL
#define O_SW09_PIN (_BV(PD1)) //SWL_BEAMER
#define O_SW0A_PIN (_BV(PD2)) //SWL_SCHRANK
#define O_SW0B_PIN (_BV(PD4)) //SWL_FLIPPER

// Anzahl der Schaltausgänge
#define NUM_OUTPUTS 12


// PWM Kanal zuordnung
// Die Zahlen sind indexes für die PWM-Ausgangs Tabelle, die mit den
// O_PWM0X_PORT werten unten gefüllt wird.

#define PWM_TAFEL    (0x00)
#define PWM_BEAMER   (0x01)
#define PWM_SCHRANK  (0x02)
#define PWM_FLIPPER  (0x03)
#define PWM_LOUNGE   (0x04)
#define PWM_KUECHE   (0x05)


#define O_PWM00_PORT (&(OCR0A))
#define O_PWM01_PORT (&(OCR0B))
#define O_PWM02_PORT (&(OCR1AL))
#define O_PWM03_PORT (&(OCR1BL))
#define O_PWM04_PORT (&(OCR2A))
#define O_PWM05_PORT (&(OCR2B))

// Anzahl der PWM-Kanäle
#define PWM_CHAN 6


// Datenstruktur, in der die aktuellen Ausgangszustände gespeichert werden können
// Die Zustände werden auch in dieser Form über I2C verschickt. 

typedef struct {
	uint16_t ports; //Zustände der Schaltusgänge als Bitmaske
	uint8_t pwmval[PWM_CHAN];
} t_outputdata;

#endif
