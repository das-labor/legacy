#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/crc16.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "canlib/spi.h"
#include "canlib/can.h"
#include "canlib/mcp2510regs.h"
#include "uart/uart.h"

//leds
#define PORT_LEDS PORTD
#define DDR_LEDS DDRD
#define PIN_LEDCL PD5
#define PIN_LEDCK PD6
#define PIN_LEDD PD7

//buspower
#define PORT_BUSPOWER PORTD
#define DDR_BUSPOWER DDRD
#define BIT_BUSPOWER PD4

//reset cause status register
#define REG_RESETCAUSE MCUCSR
#define MSK_RESETCAUSE 0x0F

//max packet data length
#define RS232CAN_MAXLENGTH 20

//ADMUX default value
//select AREF voltage reference, right-adjust conversion results
#define ADMUX_REFS (_BV(REFS0))

//ADC channels
#define CH_BUSVOLTAGE (_BV(MUX2) | _BV(MUX0))
#define CH_BUSCURRENT (_BV(MUX2))
#define CH_REF_1300MV (_BV(MUX3) | _BV(MUX2) | _BV(MUX1))
#define CH_REF_GND	  (_BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))

//adc start conversion
#define ADC_START() (ADCSRA |= _BV(ADSC))

//the timer frequency is approx. 61Hz @ 16MHz cpu. freq
#define SYS_TICK_FREQ ((F_CPU / 1024) / 256)

//hacky import of read function
extern unsigned char mcp_read(unsigned char reg);

//firmware version structure
uint8_t firmware_version[] = {FW_VERSION_MAJOR, FW_VERSION_MINOR, (FW_SVNREVISION >> 8) & 0xFF, FW_SVNREVISION & 0xFF};

//packet and data counters, as seen from the gateway looking at the host
//i.e. rx is about packets from uart and tx to uart, not from can
struct { uint32_t rx_count, tx_count, rx_size, tx_size; } pkt_cnt = {0, 0, 0, 0};

//error counters and flags from the mcp2515 chip
struct { uint8_t rx_errors, tx_errors, error_flags; } err_cnt = {0, 0, 0};

//bus power measurement struct
struct { volatile uint16_t v, i, ref, gnd; } bus_pwr = {0, 0, 0, 0};

//state of the adc interrupt state machine
volatile uint8_t adc_state;

//system counter
volatile uint16_t sys_ticks;

//commands
typedef enum
{
	RS232CAN_RESET=0x00,
	RS232CAN_SETFILTER=0x10,
	RS232CAN_PKT=0x11,
	RS232CAN_SETMODE=0x12,
	RS232CAN_ERROR=0x13,
	RS232CAN_NOTIFY_RESET=0x14,
	RS232CAN_PING_GATEWAY=0x15,
	RS232CAN_RESYNC=0x16,
	RS232CAN_VERSION=0x17,
	RS232CAN_IDSTRING=0x18,
	RS232CAN_PACKETCOUNTERS=0x19,
	RS232CAN_ERRORCOUNTERS=0x1A,
	RS232CAN_POWERDRAW=0x1B
} rs232can_cmd;

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;


/*****************************************************************************
 * CAN to UART
 */
uint16_t write_buffer_to_uart_and_crc(uint16_t crc, char* buf, uint8_t len) {
	uint8_t i;

	for (i = 0; i < len; i++) {
		crc = _crc16_update(crc, *buf);
		uart_putc( *buf++);
	}

	return crc;
}

void write_can_message_to_uart(can_message * cmsg) {
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8;//actual size of can message
	uint16_t crc;

	crc = _crc16_update(0, RS232CAN_PKT);
	crc = _crc16_update(crc, len);

	uart_putc(RS232CAN_PKT);  //command
	uart_putc(len);           //length

	crc = write_buffer_to_uart_and_crc(crc, (char*)cmsg, len); //data

	uart_putc(crc >> 8);	  //crc16
	uart_putc(crc & 0xFF);
}
/*****************************************************************************/


