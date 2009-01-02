
#define DDR(port) (*(volatile uint8_t*)((&port)-1))

#define DDR_FROM_PIN(pin) (*(volatile uint8_t*)((&pin)+1))

#define PORT_FROM_PIN(pin) (*(volatile uint8_t*)((&pin)+2))
