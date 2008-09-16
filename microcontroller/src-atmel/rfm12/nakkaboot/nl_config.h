/* define how many deciseconds (0.1s) to wait for a response
 * from the programming device before booting the application
 * code.
 */
#define NL_BOOTDELAY 20

/* Maximum size of packets transmitted and received by this
 * bootloader.
 */
#define NL_PACKETSIZE 64

/* Define which packet type to respond to
 */
#define NL_PACKETTYPE 0xE0

/* the position in the eeprom where the device's network address is stored at */
#define NL_ADDRESSPOS 0
#define NL_ADDRESSSIZE 2

/* address bitmask */
#define NL_ADDRESSMASK 0xFFFF

/* maximum failure count - increase value in noisy environments.
 *
 * This value may also be set to 0, meaning the bootloader should hang forever
 * in a certain state in case of an error. (!)
 * */
#define NL_MAXFAILS 4096

/* set verbosity for bootloader - higher value will likely increase the size */
#define NL_VERBOSITY 1
