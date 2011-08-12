
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart/uart.h"

#include "ioport.h"

#include "at45db.h"

#define BUZZER_PORT D
#define BUZZER_BIT  4

void init_timer1(){
	//TCCR1A = (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
	TCCR1A =               (1<<WGM11) | (1<<WGM10);
	TCCR1B = (1<<WGM13)|(1<<WGM12)| 2; //clk/8, TOP = OCR1A
	OCR1A  = 2000;
	SET_DDR(BUZZER);
}

void buzz(int freq){
	if(freq){
		TCCR1A = (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
		uint16_t w = 2000000ul / freq;
		OCR1A = w;
		OCR1B = w/8;
	}else{
		TCCR1A =               (1<<WGM11) | (1<<WGM10);
	}
}


//Timer 2 as 4 ms tick timer
void init_timer2(){
	TCCR2 = (1<<WGM21) | 6; //CTC, clk/256
	OCR2 = 250;
	TIMSK |= (1<<OCIE2); //Compare interrupt
}

volatile uint16_t time_ms;
volatile uint16_t timer_seconds;

ISR(TIMER2_COMP_vect){
	uint16_t ms = time_ms;
	ms += 4;
	if(ms >= 1000){
		ms = 0;
		timer_seconds++;
	}
	time_ms = ms;
}


uint32_t current_seconds;
uint32_t last_seconds;

void update_seconds(){
	last_seconds = current_seconds;
	cli();
	current_seconds = timer_seconds;
	sei();
}

//return true if a time has passed since last main loop
uint8_t time_passed(uint32_t val){
	if((current_seconds >= val) && (last_seconds < val)){
		return 1;
	}else{
		return 0;
	}
}


void statemachine(){
	if(time_passed(5)){
		buzz(1500);
	}
	if(time_passed(6)){
		buzz(0);
	}
}


int my_putc(char c, FILE * fp){
	uart_putc(c);
	return 0;
}


char test_buf[] = "Test 12345 Hallo foobar 2000";

int main(){
	init_timer1();
	init_timer2();

	uart_init();
	fdevopen (my_putc, 0);
	
	sei();
	
	printf("Hello World\r\n");
	
	df_init();
	
	//while(1){
		uint8_t x =	df_memory_status_read();
		printf("%X\r\n", x);
	//}

	printf("init done\r\n");
	
	uint8_t buf[256];
	uint16_t i;
	for(i=0;i<4;i++){
		df_memory_page_read(buf, i);
		uart_hexdump(buf, 256);
	}
	
	df_memory_page_programm(test_buf, 0);
	
	
	while(1){
		update_seconds();
		statemachine();	
	}
}
