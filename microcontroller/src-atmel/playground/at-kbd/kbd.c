#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>

#define 	cli()   __asm__ __volatile__ ("cli" ::)
#define 	sei()   __asm__ __volatile__ ("sei" ::)

typedef unsigned char byte;
typedef void(t_kbd_event_handler)(byte);

#define EVEN_PARATY	0
#define ODD_PARATY	1

#define KBD_DATA_DDR	DDRD
#define KBD_DATA_OUT_PORT	PORTD
#define KBD_DATA_IN_PORT	PIND
#define KBD_DATA_BIT	0
#define KBD_DATA_IN (((KBD_DATA_IN_PORT) & (1<<(KBD_DATA_BIT)))>>(KBD_DATA_BIT))
#define KBD_CLOCK_DDR	DDRB
#define KBD_CLOCK_PORT	PORTB
#define KBD_CLOCK_BIT	2

#define KBD_BUFFER_MASK 0xff
#define KBD_BUFFER_SIZE (KBD_BUFFER_MASK+1) //size of the ringbuffer in bytes must


/*
About the keyboard:
we use int2 for the clock so we wait for a falling edge and call.
*/

byte kbd_buffer[KBD_BUFFER_SIZE];
byte kbd_buffer_index=(KBD_BUFFER_SIZE-1); // zeigt auf das letzte element
t_kbd_event_handler	*kbd_event_handler=(void*)0;

//init KBD
/*
1) init kbd-buffer
2) set ports correct,
3) set counter correct

*/

void kbd_init(){
	int i;
	for (i=0; i<KBD_BUFFER_SIZE; ++i)
		kbd_buffer[i]=0xff;
	KBD_CLOCK_DDR &= ~(1<<KBD_CLOCK_BIT);
	KBD_DATA_DDR  &= ~(1<<KBD_DATA_BIT);
	cli();
	GICR |= (1<<INT2);		//enable INT2 in General Interupt Control Register (GICR)
	MCUCSR &= ~(1<<ISC2); 	//stitch to trigger on falling edge
	GIFR=(1<<5);		//clear INT2-Flag
	sei();
}

//KBD-Clock-Down Handler
/*

*/

INTERRUPT(SIG_INTERRUPT2){
	static byte c=0; //c counter
	static byte t=0, paraty=0;	//c temporary var. //paraty
	
	byte b=KBD_DATA_IN;
	
	++c;
	if (c==1) {			//c start-bit 
		paraty = 0;
		c -= (b);
	} else {
		if (c<=8){		//c data-bit 1-7
			t |= b?0x80:0;
			paraty ^= b;
			t>>=1;
		} else {
			if (c==9){ 	//c last data bit
				t|=b?0x80:0;
				paraty ^= b;
			} else {
				if (c==10){ //c paraty bit
					paraty ^= b;
				} else {
					if (b) c=0;
					if (paraty == ODD_PARATY){
						//c insert byte to buffer
						kbd_buffer[++kbd_buffer_index & KBD_BUFFER_MASK] = t;
						if (kbd_event_handler)	//c call user defined keyboard event handler
							kbd_event_handler(kbd_buffer_index);
					} else {
						//c what shoul we do on error? 
						//c lets simply ignore it, but you may want to do something esle
					}
					
				}
			}			
		}
	}
}

/*
 send kbd-command
*/
/*
-backup normal kbd-isr;
-deactivate normal kbd-isr
-init timer for init clocking
-generate init colocking
-install send-command kbd-isr
-kill init-timer
-send command via own kbd-isr
-reinstall normal kbd-isr

*/


#define KEY_A	0
#define KEY_B	1
#define KEY_C	2
#define KEY_D	3

#define PRESSED(s) ((byte)(0x0f & (~(1<<(s)))))

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
        
	DDRC = 0xFF; //Port C all outputs
	PORTC= 0x00;
    DDRB = 0x00; //Port B all inputs
    PORTB = 0xFF; // interne Pull-Ups an  
	
	kbd_init();
	kbd_event_handler=my_kbd_handler;
	
	while(1){
			switch (PINB&0x0f){
				case PRESSED(KEY_A):
					wait (300000);
					--display_index;
					break;
				case PRESSED(KEY_B):
					wait (300000);
					++display_index;
					break;
				case PRESSED(KEY_D):
					wait (300000);
					display_index=kbd_buffer_index;
			}
			PORTC=kbd_buffer[display_index & KBD_BUFFER_MASK];
	}

	return (0);
}


