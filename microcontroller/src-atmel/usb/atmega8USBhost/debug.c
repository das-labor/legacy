
// ---------
// - Debug -
// ---------


#ifndef NO_AVR_GCC // this won't work without <avr/io.h>

void usart_putchar(uint8_t char_to_put)
// Waits for transmit flag to become clear, then sends a single byte.
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = char_to_put;
}


void usart_putstr(char *str_to_put)
// Transmits the given string over USART.
{
    uint8_t iter;
    uint8_t char_to_put;
    for (iter = 0; (char_to_put = str_to_put[iter]); iter++) {
        usart_putchar(char_to_put);
    }
}

void send_debug()
// Sends whatever is in the print buffer out on the USART. Takes awhile.
{
    #define USART_BAUDRATE 9600
    #define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

    uint8_t index;
    
    index = 0;

    // enable usart for transmit and receive
    UCSRB |= (1 << TXEN);   
    // 8-bit chars (uint8_t)
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); 

    // load lower 8-bits of the baud rate value into the low byte of the UBRR register
    UBRRL = BAUD_PRESCALE; 
    // load upper 8-bits of the baud rate value into the high byte of the UBRR register
    UBRRH = (BAUD_PRESCALE >> 8); 

    usart_putstr(print_buffer);

    _delay_ms(10);

    #undef USART_BAUDRATE
    #undef BAUD_PRESCALE
}
#endif
