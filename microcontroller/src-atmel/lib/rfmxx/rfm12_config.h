#include <avr/io.h>

#define RFM_SPI_WAITCYCLES 20

#define RFM_PORT_NIRQ PORTB
#define RFM_PIN_NIRQ _BV(PB1)
#define RFM_DDR_NIRQ DDRB

#define RFM_PIN_SPICLK _BV(PB0)
#define RFM_PORT_SPICLK PORTB
#define RFM_DDR_SPICLK DDRB

// outgoing from MCU to transceiver
#define RFM_PIN_SPITX _BV(PB2)
#define RFM_PORT_SPITX PORTB
#define RFM_DDR_SPITX DDRB

// incoming from transceiver to MCU
#define RFM_PIN_SPIRX _BV(PB3)
#define RFM_PORT_SPIRX PORTB
#define RFM_DDR_SPIRX DDRB
