/*#################################################################################################
	Title	: TWI SLave
	Author	: Martin Junghans <jtronics@gmx.de>
	Hompage	: www.jtronics.de
	Software: AVR-GCC / Programmers Notpad 2
	License	: GNU General Public License 
	
	Aufgabe	:
	Betrieb eines AVRs mit Hardware-TWI-Schnittstelle als Slave. 
	Zu Beginn muss init_twi_slave mit der gewünschten Slave-Adresse als Parameter aufgerufen werden. 
	Der Datenaustausch mit dem Master erfolgt über die Buffer rxbuffer und txbuffer, auf die von Master und Slave zugegriffen werden kann. 
	rxbuffer und txbuffer sind globale Variablen (Array aus uint8_t).
	
	Ablauf:
	Die Ansteuerung des rxbuffers, in den der Master schreiben kann, erfolgt ähnlich wie bei einem normalen I2C-EEPROM.
	Man sendet zunächst die Bufferposition, an die man schreiben will, und dann die Daten. Die Bufferposition wird 
	automatisch hochgezählt, sodass man mehrere Datenbytes hintereinander schreiben kann, ohne jedesmal
	die Bufferadresse zu schreiben.
	Um den txbuffer vom Master aus zu lesen, überträgt man zunächst in einem Schreibzugriff die gewünschte Bufferposition und
	liest dann nach einem repeated start die Daten aus. Die Bufferposition wird automatisch hochgezählt, sodass man mehrere
	Datenbytes hintereinander lesen kann, ohne jedesmal die Bufferposition zu schreiben.

	Abgefangene Fehlbedienung durch den Master:
	- Lesen über die Grenze des txbuffers hinaus
	- Schreiben über die Grenzen des rxbuffers hinaus
	- Angabe einer ungültigen Schreib/Lese-Adresse
	- Lesezuggriff, ohne vorher Leseadresse geschrieben zu haben
	
	LICENSE:
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

//#################################################################################################*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "twislave.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//###################### Slave-Adresse
#define SLAVE_ADRESSE 0x50 								// Slave-Adresse

//###################### Macros
#define uniq(LOW,HEIGHT)	((HEIGHT << 8)|LOW)			// 2x 8Bit 	--> 16Bit
#define LOW_BYTE(x)        	(x & 0xff)					// 16Bit 	--> 8Bit
#define HIGH_BYTE(x)       	((x >> 8) & 0xff)			// 16Bit 	--> 8Bit


#define sbi(ADDRESS,BIT) 	((ADDRESS) |= (1<<(BIT)))	// set Bit
#define cbi(ADDRESS,BIT) 	((ADDRESS) &= ~(1<<(BIT)))	// clear Bit
#define	toggle(ADDRESS,BIT)	((ADDRESS) ^= (1<<BIT))		// Bit umschalten

#define	bis(ADDRESS,BIT)	(ADDRESS & (1<<BIT))		// bit is set?
#define	bic(ADDRESS,BIT)	(!(ADDRESS & (1<<BIT)))		// bit is clear?


//###################### Variablen
	uint16_t 	Variable=2345;				//Zähler
	uint16_t	buffer;
	uint16_t	low, hight;

//################################################################################################### Initialisierung
void Initialisierung(void)
	{
	cli();
	//### PORTS	
	
	//### TWI 
	init_twi_slave(SLAVE_ADRESSE);			//TWI als Slave mit Adresse slaveadr starten 
	
	sei();
	}

//################################################################################################### Hauptprogramm
int main(void)
{	
 
Initialisierung();

while(1)
    {
	//############################ write Data in txbuffer
	
	// 8Bit variable
	txbuffer[2]=3;	
	txbuffer[3]=4;		
	txbuffer[4]=5;	
	txbuffer[5]=6;	
	
	// 16Bit Variable --> 2x 8Bit Variable
	buffer		= Variable;
	txbuffer[0]	= LOW_BYTE(buffer);			//16bit --> 8bit
	txbuffer[1]	= HIGH_BYTE(buffer);		//16bit --> 8bit
	

	//############################ read Data form rxbuffer

	// 8Bit variable
	Variable	= rxbuffer[2];
	
	// 2x 8Bit Variable -->16Bit Variable
	low			= rxbuffer[0];
	hight		= rxbuffer[1];
	Variable	= uniq(low,hight);			// 2x 8Bit  --> 16Bit
	
	
	//############################
	} //end.while
} //end.main



