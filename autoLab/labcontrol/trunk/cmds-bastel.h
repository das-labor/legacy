#ifndef CMDSBASTEL_H
#define CMDSBASTEL_H

/**
 * Available commands  
 */

#define MAX_CAN_GET_TRY (100)
#define BASTELCMD_IFACE_ADDR (0xa9)
#define BASTELCMD_IFACE_PORT (0x01)
#define BASTELCMD_SENDER_ADDR (0x00)
#define BASTELCMD_SENDER_PORT (0x00)


void cmd_bastel(int argc, char *argv[]);

#endif // CMDSBASTEL_H