/*****************************************************************************
 * CMD to UART
 */
void write_cmd_to_uart(uint8_t cmd, char* buf, uint8_t len)
{
	uint16_t crc;

	crc = _crc16_update(0, cmd);
	crc = _crc16_update(crc, len);

	uart_putc(cmd); 		//command
	uart_putc(len);			//length

	if(len)
		crc = write_buffer_to_uart_and_crc(crc, buf, len);

	uart_putc(crc >> 8);	//crc16
	uart_putc(crc & 0xFF);
}
/*****************************************************************************/


/*****************************************************************************
 * Receive a message from uart non blocking.
 * Returns Message or 0 if there is no complete message.
 */

typedef enum {STATE_START, STATE_LEN, STATE_PAYLOAD, STATE_CRC} canu_rcvstate_t;

rs232can_msg	canu_rcvpkt;
canu_rcvstate_t	canu_rcvstate = STATE_START;
unsigned char 	canu_rcvlen   = 0;
unsigned char	canu_failcnt  = 0;

rs232can_msg * canu_get_nb()
{
	static char *uartpkt_data;
	static uint16_t crc, crc_in;
	unsigned char c;

	while (uart_getc_nb((char *)&c))
	{
		#ifdef DEBUG
		printf("canu_get_nb received: %02x\n", c);
		#endif
		switch (canu_rcvstate)
		{
			case STATE_START:
				if (c)
				{
					canu_rcvstate = STATE_LEN;
					canu_rcvpkt.cmd = c;
					crc = _crc16_update(0, c);
				} else
					canu_failcnt = 0;
				break;
			case STATE_LEN:
				canu_rcvlen       = c;
				if(canu_rcvlen > RS232CAN_MAXLENGTH)
				{
					canu_rcvstate = STATE_START;
					break;
				}
				canu_rcvstate     = STATE_PAYLOAD;
				canu_rcvpkt.len   = c;
				uartpkt_data      = &canu_rcvpkt.data[0];
				crc = _crc16_update(crc, c);
				break;
			case STATE_PAYLOAD:
				if(canu_rcvlen--)
				{
					*(uartpkt_data++) = c;
					crc = _crc16_update(crc, c);
				}
				else
				{
					canu_rcvstate = STATE_CRC;
					crc_in = c;
				}
				break;
			case STATE_CRC:
				canu_rcvstate = STATE_START;
				if(crc == ((crc_in << 8) | c))
				{
					canu_failcnt = 0;
					return &canu_rcvpkt;
				}
				canu_failcnt++;
				break;
		}
	}

	return NULL;
}

/*****************************************************************************/

// synchronize line
void canu_reset()
{
	unsigned char i;
	for(i=sizeof(rs232can_msg)+2; i>0; i--)
		uart_putc( (char)0x00 );
}

void process_cantun_msg(rs232can_msg *msg) {
	can_message *cmsg;

	switch (msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			can_setmode(msg->data[0]);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get();                      //alocate buffer
			memcpy(cmsg, msg->data, sizeof(can_message)); //copy can message
			can_transmit(cmsg);                           //transmit it
			pkt_cnt.rx_size += cmsg->dlc;
			pkt_cnt.rx_count ++;
			break;
		case RS232CAN_PING_GATEWAY:
			write_cmd_to_uart(RS232CAN_PING_GATEWAY, 0, 0);  // reply
			break;
		case RS232CAN_RESYNC:
			canu_reset();
			break;
		case RS232CAN_VERSION:
			write_cmd_to_uart(RS232CAN_VERSION, (char *)firmware_version, sizeof(firmware_version));
			break;
		case RS232CAN_IDSTRING:
			write_cmd_to_uart(RS232CAN_IDSTRING, (char *)FW_IDSTRING, (sizeof(FW_IDSTRING)-1)>20?sizeof(FW_IDSTRING)-1:20);
			break;
		case RS232CAN_PACKETCOUNTERS:
			write_cmd_to_uart(RS232CAN_PACKETCOUNTERS, (char *)&pkt_cnt, sizeof(pkt_cnt));
			break;
		case RS232CAN_ERRORCOUNTERS:
			err_cnt.rx_errors   = mcp_read(REC);
			err_cnt.tx_errors   = mcp_read(TEC);
			err_cnt.error_flags = mcp_read(EFLG);
			write_cmd_to_uart(RS232CAN_ERRORCOUNTERS, (char *)&err_cnt, sizeof(err_cnt));
			break;
		case RS232CAN_POWERDRAW:
			write_cmd_to_uart(RS232CAN_POWERDRAW, (char *)&bus_pwr, sizeof(bus_pwr));
			break;
		default:
			write_cmd_to_uart(RS232CAN_ERROR, 0, 0);  //send error
			break;
	}
}


