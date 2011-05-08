/*
 * Copyright:      
 * Author:         Patrick Rudolph
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.0
 * Description:    EEPROM libary for Atmegas
 * Modified:	   
 */

#include <inttypes.h>
#include <avr/eeprom.h>

#ifndef	ATMEGAEEPROM_H
#define ATMEGAEEPROM_H

// EEMEM wird bei aktuellen Versionen der avr-lib in eeprom.h definiert
// hier: definiere falls noch nicht bekannt ("alte" avr-libc)
#ifndef EEMEM
// alle Textstellen EEMEM im Quellcode durch __attribute__ ... ersetzen
#define EEMEM  __attribute__ ((section (".eeprom")))
#endif

//define your struct here
typedef struct {
    uint8_t   label[8];
    uint8_t   rom_code[8];
} eeprom_t;

eeprom_t EEMEM eeprom_status=1;
eeprom_t EEMEM eeprom_struct;

#endif