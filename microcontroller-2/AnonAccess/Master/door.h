#define MDIR_OPEN  0x01
#define MDIR_CLOSE 0x00

/* Bitmask for the Motor output pins
 * Note: If you plan to change the layout, change the motorsteps
 * array in mctl_step() as well!
 **/

#define MOTOR_MASK 0x3C
#define MOTOR_PORT PORTC
#define MOTOR_DDR DDRC

/* Amount of steps & rounds to execute. The motor is considered
 * blocked when the amount of steps executed exceeds MCTL_NUMSTEPS.
 */
#define MCTL_NUMSTEPS 800
#define MCTL_ROUNDS 3

/* Amount of steps to ignore changes in sensor brightness before a
 * change in brightness is counted as a new round.
 */
#define MCTL_ROUNDTRIGGERLEVEL 100

/* Set of commands to give to motorctl()
 */

#define MCTL_GET_STATE  0x00
#define MCTL_OPEN_DOOR  0x01
#define MCTL_CLOSE_DOOR 0x02
#define MCTL_CALIBRATE  0x03

/* Set of states this implementation may return
 */
#define MCTL_STATE_IDLE        0x00
#define MCTL_STATE_OPENING     0x11
#define MCTL_STATE_CLOSING     0x12
#define MCTL_STATE_CALIBRATING 0x13
#define MCTL_STATE_UNKNOWN     0x04
#define MCTL_STATE_BLOCKED     0x05
#define MCTL_STATE_BUSY        0x10

/* treshold level for light sensor ADC */
#define MCTL_ADC_TRESHOLD 511

/* @description Moves the motor one step into a given direction.
 * @param in_direction The parameter sets the direction. MDIR_OPEN
 * to open, and MDIR_CLOSE to close
 */
inline void mctl_step (uint8_t in_direction);

/* @description Send a "command" to the motor
 */
extern inline void mctl_ctl ( uint8_t in_action );

extern void mctl_tick ( void )
