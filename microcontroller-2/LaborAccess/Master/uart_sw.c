#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_sw.h"

#include "AvrXFifo.h"

#define BAUDRATE 38400

#define nop() __asm volatile ("nop")

#define RX1_BUFSZ 10
#define TX1_BUFSZ 10

AVRX_DECL_FIFO(Rx1Buf, RX1_BUFSZ);
AVRX_DECL_FIFO(Tx1Buf, TX1_BUFSZ);


#ifdef SUART_TXD
    #define SUART_TXD_PORT PORTD
    #define SUART_TXD_DDR  DDRD
    #define SUART_TXD_BIT  PD3
    static volatile uint16_t outframe;
#endif // SUART_TXD

#ifdef SUART_RXD
    #define SUART_RXD_PORT PORTD
    #define SUART_RXD_PIN  PIND
    #define SUART_RXD_DDR  DDRD
    #define SUART_RXD_BIT  PD2
    static volatile uint16_t inframe;
    static volatile uint8_t inbits, received;

#   ifdef _FIFO_H_
    #define INBUF_SIZE 4
    static uint8_t inbuf[INBUF_SIZE];
    fifo_t infifo;
#   else // _FIFO_H_
    static volatile uint8_t indata;
#   endif // _FIFO_H_

#endif // SUART_RXD



void InitSerial1()
{
   AVRX_INIT_FIFO(Rx1Buf);
   AVRX_INIT_FIFO(Tx1Buf);

    // Mode #4 für Timer1
    // und volle MCU clock
    // IC Noise Cancel
    // IC on Falling Edge
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS10) | (0 << ICES1) | (1 << ICNC1);

    // PoutputCompare für gewünschte Timer1 Frequenz
    OCR1A = (uint16_t) ((uint32_t) F_CPU/BAUDRATE);
   
#ifdef SUART_RXD
    SUART_RXD_DDR  &= ~(1 << SUART_RXD_BIT);
    SUART_RXD_PORT |=  (1 << SUART_RXD_BIT);
    TIFR = (1 << ICF1) | (1 << OCF1B);
    TIMSK |= (1 << TICIE1);
#else    
    TIMSK &= ~(1 << TICIE1);
#endif // SUART_RXD

#ifdef SUART_TXD
    TIFR = (1 << OCF1A);
    SUART_TXD_PORT |= (1 << SUART_TXD_BIT);
    SUART_TXD_DDR  |= (1 << SUART_TXD_BIT);
    outframe = 1;
#endif // SUART_TXD
}



#ifdef SUART_TXD
int put_c1(char c)	// Non blocking output
{
	int retc;
	retc = AvrXPutFifo(Tx1Buf, c);
	TIFR   = (1 << OCF1A);
	TIMSK |= (1 << OCIE1A); 
	return retc;
}

int put_char1( char c)	// Blocking output
{
	AvrXWaitPutFifo(Tx1Buf, c);
	TIFR   = (1 << OCF1A);
	TIMSK |= (1 << OCIE1A);
	return 0;
}
#endif // SUART_TXD


#ifdef SUART_TXD

AVRX_SIGINT(SIG_OUTPUT_COMPARE1A)
{
	IntProlog();
    sei();
    uint16_t data = outframe;
   
    if (data & 1)      SUART_TXD_PORT |=  (1 << SUART_TXD_BIT);
    else               SUART_TXD_PORT &= ~(1 << SUART_TXD_BIT);
   
    if (1 == data)
    {
    	int c = AvrXPullFifo(Tx1Buf);	// Return -1 if empty (enables interrupts)
		if (c >= 0)						// Tricky tight code: only checking sign
		{
			// frame = *.P.7.6.5.4.3.2.1.0.S   S=Start(0), P=Stop(1), *=Endemarke(1)
    		outframe = (3 << 9) | (((uint8_t) c) << 1);
		}else{
			TIMSK &= ~(1 << OCIE1A);    //Fifo is empty, so we disable the interrupt
		}
    }else{
    	outframe = data >> 1;
    }   
    Epilog();
}
#endif // SUART_TXD

#ifdef SUART_RXD
SIGNAL (SIG_INPUT_CAPTURE1)
{
    IntProlog();
    
    uint16_t icr1  = ICR1;
    uint16_t ocr1a = OCR1A;
	sei();
   
    // Eine halbe Bitzeit zu ICR1 addieren (modulo OCR1A) und nach OCR1B
    uint16_t ocr1b = icr1 + ocr1a/2;
    if (ocr1b >= ocr1a)
        ocr1b -= ocr1a;
    OCR1B = ocr1b;
   
    TIFR = (1 << OCF1B);
    TIMSK = (TIMSK & ~(1 << TICIE1)) | (1 << OCIE1B);
    inframe = 0;
    inbits = 0;
    Epilog();
}
#endif // SUART_RXD

#ifdef SUART_RXD
SIGNAL (SIG_OUTPUT_COMPARE1B)
{
	IntProlog();
    
    uint16_t data = inframe >> 1;
   
    if (SUART_RXD_PIN & (1 << SUART_RXD_BIT))
        data |= (1 << 9);
      
    uint8_t bits = inbits+1;
   
    if (10 == bits)
    {
        if ((data & 1) == 0){
            if (data >= (1 << 9))
            {
            	AvrXPutFifo(Rx1Buf, data >> 1);
            }
        }
      
        TIMSK = (TIMSK & ~(1 << OCIE1B)) | (1 << TICIE1);
        TIFR = (1 << ICF1);
    }
    else
    {
        inbits = bits;
        inframe = data;
    }
	Epilog();
}
#endif // SUART_RXD

#ifdef SUART_RXD
int get_c1(void)	// Non blocking, return status outside of char range
{
	int retc = AvrXPullFifo(Rx1Buf);
	return retc;
}

int get_char1(void)	// Blocks waiting for something
{
	return AvrXWaitPullFifo(Rx1Buf);
}
#endif // SUART_RXD

