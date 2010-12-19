#ifndef NL_CONFIG_H

/* Maximum size of packets transmitted and received by this
 * bootloader.
 */
#define NL_PACKETSIZE SPM_PAGESIZE+5

/* the position in the eeprom where the device's network address is stored at */
#define NL_ADDRESSPOS 0

/* define how many bytes to use for the address field. this will also add approx. 50 bytes
 * to the binary (!)
 **/
#define NL_ADDRESSSIZE 1

/* address bitmask */
#define NL_ADDRESSMASK 0xFF

/* maximum failure count - increase value in noisy environments.
 *
 * This value may also be set to 0, meaning the bootloader should hang forever
 * in a certain state in case of an error. (!)
 *
 * Note that this value also represents the bootdelay. Once the counter reaches its top,
 * the application is started.
 *
 * */
#define NL_MAXFAILS 0x4ffff

/* set verbosity for bootloader (0 to 2)
 *
 * setting this to values greater than 0 will add a few bytes to the binary.
 *
 * Verbosity Levels:
 *
 * 0 - STFU-Level - don't even complain on errors.
 * 1 - Complain on errors
 * 2 - Send additional debug information
 *
 * */
#define NL_VERBOSITY 0

#endif
