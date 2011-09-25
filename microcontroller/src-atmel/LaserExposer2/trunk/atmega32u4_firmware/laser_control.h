typedef struct{
	uint8_t idle;
	uint8_t mode;
	uint8_t *addr;
	uint16_t buffer_length;
	uint8_t *start_addr;
}laser_driver_t;

extern laser_driver_t laser_driver;

void laser_control_setmode(uint8_t mode);
void init_timer0();
uint8_t laser_control_getmode();
uint8_t laser_control_setaddr(uint8_t *address);
uint8_t laser_control_setbufsize(uint16_t buf);

#define LASER_MODE_OFF 0
#define LASER_MODE_INIT 1
#define LASER_MODE_IDLE 2
#define LASER_MODE_RUNNING 4

#define START_TIMER0 TCCR0B=_BV(CS01)|_BV(CS00);	//clock select	1:64
#define STOP_TIMER0 TCCR0B=0;
#define LASER_ALWAYS_ON {PORTB|=_BV(PB2);}	//TODO: use SPI modes
#define LASER_OFF {PORTB&=_BV(PB2);}

#define ENABLE_SPI_INT SPCR |= _BV(SPIE);
#define DISABLE_SPI_INT SPCR &= ~ _BV(SPIE);
#define ENABLE_SPI SPCR |= _BV(SPE);
#define DISABLE_SPI  SPCR &= ~ _BV(SPE);

#define INIT_SPI { SPCR |= _BV(MSTR)|_BV(SPR0); DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);}	//SPI is master and users clk 1:16
#define FORCE_SPI_MOSI_LOW {SPCR &= ~ _BV(SPE); PORT_SPI &= ~_BV(DD_MOSI);}	//SPI off, Pin low