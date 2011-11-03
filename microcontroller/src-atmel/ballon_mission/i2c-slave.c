#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdint.h>
//#include "uart.h"

#define TWI_SLAVE_ADR  4
#define SEND_BUF_SIZE  8
#define RECV_BUF_SIZE 12

volatile uint8_t i2c_send_buf[SEND_BUF_SIZE];
volatile uint8_t i2c_recv_buf[RECV_BUF_SIZE];

volatile static uint8_t send_idx, recv_idx, twi_state, twi_retrys, enter_isr;

volatile uint8_t i2c_data_updated;

void i2c_init() {
	TWAR = TWI_SLAVE_ADR;
	TWSR = 0;
	TWBR = 0; 
	TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
	//DDRC = _BV(6);		
	//sei();
}


ISR(TWI_vect) {
	enter_isr = 1;
	switch (TWSR & 0xF8) {
		
		case TW_ST_SLA_ACK:
			send_idx = 0;
			//TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			break;
			//uart_putc('t');
		case TW_ST_DATA_ACK:
		case TW_ST_LAST_DATA:
			//uart_putc(nib2hex[send_idx & 0xf]);
			//uart_putstr("st\n");
			if (send_idx < SEND_BUF_SIZE)
				TWDR = i2c_send_buf[send_idx];
				
			send_idx++;
			//if (send_idx < SEND_BUF_SIZE)
			//	TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			//else
			//	TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			break;
			
		case TW_SR_SLA_ACK:
			recv_idx = 0;
			//TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			break;
			//uart_putc('r');
		case TW_SR_DATA_ACK:
			//uart_putc(nib2hex[recv_idx & 0xf]);
			//uart_putstr("sr\n");
			if (recv_idx < RECV_BUF_SIZE) {
				i2c_recv_buf[recv_idx] = TWDR;
				recv_idx++;
			} else {
				i2c_data_updated = 1;
				TWDR;
			}
			
			//if (recv_idx < RECV_BUF_SIZE)	 
			//	TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			//else	 
			//	TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); // next byte
			
			break;
			
		case TW_ST_DATA_NACK:
			//uart_putc('n');
			//TWDR;
			//TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN) | _BV(TWIE); 
			break;		
			
		case TW_SR_STOP:
			//uart_putc('a');
			//TWDR;
			
			break;
		
		default:
			//uart_putc(nib2hex[TWSR >> 4]);
			//uart_putc(nib2hex[TWSR & 0x0f]);
			//uart_putstr(" error\n");
			TWDR; 
			//TWCR = _BV(TWEA) | _BV(TWIE) | _BV(TWEN) | _BV(TWINT); // next byte
			break;
	}
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA) | _BV(TWIE); 
}


/*

uint8_t nib2hex[16] = "0123456789abcdef"; 

static void PrintSignedShortFormated(signed short value) {
    unsigned char sBuf0[7];
    unsigned char *sBuf = &sBuf0[6];
    unsigned char  minus;
    unsigned short valueAlt;   // Alte Wert f<FC>r schneller % 10 Berechnung
    unsigned short valueCalc;  // muss unsigned weil signed short kann kein 32768
    
    *sBuf = 0;       // String Terminierung
    if (value < 0) { // Vorzeichen behandeln
      minus = '-';
      valueCalc = -value;
    } else {
      minus = ' ';
      valueCalc = value;
    }
    if (!valueCalc) 
      *--sBuf = '0';
    else while (valueCalc) {
      valueAlt = valueCalc;
      valueCalc /= 10;
      //valueCalc = div10(valueCalc); 
      // schneller als  *--sBuf = '0' + (valueAlt % 10)
      *--sBuf = '0' + valueAlt - (valueCalc * 10); 
    } 
    *--sBuf = minus;                  // Minuszeichen vor die h<F6>chstwertigste Stelle setzen
    while (sBuf > sBuf0)              // mit Leerzeichen auff<FC>llen (f<FC>r Formatierung)  
      *--sBuf = ' ';
    uart_putstr(sBuf0); // String ausgeben
}
*/

/*
void main_foo() {
	uint8_t i;
	uart_init();
	uart_putstr("I2C Slave Test\n");
	i2c_init();
	while (1) {	
		i2c_send_buf[0] = 0x12;
		i2c_send_buf[1] = 0x34;
		i2c_send_buf[2] = 0x56;
		i2c_send_buf[3] = 0x78;
		if (enter_isr) {
			uart_putstr(" Com active  recvied ");
			uart_hexdump(i2c_recv_buf, RECV_BUF_SIZE);
			//PrintSignedShortFormated(((uint16_t) i2c_recv_buf[2] << 8) | i2c_recv_buf[3]);
			//PrintSignedShortFormated(((uint16_t) i2c_recv_buf[6] << 8) | i2c_recv_buf[7]);
			uart_putstr("\n");
			//for (i = 0; i < RECV_BUF_SIZE; i++) {
			//	i2c_recv_buf[i] = 0;
			//}
			//TWCR = _BV(TWEA) | _BV(TWIE) | _BV(TWEN) | _BV(TWINT);
			//TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
			enter_isr = 0;
		} else {
			TWCR = 0; // reset
			TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
		}
		_delay_ms(100);
	}
}
*/

