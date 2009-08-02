#include "config.h"
#include "util.h"
#include "api.h"
#include "testAnim.h"
#include "spike_hw.h"
#include "colorMatrix.h"
#include "colorSnake.h"
#include "fileParser.h"
#include "plasmaAnims.h"

// Playlist
void *display_loop(void * unused)  {
	while (1) {
		plasmaSea();
		plasmaBall();
		testBlur();
		uart_putstr("plasmaTest()\n");
		plasmaTest();
		uart_putstr("spirale()\n");
		spirale();
		
		playPlaylist("anims/playlist.apl");
				
		playAnimFile("anim1.prn");
		uart_putstr("colorSnakle()\n");
		colorSnake();
		uart_putstr("colorMatrix()\n");
		colorMatrix();
		//uart_putstr("flashLight()\n");
		//flashLight();
		uart_putstr("plasmaSnake()\n");
		plasmaSnake();
		//uart_putstr("funkyBeat()\n");
		//funkyBeat();
		//uart_putstr("symetricRoutes()\n");
		//symetricRoutes();	

		uart_putstr("plasmaSnake()\n");
		plasmaSnake();
	    	//uart_putstr("shiftTest()\n");
		//shiftTest();
		uart_putstr("fadeTest()\n");
		fadeTest();
	    	//uart_putstr("symetricRoutes()\n");
		//symetricRoutes();
		//uart_putstr("cubes()\n");
		//cubes();
		//uart_putstr("brightnesTest()\n");	
		//brightnesTest();
		uart_putstr("movingArrows()\n");
		movingArrows();
		uart_putstr("plasmaSnake()\n");
		plasmaSnake();
		uart_putstr("upgoingRandom()\n");
		upgoingRandom();
		uart_putstr("planeBall()\n");
		planeBall();
		uart_putstr("wobbeln()\n");
		wobbeln();
		uart_putstr("plasmaSnake()\n");
		plasmaSnake();
		//uart_putstr("snake()\n");
		//snake();
		uart_putstr("movingCubes()\n");
		movingCubes();
		uart_putstr("plasmaSnake()\n");
		plasmaSnake();
		uart_putstr("symetricRandom()\n");
		symetricRandom();
		uart_putstr("testAnim()\n");
		testAnim();
		//uart_putstr("fnordLicht()\n");
		//fnordLicht();
		
	}
	return 0;
}
