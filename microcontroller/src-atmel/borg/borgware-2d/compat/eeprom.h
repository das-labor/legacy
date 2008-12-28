
//EEPPROM compatibility support for simulator

#ifdef AVR
	#include <avr/eeprom.h>
#else
	#include <stdint.h>
	
	void 	eeprom_write_byte (uint8_t *p, uint8_t value);
	void 	eeprom_write_word (uint16_t *p, uint16_t value);
	
	uint8_t  eeprom_read_byte (const uint8_t *p);
	uint16_t eeprom_read_word (const uint16_t *p);
	
	#define eeprom_busy_wait()
	#define 	EEMEM   __attribute__((section(".eeprom")))
	
#endif
