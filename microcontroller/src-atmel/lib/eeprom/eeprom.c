/*
 * Copyright:      
 * Author:         Patrick Rudolph mailto:siro@das-labor.org
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        0.1
 * Description:    EEPROM libary for Atmegas
 * Modified:	   
 */

#include <avr/eeprom.h>
#include "eeprom.h"

uint8_t EEMEM eeprom_status = 1;
eeprom_t EEMEM eeprom_struct ;

//saves given struct to eeprom memory 
void eeprom_write(eeprom_t* s)
{
	eeprom_write_block(s,&eeprom_struct,sizeof(eeprom_t));	//write s to eeprom_struct in eeprom
	if(!(eeprom_read_byte(&eeprom_status) == 0xAA))
		eeprom_write_byte(&eeprom_status, 0xAA);
}

//loads struct from eeprom memory to given pointer
uint8_t eeprom_read(eeprom_t* s)
{
	if(!(eeprom_read_byte(&eeprom_status) == 0xAA))
		return 0;
	eeprom_read_block(s,&eeprom_struct,sizeof(eeprom_t));
	return 1;
}




