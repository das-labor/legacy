#ifndef HARDWARE_H
#define HARDWARE_H
#include <avr/io.h>

/* Steinzeit...
	#define	PROGTAST			PC0
	#define PROGTAST_DDR		DDRC
	#define PROGTAST_PORT		PORTC
	#define PROGTAST_PIN		PINC
	
	#define PROGLED				PB2
	#define PROGLED_DDR			DDRB
	#define PROGLED_PORT		PORTB
*/

	#define BUZZ_A				PD5
	#define BUZZ_B				PD4
	#define BUZZ_PORT			PORTD
	
	#define DCF					PD5			// OCR1A
	#define DCF_DDR				DDRD
	#define DCF_PORT			PORTD

/* Layer0 (L0_...) ist genau genommen der Hardwarelayer
 * für das HCS. Auch wenn es nicht viel mehr als der
 * RS485-Treiber ist, wird hier alles direkt an der
 * Hardware mit L0 gekennzeichnet. Layer1 greift dann
 * auf diese Schnittstellen zu.
 */
	#define TXEN_CHECK 			1		// Wenn einkompiliert, dann wird im Timerinterrupt auf festhängenden TXEN getestet

	#define L0_RXD				PD0
	#define L0_RXD_DDR			DDRD
	#define L0_RXD_PORT			PORTD
	#define L0_RXD_PIN			PIND
	
	#define L0_TXD				PD1
	#define L0_TXD_DDR			DDRD
	#define L0_TXD_PORT			PORTD
	#define L0_TXD_PIN			PIND
	
	#define L0_DRIVER_EN		PB3
	#define L0_DRIVER_EN_DDR	DDRB
	#define L0_DRIVER_EN_PORT	PORTB 
	 
	#define L0_USE_UART			1
	//#define L0_USE_UART0		1
	
	// Hier definitionen für Devices mit nur einer UART.
	#ifdef L0_USE_UART
		#define L0_UDR		UDR
		#define	L0_UBRRH	UBRRH
		#define L0_UBRRL	UBRRL
		#define L0_UCSRA	UCSRA
		#define L0_UCSRB	UCSRB
		#define L0_UCSRC	UCSRC
		#define L0_UDRIE	UDRIE
		
		// UCSRA Stuff	(RXC,TXC,UDRE,FE,DOR,PE,U2X,MPCM)
		#define L0_RXC		RXC
		#define L0_TXC		TXC
		#define L0_UDRE		UDRE
		
		// UCSRB Stuff (RXCIE,TXCIE,UDRIE,RXEN,TXEN,UCSZ2,RXB8,TXB8)
		#define L0_RXCIE	RXCIE
		#define L0_TXCIE	TXCIE
		#define L0_RXEN		RXEN
		#define L0_TXEN		TXEN	
		
		// UCSRC Stuff (URSEL,UMSEL,UPM1,UPM0,USBS,UCSZ1,UCSZ0,UCPOL)
		#define L0_URSEL	URSEL
		#define L0_UMSEL	UMSEL
		#define L0_UPM1		UPM1
		#define L0_UPM0		UPM0
		#define L0_USBS		USBS
		#define L0_UCSZ1	UCSZ1
		#define L0_UCSZ0	UCSZ0
		#define L0_UCPOL	UCPOL	

		// Interruptvektoren
		#define L0_SIG_UART_RECV	SIG_UART_RECV
		#define L0_SIG_UART_DATA	SIG_UART_DATA
		#define L0_SIG_UART_TRANS	SIG_UART_TRANS
	#endif 

	// Hier definitionen für Devices mit mehreren UARTs, UART0.
	#ifdef L0_USE_UART0
		#define L0_UDR		UDR0
		#define	L0_UBRRH	UBRR0H
		#define L0_UBRRL	UBRR0L
		#define L0_UCSRA	UCSR0A
		#define L0_UCSRB	UCSR0B
		#define L0_UCSRC	UCSR0C
		#define L0_UDRIE	UDRIE0
		
		// UCSRA Stuff	(RXC,TXC,UDRE,FE,DOR,PE,U2X,MPCM)
		#define L0_RXC		RXC0
		#define L0_TXC		TXC0
		#define L0_UDRE		UDRE0
		
		// UCSRB Stuff (RXCIE,TXCIE,UDRIE,RXEN,TXEN,UCSZ2,RXB8,TXB8)
		#define L0_RXCIE	RXCIE0
		#define L0_TXCIE	TXCIE0
		#define L0_RXEN		RXEN0
		#define L0_TXEN		TXEN0	
		
		// UCSRC Stuff (URSEL,UMSEL,UPM1,UPM0,USBS,UCSZ1,UCSZ0,UCPOL)
		#define L0_URSEL	URSEL0
		#define L0_UMSEL	UMSEL0
		#define L0_UPM1		UPM01
		#define L0_UPM0		UPM00
		#define L0_USBS		USBS0
		#define L0_UCSZ1	UCSZ01
		#define L0_UCSZ0	UCSZ00
		#define L0_UCPOL	UCPOL0	

		// Interruptvektoren
		#define L0_SIG_UART_RECV	SIG_USART0_RECV
		#define L0_SIG_UART_DATA	SIG_USART0_DATA
		#define L0_SIG_UART_TRANS	SIG_USART0_TRANS
	#endif 
	 
	#define L0_UDRIE_enable()  	L0_UCSRB |= (1<<L0_UDRIE)
	#define L0_UDRIE_disable() 	L0_UCSRB &= ~(1<<L0_UDRIE)
	#define L0_RX_enable() 		L0_UCSRB |= (1<<L0_RXEN)
	#define L0_RX_disable()		L0_UCSRB &= ~(1<<L0_RXEN)
	// Bus übernehmen
	#define L0_DRIVER_enable()	L0_DRIVER_EN_PORT |= (1<<L0_DRIVER_EN)
	// Bus zurückgeben
	#define L0_DRIVER_disable()	L0_DRIVER_EN_PORT &= ~(1<<L0_DRIVER_EN) 
   
	//! Set the baudrate to 19,200 bps (23) / 38,400 bps (11) using a 7.3728MHz crystal 
	//#define L0_BAUDRATE	11
	
