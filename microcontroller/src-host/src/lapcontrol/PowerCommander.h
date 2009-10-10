/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>

#define I2CADRESSE 15 // I2C Adresse

#define HASDATA (0x01)
#define HASNDATA (0x00)

// der schalter generiert $Anzahl von events pro sekunde

// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

/* 
   Actionen als defines
*/
#define F_SW_OFF      (0x00)
#define F_SW_ON       (0x01)
#define F_SW_STATUS   (0x02)

#define F_PWM_SET     (0x00)
#define F_PWM_GET     (0x01)


#define F_NDEF (0xFF)

#define D_NDEF (0x00)

/*
	funktionsklassen - switch oder PWMs
*/
#define C_SW  (0x00)
#define C_PWM (0x01)
#define C_VIRT (0x02)
#define C_NDEF (0xFF)

/*
  schalter Objekte als defines
*/
#define O_SW00 (0x00)
#define O_SW01 (0x01)
#define O_SW02 (0x02)
#define O_SW03 (0x03)
#define O_SW04 (0x04)
#define O_SW05 (0x05)
#define O_SW06 (0x06)
#define O_SW07 (0x07)
#define O_SW08 (0x08)
#define O_SW09 (0x09)
#define O_SW10 (0x0A)
#define O_SW11 (0x0B)
#define O_SW12 (0x0C)

#define O_NDEF (0xFF)

// switch lampe
#define SWL_TAFEL       (O_SW08)
#define SWL_BEAMER      (O_SW09)
#define SWL_SCHRANK     (O_SW10)
#define SWL_FLIPPER     (O_SW11)
#define SWL_LOUNGE      (O_SW03)
#define SWL_KLO         (O_SW05)
#define SWL_KUECHE      (O_SW00)
#define SWL_VORTRAG     (O_SW02)

// switch andere
// Hauptschuetz
#define SWA_HS          (O_SW06)
// beamer anaus
#define SWA_BEAMER      (O_SW01)
// freier schalter
#define SWA_FREE        (O_SW07)
// tischsteckdose
#define SWA_STECKDOSEN  (O_SW04)
// Opto 230V Hauptschuetze
#define SWA_230Haupt    (O_SW12)

// PWMs als Objekte
#define O_PWM00 (0x00)
#define O_PWM01 (0x01)
#define O_PWM02 (0x02)
#define O_PWM03 (0x03)
#define O_PWM04 (0x04)
#define O_PWM05 (0x05)
#define O_PWM_NDEF (0xFF)

#define O_VIRT00 (0x00)

#define VIRT_POWER (O_VIRT00)

#define PWM_TAFEL    (O_PWM00)
#define PWM_BEAMER   (O_PWM01)
#define PWM_SCHRANK  (O_PWM02)
#define PWM_FLIPPER  (O_PWM03)
#define PWM_LOUNGE   (O_PWM04)
#define PWM_KUECHE   (O_PWM05)


#endif
