#include <stdio.h>
#include <SDL/SDL.h>

#include <SDL/SDL_thread.h>

#include "config.h"

#include "touchscreen.h"

#define BPP 4
#define DEPTH 32

volatile uint8_t quit_thread;

int threadFunktion(void *nichtVerwendet)
{
    
    init_main_window();
	
	while(!quit_thread){
		handle_touchscreen();
		netvar_handle_events();
		sleep(10);
	}

    printf("Thread beendet sich.\n");
    return(0);
}

pixel touch_state = (pixel){-1,-1};


int WinMain(int argc, char* argv[])
{
	SDL_Surface *screen;
	SDL_Event event;
	
	int keypress = 0;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
	
	if (!(screen = SDL_SetVideoMode(X_SIZE, Y_SIZE, DEPTH, SDL_HWSURFACE)))
	{
	    SDL_Quit();
	    return 1;
	}
	
	SDL_Thread *thread;
	
	thread = SDL_CreateThread(threadFunktion, NULL);
	if ( thread == NULL ) {
	    fprintf(stderr, "Thread konnte nicht gestartet werden: %s\n", SDL_GetError());
	    return 1;
	}
	
	uint8_t mouse_down = 0;
	
	while(!keypress) 
	{
		//DrawScreen(screen,h++);
		draw_virtual_lcd(screen);
		while(SDL_PollEvent(&event)) 
		{      
			switch (event.type) 
			{
				case SDL_QUIT:
					keypress = 1;
				break;
				case SDL_KEYDOWN:
					keypress = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouse_down = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_down = 0;
					break;
				
			}
		}
		
		if(mouse_down){
			int x,y;
			SDL_GetMouseState(&x, &y);
			touch_state = (pixel){x,y};
			

		}else{
			touch_state = (pixel){-1,-1};
		}
		sleep(15);
	}
	
	quit_thread = 1;
	SDL_WaitThread(thread, NULL);
	SDL_Quit();
	
	return 0;
}



