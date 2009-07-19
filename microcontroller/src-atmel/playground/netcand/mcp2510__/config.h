#ifndef CONFIG_H
#define CONFIG_H

// can.[ch] defines
#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
//#define CAN_HANDLEERROR
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt
#define F_MCP 16000000
#define MCP_INT_VEC INT1_vect
#define MCP_INT_MASK INT1
#define MCP_INT_FLAG ISC11


#endif
