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
//REFS1 REFS0
//    0     0  AREF, Internal Vref turned off
//    0     1  AVCC with external capacitor at AREF pin
//    1     0  Reserved
//    1     1  Internal 2.56V Voltage Reference with external capacitor at AREF pin
#define ADMUX_REFS (_BV(REFS0))

//ADC channels
#define CH_BUSVOLTAGE (_BV(MUX2) | _BV(MUX0))
#define CH_BUSCURRENT (_BV(MUX2))
#define CH_REF_1300MV (_BV(MUX3) | _BV(MUX2) | _BV(MUX1))
#define CH_REF_GND    (_BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))

//adc start conversion
#define ADC_START() (ADCSRA |= _BV(ADSC))

//the timer frequency is approx. 61Hz @ 16MHz cpu. freq
#define SYS_TICK_FREQ ((F_CPU / 1024) / 256)

//our own control "register" bit flags
#define FLAG_AUTOREPORT_PSTATS		0
#define FLAG_AUTOREPORT_POWERDRAW	1
#define FLAG_BUSPOWER			2
#define FLAG_RESET			7

//hacky import of read function
extern unsigned char mcp_read(unsigned char reg);

//firmware version structure
static uint8_t firmware_version[] = {FW_VERSION_MAJOR, FW_VERSION_MINOR, (FW_SVNREVISION >> 8) & 0xFF, FW_SVNREVISION & 0xFF};

//packet and data counters, as seen from the gateway looking at the host
//i.e. rx is about packets from uart and tx to uart, not from can
static struct { uint32_t rx_count, tx_count, rx_size, tx_size; } pkt_cnt = {0, 0, 0, 0};

//error counters and flags from the mcp2515 chip
static struct { uint8_t rx_errors, tx_errors, error_flags; } err_cnt = {0, 0, 0};

//bus power measurement struct
static volatile struct { volatile uint16_t v, i, ref, gnd; } volatile bus_pwr = {0, 0, 0, 0};

//state of the adc interrupt state machine
static volatile uint8_t adc_state;

//system counter
static volatile uint16_t sys_ticks;

//our own control "register"
static uint8_t ctrl_reg = 0;

//schedule counters
static uint16_t adc_last_schedule_time, autoreport_last_schedule_time;

// reset cause
static uint8_t reset_cause = 0;

//commands
typedef enum {
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
	RS232CAN_POWERDRAW=0x1B,
	RS232CAN_READ_CTRL_REG=0x1C,
	RS232CAN_WRITE_CTRL_REG=0x1D,
	RS232CAN_GET_RESETCAUSE=0x1E
} rs232can_cmd;

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;

//forward declarations
#ifdef BUSPOWER_SWITCH
static void buspower(uint8_t on);
#endif // BUSPOWER_SWITCH
static void syscontrol(uint8_t ctrl_reg_new);


/*****************************************************************************
 * CAN to UART
 */
static uint16_t write_buffer_to_uart_and_crc(uint16_t crc, char *buf, uint8_t len)
{
	uint8_t i;

	for (i = 0; i < len; i++) {
		crc = _crc16_update(crc, *buf);
		uart_putc(*buf++);
	}

	return crc;
}

static void write_can_message_to_uart(can_message *cmsg)
{
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8; // actual size of can message
	uint16_t crc;

	crc = _crc16_update(0, RS232CAN_PKT);
	crc = _crc16_update(crc, len);

	uart_putc(RS232CAN_PKT);  //command
	uart_putc(len);           //length

	crc = write_buffer_to_uart_and_crc(crc, (char *) cmsg, len); //data

	uart_putc(crc >> 8);      //crc16
	uart_putc(crc & 0xFF);
}
/*****************************************************************************/


/*****************************************************************************
 * CMD to UART
 */
