/* Here is a small tutorial how to use this simulator
 * 
 * Even if this simualtor does not emulates the atmel atmega32 it has
 * produced very good results. But you have to think about programming
 * a microcontroller programm and not a pc program.
 * Don't use floating point, and use integer in the smallest possible way.
 * One problem is that int on the avr is 16 bit and on the pc 32 bit. so please 
 * use short insted.
 * 
 * In order to write an new animation you'll have to write a function.
 * In this function you can use the api, that is defint in pixel3d.h. Please
 * read the documentation in the api.h and apic.
 *
 */ 
 
#include "programs.h"

/* Die wichtigsen Hilfsfunktion um Animationen zu prorammieren sind folgende:
 *
 * Mit der Funktion setpixel3d(pixel3d p, unsigned char value)
 * können einzelne LEDs mit den Farbwert value gesetzt werden.
 * 0 = Aus   1 = Dunkel   2 = Mittel   3 = Hell
 * Welche LED gesetzt werden soll wird mit Hilfe der Strucktur pixel3d gemacht.
 * mit setpixel3d((pixel3d) {1, 2, 3}, 2); setzt man an der Position x = 1,
 * y = 2 und z = 3. Die z-Achse zeigt oben, x und y Achse verlaufen horizontal.
 *      z ^   ^ y        
 *        |  /        
 *        | /          
 *        |/                
 *        -------> x
 * Die LED an dem Schnittpunkt der drei Achsen hat x=0, y=0 und z=0.    
 *
 * Mit der Funktion clear_screen(unsigned char value) kann der gesamte Würfel auf
 * einen Farbwerdet gesetzt werden.
 *
 * Mit der Funktion wait(unsigned int ms) wartet das Programm ms viele 
 * Milisekunken (tausentstel Sekunden).
 */

/* Mit Hilfe der Funktion set_pixel3d() sollen alle 8 Ecken der Würfels zum 
 * leuchten gebracht werden und anschließend soll eine Sekunde gewarten werden.
 */
void aufgabe1()
{
	
}

/* Tragen sie in der Datei displayLoop.c (sozusagen in der Playlist) die funktion
 * aufgabe2(); ein so dass sie vor aufgabe1(); ausgeführt wird.
 * Und jetzt kommen wir zur eigentlichen Aufgabe.
 * Es sollen alle LEDs der untersten Ebene zum leuchten zu bringen. Ich empfehle
 * dafür 2 in einander geschachtelte for schleifen zu benutzen.
 * Anschließend soll wieder 1 Sekunde gewartet werden.
 * (maximal 10 Zeilen, Zeilenlänge nicht über 80 Zeichen, ohne die Funktion
 *  set_plane())
 */  
void aufgabe2()
{
}

unsigned char x, y, z;

/* programmieren sie ein dreisimensionales schachbrettmuster
 */
void aufgabe3() 
{
	unsigned char x, y, z;
	for (z = 0; z < LEN_Z; z++) {
		for (y = 0; y < LEN_Y; y++) {
			for (x = 0; x < LEN_X; x++) {
				setpixel3d((pixel3d) {x, y, z}, ((x+y+z)%2) ? 3 : 1);
			}
		}
	}
	wait(1000);
}

/* programmieren sie begendes Strichmänchen. 
 * (Tip mit drawLine3D kann man konfortabel dreidimensionale linien zeichenen)
 */
void aufgabe4()
{
}

/* by Jim Ulery  http://www.azillionmonkeys.com/qed/ulerysqroot.pdf  */
static unsigned isqrt(unsigned long val) {
	unsigned long temp, g=0, b = 0x8000, bshft = 15;
	do {
		if (val >= (temp = (((g << 1) + b)<<bshft--))) {
		   g += b;
		   val -= temp;
		}
	} while (b >>= 1);
	return g;
}


void growingSphere() {
	
	

}


