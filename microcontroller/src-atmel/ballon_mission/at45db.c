
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "spi.h"

#define CLEAR_CS() PORTB &= ~(1<<PB4)
#define SET_CS() PORTB |= (1<<PB4)



uint8_t df_memory_status_read(){
	uint8_t retval;
	CLEAR_CS();
	spi_data(0x57);
	retval =  spi_data(0);
	SET_CS();
	return retval;
}


void df_busy_wait(){
	while(!(df_memory_status_read() & 0x80));
}


void df_memory_page_read(uint8_t * buffer, uint16_t page){
	df_busy_wait();
	
	CLEAR_CS();
	spi_data(0x52);
	spi_data(page >> 7);
	spi_data((page<<1) & 0xff);
	spi_data(0);
	spi_data(0);
	spi_data(0);
	spi_data(0);
	spi_data(0);
	
	int x;
	for(x=0; x<256; x++){
		buffer[x] = spi_data(0);
	}
	SET_CS();
}


void df_memory_page_programm(uint8_t * buffer, uint16_t page){
	df_busy_wait();
	
    CLEAR_CS();
	spi_data(0x82);
	spi_data(page >> 7);
	spi_data((page<<1) & 0xff);
	spi_data(0);
	int x;
	for(x=0; x<256; x++){
		spi_data(buffer[x]);
	}
	SET_CS();
}


void df_erase(){
	/*
	
	uint16_t i;
	
	uint8_t buf[256];
	for(i=0;i<256;i++){
		buf[i] = 0xff;
	}
	
	for(i=0;i<2048;i++){
		buzz(1000);
		_delay_ms(100);
		buzz(0);
		df_memory_page_programm(buf, i);
	}
	*/
}


void df_init(void){
   cli();

   spi_init();                              /* Initialize SPI */
   /* Reset the device */
   //RST();
   CLEAR_CS();

   /* Skip any pending bits */
   unsigned char i;
   for(i = 0; i < 20; i++)
      spi_data(0x00);

   #ifdef WDT_ENABLE
      wdt_reset();                        /* Watchdog reset */
   #endif

   _delay_us(20);
   SET_CS();
   _delay_us(1);

   sei();
}
