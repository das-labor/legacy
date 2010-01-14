
#include "../../config.h"
#include "../../compat/pgmspace.h"
#include "../../menu/menu.h"
#include "../../pixel.h"
#include "../../random/prng.h"
#include "../../util.h"
#include "../../joystick/joystick.h"

// if defined, joystick controls are NOT as "seen"
// by the snake but absolute, that is, if pressing
// up, snake goes up, etc.
#define GAME_SNAKE_NEWCONTROL

	// MSB is leftmost pixel
static uint8_t icon[8] PROGMEM =
	{0xff, 0x81, 0xbd, 0xa5, 0xa5, 0xad, 0xa1, 0xbf};  // Snake icon

void snake_game();

game_descriptor_t snake_game_descriptor __attribute__((section(".game_descriptors"))) ={
	&snake_game,
	icon,
};

void snake_game() {
	pixel pixels[64] = {{4, NUM_ROWS-2},{4, NUM_ROWS-3}};
	pixel * head = &pixels[1];
	pixel * tail = &pixels[0];
	pixel old_head;
	pixel apples[10];
	uint8_t joy, joy_old=0xff, joy_cmd=0xff;

	unsigned char x, y, dead = 0;
	unsigned char apple_num = 0;
	direction dir = up;
	
	unsigned char apple_found = 0;
	unsigned char j;

	clear_screen(0);
	
	// zeichne Rahmen
	for (x = 0; x < NUM_COLS; x++) {
		for (y = 0; y < NUM_ROWS; y++) {
			if (((x == 0) || (x == NUM_COLS-1)) || 
			    ((y == 0) || (y == NUM_ROWS-1))) {
				setpixel((pixel) {x, y}, 3);
			}
		}
	}
	
	x = 0;
	while (1) {

		x++;
		old_head = *head;
		++head;
		if (head == pixels + 64) 
				head = pixels;

#ifdef GAME_SNAKE_NEWCONTROL			
		if (joy_cmd != 0xff)
		{
			if ( (dir == left  && joy_cmd != right) ||
			     (dir == right && joy_cmd != left)  ||
			     (dir == up    && joy_cmd != down)  ||
			     (dir == down  && joy_cmd != up)    )
				dir = joy_cmd;
		}
#else
		if (joy_cmd == right) {
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} else if (joy_cmd == left) {
				dir = direction_r(dir);
				dir = direction_r(dir);
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} 
#endif
			
		// kopf einen weiter bewegen
		*head = next_pixel(old_head, dir);
						
		apple_found = 0;	
		
		// pr?fen ob man auf nen Apfel drauf ist 
		for (j = 0; j < apple_num; j++) {
			if ( ( head->x == apples[j].x) && 
			     (head->y == apples[j].y) ){
				apple_found = 1;
				for(; j < apple_num - 1; j++) {
					apples[j] = apples[j+1];
				}
				apple_num--;
				goto apple_se;
			}
		}
		if (get_pixel(*head)) {
			dead = 1;
		}
		apple_se:

		if (!dead) {	
			setpixel(*head, 3);
			
			// setze neue ?pfel
			if ( (apple_num < 9) && (random8() < 10) ) {
				pixel new_apple = (pixel) {(random8() % (NUM_COLS-2))+1, 
										   (random8() % (NUM_ROWS-2))+1};
				if (!get_pixel(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
            // l?sche Ende
			if (!apple_found && !dead) {
				clearpixel(*tail);
				if (++tail == pixels + 64) 
					tail = pixels;
			}
		} else {
			while (tail != head) {
				clearpixel(*tail);
				if ((++tail) > pixels + 64) 
					tail = pixels;
				wait (60);
			}
			break;
		}
		
		for (j = 0; j < apple_num; j++) {
			if (x % 2) {
				setpixel(apples[j], 3);
			} else {
				clearpixel(apples[j]);
			}
		}
		for(j=0;j<20;j++){
			if(JOYISLEFT){
				joy = left;
			}else if(JOYISRIGHT){
				joy = right;
#ifdef GAME_SNAKE_NEWCONTROL
			}else if(JOYISUP) {
				joy = up;
			} else if(JOYISDOWN) {
				joy = down;
#endif
			}else{
				joy = 0xff;
			}
			if(joy != joy_old){
				joy_cmd = joy;
			}
			joy_old = joy;
			wait (5);
		}
	}
}
