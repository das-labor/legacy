/* Part of farbborg. License GPLV3 or later. Copyright by Martin Ongsiek 2007.
 * Programminginterface for animations in the simulator.
 */
#ifndef API_H
#define API_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include "util.h"

// In the most cases only the BASIC API is used!

// BASIC API -------------------------------------------------------------------

#define R 0 //! array index for red
#define G 1 //! array index for green
#define B 2 //! array index for blue

//! Data structure for a 3 dimensional pixel.
/*!	Its used for storing, overgive and compute one voxel.
 *	A common use is also as typecast. For example:
 *
 *   setVoxel((voxel) {1, 2, 3}, blue);
 *
 *     z |   /
 *       |  / y
 *  left | /   down
 *     0 |/________
 *        0       x
 *         front
 */
typedef struct {
	uint8_t x, y, z;
} voxel;

//! Data structure for a color.
/*!	Its used for storing, overgive and compute one color.
 *	There are pedefined colors. They are used like this:
 *   clearScreen(red);
 *  A common use is also as typecast. For example:
 *   clearScreen((color) {255, 0, 0});
 */
typedef struct {
	uint32_t r, g, b;
} color;

//! Predefined colors.
extern color black, white, red, green, blue;

//! working-display-memory (double buffering)
/*! Its a array adressed with the z, y, x coordinates and as last the colorpart
 * 0 (R) for red, 1 (G) for green and 2 (B) for blue.
 * for example:
 * image[3][1][2][G] = 255;
 * sets the green part of the voxel with the coordinate x = 2, y = 1 and z = 3
 * to the value 255.
 * Changes are only shown on the farbborg if you call swapAndWait() or fade()!
 */
extern uint32_t imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

//! Sets the whole cube to a color without swap.
/*! Sets the whole cube image to the color c.
 *  This is only shown on the farbborg if you call swapAndWait() or fade()!
 */
void clearImage(color c);
//! Sets a Voxel to a color
/*! Sets the Voxel on the position pos.x, pos.y, pox.z
 *  to the color c.
 *
 *     z |   /
 *       |  / y
 *  left | /   down
 *     0 |/________
 *        0       x
 *         front
 *
 *  This is only shown on the farbborg if you call swapAndWait() or fade()!
 */
void setVoxel(voxel pos, color c);
//! Copy imag into farborg and wait.
/*! This function flushes the working-image-memory imag
 *  to the pwm-hardware unit or the simulator.
 *  After this it waits ms millisecoungds.
 */
void swapAndWait(unsigned int ms);
//! Fades smoothly to imag.
/*! This function fades form displayed image in "steps" steps a "msProStep"
 *  millisecounds to the current image.
 */
void fade(unsigned int msProStep, unsigned int steps);
//! Prints a text an a value as integer.
/*! It prints <txt> = <val>  for debug issues.
 *  for example: t = 34; dP("t", t);
 *  prints: t = 34 on stdout
 *  val is display a signed number.
 *  Its only available in simulator, so please remove if not needed!!!
 */
void dP(char* txt, int32_t val);

// ADVANCED API ----------------------------------------------------------------


//! For the simulator you have 3 debug_values, that are shown. debug_val_1 can be changed through keys.

extern volatile int debug_val_1, debug_val_2, debug_val_3;


//! Sets the whole cube to a color with swap.
/*! Sets the whole cube to the color c and display it.
 *  swapAndWait() or fade() is NOT needed.
 */
void clearScreen(color c);
//! Puts a char to stdout on simulator and uart on the farbborg.
void uart_putchar(char c);
//! Puts a string to stdout on simulator and uart on the farbborg.
void uart_putstr(char *str);
//! Enum for direction based programming (snake)
/*! Mostly used by the snake aninations. Its needed for the funktions:
 *
 *  	voxel getNextVoxel(voxel pos, direction d);
 *  	direction direction_r(direction dir);
 *      void shift(direction dir);
 *
 *  It gives the direction a name.
 *
 *           back
 *         -------
 *        / up  / |
 *       -------  | right
 * left  |front|  |
 *       |     | /
 *       -------
 *         down
 */
typedef enum {
	noDirection = 0,
	up, down,
	right, left,
	forward, back
} direction;
//! Returns 0 if color of voxel pos is equals black, otherwise 1
unsigned char isVoxelSet(voxel pos);
//! returns the coordinates of the voxel that lays in direction d of pos.(snake)
voxel getNextVoxel(voxel pos, direction d);
//! Change the direction by turning right. (snake)
direction direction_r(direction dir);
//! Gives the color back of the position pos.
color getColor(voxel pos);
//! Shift the whole image in the given direction dir and fills with black.
void shift(direction dir);
//!
/*! You can edit a 3x3x3 cube that is located at 2 <= x <= 4, 2 <= y <= 4 and
 *  2 <= z <= 4 of the real image. Evey voxel is imdiate mirrored in the
 *  7 other cubes. You can easily draw symetric things with it.
 */
void setSymetricVoxel(voxel pos, color c);
//! Blures the whole cube with an 3d gaus filter.
void blur();
//! Makes the image brighter, so that the brithest colorpart is now 255.
void normalize();
//! Sets a voxel with to the color h of HSW colorspace. (rainbowcolors)
void setVoxelH(int x, int y, int z, int h);
//! Draws a 3D Line from p1 to p2 with color value.
void drawLine3D(char px1, char py1, char pz1,
 			    char px2, char py2, char pz2, color value);
//! A very simple pseudorandomgenerator.
unsigned char easyRandom();

// ADVANCED HELP FUNCTIONS -----------------------------------------------------
//! flotingpoint variant of setVoxelH (deprecated)
void setVoxelH_f(int x, int y, int z, float h);

//! Fast ingeger square-root calculation. (plasma)
unsigned isqrt(unsigned long val) ;

//! Converts the H part of HSV colorspace to RGB.
color HtoRGB(int h31bit);

// Sinustabelle
// für plasma
int32_t Sine(int32_t phase);
int32_t Cosi(int32_t phase);

// für 3D  // TODO raus damit
char Sin(unsigned char a);
#define Cos(a) Sin((a)+16)

void rotate(char a, char b, char c, voxel* points,
			voxel* resPoints, int numPoint, voxel rotP);
void scale(char sx, char sy, char sz, voxel* points,
			voxel* resPoints, int numPoint, voxel scaleP);

#endif // API_H
