#ifndef CMDSPOWERCOMMANDER_H
#define CMDSPOWERCOMMANDER_H

/**
 * Available commands  
 */

#define MAX_CAN_GET_TRY (100)
#define POWERCMD_IFACE_ADDR (0x02)
#define POWERCMD_IFACE_PORT (0x01)
#define POWERCMD_SENDER_ADDR (0x00)
#define POWERCMD_SENDER_PORT (0x00)


void cmd_powercommander(int argc, char *argv[]);

#endif // CMDSBASE_H
