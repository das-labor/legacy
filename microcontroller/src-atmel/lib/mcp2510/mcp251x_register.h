#ifndef _MCP251X_REGISTER_H
#define _MCP251X_REGISTER_H

/*
 * MCP2510 Transmitter Registers
 */

//transmit buffer n control register
 #define TXB0CTRL 0x30
 #define TXB1CTRL 0x40
 #define TXB2CTRL 0x50

//transmit buffer n control register bits
#define ABTF 6
#define MLOA 5
#define TXERR 4
#define TXREQ 3
#define TXP1 1
#define TXP0 0

//TXnRTS pin control and status register
#define TXRTSCTRL 0x0D

//TXnRTS pin control and status register bits
#define B2RTS 5
#define B1RTS 4
#define B0RTS 3
#define B2RTSM 2
#define B1RTSM 1
#define B0RTSM 0

//transmit buffer n standard identifier high + low
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define TXB1SIDH 0x41
#define TXB1SIDL 0x42
#define TXB2SIDH 0x51
#define TXB2SIDL 0x52

//transmit buffer n standard identifier low bits
#define EXIDE 3

//transmit buffer n extended identifier high + low
#define TXB0EID8 0x33
#define TXB0EID0 0x34
#define TXB1EID8 0x43
#define TXB1EID0 0x44
#define TXB2EID8 0x53
#define TXB2EID0 0x54

//transmit buffer n data length code
#define TXB0DLC 0x35
#define TXB1DLC 0x45
#define TXB2DLC 0x55

//transmit buffer n data length code bits
#define RTR 6

//transmit buffer n data field byte 0
#define TXB0D0 0x36
#define TXB1D0 0x46
#define TXB2D0 0x56


/*
 * MCP2510 Receiver Registers
 */

//receive buffer n control register
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70

//receive buffer n control register bits
#define RXM1 6
#define RXM0 5
#define RXRTR 3
#define FILHIT0 0

//receive buffer 0 control register bits
#define BUKT 2
#define BUKT1 1

//receive buffer 1 control register bits
#define FILHIT2 2
#define FILHIT1 1

//RXnBF pin control and status register
#define BFPCTRL 0x0C

//RXnBF pin control and status register bits
#define B1BFS 5
#define B0BFS 4
#define B1BFE 3
#define B0BFE 2
#define B1BFM 1
#define B0BFM 0

//receive buffer n standard identifier high + low
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB1SIDH 0x71
#define RXB1SIDL 0x72

//receive buffer n standard identifier low bits
#define SRR 4
#define IDE 3

//receive buffer n extended identifier mid + low
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB1EID8 0x73
#define RXB1EID0 0x74

//receive buffer n data length code
#define RXB0DLC 0x65
#define RXB1DLC 0x75

//receive buffer n data length code bits
#define RTR 6

//receive buffer n data field byte 0
#define RXB0D0 0x66
#define RXB0D0 0x76

//acceptance filter n standard identifier high + low
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19

//acceptance filter n extended ifentifier high + low
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B

//acceptance filter mask n standard identifier high + low
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25

//acceptance filter mask n extended identifier high + low
#define RXM0EID8 0x22
#define RXM0EID0 0x23
#define RXM1EID8 0x26
#define RXM1EID0 0x27


/*
 * MCP2510 Bit Timing Configuration Registers
 */

//configuration register 1
#define CNF1 0x2A

//configuration register 1 bits
#define SJW1 7
#define SJW0 6

//configuration register 2
#define CNF2 0x29

//configuration register 2 bits
#define BTLMODE 7
#define SAM 6
#define PHSEG12 5
#define PHSEG11 4
#define PHSEG10 3
#define PHSEG2 2
#define PHSEG1 1
#define PHSEG0 0

//configuration register 3
#define CNF3 0x28

//configuration register 3 bits
#define WAKFIL 6
#define PHSEG22 2
#define PHSEG21 1
#define PHSEG20 0


/*
 * MCP2510 Error Registers
 */

//transmitter error counter
#define TEC 0x1C

//receiver error counter
#define REC 0x1D

//error flag register
#define EFLG 0x2D

//error flag register bits
#define RX1OVR 7
#define RX0OVR 6
#define TXBO 5
#define TXEP 4
#define RXEP 3
#define TXWAR 2
#define RXWAR 1
#define EWARN 0


/*
 * MCP2510 Interrupt Registers
 */

//interrupt enable register
#define CANINTE 0x2B

//interrupt enable register bits
#define MERRE 7
#define WAKIE 6
#define ERRIE 5
#define TX2IE 4
#define TX1IE 3
#define TX0IE 2
#define RX1IE 1
#define RX0IE 0

//interrupt flag register
#define CANINTF 0x2C

//interrupt flag register bits
#define MERRF 7
#define WAKIF 6
#define ERRIF 5
#define TX2IF 4
#define TX1IF 3
#define TX0IF 2
#define RX1IF 1
#define RX0IF 0


/*
 * MCP2510 CAN Registers
 */

//CAN control register
#define CANCTRL 0x0F

//CAN control register bits
#define REQOP2 7
#define REQOP1 6
#define REQOP0 5
#define ABAT 4
#define CLKEN 2
#define CLKPRE1 1
#define CLKPRE0 0

//CAN status register
#define CANSTAT 0x0E

//CAN status register bits
#define OPMOD2 7
#define OPMOD1 6
#define OPMOD0 5
#define ICOD2 3
#define ICOD1 2
#define ICOD0 1


/*
 * MCP2510 SPI Instructions
 */

#define RESET 0xC0
#define READ 0x03
#define WRITE 0x02
#define RTS 0x80
#define RTS_TXB0 0x81
#define RTS_TXB1 0x82
#define RTS_TXB2 0x84
#define READ_STATUS 0xA0
#define BIT_MODIFY 0x05


/*
 * MCP2515 Additions
 */
#ifdef MCP2515


/*
 * MCP2515 Additional Register Bits
 */

//configuration register 3
#define SOF 7


//can control register one shot mode bit
#define OSM 3


/*
 * MCP2515 Additional SPI Instructions
 */

#define RX_STATUS 0xB0
#define READ_RX_BUFFER 0x90
#define LOAD_TX_BUFFER 0x40

#endif //MCP2515

#endif //_MCP251X_REGISTER_H
