/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>

#define I2CADRESSE 15 // I2C Adresse
#define POWERCMD_IFACE_ADDR (0x02)
#define POWERCMD_IFACE_PORT (0x01)
#define POWERCMD_SENDER_ADDR (0x00)
#define POWERCMD_SENDER_PORT (0x00)


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
#define SWL_KUECHE      (O_SW00)
#define SWL_VORTRAG     (O_SW02)

// switch andere
// beamer anaus
#define SWA_BEAMER      (O_SW01)
// tischsteckdose
#define SWA_STECKDOSEN  (O_SW04)
// Toilette Lich + Steckdosen
#define SWA_KLO         (O_SW05)
// Hauptschuetz
#define SWA_HS          (O_SW06)
// freier schalter
#define SWA_FREE        (O_SW07)

// PWMs als Objekte
#define O_PWM00 (0x00)
#define O_PWM01 (0x01)
#define O_PWM02 (0x02)
#define O_PWM03 (0x03)
#define O_PWM04 (0x04)
#define O_PWM05 (0x05)
#define O_PWM_NDEF (0xFF)

#define O_VIRT00 (0x00)
#define O_VIRT01 (0x01)
#define O_VIRT02 (0x02)

#define VIRT_POWER (O_VIRT00)
#define VIRT_VORTRAG (O_VIRT01)
#define VIRT_VORTRAG_PWM (O_VIRT02)

#define PWM_TAFEL    (O_PWM00)
#define PWM_BEAMER   (O_PWM01)
#define PWM_SCHRANK  (O_PWM02)
#define PWM_FLIPPER  (O_PWM03)
#define PWM_LOUNGE   (O_PWM04)
#define PWM_KUECHE   (O_PWM05)


// ports - addressen
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
#define O_SW0B_PORT (&(PORTB))
#define O_SW0C_PORT (&(PORTD))

// PINs - uin8_t
#define O_SW00_PIN (PC3)
#define O_SW01_PIN (PC2)
#define O_SW02_PIN (PC1)
#define O_SW03_PIN (PC0)
#define O_SW04_PIN (PB5)
#define O_SW05_PIN (PB4)
#define O_SW06_PIN (PB0)
#define O_SW07_PIN (PD7)
#define O_SW08_PIN (PD0)
#define O_SW09_PIN (PD1)
#define O_SW0A_PIN (PD2)
#define O_SW0B_PIN (PB7)
#define O_SW0C_PIN (PD4)

#define O_PWM00_PORT (&(OCR0A))
#define O_PWM01_PORT (&(OCR0B))
#define O_PWM02_PORT (&(OCR1AL))
#define O_PWM03_PORT (&(OCR1BL))
#define O_PWM04_PORT (&(OCR2A))
#define O_PWM05_PORT (&(OCR2B))

// Anordnen von Schaltern in einer Matrix
// so, dass wird die mittels i++ durchgehen koennen


struct t_i2cproto {
	uint8_t class;
	uint8_t object;
	uint8_t fkt;
	uint8_t has_in_data;
	uint8_t has_out_data;
	uint8_t in_data;
	uint8_t out_data;
};


#endif
