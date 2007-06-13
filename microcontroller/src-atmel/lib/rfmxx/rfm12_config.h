#include <avr/io.h>

#define RFM_SPI_WAITCYCLES 20

// nirq
#define RFM_PORT_NIRQ PORTD
#define RFM_PIN_NIRQ _BV(PD1)
#define RFM_DDR_NIRQ DDRD

// spiclk
#define RFM_PIN_SPICLK _BV(PD3)
#define RFM_PORT_SPICLK PORTD
#define RFM_DDR_SPICLK DDRD

// outgoing from MCU to transceiver
#define RFM_PIN_SPITX _BV(PD4)
#define RFM_PORT_SPITX PORTD
#define RFM_DDR_SPITX DDRD

// incoming from transceiver to MCU
#define RFM_PIN_SPIRX _BV(PD0)
#define RFM_PORT_SPIRX PORTD
#define RFM_DDR_SPIRX DDRD

// chip select (nSEL)
#define RFM_PIN_CS _BV(PD2)
#define RFM_PORT_CS PORTD
#define RFM_DDR_CS DDRD
