#define PIN_DATA        PINB
#define DDR_DATA        DDRB
#define PORT_DATA       PORTB

#define PORT_HANDSHAKE  PORTD
#define DDR_HANDSHAKE   DDRD
#define PIN_HANDSHAKE   PIND

#define BIT_ATN PD0
#define BIT_ACK PD1

#define TEST_ATN()        (PIN_HANDSHAKE & _BV(BIT_ATN))
