/* These is a headerfile for all animation programms. I think it is not
 * nessesary to have for every c-file a seperate headerfile. Through this
 * you have also a better overview about the animations.
 *
 * The Comments above the Animations shows the name of the c-file it that they 
 * are defined.
 */
 
#ifndef PROGRAMS_H
#define PROGRAMS_H 

//Includefiles that are needed for nearly every animations
#ifdef AVR
#  include "config.h"
#  include "borg_hw.h"
#  include "uart.h"
#  include "pixel.h"
#  include "scrolltext.h"
#  include <avr/io.h>
#else
#  include "pixel3d.h"
#  include "scrolltext2.h"
#endif

#include "util.h"
#include "joystick.h"

// ----------------------------------------------------------------------------

// displaythread function
void *display_loop(void * unused);

// ----------------------------------------------------------------------------

//testProgramms.c
void test1();
void test2();
void test3();
void joystick_test();
void setplane_test();

//randomAnimations.c
void snake3d();
void matrix();
void feuer();

//rotatedAnimations.c
void spirale();
void rotatePixmap(char animatioNo);
void laborZylinder();
void rotatedScaledCube(); 
void testRotate();
void flury();

//games.c
void pong();
void snake3dJoystick();

//basic.c
void growingCubeFilled();
void coolAnim();
void waves();
void movingArrows();
void planeAnimation2(unsigned char ms);
void gameOfLife(unsigned char, unsigned int);
void joern1();
void serialStream();

//tutorial.c

#endif // PROGRAMS_H

