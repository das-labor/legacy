/* this keyboard interface is based on avr appnote 313 
	keyboard connected to PORTD
	CLOCK   PIND2 (int0)
	DATAPIN PIND4
	to use call init_kb(); sei(); in main
	to get a char getcharkb or getcharx

 modified by Patrick Rudolph 2010
   changed:
   * INT1-Interrupt can now be used (was blocked)
   * enable pullups in init_kb()
   * use #define KB_USE_INT 1/0 to select INT0 or INT1 as clock
   * set pins as input
   * use the correct ISR
   * remove unnecessary edge triggering
   * use GICR instead of GIMSK
*/

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "kb.h"

#define  BUFF_SIZE 16

void decode(unsigned char sc);
void put_kbbuff(unsigned char c);

volatile unsigned char edge, bitcount;                // 0 = neg.  1 = pos.

unsigned char kb_buffer[BUFF_SIZE];
volatile unsigned char *inpt, *outpt;
volatile unsigned char buffcnt;


void init_kb(void)
{
	KB_SEL_DDR	//make pins input
    KB_EN_PULLUPS   //enable pullups
		
    inpt =  kb_buffer;                        // Initialize buffer
    outpt = kb_buffer;
    buffcnt = 0;
#if KB_USE_INT == 0
    GICR|=_BV(INT0);
	MCUCR |= _BV(ISC00); // Set interrupt on any logic change
#else
	GICR|=_BV(INT1);
	MCUCR |= _BV(ISC10); // Set interrupt on any logic change
#endif
    edge = 0;                                // 0 = falling edge  1 = rising edge
    bitcount = 11;
}

#if KB_USE_INT == 0
ISR (INT0_vect)
#else
ISR (INT1_vect)
#endif
{
    static unsigned char data;                // Holds the received scan code
    if (!edge)                                // Routine entered at falling edge
    {
        if(bitcount < 11 && bitcount > 2)    // Bit 3 to 10 is data. Parity bit,
        {                                    // start and stop bits are ignored.
            data = (data >> 1);
            if((PORT_KB & (1<<DATAPIN)))
            {
                data = data | 0x80;            // Store a '1'
            }
        }
        edge = 1;
    } 
    else
    {                               // Routine entered at rising edge
        edge = 0;
        bitcount-=1;
        if(bitcount == 0)                    // All bits received
        {
            decode(data);
            bitcount = 11;
        }
    }
}
   char PROGMEM CodingUnshiftedEN[]=
   {/*   0    1    2    3    4    5    6    7    8    9    a    b    c    d    e   f*/
/*0*/    0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,'^' ,0   ,
/*1*/    0   ,0   ,0   ,0   ,0   ,'q' ,'1' ,0   ,0   ,0   ,'y' ,'s' ,'a' ,'w' ,'2' ,0   ,
/*2*/    0   ,'c' ,'x' ,'d' ,'e' ,'4' ,'3' ,0   ,0   ,' ' ,'v' ,'f' ,'t' ,'r' ,'5' ,0   ,
/*3*/    0   ,'n' ,'b' ,'h' ,'g' ,'z' ,'6' ,0   ,0   ,',' ,'m' ,'j' ,'u' ,'7' ,'8' ,0   ,
/*4*/    0   ,',' ,'k' ,'i' ,'o' ,'0' ,'9' ,0   ,0   ,'.' ,'?' ,'l' ,':' ,'p' ,'-' ,0   ,
/*5*/    0   ,0   ,'\'',0   ,'[' ,'+' ,0   ,0   ,0   ,0   ,13  ,']' ,0   ,'|', 0   ,0   ,
/*6*/    0   ,'<' ,0   ,0   ,0   ,0   ,8   ,0   ,0   ,'1' ,0   ,'4' ,'7' ,0   ,0   ,0   ,
/*7*/    '0' ,',' ,'2' ,'5' ,'6' ,'8' ,0x1b,0   ,0   ,'+' ,'3' ,'-' ,'*' ,'9' ,0   ,0   
   };
   char PROGMEM CodingShiftedEN[]=
   {
/*0*/	 0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,9   ,0   ,0   ,
/*1*/	 0   ,0   ,0   ,0   ,4   ,'Q' ,'!' ,0   ,0   ,0   ,'Y' ,'S' ,'A' ,'W' ,'@' ,0   ,
/*2*/	 0   ,'C' ,'X' ,'D' ,'E' ,'$' ,'#' ,0   ,0   ,' ' ,'V' ,'F' ,'T' ,'R' ,'%' ,0   ,
/*3*/	 0   ,'N' ,'B' ,'H' ,'G' ,'Z' ,'^' ,0   ,0   ,',' ,'M' ,'J' ,'U' ,'&' ,'*' ,0   ,
/*4*/	 0   ,'<' ,'K' ,'I' ,'O' ,')' ,'(' ,0   ,0   ,'>' ,'/' ,'L' ,';' ,'P' ,'_' ,0   ,
/*5*/	 0   ,0   ,'"' ,0   ,'{' ,'=' ,0   ,0   ,0   ,0   ,13  ,'}' ,0   ,'\\',0   ,0   ,
/*6*/	 0   ,'>' ,0   ,0   ,0   ,0   ,8   ,0   ,0   ,'1' ,0   ,'4' ,'7' ,0   ,0   ,0   ,
/*7*/	 '0' ,';' ,'2' ,'5' ,'6' ,'8' ,0x1b,0   ,0   ,'+' ,'3' ,'-' ,'*' ,'9' ,0   ,0   
   };