static void write_cmd_to_uart(uint8_t cmd, char *buf, uint8_t len)
{
	uint16_t crc;

	crc = _crc16_update(0, cmd);
	crc = _crc16_update(crc, len);

	uart_putc(cmd);		//command
	uart_putc(len);		//length

	if (len)
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

static rs232can_msg	canu_rcvpkt;
static canu_rcvstate_t	canu_rcvstate = STATE_START;
static unsigned char	canu_rcvlen   = 0;
static unsigned char	canu_failcnt  = 0;

static rs232can_msg *canu_get_nb(void)
{
	static char *uartpkt_data;
	static uint16_t crc, crc_in;
	unsigned char c;

	while (uart_getc_nb((char *) &c)) {
		#ifdef DEBUG
		printf("canu_get_nb received: %02x\n", c);
		#endif
		switch (canu_rcvstate) {
			case STATE_START:
				if (c) {
					canu_rcvstate = STATE_LEN;
					canu_rcvpkt.cmd = c;
					crc = _crc16_update(0, c);
				} else
					canu_failcnt = 0;
				break;
			case STATE_LEN:
				canu_rcvlen       = c;
				if (canu_rcvlen > RS232CAN_MAXLENGTH) {
					canu_rcvstate = STATE_START;
					break;
				}
				canu_rcvstate     = STATE_PAYLOAD;
				canu_rcvpkt.len   = c;
				uartpkt_data      = &canu_rcvpkt.data[0];
				crc = _crc16_update(crc, c);
				break;
			case STATE_PAYLOAD:
				if (canu_rcvlen--) {
					*(uartpkt_data++) = c;
					crc = _crc16_update(crc, c);
				} else {
					canu_rcvstate = STATE_CRC;
					crc_in = c;
				}
				break;
			case STATE_CRC:
				canu_rcvstate = STATE_START;
				if (crc == ((crc_in << 8) | c)) {
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
static void canu_reset(void)
{
	unsigned char i;
	for (i = sizeof(rs232can_msg) + 2; i > 0; i--)
		uart_putc('\0');
}


static void process_cantun_msg(rs232can_msg *msg)
{
	can_message *cmsg;

	switch (msg->cmd) {
		case RS232CAN_RESET:
			wdt_enable(WDTO_15MS);
			while (1);
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			mcp_setmode(msg->data[0]);
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
			write_cmd_to_uart(RS232CAN_VERSION, (char *) firmware_version, sizeof(firmware_version));
			break;
		case RS232CAN_IDSTRING:
			write_cmd_to_uart(RS232CAN_IDSTRING, (char *) FW_IDSTRING, (sizeof(FW_IDSTRING) - 1) > 20 ? sizeof(FW_IDSTRING) -1 : 20);
			break;
		case RS232CAN_PACKETCOUNTERS:
			write_cmd_to_uart(RS232CAN_PACKETCOUNTERS, (char *) &pkt_cnt, sizeof(pkt_cnt));
			break;
		case RS232CAN_ERRORCOUNTERS:
			err_cnt.rx_errors   = mcp_read(REC);
			err_cnt.tx_errors   = mcp_read(TEC);
			err_cnt.error_flags = mcp_read(EFLG);
			write_cmd_to_uart(RS232CAN_ERRORCOUNTERS, (char *) &err_cnt, sizeof(err_cnt));
			break;
		case RS232CAN_POWERDRAW:
			write_cmd_to_uart(RS232CAN_POWERDRAW, (char *) &bus_pwr, sizeof(bus_pwr));
			break;
		case RS232CAN_READ_CTRL_REG:
			write_cmd_to_uart(RS232CAN_READ_CTRL_REG, (char *) &ctrl_reg, sizeof(ctrl_reg));
			break;
		case RS232CAN_WRITE_CTRL_REG:
			syscontrol((uint8_t)msg->data[0]);
			write_cmd_to_uart(RS232CAN_WRITE_CTRL_REG, (char *) &ctrl_reg, sizeof(ctrl_reg));
			break;
		case RS232CAN_GET_RESETCAUSE:
			write_cmd_to_uart(RS232CAN_GET_RESETCAUSE, (char *) &reset_cause, 1);
			break;
		default:
			write_cmd_to_uart(RS232CAN_ERROR, 0, 0);  //send error
			break;
	}
}

#ifdef BUSPOWER_SWITCH
static void buspower(uint8_t on)
{
	if (on) {
		DDR_BUSPOWER |= (1<<BIT_BUSPOWER);
		PORT_BUSPOWER |= (1<<BIT_BUSPOWER);
	} else {
		DDR_BUSPOWER &= ~(1<<BIT_BUSPOWER);
		PORT_BUSPOWER &= ~(1<<BIT_BUSPOWER);
	}
}
#endif // BUSPOWER_SWITCH

#ifdef LED_SUPPORT
static void led_init()
{
	DDR_LEDS |= (1<<PIN_LEDD) | (1<<PIN_LEDCL) | (1<<PIN_LEDCK);
	PORT_LEDS |= (1<<PIN_LEDCL);
}


static void led_set(unsigned int stat)
{
	unsigned char x;
	for (x = 0; x < 16; x++) {
		if (stat & 0x01)
			PORT_LEDS |= (1<<PIN_LEDD);
		else
			PORT_LEDS &= ~(1<<PIN_LEDD);
		stat >>= 1;
		PORT_LEDS |= (1<<PIN_LEDCK);
		PORT_LEDS &= ~(1<<PIN_LEDCK);
	}
}
#endif // LED_SUPPORT

#ifdef POWER_MEASUREMENT

//setup adc operations
static void adc_init(void)
{
	//default to first channel
	adc_state = CH_BUSVOLTAGE;

	//set adc mux default settings
	ADMUX = ADMUX_REFS | (CH_BUSVOLTAGE & 0x0F);

	//slowest adc clock (prescaler 128)
	ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	//set adc enable, set interrupt enable, clear pending ints
	ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADIF);
}

//measure internal references and calibrate
static void adc_calibrate(void)
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
	while (ADCSRA & _BV(ADIF));

	//switch adc and state machine to reference measuring
	adc_state = CH_REF_GND;
	ADMUX = ADMUX_REFS | (CH_REF_GND & 0x0F);

	//configure adc noise reduction sleep mode
	MCUCR &= ~(_BV(SM2) | _BV(SM1));
	MCUCR |= _BV(SE) | _BV(SM0);

	//start ADC by issueing sleep instruction
	sleep_cpu();

	//wait for this to finish
	while (((ADCSRA & _BV(ADIF)) != 0) && (adc_state != CH_BUSVOLTAGE));

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
	switch (adc_state) {
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

#endif // POWER_MEASUREMENT

//setup timer0 to simply count up
static void timer0_init(void)
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
static void sys_init(void)
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//init all subsystems
#ifdef LED_SUPPORT
	led_init();
#endif // LED_SUPPORT
#ifdef BUSPOWER_SWITCH
	buspower(1);
#endif // BUSPOWER_SWITCH
	uart_init();
	spi_init();
	wdt_reset();
	can_init();
#ifdef POWER_MEASUREMENT
	adc_init();
#endif // POWER_MEASUREMENT
	timer0_init();

	//init ctrl reg
	ctrl_reg = _BV(FLAG_BUSPOWER);

	//enable interrupts (all systems go!)
	sei();

	//calibrate adc
	//adc_calibrate();

	//enable watchdog
	wdt_enable(WDTO_250MS);

	//sync line
	canu_reset();
}


static void syscontrol(uint8_t ctrl_reg_new)
{
	uint8_t changes = ctrl_reg_new ^ ctrl_reg;

	if (ctrl_reg_new & _BV(FLAG_RESET)) {
		wdt_enable(WDTO_15MS);
		while (23);
	}
#ifdef BUSPOWER_SWITCH
	if (changes & _BV(FLAG_BUSPOWER))
		buspower(ctrl_reg_new & _BV(FLAG_BUSPOWER));
#endif // BUSPOWER_SWITCH
	if (ctrl_reg_new & (_BV(FLAG_AUTOREPORT_POWERDRAW) | _BV(FLAG_AUTOREPORT_PSTATS)))
		autoreport_last_schedule_time = 0;

	if (changes)
		ctrl_reg = ctrl_reg_new;
}

can_message test = {0xffffffff, 8, {1,2,3,4,5,6,7,8}};

int main(void)
{
	// get reset cause
	reset_cause = REG_RESETCAUSE & MSK_RESETCAUSE;
	// after read reset MCU Control and Status Register Flags else reset cause get unclear
	REG_RESETCAUSE = 0;

#if defined(LED_SUPPORT) || defined(LED_SUPPORT_MCP)
	static uint16_t leds, leds_old;
#endif // LED_SUPPORT

	//init
	sys_init();

	//notify host that we had a reset
	write_cmd_to_uart(RS232CAN_NOTIFY_RESET, (char *) &reset_cause, 1);

	//store system counter
	adc_last_schedule_time = autoreport_last_schedule_time = sys_ticks;
	while (1) {
		rs232can_msg *rmsg;
		can_message *cmsg;

		wdt_reset();

		//transmission from host to can
		rmsg = canu_get_nb();
		if (rmsg)
			process_cantun_msg(rmsg);
		else if (canu_failcnt > 1) {
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
			//write_can_message_to_uart(&test);
			can_free(cmsg);
		}

#if defined(LED_SUPPORT) || defined(LED_SUPPORT_MCP)
		//update leds
		leds = (pkt_cnt.rx_count << 8) | pkt_cnt.tx_count;
		if (leds != leds_old) {
			leds_old = leds;
#ifdef LED_SUPPORT_MCP
			mcp_setled(0, leds & 1);
#elif LED_SUPPORT_LABNODE
			PORT_LED ^= (1 << BIT_LED);
#else
			led_set(leds);
#endif // LED_SUPPORT_MCP
		}
#endif // LED_SUPPORT
#ifdef POWER_MEASUREMENT
		// schedule adc measurements, approx. twice a second
		// the timer frequency is approx. 61Hz @ 16MHz cpu. freq
		// so our delta should be around SYS_TICK_FREQ / 2
		if ((sys_ticks - adc_last_schedule_time) > (SYS_TICK_FREQ / 2)) {
			adc_last_schedule_time = sys_ticks;
			ADC_START();
		}
#endif // POWER_MEASUREMENT
		// schedule autoreport functions approx once a second
		if ((ctrl_reg & (_BV(FLAG_AUTOREPORT_POWERDRAW) | _BV(FLAG_AUTOREPORT_PSTATS))) > 0 && (sys_ticks - autoreport_last_schedule_time) > (SYS_TICK_FREQ)) {
			autoreport_last_schedule_time = sys_ticks;

			if (ctrl_reg & _BV(FLAG_AUTOREPORT_PSTATS))
				write_cmd_to_uart(RS232CAN_PACKETCOUNTERS, (char *) &pkt_cnt, sizeof(pkt_cnt));
#ifdef POWER_MEASUREMENT
			if (ctrl_reg & _BV(FLAG_AUTOREPORT_POWERDRAW))
				write_cmd_to_uart(RS232CAN_POWERDRAW, (char *) &bus_pwr, sizeof(bus_pwr));
#endif // POWER_MEASUREMENT
		}
	}
}

//early watchdog disable function
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
void wdt_init(void)
{
	wdt_disable();
	return;
}
