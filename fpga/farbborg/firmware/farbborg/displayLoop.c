#include "config.h"
#include "util.h"
#include "api.h"
#include "testAnim.h"
#include "spike_hw.h"
#include "fileParser.h"
#include "plasmaAnims.h"
#include "animations.h"

#define ADD_ANIMATION(function_ptr) \
        uart_putstr("playing " #function_ptr ""); \
		function_ptr (); 

// Playlist
void *display_loop(void * unused)  {
	while (1) {
		ADD_ANIMATION(took);
		ADD_ANIMATION(Benny);
		ADD_ANIMATION(police); 
		ADD_ANIMATION(rotatingHeard);
		ADD_ANIMATION(plasmaSea);
		ADD_ANIMATION(plasmaBall);
		ADD_ANIMATION(testBlur);
		ADD_ANIMATION(plasmaTest);
		ADD_ANIMATION(spirale);	
		playPlaylist("anims/playlist.apl");
		playAnimFile("anim1.prn");
		ADD_ANIMATION(colorSnake);
		colorMatrix();
		//ADD_ANIMATION(flashLight);
		ADD_ANIMATION(plasmaSnake);
		//ADD_ANIMATION(funkyBeat);
		//ADD_ANIMATION(symetricRoutes);	

		ADD_ANIMATION(plasmaSnake);
		//ADD_ANIMATION(shiftTest);
		ADD_ANIMATION(fadeTest);
		ADD_ANIMATION(symetricRoutes);
		ADD_ANIMATION(cubes);
		//ADD_ANIMATION(brightnesTest);
		ADD_ANIMATION(movingArrows);
		ADD_ANIMATION(upgoingRandom);
		ADD_ANIMATION(planeBall);
		ADD_ANIMATION(wobbeln);
		ADD_ANIMATION(plasmaSnake);
		ADD_ANIMATION(movingCubes);
		ADD_ANIMATION(plasmaSnake);
		ADD_ANIMATION(symetricRandom);
		ADD_ANIMATION(testAnim);
		//ADD_ANIMATION(fnordLicht);
	}
	return 0;
}
