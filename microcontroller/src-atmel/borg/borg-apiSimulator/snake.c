#include "pixel.h"
#include "joystick.h"
#include <stdint.h>

extern unsigned char random();

void snake_game() {
	pixel pixels[64] = {{4, 14},{4, 13}};
	pixel * head = &pixels[1];
	pixel * tail = &pixels[0];
	pixel old_head;
	pixel apples[10];
	uint8_t joy, joy_old, joy_cmd;

	unsigned char x, y, dead = 0;
	unsigned char apple_num = 0;
	direction dir = up;
	
	unsigned char dead_cnt = 0;
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
			
		if (joy_cmd == right) {
				dir = direction_r(dir);
				dir = direction_r(dir);
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} else if (joy_cmd == left) {
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} 
			
		// kopf einen weiter bewegen
		*head = next_pixel(old_head, dir);
						
		apple_found = 0;	
		
		// pr�fen ob man auf nen Apfel drauf ist 
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
			
			// setze neue �pfel
			if ( (apple_num < 9) && (random() < 10) ) {
				pixel new_apple = (pixel) {(random() % (NUM_COLS-2))+1, 
										   (random() % (NUM_ROWS-2))+1};
				if (!get_pixel(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
            // l�sche Ende
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
