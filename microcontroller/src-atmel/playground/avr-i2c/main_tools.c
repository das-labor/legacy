

#include "config.h"
#include "debug.h"
#include "uart/uart.h"
#include <inttypes.h>
#include <avr/io.h>
//#include <util/delay.h>
#include <stdio.h> // itoa()
#include "24Cxxx.h"
#include "i2c_tools.h"
#include "main_tools.h"

#include "mcp9800.h"

/****************************************************
 ****************************************************
 *  FUNCTIONS
 ****************************************************
 ****************************************************/

/****************************************************
 *  main_tools_init()
 * **************************************************/
void main_tools_init(){
    i2c_init();
    E24C_init();
}


/****************************************************
 *  console_dbg()
 * **************************************************/
void console_dbg(){
	char c;
	uart_putstr("\r\n*** console debug ***\r\npress 'q' to leave\r\n");
	do {
		c = uart_getc();
		uart_putstr("Got char: 0x");
		uart_putbyte(c);
		uart_putstr(" (");
		if(c>=32){
			uart_putc(c);
		} else {
			uart_putstr("<32");
		}
		uart_putstr(" )\r\n");
	} while (c != 'q');
}


/****************************************************
 *  uart_putbyte()
 * **************************************************/
void uart_putbyte(uint8_t b){
	char table[]={'0','1','2','3','4','5','6','7',
				  '8','9','A','B','C','D','E','F'};
	uart_putc(table[b>>4]);
	uart_putc(table[b&0xF]);
}


/****************************************************
 *  i2c_detect()
 * **************************************************/
void i2c_detect(i2c_detect_t table){
	uint8_t j;
	for(j=0; j<32; ++j)
		table[j]=0;
	uart_putstr("Detecting devices:\r\n");
	for (j=0; j<254; j+=2){
		
		uart_putstr("Checking: ");
		uart_putbyte(j);
		
		uint8_t state;
		if ((state=i2c_start(j+I2C_READ)&0xF8) == 0x40){
			uart_putstr("\t");
			uart_putbyte(j);
			uart_putstr(" READ");
			table[(j+I2C_READ)>>3] |= 1<<((j+I2C_READ)&0x7);
		}
		if ((state=i2c_start(j+I2C_WRITE)&0xF8) == 0x18){
			uart_putstr("\t");
			uart_putbyte(j);
			uart_putstr(" WRITE");
			table[(j+I2C_WRITE)>>3] |= 1<<((j+I2C_WRITE)&0x7);
		}
		
		if(!((table[(j+I2C_READ)>>3]&(1<<((j+I2C_READ)&0x7)))||
			(table[(j+I2C_WRITE)>>3]&(1<<((j+I2C_WRITE)&0x7)))) ){
			uart_putstr(TERM_LN_ERASE"\r");
		} else {
			uart_putstr("\r\n");
		}
	}	
	i2c_stop();
}


/****************************************************
 *  eeprom_dump_byte()
 * **************************************************/
void eeprom_dump_byte(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
    uint8_t j=0;
    uart_putstr("EEPROM-Dump (Byte-Mode):\r\n");
    for (i=start; i<(start+length); i+=ROW_SIZE){
	    uart_putstr("0x");
	    uart_putbyte(HIGH(i));
	    uart_putbyte(LOW(i));
	    uart_putstr(":");
	    for (j=0; j<ROW_SIZE; ++j){
	   // fix this
	    		uart_putc(' ');
			uart_putbyte(E24C_byte_read(dev, i+j));
	    }
	    uart_putstr("\r\n");
    } 
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
		uart_putstr("0x");
		uart_putbyte(HIGH(i));
		uart_putbyte(LOW(i));
		uart_putstr(":");
		for (j=0; j<ROW_SIZE; ++j){
			uart_putc(' ');
			uart_putbyte(buffer[j]);
		}
		uart_putc('\t');
		for (j=0; j<ROW_SIZE; ++j){
			uart_putc((buffer[j]<32)?'.':buffer[j]);
		}
		uart_putstr("\r\n");
	}
	E24C_block_read(dev, i, buffer, (start+length-i));
	uart_putstr("0x");
	uart_putbyte(HIGH(i));
	uart_putbyte(LOW(i));
	uart_putstr(":");
	for (j=0; j<ROW_SIZE; ++j){
		uart_putc(' ');
		if (j<(start+length-i)){
			uart_putbyte(buffer[j]);
		} else {
			uart_putc(' ');
			uart_putc(' ');
		} 
	}
	uart_putc('\t');
	for (j=0; j<start+length-i; ++j){
		uart_putc((buffer[j]<32)?'.':buffer[j]);
	}
	uart_putstr("\r\n");
	
}


/****************************************************
 *  eeprom_set_byte()
 * **************************************************/
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;	
	uart_putstr("EEPROM-Setting:\r\n");
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			uart_putbyte(buffer[j]=LOW((i+j)));
            	uart_putc(' ');	
        		E24C_byte_write(dev, i+j, buffer[j]);
         	i2c_ack_poll(dev+I2C_WRITE);
            }
        uart_putstr("\r\n");  
    }
}


/****************************************************
 *  eeprom_set_page()
 * **************************************************/
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	uart_putstr("EEPROM-Setting:\r\n");
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			uart_putbyte(buffer[j]=LOW((i+j)));
            	uart_putc(' ');	
        		}
        uart_putstr("\r\n");  
        E24C_page_write(dev, i, buffer, ROW_SIZE);
		i2c_ack_poll(dev+I2C_WRITE);
    }
}




/****************************************************
 *  thermo_dump()
 * **************************************************/
void thermo_dump(i2c_addr_t dev){
	char c, tempstr[20];
	mcp9800_temp_t temp, temp_old;
	uart_putstr("\r\n*** Thermo dump ***\n\r");
	uart_putstr("Config-Reg: 0x");
	uart_putbyte(i2c_get_8bit_reg(dev,conf_reg));
	uart_putstr("\r\n");
	uart_putstr("Stetting 12-bit resolution.\r\n");
	mcp9800_set_resolution(dev,mcp9800_12bit);
	while (!uart_getc_nb(&c)){	
		temp_old = mcp9800_get_temp(dev);
		uart_putstr("temperature: 0x");
		uart_putbyte(HIGH(temp));
		uart_putstr(".0x");
		uart_putbyte(LOW(temp));
		uart_putstr(" (");
		uart_putstr(itoa((int)HIGH(temp),tempstr, 10));
		uart_putstr(".");
		if (LOW(temp)<100)
			uart_putc('0');
		if (LOW(temp)<10)
			uart_putc('0');
		uart_putstr(utoa((int)LOW(temp),tempstr, 10));
		uart_putstr(")");
		do{
			temp = mcp9800_get_temp(dev);
		}while(temp==temp_old);
		//uart_putstr(TERM_LN_ERASE"\r");
		uart_putc('\r');
	}
	
	uart_putstr("\r\n");
}
