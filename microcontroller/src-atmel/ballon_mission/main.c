
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "logger.h"

#include "uart/uart.h"

#include "ioport.h"

#include "at45db.h"

#include "i2c-slave.h"

#define BUZZER_PORT D
#define BUZZER_BIT  4

#define PYRO_FIRE_PORT D
#define PYRO_FIRE_BIT  5

#define PYRO_TEST_PORT D
#define PYRO_TEST_BIT  6

#define CAM_BUTTON_PORT C
#define CAM_BUTTON_BIT  3

#define CAM_POWER_PORT C
#define CAM_POWER_BIT  2

//2 Stunden 10 min Pyro
#define PYRO_TIME  7800
//#define PYRO_TIME  30

//20 minuten pro video
#define CAM_TIME   1200

//3 Stunden bis der piezo trötet
#define PIEZO_TIME 10800


#define RXD_PIN_PORT D
#define RXD_PIN_BIT  0



void init_buzzer(){
	//TCCR1A = (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
	//TCCR1A =               (1<<WGM11) | (1<<WGM10);
	//TCCR1B = (1<<WGM13)|(1<<WGM12)| 2; //clk/8, TOP = OCR1A
	//OCR1A  = 2000;
	SET_DDR(BUZZER);
}

void buzz(int freq){
	if(freq){
		//TCCR1A = (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
		//uint16_t w = 2000000ul / freq;
		//OCR1A = w;
		//OCR1B = w/4;
		OUTPUT_ON(BUZZER);
	}else{
		//TCCR1A =               (1<<WGM11) | (1<<WGM10);
		OUTPUT_OFF(BUZZER);
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


uint16_t current_second;
uint16_t last_second;
uint8_t preflight = 1;

void update_seconds(){
}

//return true if a time has passed since last main loop
uint8_t time_passed(uint16_t val){
	if((current_second >= val) && (last_second < val)){
		return 1;
	}else{
		return 0;
	}
}

uint16_t pyro_time = PYRO_TIME;

void statemachine(){
	if(preflight){
		static uint8_t check_pyro;
		static uint8_t check_jumper;
		logger_show();
		if(time_passed(1)){
			buzz(0);
			if(INPUT(RXD_PIN)){
				goto uart_mode;
			}
		}
		if(time_passed(2)){
			buzz(1000);
		}
		if(time_passed(3)){
			buzz(0);
		}
		if(time_passed(4)){
			buzz(1000);
		}
		if(time_passed(5)){
			buzz(0);
			check_pyro = 1;
		}
		
		if(time_passed(10)){
			buzz(1000);
			_delay_ms(100);
			buzz(0);
			df_erase();
			buzz(1000);
			_delay_ms(100);
			buzz(0);

			check_jumper = 1;
			
		}
		
		if(check_pyro){
			if(!INPUT(PYRO_TEST)){
				buzz(2000);
			}else{
				buzz(0);
			}
		}
		
		if(check_jumper && INPUT(RXD_PIN)){
			preflight = 0;
			current_second = 0;
			last_second = 0;
			cli();
			time_ms = 0;
			timer_seconds = 0;
			sei();
		}
	}else{
		static uint8_t piezo_quaek;
		static uint8_t piezo_timer = 4;
		logger_tick();
		if(time_passed(1)){
			buzz(1500);
		}
		if(time_passed(6)){
			buzz(0);
		}
		
		if(time_passed(pyro_time-3)){//cut down
			buzz(1000);
		}
		
		if(time_passed(pyro_time)){//cut down
			OUTPUT_ON(PYRO_FIRE);
			buzz(0);
		}
		if(time_passed(pyro_time + 1)){//cut down
			OUTPUT_OFF(PYRO_FIRE);
		}
		if(time_passed(pyro_time + 20)){//cut down
			OUTPUT_ON(PYRO_FIRE);
		}
		if(time_passed(pyro_time + 21)){//cut down
			OUTPUT_OFF(PYRO_FIRE);
		}
		
		if(time_passed(PIEZO_TIME)){
			piezo_quaek = 1;
		}
		
		if(piezo_quaek){
			piezo_timer --;
			if(piezo_timer == 3)
				buzz(1000);
			if(piezo_timer == 2)
				buzz(0);
			if(piezo_timer == 1)
				buzz(1500);
			if(piezo_timer == 0){
				buzz(0);
				piezo_timer = 30;
			}
		}
		
		static uint8_t allready_undervoltage;
		if(undervoltage){
			if(!allready_undervoltage){
				pyro_time = current_second + 4;
			}
			allready_undervoltage = 1;
		}
	}
	
	return;
	
	uart_mode:
	dump_log();
	while(1){
		if(uart_getc() == 'd'){
				
		}
	}
}

void cam_timer(){
	static uint16_t cam_time;
	static uint8_t shutoff;
	
	OUTPUT_OFF(CAM_BUTTON);
	if(cam_time == 20){
		if(shutoff){
			OUTPUT_OFF(CAM_POWER);
		}else{
			OUTPUT_ON(CAM_BUTTON);
		}
	}
	
	if(cam_time == CAM_TIME + 20){
		OUTPUT_ON(CAM_BUTTON);
		cam_time = 9;
	}
	
	cam_time++;
	
	if(undervoltage){
		if(!shutoff){
			shutoff = 1;
			cam_time = CAM_TIME; //20 more seconds
		}
	}
}

int my_putc(char c, FILE * fp){
	uart_putc(c);
	return 0;
}


char test_buf[] = "Test 12345 Hallo foobar 2000";

int main(){
	init_buzzer();
	init_timer2();

	logger_init();
	
	i2c_init();

	SET_DDR(PYRO_FIRE);
	SET_DDR(CAM_BUTTON);
	SET_DDR(CAM_POWER);
	OUTPUT_ON(CAM_POWER);


	uart_init();
	fdevopen (my_putc, 0);
	
	sei();
	
	df_init();
	
	printf("init done\r\n");
	
	buzz(1000);
	
	while(1){
		last_second = current_second;
		cli();
		current_second = timer_seconds;
		sei();
		if(current_second != last_second){
			statemachine();
			cam_timer();
		}
	}
}
