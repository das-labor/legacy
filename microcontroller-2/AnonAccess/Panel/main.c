#include "config.h"
#include "lcd_tools.h"
#include "keypad.h"
#include "spi.h"
#include "uart.h"
#include "menu.h"
#include "lop.h"
#include "i2c.h"
#include "i2c_tools.h"
#include "24Cxxx.h"
#include "hwrnd.h"
#include "lop_debug.h"
#include "rtc.h"
#include <stdint.h>
//Variablen und Konstanten
uint8_t ROW_SIZE=0;


lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};

/******************************************************************************/

// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	//let lop handle the received byte.
	lop_recieve_byte(&lop0,b);
}

/******************************************************************************/

void sendportmsg(uint8_t b){
}

void lop0_streamsync(void){	
}

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

void lop0_messagerx(uint16_t length, uint8_t * msg){
	
}

void lop0_streamrx(uint8_t b){
	
}

/****************************************************
 *  eeprom_dump_page()
 * **************************************************/
void eeprom_dump_page(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	uart_putstr("EEPROM-Dump (Page-Mode):\r\n"); 
	for (i=start; i<(start+length-ROW_SIZE); i+=ROW_SIZE){
		E24C_block_read(dev, i, buffer, ROW_SIZE);
//		uart_putstr("0x");
//		uart_putbyte(HIGH(i));
//		uart_putbyte(LOW(i));
//		uart_putstr(":");
		for (j=0; j<ROW_SIZE; ++j){
//			uart_putc(' ');
//			uart_putbyte(buffer[j]);
		}
//		uart_putc('\t');
		for (j=0; j<ROW_SIZE; ++j){
//			uart_putc((buffer[j]<32)?'.':buffer[j]);
		}
//		uart_putstr("\r\n");
	}
	E24C_block_read(dev, i, buffer, (start+length-i));
//	uart_putstr("0x");
//	uart_putbyte(HIGH(i));
//	uart_putbyte(LOW(i));
//	uart_putstr(":");
	for (j=0; j<ROW_SIZE; ++j){
//		uart_putc(' ');
		if (j<(start+length-i)){
//			uart_putbyte(buffer[j]);
		} else {
			uart_putc(' ');
			uart_putc(' ');
		} 
	}
//	uart_putc('\t');
	for (j=0; j<start+length-i; ++j){
//		uart_putc((buffer[j]<32)?'.':buffer[j]);
	}
//	uart_putstr("\r\n");
	
}

int main(void){

	//Initialisierung
	lcd_init();
	keypad_init();	
	uart_init();
	spi_init();
	i2c_init();
    E24C_init();
 	rtc_init();
 	
 	prng_init();
 
	//Set I2C SPEED 
	i2c_set_speed(I2C_SPEED_FASTEST);
	
	//Timer 2 initialisieren
	TCCR2 = 0x0D;
	TCNT2 = 0;
	OCR2  = 0xF0;

	// Timer 2 Interrupt
	//TIMSK= _BV(OCIE2);
	
	//set handlers for the outputs from lop
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	uart_hook = onuartrx;
	
	//Prog
	lcd_gotopos (1,1);
	lcd_writetext ("Panel Test");

	//Interupts global aktivieren
	sei();
	
	lop_dbg_str(&lop0,"\r\nMAIN\r\n");
	
	
	while(1){
		master_menu();
	}
	
	return 0;
} 

/*
// Interupt fuer Timer 2 (abfragen des Keypads und Menuhandler)
ISR(TIMER2_COMP_vect)
{
	akey = read_keypad();
	
	// Debug
	lcd_gotopos (4,19);
	lcd_writechar (akey);

}
*/

