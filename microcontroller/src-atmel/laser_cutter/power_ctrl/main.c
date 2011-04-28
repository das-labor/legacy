
#include <avr/interrupt.h>
#include "uart/uart.h"
#include "ioport.h"
#include "../lib/com/com.h"

#include "adc.h"
#include "hardware.h"

uint8_t state_main_on;

uint8_t command_auto;

uint8_t command_zuenden;
uint8_t state_zuenden;

uint8_t command_500V_psu;
uint8_t state_500V_psu;

uint8_t command_simmer_psu;
uint8_t state_simmer_psu;

uint16_t simmer_i_soll;
uint16_t simmer_i_ist;
uint16_t simmer_u;

#define CS_WAIT_SYNC     0
#define CS_CMD1          1
#define CS_CMD2          2
#define CS_CHK1          3
#define CS_CHK2          4
#define CS_SIMMER_SOLL_L 5
#define CS_SIMMER_SOLL_H 6
#define CS_SLAVE1_REQ    7


uint8_t shift_port =0;  //Speichert den aktuellen Zustand der Ausgänge

//Setzt einen Port am Schieberegister
#define SET_SHIFT_PORT(PIN)  shift_port |=  PIN; shift_update(shift_port);
#define CLEAR_SHIFT_PIN(PIN) shift_port &= ~PIN; shift_update(shift_port);


//Ladegerät an/aus schalten
#define NT_ON   SET_SHIFT_PORT(SHIFT_NT_ON)
#define NT_OFF  CLEAR_SHIFT_PORT(SHIFT_NT_ON)




//Sendet den aktuellen Status ab das Schieberegister
void shift_update(void) {
    OUTPUT_OFF(SHIFT_SS);           //SS low

    SPDR = shift_port;                    //Daten senden
    while(!(SPSR & (1<<SPIF)));

    OUTPUT_ON(SHIFT_SS);            //SS High -->Daten an den Ausgang
}



//Initialisiert die Schieberegister-Ansteuerung
void init_shift() {
    //Output Enable H --> Ausgänge Z
    SET_DDR(SHIFT_OE);
    OUTPUT_ON(SHIFT_OE);

    //Latch-Clock auf Masse
    SET_DDR(SHIFT_SS);
    OUTPUT_OFF(SHIFT_SS);

    //Datenleitungen als Ausgang
    SET_DDR(SHIFT_MOSI);
    SET_DDR(SHIFT_SCK);

    //SPI: Enable, Master, f/16
    SPCR |= (1<< SPE) | (1 << MSTR) | (1 << SPR0);

    send_shift(0);

    //Ausgang aktiv
    OUTPUT_OFF(SHIFT_OE);
}

void put_uint16(uint16_t i){
	uart_putc(i & 0xff);
	uart_putc(i>>8);
}

void io_init(){
    SET_DDR(NT_INHIBIT);
	OUTPUT_ON(NT_INHIBIT);  //Netzteil gestoppt
}


void pwm_init(){
    //8bit fast pwm, non inverting
	TCCR1A = (1<<COM1A1) | (1<<WGM10); //Fast PWM 8 bit
	TCCR1B = (1<<WGM12)  | (1<<CS10) ; //clk/1
	OCR1A  = 0; // current off
	SET_DDR(PWM_U_SOLL);
}


void slave_com(){
	uint8_t c, res;
	static uint8_t tmp;
	static uint8_t com_state;
	static uint8_t cmd1, cmd2, chk1, chk2;
	static uint8_t poll_num;
	res = uart_getc_nb((char*)&c);
	if(res == 1){
		switch(com_state){
			case CS_WAIT_SYNC:
				if(c == 0x42){
					com_state = CS_CMD1;
				}
				break;
			case CS_CMD1:
				cmd1 = c;
				com_state = CS_CMD2;
				break;
			case CS_CMD2:
				cmd2 = c;
				com_state = CS_CHK1;
				break;
			case CS_CHK1:
				chk1 = c;
				com_state = CS_CHK2;
				break;
			case CS_CHK2:
				chk2 = c;
				if( (cmd1 == (chk1 ^ 0xff)) && (cmd2 == (chk2 ^ 0xff)) ) {
					state_main_on      = (cmd1 & MSK_MAIN_CMD)     ? 1:0;
					command_auto       = (cmd1 & MSK_AUTO_CMD)     ? 1:0;
					command_zuenden    = (cmd1 & MSK_ZUEND_CMD)    ? 1:0;
					command_500V_psu   = (cmd1 & MSK_500V_PSU_CMD) ? 1:0;
					command_simmer_psu = (cmd1 & MSK_SIMMER_CMD)   ? 1:0;

				}
				com_state = CS_SIMMER_SOLL_L;
				break;
			case CS_SIMMER_SOLL_L:
				tmp = c;
				com_state = CS_SIMMER_SOLL_H;
				break;
			case CS_SIMMER_SOLL_H:
				simmer_i_soll = ((uint16_t)c<<8) | tmp;
				com_state = CS_SLAVE1_REQ;
				break;

			case CS_SLAVE1_REQ:{
				uint8_t stat;
				stat =   (state_500V_psu        ? MSK_500V_PSU_STATE  : 0)
	        			|(state_zuenden         ? MSK_ZUEND_STATE     : 0)
	        			|(state_simmer_psu      ? MSK_SIMMER_STATE    : 0);

				poll_num = c & 0x0f;

				uart_putc(stat);
				if(poll_num == 0){
					put_uint16(simmer_i_ist);
				}else if(poll_num == 1){
					put_uint16(simmer_u);
				}
				com_state = CS_WAIT_SYNC;
				}break;
		}
	}
}

void statemachine (){
	if(state_main_on){
		state_simmer_psu = command_simmer_psu;
		state_500V_psu   = command_500V_psu;
	}else{
		state_simmer_psu = 0;
		state_500V_psu   = 0;
	}
}


void set_outputs(){
	static uint8_t old_zuenden;
	if(state_simmer_psu){
		OUTPUT_OFF(SIMMER); //is inverted (PS-ON of PC-PSU)
	}else{
		OUTPUT_ON(SIMMER);
	}
	if(state_500V_psu){
		OUTPUT_ON(PSU_500V);
	}else{
		OUTPUT_OFF(PSU_500V);
	}

	if(command_zuenden && (! old_zuenden)){
		OUTPUT_ON(ZUENDUNG);
	}else{
		OUTPUT_OFF(ZUENDUNG);
	}
	old_zuenden = command_zuenden;

	OCR1A = (simmer_i_soll * 192) / 256 ;
}



int main(){
	io_init();
	pwm_init();
	uart_init();
	init_adc();
	init_shift();

	sei();


	while(1){
		simmer_i_ist = (35 * adc_i) / 128;
		if(simmer_i_ist > 20){
			state_zuenden = 1;
		}else{
			state_zuenden = 0;
		}

		slave_com();
		statemachine();
		set_outputs();
	}
}
