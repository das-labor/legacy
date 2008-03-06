
#include <avr/io.h>

extern uint16_t bus_read_helper();
extern void bus_write_helper();

#define PORT_DATA PORTC

uint8_t bus_read(uint16_t addr){
	SPDR = addr>>8;
	while((SPSR & (1<<SPIF)) == 0);
	SPDR = (uint8_t)addr;
	while((SPSR & (1<<SPIF)) == 0);
	
	return bus_read_helper() >> 7 ;
}



void bus_write(uint16_t addr, uint8_t data){
	SPDR = addr>>8;
	while((SPSR & (1<<SPIF)) == 0);
	SPDR = (uint8_t)addr;
	while((SPSR & (1<<SPIF)) == 0);

	PORT_DATA = data>>1;
	PORTD = data<<7;
	
	bus_write_helper();
	PORT_DATA = 0xff;
	PORTD |= 0x80;
}


void spi_init(){
	
	DDRB |= 0xb0; //MOSI, SCK and SS to output
	
	//DORD = 1 -> LSB first
	SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA);


}
