#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "spi.h"
#include "uart.h"
#include "mcp2510/can.h"
#include "rfm12/rfm12.h"
#include "uip/uip.h"
#include "uip/uip_arp.h"
#include "uip/uip_neighbor.h"
#include "enc28j60.h"
#include "network.h"
#include "timer.h"

int main(void)
{
    DDRB = 0x16; //(1 << DDB1) | (1 << DDB2) | (1 << DDB4); // 16
    PORTB = 0x16; //(1 << PB1) | (1 << PB2) | (1 << PB4); // 16
    DDRA = 0x01;
    timer_init();
    uart_init(25);
    sei();
    uart_puts_P("Booting ...\n");
    _delay_ms(1000); 

    PORTA = 0x01;
    spi_init();
    uart_puts_P("Init network ...\n");
    network_init();
    uart_puts_P("Init can ...\n");
    can_init();
    uart_puts_P("Init rfm12 ...\n");
    /* init interrupt */
    rfm12_init();
    //uart_puts("enc28j60 revision 0x%x\n", read_control_register(REG_EREVID));



    can_setled(0, 1);

    uart_puts_P("Init done ...\n");
    
    PORTA = 0x00;
    uint16_t i = 0;
    while(1)
    {
        network_process();
        
        timer_process();
        if (i==0)
            PORTA = 0x00;
        if (i==32768)                               
            PORTA = 0x01;
        i++;
    }
}
