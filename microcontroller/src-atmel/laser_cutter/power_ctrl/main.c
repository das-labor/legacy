
#include <avr/interrupt.h>
#include "uart/uart.h"
#include "ioport.h"
#include "../lib/com/com.h"
#include "pulse_isr.h"

#include "adc.h"
#include "hardware.h"

uint8_t state_main_on;

uint8_t command_auto;

uint8_t command_power_psu;
uint8_t state_power_psu;

uint8_t command_pumpe;
uint8_t state_pumpe;

uint8_t command_charge;
uint8_t state_charge;

uint8_t command_fire;

uint16_t power_u_soll;
uint16_t power_u_ist;

uint8_t command_zuenden;
uint8_t state_zuenden;

uint8_t command_500V_psu;
//uint8_t state_500V_psu;

uint8_t command_simmer_psu;
//uint8_t state_simmer_psu;

uint16_t simmer_i_soll;
uint16_t simmer_i_ist;
uint16_t simmer_u;

uint16_t fire_period = 1000;


#define CS_WAIT_SYNC     0
#define CS_CMD1          1
#define CS_CMD2          2
#define CS_CHK1          3
#define CS_CHK2          4
#define CS_SIMMER_SOLL_L 5
#define CS_SIMMER_SOLL_H 6
#define CS_POWER_SOLL_L  7
#define CS_POWER_SOLL_H  8
#define CS_PERIOD_L      9
#define CS_PERIOD_H     10
#define CS_SLAVE1_REQ   11
#define CS_SLAVE2_REQ   12



uint8_t shift_port =0;  //Speichert den aktuellen Zustand der Ausgänge


//Sendet den aktuellen Status ab das Schieberegister
void shift_update(void) {
    SPDR = shift_port;                    //Daten senden
    while(!(SPSR & (1<<SPIF)));

    OUTPUT_ON(SHIFT_SS);            //SS High -->Daten an den Ausgang
    OUTPUT_OFF(SHIFT_SS);           //SS low

}



//Initialisiert die Schieberegister-Ansteuerung
void init_shift() {
    //Output Enable H --> Ausgänge Z
    OUTPUT_ON(SHIFT_OE);
    SET_DDR(SHIFT_OE);

    //Latch-Clock auf Masse
    SET_DDR(SHIFT_SS);
    OUTPUT_OFF(SHIFT_SS);

    //Datenleitungen als Ausgang
    SET_DDR(SHIFT_MOSI);
    SET_DDR(SHIFT_SCK);

    //SPI: Enable, Master, f/16
    SPCR |= (1<< SPE) | (1 << MSTR) | (1 << SPR0);

    shift_port = 0;
	shift_update();

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

	SET_DDR(FIRE);

	SET_DDR(NT_POWER);

	PORTC = 0xff;//Pullups an Netzteil ausgängen

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
					command_pumpe      = (cmd2 & MSK_PUMPE_CMD)    ? 1:0;
					command_power_psu  = (cmd2 & MSK_POWER_PSU_CMD)? 1:0;
					command_fire       = (cmd2 & MSK_FIRE_CMD)     ? 1:0;
					command_charge     = (cmd2 & MSK_CHARGE_CMD)   ? 1:0;
				}
				com_state = CS_SIMMER_SOLL_L;
				break;
			case CS_SIMMER_SOLL_L:
				com_state = CS_SIMMER_SOLL_H;
				break;
			case CS_SIMMER_SOLL_H:
				com_state = CS_POWER_SOLL_L;
				break;
			case CS_POWER_SOLL_L:
				tmp = c;
				com_state = CS_POWER_SOLL_H;
				break;
			case CS_POWER_SOLL_H:
				power_u_soll = ((uint16_t)c<<8) | tmp;
				com_state = CS_PERIOD_L;
				break;
			case CS_PERIOD_L:
				tmp = c;
				com_state = CS_PERIOD_H;
				break;
			case CS_PERIOD_H:
				fire_period = ((uint16_t)c<<8) | tmp;
				com_state = CS_SLAVE1_REQ;
				break;
			case CS_SLAVE1_REQ:
				com_state = CS_SLAVE2_REQ;
				break;
			case CS_SLAVE2_REQ:{
				uint8_t stat;
				stat =   (state_pumpe        ? MSK_PUMPE_STATE         : 0)
	        			|(state_power_psu    ? MSK_POWER_PSU_STATE     : 0)
						|(state_charge       ? MSK_CHARGE_STATE        : 0) ;

				poll_num = c & 0x0f;

				uart_putc(stat);
				uart_putc(0);
				
				if(poll_num == 0){
					put_uint16(power_u_ist);
				}else if(poll_num == 1){
					//
				}
				com_state = CS_WAIT_SYNC;
				}break;
		}
	}
}

void statemachine (){
	if(state_main_on){
		state_power_psu = command_power_psu;
	}else{
		state_power_psu = 0;
	}

	state_pumpe = command_pumpe;
	state_charge = INPUT(NT_END_OF_CHARGE) ? 0 : 1;
}


void set_outputs(){
	if(state_power_psu){
		OUTPUT_ON(NT_POWER);
		SET_SHIFT_PORT(REL_NT_POWER);
	}else{
		OUTPUT_OFF(NT_POWER);
		CLEAR_SHIFT_PORT(REL_NT_POWER);
	}

	if(state_pumpe){
		SET_SHIFT_PORT(REL_PUMPE);
	}else{
		CLEAR_SHIFT_PORT(REL_PUMPE);
	}
	
	if(state_main_on){
		SET_SHIFT_PORT(REL_NT_CONTROL);
		SET_SHIFT_PORT(REL_SIMMER);
	}else{
		CLEAR_SHIFT_PORT(REL_NT_CONTROL);
		CLEAR_SHIFT_PORT(REL_SIMMER);
	}

	OCR1A = (power_u_soll * 16) / 94 ;
}



int main(){
	io_init();
	pwm_init();
	uart_init();
	init_adc();
	init_shift();
	init_pulse();

	sei();


	while(1){
		//442 adc val = 1400V
		power_u_ist = (53 * adc_uwave) / 16;

		slave_com();
		statemachine();
		set_outputs();
	}
}
