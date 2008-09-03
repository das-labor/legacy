/* Here is a small tutorial how to use this simulator
 * 
 * Even if this simualtor does not emulates the atmel atmega32 it has
 * produced very good results. But you have to think about programming
 * a microcontroller programm and not a pc program.
 * Don't use floating point, and use integer in the smallest possible way.
 * One problem is that int on the avr 16 bit and on the pc 32 bit. so please use short insted.
 * 
 * In order to write an new animation you'll have to write a function.
 * In this function you can use the api, that is defint in pixel3d.h. Please
 * read the documentation in the pixel3d.h and pixel3d.c.
 *
 */
 
// other headers are needed then it is compiled for the microcontroller.
#ifdef AVR
#  include "config.h"
#  include "borg_hw.h"
#  include "uart.h"
#  include "pixel.h"
#  include "scrolltext.h"
#  include <avr/io.h>
#else
#  include "api.h"
#  include "scrolltext2.h"
#endif

#include "util.h"
#include "joystick.h"
#include "programs.h"