void buspower_on() {
	DDR_BUSPOWER |= (1<<BIT_BUSPOWER);
	PORT_BUSPOWER |= (1<<BIT_BUSPOWER);
}

void led_init() {
	DDR_LEDS |= (1<<PIN_LEDD)|(1<<PIN_LEDCL)|(1<<PIN_LEDCK);
	PORT_LEDS |= (1<<PIN_LEDCL);
}

void led_set(unsigned int stat) {
	unsigned char x;
	for (x = 0; x < 16; x++) {
		if (stat & 0x01) {
			PORT_LEDS |= (1<<PIN_LEDD);
		} else {
			PORT_LEDS &= ~(1<<PIN_LEDD);
		}
		stat >>= 1;
		PORT_LEDS |= (1<<PIN_LEDCK);
		PORT_LEDS &= ~(1<<PIN_LEDCK);
	}
}

//setup adc operations
void adc_init()
{
	//setup adc pins
	DDRC = 0xCF;

	//default to first channel
	adc_state = CH_BUSVOLTAGE;

	//set adc mux default settings
	ADMUX = ADMUX_REFS | (CH_BUSVOLTAGE & 0x0F);

	//slowest adc clock (prescaler 128)
	ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	//set adc enable, set interrupt enable, clear pending ints
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADIF);
}

//measure internal references and calibrate
void adc_calibrate()
{
	//disable interrupts during low-noise measurement
	cli();
	#ifdef UART_INTERRUPT
		//buffer old int flags, as not all ints are enabled on all devices
		uint8_t old_uart_int_flags = UCSRB & (_BV(RXCIE) | _BV(TXCIE));
		UCSRB &= ~(_BV(RXCIE) | _BV(TXCIE));
	#endif
	#ifdef CAN_INTERRUPT
		GICR &= ~_BV(INT0);
	#endif
	TIMSK &= ~_BV(TOIE0);
	sei();

	//wait for ongoing measurements to finish
	while(ADCSRA & _BV(ADIF));

	//switch adc and state machine to reference measuring
	adc_state = CH_REF_GND;
	ADMUX = ADMUX_REFS | (CH_REF_GND & 0x0F);

	//configure adc noise reduction sleep mode
	MCUCR &= ~(_BV(SM2) | _BV(SM1));
	MCUCR |= _BV(SE) | _BV(SM0);

	//start ADC by issueing sleep instruction
	sleep_cpu();

	//wait for this to finish
	while(((ADCSRA & _BV(ADIF)) != 0) && (adc_state != CH_BUSVOLTAGE));

	//disable sleep mode
	MCUCR &= ~_BV(SE);

	//re-enable interrupts
	cli();
	TIMSK |= _BV(TOIE0);
	#ifdef CAN_INTERRUPT
		GICR |= _BV(INT0);
	#endif
	#ifdef UART_INTERRUPT
		UCSRB |= old_uart_int_flags;
	#endif
	sei();
}

