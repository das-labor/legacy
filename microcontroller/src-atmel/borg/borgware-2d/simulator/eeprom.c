
//EEPPROM compatibility support for simulator

#include <stdint.h>
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>

#define EEPROM_SIZE 1024

static FILE * fp;

static uint8_t eemem[EEPROM_SIZE];

static uint8_t inited;
static void init(){
	if(!inited){
		inited = 1;
		char* filename = ".simulated_eeprom.bin"; 
		size_t size;
		fp = fopen(filename, "r+");
		if(fp == 0){
			fp = fopen(filename, "w+");
			if(fp == 0){
				printf("Failed to open %s\n",filename );
				exit (1);
			}
		}
		size = fread (eemem, 1, EEPROM_SIZE, fp);
		if(size < EEPROM_SIZE){
			memset(eemem, 0xff, EEPROM_SIZE);
		}
	}
}

extern uint8_t _eeprom_start__[];

uint16_t conv_addr(uint8_t * p){
	uint16_t addr;
	addr = (unsigned int)p - (unsigned int)_eeprom_start__;
	if(addr >= EEPROM_SIZE){
		printf ("warning: eeprom write to %X\n",addr);
	}
	addr &= (EEPROM_SIZE-1);
	return addr;
}

void 	eeprom_write_byte (uint8_t *p, uint8_t value){
	printf("sim eeprom write [%04X]=%02X\n", conv_addr(p), value);
	init();
	eemem[conv_addr(p)] = value;
	fseek(fp, 0, SEEK_SET);
	fwrite(eemem, 1, EEPROM_SIZE, fp);
}

void 	eeprom_write_word (uint16_t *p, uint16_t value){
	printf("sim eeprom write [%04X]=%04X\n", conv_addr(p), value);

	init();
	eemem[conv_addr((uint8_t*)p)  ] = value & 0xff;
	eemem[conv_addr((uint8_t*)p)+1] = value >> 8;
	
	fseek(fp, 0, SEEK_SET);
	fwrite(eemem, 1, EEPROM_SIZE, fp);
	fflush(fp);
}

uint8_t  eeprom_read_byte (uint8_t *p){
	init();
	return eemem[conv_addr(p)];
}

uint16_t eeprom_read_word (uint16_t *p){
	init();
	return eemem[conv_addr((uint8_t*)p)] | (eemem[conv_addr((uint8_t*)p)+1]<<8);
}
