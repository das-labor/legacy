#include <avr/io.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define PS2_C
#include "ps2.h"

#define PORT_PS2 PORTD
#define DDR_PS2 DDRD
#define PIN_PS2 PIND
#define PIN_CLK PD3
#define PIN_DATA PD6

#define CLK_L (!(PIN_PS2 & (1<<PIN_CLK)))
#define CLK_H (PIN_PS2 & (1<<PIN_CLK))
#define DATA_L (!(PIN_PS2 & (1<<PIN_DATA)))
#define DATA_H (PIN_PS2 & (1<<PIN_DATA))

#define S_CLK_L() DDR_PS2 |=  (1<<PIN_CLK)
#define S_CLK_H() DDR_PS2 &=  ~(1<<PIN_CLK)

#define S_DATA_L() DDR_PS2 |=  (1<<PIN_DATA)
#define S_DATA_H() DDR_PS2 &=  ~(1<<PIN_DATA)

#define BUFFER_SIZE 20

unsigned char TX_BUFFER[BUFFER_SIZE];
unsigned char *TX_HEAD=TX_BUFFER, * TX_TAIL=TX_BUFFER;

unsigned char RX_BUFFER[BUFFER_SIZE];
unsigned char *RX_HEAD=RX_BUFFER, * RX_TAIL=RX_BUFFER;

unsigned char STATE, BF_COUNT;
//States:
//0 - Idle
//1 - TX

unsigned char NEW_DATA=0, DATAR;


SIGNAL(SIG_OVERFLOW0){
	static unsigned char count=0, datas, datar, parity, tx_bytenum = 0, tx_bytecnt = 0;
	TCNT0 = 256-(F_CPU/(24000l*8));
	switch (STATE){
		case 0:
			//PORTC &= 0xEF;
			if(CLK_L){
				BF_COUNT = 2;
			}else if(DATA_L){
				count = 0;
				STATE = 20;
				datar = 0;
			}else if(BF_COUNT){
				BF_COUNT--;
			}else if(TX_TAIL != TX_HEAD){
				BF_COUNT = 2;
				STATE = 1;
				switch(tx_bytecnt){
					case 0:
						tx_bytenum = *TX_TAIL;
						if(++TX_TAIL == TX_BUFFER + BUFFER_SIZE) TX_TAIL = TX_BUFFER;
						tx_bytecnt = 1;
					case 1:
						datas = *TX_TAIL;
						break;
					case 2:
						if(TX_TAIL+1 == TX_BUFFER + BUFFER_SIZE){
							datas = *TX_BUFFER;
						}else{
							datas = *(TX_TAIL +1);
						
						}
					break;
				}
				S_DATA_L();
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
						if(CLK_L){
							STATE = 0;
							tx_bytecnt = 1;
							S_DATA_H();
						}else{
							if(datas & 0x01){
								S_DATA_H();
							
								parity ^= 1;
							}else{
								S_DATA_L();
							}
							S_CLK_L();
						}
						datas>>=1;
						break;
					case 1:
						S_CLK_H();
						break;
				}
				count++;
			}else{
				if(CLK_L){
					STATE = 0;
					tx_bytecnt = 1;
					S_DATA_H();
				}else{ 
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
			if(CLK_L){
				STATE = 0;
				tx_bytecnt = 1;
				S_DATA_H();
			}else{
				S_DATA_H();
				S_CLK_L();
				STATE = 6;
			}
			break;
		case 6:
			S_CLK_H();
			STATE = 0;
			switch(tx_bytecnt){
				case 1:
					if(tx_bytenum == 2){
						tx_bytecnt = 2;
					}else{
						if(++TX_TAIL == TX_BUFFER + BUFFER_SIZE) TX_TAIL = TX_BUFFER;
						tx_bytecnt = 0;
					}
					break;
				case 2:
					if(++TX_TAIL == TX_BUFFER + BUFFER_SIZE) TX_TAIL = TX_BUFFER;
					if(++TX_TAIL == TX_BUFFER + BUFFER_SIZE) TX_TAIL = TX_BUFFER;
					tx_bytecnt = 0;
			}
			break;
		case 20:
			if(count < 16){
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
				count = 0;
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


inline void ps2_buffer(unsigned char byte){
	int tmp;
	tmp = (unsigned int)TX_HEAD - (unsigned int)TX_TAIL;
	if(tmp <0) tmp += BUFFER_SIZE;
	
	if(tmp < BUFFER_SIZE-1){
		*TX_HEAD = byte; 
	}
	if(++TX_HEAD == TX_BUFFER + BUFFER_SIZE) TX_HEAD = TX_BUFFER;
}

void ps2_put(unsigned char size, unsigned char byte1, unsigned char byte2){
	cli();
	ps2_buffer(size);
	ps2_buffer(byte1);
	if(size == 2){
		ps2_buffer(byte2);
	}
	sei();
}

unsigned char ps2_get_nb(unsigned char * byte){
	if (RX_TAIL != RX_HEAD){
		*byte = *RX_TAIL;
		if(++RX_TAIL == RX_BUFFER + BUFFER_SIZE) RX_TAIL = RX_BUFFER;
		return 1;
	}
	return 0;
}


void key_press(ps2_key key){
	unsigned char data = pgm_read_byte(&key_table[key]);
	ps2_put(1,data,0);
	ps2_put(2, 0xf0, data);
}

void key_make(ps2_key key){
	unsigned char data = pgm_read_byte(&key_table[key]);
	ps2_put(1, data, 0);
}

void key_break(ps2_key key){
	unsigned char data = pgm_read_byte(&key_table[key]);
	ps2_put(2, 0xf0, data);
}

void init_ps2(){
	PORT_PS2 &= ~(1<<PIN_CLK | 1<<PIN_DATA);
	DDR_PS2 &= ~(1<<PIN_CLK | 1<<PIN_DATA);

	TCCR0 = 0x02; //Timer 0 to ck/8
	TIMSK |= 1<<TOIE0;
}