//adc interrupt to switch channels
ISR(ADC_vect)
{
	switch(adc_state)
	{
		case CH_BUSVOLTAGE:
			bus_pwr.v = ADC;
			adc_state = CH_BUSCURRENT;
			ADMUX = ADMUX_REFS | (CH_BUSCURRENT & 0x0F);
			ADC_START();
			break;
		case CH_BUSCURRENT:
			bus_pwr.i = ADC;
			adc_state = CH_BUSVOLTAGE;
			ADMUX = ADMUX_REFS | (CH_BUSVOLTAGE & 0x0F);
			break;
		case CH_REF_GND:
			//measure ground to discharge the s/h cap
			bus_pwr.gnd = ADC;
			adc_state = CH_REF_1300MV;
			//select internal bandgap and wait 70uS to charge s/h cap
			ADMUX = ADMUX_REFS | (CH_REF_1300MV & 0x0F);
			_delay_us(70);
			//enter (previously configured) adc noise reduction sleep
			sleep_cpu();
			break;
		case CH_REF_1300MV:
			bus_pwr.ref = ADC;
			adc_state = CH_BUSVOLTAGE;
			ADMUX = ADMUX_REFS | (CH_BUSVOLTAGE & 0x0F);
			break;
	}
}

//setup timer0 to simply count up
void timer0_init()
{
	//enable timer with prescaler 1024
	TCCR0 = _BV(CS02) | _BV(CS00);

	//reset counter
	TCNT0 = 0;

	//enable interrupt
	TIMSK |= _BV(TOIE0);
	TIFR  |= _BV(TOV0);
}

//timer0 int acts as system counter
ISR(TIMER0_OVF_vect)
{
	sys_ticks++;
}

//init the complete system
void sys_init()
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//init all subsystems
	led_init();
	buspower_on();
	uart_init();
	spi_init();
	can_init();
	adc_init();
	timer0_init();

	//enable interrupts (all systems go!)
	sei();

	//calibrate adc
	//adc_calibrate();

	//enable watchdog
	wdt_enable(WDTO_250MS);

	//sync line
	canu_reset();
}

int main() {
	static uint16_t leds, leds_old;
	uint16_t adc_last_schedule_time;

	//init
	sys_init();

	//notify host that we had a reset
	adc_last_schedule_time = REG_RESETCAUSE & MSK_RESETCAUSE;
	write_cmd_to_uart(RS232CAN_NOTIFY_RESET, (char*)&adc_last_schedule_time, 1);

	//begin can operations
	can_setmode(normal);
	can_setled(0, 1);

	//store system counter
	adc_last_schedule_time = sys_ticks;
	while (1) {
		rs232can_msg  *rmsg;
		can_message *cmsg;

		wdt_reset();

		//transmission from host to can
		rmsg = canu_get_nb();
		if (rmsg)
			process_cantun_msg(rmsg);
		else if(canu_failcnt > 1)
		{
			canu_reset();
			write_cmd_to_uart(RS232CAN_RESYNC, 0, 0);
			canu_failcnt = 0;
		}

		//transmission from can to host
		cmsg = can_get_nb();
		if (cmsg) {
			pkt_cnt.tx_count ++;
			pkt_cnt.tx_size += cmsg->dlc;
			write_can_message_to_uart(cmsg);
			can_free(cmsg);
		}

		//update leds
		leds = (pkt_cnt.rx_count << 8) | pkt_cnt.tx_count;
		if (leds != leds_old) {
			leds_old = leds;
			led_set(leds);
		}

		//schedule adc measurements, approx. twice a second
		//the timer frequency is approx. 61Hz @ 16MHz cpu. freq
		//so our delta should be around SYS_TICK_FREQ/2
		if((sys_ticks - adc_last_schedule_time) > (SYS_TICK_FREQ / 2))
		{
			adc_last_schedule_time = sys_ticks;
			ADC_START();
		}
	}

	return 0;
}
