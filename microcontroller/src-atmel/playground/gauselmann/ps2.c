#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define PS2_C
#include "ps2.h"

#define PORT_PS2 PORTD
#define DDR_PS2 DDRD
#define PIN_PS2 PIND
#define PIN_CLK PD2
#define PIN_DATA PD3

#define CLK_L (!(PIN_PS2 & (1<<PIN_CLK)))
#define CLK_H (PIN_PS2 & (1<<PIN_CLK))
#define DATA_L (!(PIN_PS2 & (1<<PIN_DATA)))
#define DATA_H (PIN_PS2 & (1<<PIN_DATA))

#define S_CLK_L() DDR_PS2 |=  (1<<PIN_CLK)
#define S_CLK_H() DDR_PS2 &=  ~(1<<PIN_CLK)

#define S_DATA_L() DDR_PS2 |=  (1<<PIN_DATA)
#define S_DATA_H() DDR_PS2 &=  ~(1<<PIN_DATA)

#define TX_BUFFER_SIZE 20
#define BUFFER_SIZE 20


uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_buffer_size;//counts how many bytes are in buffer
volatile uint8_t tx_buffer_p;//position in buffer for reading next byte from

unsigned char RX_BUFFER[BUFFER_SIZE];
unsigned char * volatile RX_HEAD=RX_BUFFER, * volatile RX_TAIL=RX_BUFFER;

unsigned char STATE, BF_COUNT;
//States:
//0 - Idle
//1 - TX

unsigned char NEW_DATA=0, DATAR;


static inline uint8_t tx_buffer_get(){
	uint8_t b = tx_buffer[tx_buffer_p];
	tx_buffer_p++;
	if(tx_buffer_p == TX_BUFFER_SIZE)
		tx_buffer_p = 0;
	tx_buffer_size--;
	return b;
}


SIGNAL(SIG_OVERFLOW0){
	static unsigned char count, datas, tx_data[3], datar, parity, tx_bytenum, tx_bytecnt;
	TCNT0 = 256-(F_CPU/(24000l*8));
	switch (STATE){
		case 0:
			if(CLK_L){
				BF_COUNT = 2;
			}else if(DATA_L){
				//data low means PC wants to transmit
				count = 0;
				STATE = 20;
				datar = 0;
			}else if(BF_COUNT){
				BF_COUNT--;
			}else{
				if( (tx_bytenum == 0) && tx_buffer_size ){
					//load new code from buffer, if there is one, and we haven't got one yet
					tx_bytenum = tx_buffer_get(); //first byte in buffer is length of scancode
					uint8_t x;
					for(x=0; x<tx_bytenum; x++){
						tx_data[x] = tx_buffer_get();
					}
					tx_bytecnt = 0;
				}
				if(tx_bytenum){
					datas = tx_data[tx_bytecnt];
					S_DATA_L();
					BF_COUNT = 2;
					STATE = 1;
				}
			}
			break;
		case 1:
			S_CLK_L();
			STATE = 2;
			break;
		case 2:
			S_CLK_H();
			STATE = 3;
			parity = 1;
			count = 0;
			break;
		case 3:
			if(count<16){
				switch(count%2){
					case 0:
						if(CLK_L){ //is PC claiming the Bus back?
							STATE = 0; //yes: go back to start
							tx_bytecnt = 0; //and begin retransmitting the code from first byte
							S_DATA_H();
						}else{
							if(datas & 0x01){
								S_DATA_H();
							
								parity ^= 1;
							}else{
								S_DATA_L();
							}
							datas>>=1;
							S_CLK_L();
						}
						break;
					case 1:
						S_CLK_H();
						break;
				}
				count++;
			}else{
				if(CLK_L){ //is PC claiming the Bus back?
					STATE = 0;
					tx_bytecnt = 0;
					S_DATA_H();
				}else{ //transmit parity bit
					if(parity){
						S_DATA_H();
					}else{
						S_DATA_L();
					}
					S_CLK_L();
					STATE = 4;
				}
			}
			break;
		case 4:
			S_CLK_H();
			STATE = 5;
			break;
		case 5:
			if(CLK_L){//is PC claiming Bus back or parity failure?
				STATE = 0;
				tx_bytecnt = 0;
				S_DATA_H();
			}else{
				S_DATA_H();
				S_CLK_L();
				STATE = 6;
			}
			break;
		case 6:
			//done transmitting this byte.
			S_CLK_H();
			STATE = 0;
			tx_bytecnt++;
			if(tx_bytecnt == tx_bytenum){ //last byte?
				tx_bytenum = 0;//yes: reset bytenum
			}
			break;

		//beginning of receive Statemachine
		case 20:
			if(count < 16){
				//clock in data
				switch(count%2){
					case 0:
						S_CLK_L();
						break;
					case 1:
						S_CLK_H();
						datar >>= 1;
						datar |= DATA_H?0x80:0;
						break;
				}
				count++;
			}else{
				S_CLK_L();
				STATE = 21;
			}
			break;
		case 21:
			S_CLK_H();
			STATE = 22;
			break;
		case 22:
			S_CLK_L();
			STATE = 23;
			break;
		case 23:
			S_CLK_H();
			S_DATA_L();
			STATE = 24;
			break;
		case 24:
			S_CLK_L();
			STATE = 25;
			break;
		case 25:
			S_CLK_H();
			S_DATA_H();
			STATE = 0;
			*RX_HEAD = datar;
			if(++RX_HEAD == RX_BUFFER+BUFFER_SIZE) RX_HEAD = RX_BUFFER;
			break;
	}
}

