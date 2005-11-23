// kbd-demo.c
/*
What it does:
	It displays the contetn of the keyboard-ringbuffer at the LEDs on port-B.
	You can navigate throu the ringbuffer with key-A (backwards), key-B (forwards) and key-D (goto last installed byte)
*/


#include "config.h"
#include "debug.h"
#include <inttypes.h>
#include <avr/io.h>
#include "at-kbd.h"
#include "key-names.h"

byte display_index;


void my_kbd_handler(byte index){
	display_index=index;
	PORTC=kbd_buffer[display_index & KBD_BUFFER_MASK];
}

void wait(int x){
	while (x)
		--x;
}

int main (void)
{
 	DEBUG_INIT();
 	       
	DDRC = 0xFF; //Port C all outputs
	PORTC= 0x00;
    DDRB = 0x00; //Port B all inputs
    PORTB = 0xFF; // interne Pull-Ups an  
	
	kbd_init();
	kbd_event_handler=my_kbd_handler;
	
	while(1){
			/*c it would be unwise to use KEY_C since it also puts the clockline down */
			switch (PINB&0x0f){
				case PRESSED(KEY_A):
					wait (30000);
				//	--display_index;
					{
					//	send_kbd_byte(0xFF);
						
						byte oldstate=display_index;
						send_kbd_byte(0xED); //c LED status func
					//	while (oldstate==display_index)
					//		;
						_delay_ms(100);
						send_kbd_byte(0x02); //c turn NUM-Lock LED on
					
					}
					break;
				case PRESSED(KEY_B):
					wait (30000);
					--display_index;
					break;
				case PRESSED(KEY_D):
					wait (30000);
					display_index=kbd_buffer_index;
			}
			PORTC=kbd_buffer[display_index & KBD_BUFFER_MASK];
	}

	return (0);
}


