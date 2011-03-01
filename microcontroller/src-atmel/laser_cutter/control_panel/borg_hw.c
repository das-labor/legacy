
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"
#include "ioport.h"

//Row port
#define ROW_PORT  PORTC
#define ROW_DDR   DDRC
#define ROW_MASK  0xf0
#define ROW_START 0x10

//Switch Port
#define SWITCH_PORT PORTC
#define SWITCH_PIN  PINC
#define SWITCH_MASK 0x0f

//LED Port
#define LED_PORT PORTA
#define LED_DDR  DDRA
#define LED_MASK 0x0f

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define BIT_SCK  PB7
#define BIT_MOSI PB5
#define BIT_SS   PB4


//encoder
#define PHASE_A_PORT D
#define PHASE_A_BIT  5

#define PHASE_B_PORT D
#define PHASE_B_BIT  4


void spi_out(uint8_t d){
	uint8_t x;
	for(x=0;x<8;x++){
		if(d & 0x80){
			SPI_PORT |= (1<<BIT_MOSI);
		}else{
			SPI_PORT &= ~(1<<BIT_MOSI);
		}
		SPI_PORT |= (1<<BIT_SCK);
		d <<= 1;
		SPI_PORT &= ~(1<<BIT_SCK);
	}
	SPI_PORT |=  (1<<BIT_SS);
	SPI_PORT &= ~(1<<BIT_SS);
	
}

void spi_init(){
	SPI_DDR |= (1<<BIT_SS) | (1<<BIT_SCK) | (1<<BIT_MOSI);
}


//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];
uint8_t display[NUM_DISPLAYS][NUM_ROWS];

volatile uint8_t keys[4];

inline void busywait() {
	//unsigned char i;
	//for(i=0;i<20;i++){
	//	asm volatile("nop");
	//}
}


//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	ROW_PORT = (ROW_PORT & ~ROW_MASK); //blank

	LED_PORT = (LED_PORT & ~LED_MASK) | (LED_MASK & pixmap[0][row][0]);//set LED pins to new value

	uint8_t x;
	for(x=0;x<NUM_DISPLAYS;x++){
		spi_out(display[x][row]);
	}
	
	ROW_PORT = (ROW_PORT & ~ROW_MASK) | (ROW_START<<row); //select row
	
}

inline void checkkeys(uint8_t row){
	keys[row] = SWITCH_PIN & SWITCH_MASK;
}

volatile uint8_t enc_delta;

inline void update_encoder(){
	static int8_t last;
	int8_t new, diff;

	new = 0;
	if( INPUT(PHASE_A) )
		new = 3;
	if( INPUT(PHASE_B) )
		new ^= 1;					// convert gray to binary
	diff = last - new;				// difference last - new
	if( diff & 1 ){				    // bit 0 = value (1)
		last = new;					// store new as next last
		enc_delta += (diff & 2) - 1;// bit 1 = direction (+/-)
	}
}

#if defined(__AVR_ATmega644__) | defined(__AVR_ATmega644P__)
	SIGNAL(SIG_OUTPUT_COMPARE0A)
#else
	SIGNAL(SIG_OUTPUT_COMPARE0)
#endif
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	//Watchdog zurücksetzen
	wdt_reset();

	//Taster von der letzten Zeile prüfen
	checkkeys(row);

	//Zeile und Ebene inkrementieren
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
	}

	//Die aktuelle Zeile in der aktuellen Ebene ausgeben
	rowshow(row, plane);
	
	update_encoder();	
}


int8_t encoder_read( void )			// read two step encoders
{
  int8_t val;

  cli();
  val = enc_delta;
  enc_delta &= 1;
  sei();
  return val >> 1;
}


// Den Timer, der denn Interrupt auslöst, initialisieren
void timer0_on(){
#if defined(__AVR_ATmega644__) | defined(__AVR_ATmega644P__)
  TCCR0A = (1<<WGM01);//CTC
  TCCR0B = (1<<CS02); //clk / 256
  OCR0A = 20;
  TIMSK0 |= (1<<OCIE0A);

#else
  /* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
  		CS02 CS01 CS00
  		 0    0    0	       stop
  		 0    0    1       clk
  		 0    1    0       clk/8
  		 0    1    1       clk/64
  		 1    0    0       clk/256
  		 1    0    1       clk/1024
  	
  */
  	TCCR0 = 0x0C;	// CTC Mode, clk/256
  	TCNT0 = 0;	// reset timer
  	OCR0  = 20;	// Compare with this value
  	TIMSK = 0x02;	// Compare match Interrupt on
#endif
}

void borg_hw_init(){
	ROW_DDR     |= ROW_MASK;	//Ausgang
	LED_DDR     |= LED_MASK;	//Ausgang
	SWITCH_PORT |= SWITCH_MASK; //Pullups

	ROW_PORT    |= ROW_START;   //erste Zeile schonmal aktivieren, 
								//damit die ersten Tasten beim ersten
								//Interrupt schon gelesen werden können

	OUTPUT_ON(PHASE_A); //Pullups for rotary encoder
	OUTPUT_ON(PHASE_B); //Pullups for rotary encoder
	
	spi_init();

	timer0_on();



	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}

void set_led(uint8_t num, uint8_t value){
	uint8_t mask;
	uint8_t row = num / 4;
	mask = (1<<(num%4));
	
	if(value){
		pixmap[0][row][0] |= mask;
	}else{
		pixmap[0][row][0] &= ~mask;
	}
}
