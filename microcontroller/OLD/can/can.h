#ifndef CAN_H
#define CAN_H

typedef enum { normal, sleep, loopback, listenonly, config } mcp_mode_t ;

/* MCP */
void mcp_reset();
void mcp_write(unsigned char reg, unsigned char data);
void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len);
unsigned char mcp_read(unsigned char reg);
void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len);
void mcp_setmode( mcp_mode_t mode );
void mcp_setfilter();

/* CAN */ 
void  can_init();
void  can_send(char *buf, unsigned char len);
char *can_rcvpacket();

#endif
