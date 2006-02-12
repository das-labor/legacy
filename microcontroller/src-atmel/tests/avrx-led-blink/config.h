#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define TMC8_CK256 (1<<CS02)

#define F_CPU 16000000L
#define F_MCP F_CPU

#define TICKRATE 2500

#define LED 	PORTC           // Change this per your hardware and jumper setup
#define LEDDDR 	DDRC

