/* door.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <avr/io.h>

#define MDIR_OPEN  0x01
#define MDIR_CLOSE 0x00

/* Bitmask for the Motor output pins
 * Note: If you plan to change the layout, change the motorsteps
 * array in mctl_step() as well!
 **/

#define MOTOR_MASK 0x3C
#define MOTOR_PORT (PORTC)
#define MOTOR_DDR (DDRC)

/* Amount of steps & rounds to execute. The motor is considered
 * blocked when the amount of steps executed exceeds MCTL_NUMSTEPS.
 */
#define MCTL_NUMSTEPS 1200
#define MCTL_ROUNDS 3

/* Amount of steps to ignore changes in sensor brightness before a
 * change in brightness is counted as a new round.
 */
#define MCTL_ROUNDTRIGGERLEVEL 500

/* Set of commands to give to motorctl()
 */

#define MCTL_GET_STATE  0x06
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
extern uint8_t mctl_ctl ( uint8_t in_action );
extern void mctl_tick ( void );
extern void mctl_init ( void );
