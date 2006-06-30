#include "stdio.h"
#include "ser.h"
#include "80c451.h"


enum media {serial, display, port};    // You expand the list to match you're purpose


int media;


void putchar(char);                    // Prototype declaration.
// void disp_putc(char c);
// void port_putc(char c);


void main(void)
{
    ser_init();                        // Initial setup of the internal UART using the 
                                       // library function "ser_init" from ser.h
                                       // "ser_init" however dosn't enable all interrupts (EA) 
                                       // only the serial interrupt (ES) so WE have to do this.
    EA = 1;                            // Enable all interrupts including ES (serial irq)

    media = serial;                    // Set standard IO to the internal UART
    printf_small("Hello World\n");     // Well you know :-)
    
    media = display;                   // Set standard IO to a display device
    printf_small("Hello World\n");
    
    media = port;                      // Set standard IO to port in this case port1
    printf_small("0123456789ABCDEF");  // tricked you didn't I :-)
}


void putchar(char c)
{
    switch(media)
    {
        case serial  : ser_putc(c);    // Send the char to internal UART using the
        break;                         // library function "ser_putc" from ser.h
        
        
/*        case display : disp_putc(c);   // Send the char to the display using the
        break;                         // function disp_putc
        
        
        case port    : port_putc(c);   // Send the char to the port using the
        break;                         // function port_putc
*/        
        default      : ser_putc(c);    // Let's use the Internal UART as default.
    }
}


/*
void disp_putc(char c)                 // Here YOU define how the char is send to the
{                                      // display.
}
*/

/*
void port_putc(char c)                 // Here YOU define how the char is send to the
{                                      // port.
}
*/
