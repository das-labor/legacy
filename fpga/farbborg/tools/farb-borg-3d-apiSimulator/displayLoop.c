#include "config.h"
#include "api.h"
#include "testAnim.h"
#include "spike_hw.h"
#include "fileParser.h"
#include "plasmaAnims.h"
#include "animations.h"
#include "main.h"

extern char *animStr;
extern unsigned int curFrame;

// THIS Makro starts a animation through a function pointer.
// Due to preprocessor trix it genarates to strings
// The first sets the animation name to the name of the function. 
// (And resets the wait counter and stops the skipping.)
// And the secound string is displayes "playing" and the function
// name on the terminal.
#define ADD_ANIMATION(function_ptr) \
        uart_putstr("playing " #function_ptr ""); \
        setAnimName("" #function_ptr ""); \
		function_ptr (); 

//typedef struct  

// Playlist
void *display_loop(void * unused)  {
	playPlaylist("anims/playlist.apl");
	ADD_ANIMATION(movingCubes);
	ADD_ANIMATION(rotatingHeard);
	ADD_ANIMATION(plasmaBall);
	ADD_ANIMATION(plasmaSea);
	ADD_ANIMATION(fnordLicht);
	//while (1) {
	//}
	glutDestroyWindow(win);
    exit(0);
}
