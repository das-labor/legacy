
#include <stdint.h>
#include <SDL/SDL.h>

#include "gui_lib/graphics.h"
#include "config.h"


extern uint8_t draw_color;
static uint8_t lcd_screen [X_SIZE * Y_SIZE];


void lcd_graphics_plot_pixel(uint16_t x, uint16_t y, uint8_t draw_color){
	x %= X_SIZE;
	y %= Y_SIZE;
	
	lcd_screen [y*X_SIZE + x] = draw_color;
}

void g_clear_screen(void){
	int x;
	for(x=0;x<X_SIZE*Y_SIZE;x++){
		lcd_screen[x] = 0;
	}
}

void g_draw_icon(uint16_t x, uint16_t y, icon_t * i){
	uint16_t w;
	uint16_t h;
	
	uint16_t byte_cnt = 0;
	uint8_t msk;
	uint8_t d = 0;
	
	h = i->height;
	while(h--){
		uint16_t xi = x;
		msk = 0x00;
		w = i->width;
		while(w--){
			if(msk == 0x00){
				d = i->data[byte_cnt++];
				msk = 0x01;
			}
			if(d & msk){
				lcd_graphics_plot_pixel(xi,y,draw_color);
			}
			xi++;
			msk <<= 1;
		}
		y++;
	}
}


void draw_virtual_lcd(SDL_Surface *screen)
{
    Uint32 bg_color;
	Uint32 fg_color;

	if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
	  
 
    bg_color = SDL_MapRGB( screen->format, 0xf0, 0xf0, 0xf0);
  	fg_color = SDL_MapRGB( screen->format, 0x30, 0x30, 0x30);
  
  	int x,y,ytimesw;
  	for(y = 0; y < Y_SIZE; y++ ) 
    {
        ytimesw = y*screen->pitch / 4;
        for( x = 0; x < X_SIZE; x++ ) 
        {
            ((Uint32*) screen->pixels ) [x+ytimesw] = lcd_screen[y*X_SIZE + x] ? fg_color:bg_color;
        }
    }
    
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