/* In Layer 1 gibt es Hardwareseitig nur den Adresssetztaster
 * und die Status-LEDs.
 */
	#define L1_LED_red_on()			PORTC |= (1<<PC2)
	#define L1_LED_red_off()		PORTC &= ~(1<<PC2)
	#define L1_LED_green_on()		PORTC |= (1<<PC3)
	#define L1_LED_green_off()		PORTC &= ~(1<<PC3)

	#define DCF_on()				DCF_DDR |= (1<<DCF)
	#define DCF_off()				DCF_DDR &= ~(1<<DCF)


	#define L1_BUTTON_pressed()		!(PIND & (1<<PD2))
	
	/*	Bedeutung der LED-Farben
		- aus: 			kein Link
		- Dauerrot: 	keine gültige Adresse im EEPROM
		- Dauergelb: 	Adressfindung
		- grün blitzt:	Device erhält Rahmen
		- rot blitzt: 	Device erhält Daten von anderem Device
		- r+g blitzt:	Device sendet gefüllten Rahmen
	
	*/

/* Sehr allgemeine Definitionen
 * ============================
 */
	// DDRD = 1 = Ausgang
	#define DDRA_INIT			0b00000000
	#define PORTA_INIT			0b11111111
	#define DDRB_INIT			0b00001000
	#define PORTB_INIT			0b11110111
	#define DDRC_INIT			0b00001100
	#define PORTC_INIT			0b11110011
	#define DDRD_INIT			0b00110010
	#define PORTD_INIT			0b11011111


#endif