void decode(unsigned char sc)
{
// Coding us-keyboard, geman keyboard contains multi-byte chars 
// Vector ist indexed by scancode and contains the ascii code at
// the indexed position

    static unsigned char is_up=0, shift = 0, mode = 0;                           

    if (!is_up)                // Last data received was the up-key identifier   
    {                                                                            
        switch (sc)
        {
          case 0xF0 :        // The up-key identifier
            is_up = 1;
            break;

          case 0x12 :        // Left SHIFT
            shift = 1;
            break;

          case 0x59 :        // Right SHIFT
            shift = 1;
            break;

          case 0x05 :        // F1
            if(mode == 0)
                mode = 1;    // Enter scan code mode
            if(mode == 2)
                mode = 3;    // Leave scan code mode
            break;

          default:
            if((mode == 0) || (mode == 3))        // If ASCII mode
            {
              if (sc<128)
              {
                if(!shift)                    // If shift not pressed,
                     {char c=(char) pgm_read_byte(&(CodingUnshiftedEN[sc]));put_kbbuff(c);}
                else
                     {char c=(char) pgm_read_byte(&(CodingShiftedEN[sc]));put_kbbuff(c);}
              }
            }
            else{                            // Scan code mode
		put_kbbuff(sc);
            }

            break;
        }
    } else {
        is_up = 0;                            // Two 0xF0 in a row not allowed
        switch (sc)
        {
          case 0x12 :                        // Left SHIFT
            shift = 0;
            break;

          case 0x59 :                        // Right SHIFT
            shift = 0;
            break;

          case 0x05 :                        // F1
            if(mode == 1)
                mode = 2;
            if(mode == 3)
                mode = 0;
            break;
          case 0x06 :                        // F2
            //clr();
            break;
        }
    }
}

void put_kbbuff(unsigned char c)
{

    if (buffcnt<BUFF_SIZE)                        // If buffer not full
    {
        *inpt = c;                                // Put character into buffer
        inpt++;                                    // Increment pointer

        buffcnt++;

        if (inpt >= kb_buffer + BUFF_SIZE)        // Pointer wrapping
            inpt = kb_buffer;
    }
}

int getcharkb(void)
{
    int byte;

    while(buffcnt == 0);                        // Wait for data

    byte = *outpt;                                // Get byte
    outpt++;                                    // Increment pointer

    if (outpt >= kb_buffer + BUFF_SIZE)            // Pointer wrapping
        outpt = kb_buffer;
    buffcnt--;                                    // Decrement buffer count
    return byte;
}

unsigned char getcharx(void)
{
    unsigned char byte=0;

    if(buffcnt > 0)                        // Wait for data
    {
    byte = *outpt;                                // Get byte
    outpt++;                                    // Increment pointer

    if (outpt >= kb_buffer + BUFF_SIZE)            // Pointer wrapping
        outpt = kb_buffer;
    buffcnt--;                                    // Decrement buffer count
    }
    return byte;
}
