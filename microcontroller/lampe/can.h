#ifndef CAN_H
#define CAN_H

#define CAN_RXBUFSIZE 128

typedef enum { normal, sleep, loopback, listenonly, config } mcp_mode_t ;

typedef struct{
	unsigned char flags;
	unsigned char addr_src;
	unsigned char addr_dest;
	unsigned char port_src;
	unsigned char port_dest;
	unsigned char dlc;
	unsigned char data[8];

}can_message;


/* MCP */
void mcp_reset();
void mcp_write(unsigned char reg, unsigned char data);
void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len);
unsigned char mcp_read(unsigned char reg);
void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len);
void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val);
unsigned char mcp_status();
unsigned char mcp_rx_status();
void mcp_setmode( mcp_mode_t mode );
void mcp_setfilter();

/* CAN */ 
void  can_init();
void  can_send(char *buf, unsigned char len);
char *can_rcvpacket();

can_message * can_buffer_get();
void can_transmit();

can_message *can_get();
can_message *can_get_nb();

#endif
