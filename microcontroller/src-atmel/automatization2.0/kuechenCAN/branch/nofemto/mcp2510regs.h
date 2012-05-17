#ifndef MCP2551REGS_H
#define MCP2551REGS_H

//Registers
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define BFPCTRL 0x0C
#define TXRTSCTRL 0x0D
#define CANSTAT 0x0E
#define CANCTRL 0x0F
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B
#define TEC 0x1C
#define REC 0x1D
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EID8 0x22
#define RXM0EID0 0x23
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM1EID8 0x26
#define RXM1EID0 0x27
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A
#define CANINTE 0x2B
	#define MERRE 7
	#define WAKIE 6
	#define ERRIE 5
	#define TX2IE 4
	#define TX1IE 3
	#define TX0IE 2
	#define RX1IE 1
	#define RX0IE 0
#define CANINTF 0x2C
	#define MERRF 7
	#define WAKIF 6
	#define ERRIF 5
	#define TX2IF 4
	#define TX1IF 3
	#define TX0IF 2
	#define RX1IF 1
	#define RX0IF 0
#define EFLG 0x2D
#define TXB0CTRL 0x30
	#define TXREQ 3
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
	#define EXIDE 3
#define TXB0EID8 0x33
#define TXB0EID0 0x34
#define TXB0DLC 0x35
#define TXB0D0 0x36 

#define RXB0CTRL 0x60
	#define RXM1 6
	#define RXM0 5
	#define RXRTR 3
	// Bits 2:0 FILHIT2:0
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC 0x65
#define RXB0D0 0x66 

//Command Bytes
#define RESET 0xC0
#define READ 0x03
#define READ_RX_BUFFER 0x90
#define WRITE 0x02
#define LOAD_TX_BUFFER 0x40
#define RTS 0x80
#define READ_STATUS 0xA0
#define RX_STATUS 0xB0
#define BIT_MODIFY 0x05

#endif
