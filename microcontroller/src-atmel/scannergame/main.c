//system initialization
void init()
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//debug LED output
	DDRD |= _BV(PD7);
	
	// Must be output else SPI will fail
//	DDRB |= _BV(PB2);

	//initialize ir subsystem
	ir_init();

	//initialize spi port
	spi_init();
	
	myaddr = eeprom_read_byte(0x00);
	
	//turn on interrupts
	sei();
}

int main(void)
{	
	//system initialization
	init();
		
	//the main loop continuously handles can messages
	while (1)
	{	
		//check uart: TODO

		PORTD &= ~_BV(PD7);
	}
}
