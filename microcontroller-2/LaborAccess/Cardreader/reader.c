
#include "reader.h"
#include "hd44780.h"

#include "foo.h"
#include "i2csw.h"

#define DDR_POWER DDRC
#define PORT_POWER PORTC
#define BIT_POWER 0

#define PORT_SWITCH PORTC
#define PIN_SWITCH PINC
#define BIT_SWITCH 5


#define LOAD_OK 0
#define LOAD_ERROR 1


#define CARDPOWER_ON() PORT_POWER &= ~(1<<BIT_POWER)
#define CARDPOWER_OFF() PORT_POWER |= (1<<BIT_POWER)

#define CARD_IS_IN ((~PIN_SWITCH) & (1<<BIT_SWITCH))


TimerControlBlock   pollTimer, timeoutTimer;             // Declare the control blocks needed for timers


void chipdrive_init(){
	PORT_SWITCH |= (1<<BIT_SWITCH);
	CARDPOWER_OFF();
	DDR_POWER |= (1<<BIT_POWER);
}


//wait for card to be inserted and load it
u08 chipdrive_load(){
	u08 state, oldstate = 0, c;
	//wait until a card is inserted
	while(1){
		if(CARD_IS_IN) break;
		AvrXDelay(&pollTimer, 50);
	}

	AvrXDelay(&pollTimer, 50);
		
	return LOAD_OK;
}

u08 chipdrive_eject(){
	while(1){
		if(CARD_IS_IN==0) break;
		AvrXDelay(&pollTimer, 50);
	}
	return LOAD_OK;
	
}


AVRX_GCC_TASKDEF(reader, 200, 3)
{
	u08 error_count = 0;
	u08 data[6];
	// hd44780_init();
	hd44780_clear_line(0);
			
	hd44780_print("reader");
	chipdrive_init();
	i2cInit();
	
	while(1){
		if (chipdrive_load() != LOAD_OK){
			error_count ++;
		}else{
			uint8_t tmp;
			CARDPOWER_ON();
			hd44780_clear_line(1);
			hd44780_print("Inserted");
			tmp = i2cEeDetect();
			i2cEeRead(0, 6, data);
			hd44780_clear_line(0);
			hexdump_d(data, 3);
			hd44780_clear_line(1);
			hexdump_d(data+3, 3);
			
			AvrXDelay(&pollTimer, 100);
			CARDPOWER_OFF();
		
		}

		if (chipdrive_eject() != LOAD_OK){
			error_count++;
			if (error_count > 2){
				AvrXDelay(&pollTimer, 30000);
			}
		}else{
			//only reset errors on correct eject, because
			//on correct load the card could still be always
			//jammed when ejecting.
			error_count = 0;
			hd44780_clear_line(1);
			hd44780_print("Card out");
			
		}
		
	}
}