//internal function for storing 1 byte in tx_buffer
//doesn't check for overflows
static void tx_buffer_put(uint8_t byte){
	uint8_t p;
	p = tx_buffer_p + tx_buffer_size;
	if(p >= TX_BUFFER_SIZE)
		p -= TX_BUFFER_SIZE;
	tx_buffer[p] = byte;
	tx_buffer_size++;
}

//user function, put scancode in buffer
void ps2_put(uint8_t size, uint8_t * data ){
	if(size + tx_buffer_size > TX_BUFFER_SIZE)
		return;
	cli();
	tx_buffer_put(size);
	uint8_t x;
	for(x=0;x<size;x++){
		tx_buffer_put(data[x]);
	}	
	sei();
}

//get 1 commandbyte from host
unsigned char ps2_get_nb(unsigned char * byte){
	if (RX_TAIL != RX_HEAD){
		*byte = *RX_TAIL;
		if(++RX_TAIL == RX_BUFFER + BUFFER_SIZE) RX_TAIL = RX_BUFFER;
		return 1;
	}
	return 0;
}


/*
void key_press(ps2_key key){
	unsigned char data = pgm_read_byte(&key_table[key]);
	ps2_put(1,data,0);
	ps2_put(2, 0xf0, data);
}
*/

void key_make(uint16_t data){
	if(tx_buffer_size + 2 > TX_BUFFER_SIZE)
		return;
	cli();
	if(data & 0x100){ //extended scancode?
		tx_buffer_put(2);//size
		tx_buffer_put(0xe0);
	}else{
		tx_buffer_put(1);//size
	}
	tx_buffer_put((uint8_t)data);
	sei();
}


void key_break(uint16_t data){
	if(tx_buffer_size + 2 > TX_BUFFER_SIZE)
		return;
	cli();
	if(data & 0x100){ //extended scancode?
		tx_buffer_put(3);//size
		tx_buffer_put(0xe0);
	}else{
		tx_buffer_put(2);//size
	}
	tx_buffer_put(0xf0);
	tx_buffer_put((uint8_t)data);
	sei();
}

//blocking versions
void key_make_b(uint16_t data){
	key_make(data);
	while(tx_buffer_size);
}

void key_break_b(uint16_t data){
	key_break(data);
	while(tx_buffer_size);
}

void init_ps2(){
	PORT_PS2 &= ~(1<<PIN_CLK | 1<<PIN_DATA);
	DDR_PS2 &= ~(1<<PIN_CLK | 1<<PIN_DATA);

	TCCR0 = 0x02; //Timer 0 to ck/8
	TIMSK |= 1<<TOIE0;
	
	//key_make(0xaa);

}

