#define PIN_DATA        PINB
#define DDR_DATA        DDRB
#define PORT_DATA       PORTB

#define PORT_HANDSHAKE  PORTD
#define DDR_HANDSHAKE   DDRD
#define PIN_HANDSHAKE   PIND

#define BIT_ATN PD0
#define BIT_ACK PD1

#ifdef HW_REV_2
	#define DATABUS_REVERSED
#endif

#ifdef HW_REV_2
	#define PORT_CONTROL PORTD
	#define DDR_CONTROL DDRD
	
	#define BIT_LP PD2
	#define BIT_FLM PD3
	#define BIT_XCK_ENABLE PD5
	
	#define DDR_DISPLAY_ON DDRE
	#define PORT_DISPLAY_ON PORTE
	#define BIT_DISPLAY_ON PE2
	
	#define DDR_M DDRD
	#define PORT_M PORTD
	#define BIT_M PD4
	
	#define DDR_POWER DDRE
	#define PORT_POWER PORTE
	#define BIT_POWER PE0
#else
	#define PORT_CONTROL PORTD
	#define DDR_CONTROL DDRD
	
	#define DDR_DISPLAY_ON DDRD
	#define PORT_DISPLAY_ON PORTD
	#define BIT_DISPLAY_ON PD2
	#define BIT_LP PD3
	#define BIT_FLM PD4
	#define BIT_XCK_ENABLE PD5
	
	#define DDR_M DDRE
	#define PORT_M PORTE
	#define BIT_M PE2
	
	#define DDR_POWER DDRE
	#define PORT_POWER PORTE
	#define BIT_POWER PE0
#endif


#define X_SIZE 320ul
#define Y_SIZE 240ul

#define LCD_WIDTH  X_SIZE
#define LCD_HEIGHT Y_SIZE


#ifdef BIG_DISPLAY
	#define INTERFACE_BITS 8
#else
	#define INTERFACE_BITS 4
#endif

#define TEST_ATN()        (PIN_HANDSHAKE & _BV(BIT_ATN))
