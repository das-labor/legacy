
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "../random/prng.h"
#include "snake.h"
#include "../joystick.h"

#define RANDOM8() (random8())

void snake(){
	pixel pixels[64];
	pixels[0] = (pixel){NUM_COLS/2, NUM_ROWS/2};
	pixels[1] = (pixel){NUM_COLS/2, (NUM_ROWS/2)-1};
	
	pixel * head = &pixels[1];
	pixel * tail = &pixels[0];
	pixel old_head;

	pixel apples[10];
	unsigned char apple_num = 0;
	
	direction dir = up;

	clear_screen(0);

	unsigned char x=0, dead=0;
	while(1){
		x++;
		old_head = *head;
		if(++head == pixels + 64) head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for(j=0;j<apple_num;j++){
			unsigned char i;
			for(i=0;i<4;i++){
				if ( (next_pixel(old_head, i).x == apples[j].x) && (next_pixel(old_head, i).y == apples[j].y) ){
					apple_found = 1;
					dir = i;
					for(;j<apple_num-1;j++){
						apples[j]=apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:

		if(apple_found){
		
		}else{
			while(get_next_pixel(old_head, dir)){
				if((dead_cnt++)==4){
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}

		if(!dead){
			*head = next_pixel(old_head, dir);
			setpixel(*head, 3);
		
			if((RANDOM8()&0xff)<80){
				unsigned char j;
				unsigned char nextapple=0, distx, disty, shortdist=255, xy=0;
				if(!apple_num){
					dir = RANDOM8()%4;
				}else{
					for(j=0;j<apple_num;j++){
						if(head->x > apples[j].x){
							distx = head->x - apples[j].x;
						}else{
							distx = apples[j].x - head->x;
						}
						if(head->y > apples[j].y){
							disty = head->y - apples[j].y;
						}else{
							disty = apples[j].y - head->y;
						}
						if ((distx + disty) < shortdist){
							shortdist = distx + disty;
							nextapple = j;
							xy = (distx > disty)?1:0;
						}
					}
					if(xy){
						dir = (apples[nextapple].x > head->x)?left:right;
					}else{
						dir = (apples[nextapple].y > head->y)?down:up;
					}
				}
			}

			if( (apple_num<9) && ((RANDOM8()&0xff)<10) ){
				pixel new_apple = (pixel){RANDOM8()%NUM_COLS,RANDOM8()%NUM_ROWS};
				if(!get_pixel(new_apple)){
					apples[apple_num++]=new_apple;
				}
			}

			if(!apple_found){
				clearpixel(*tail);
				if(++tail == pixels + 64) tail = pixels;
			}
		}else{
			while(tail != head){
				clearpixel(*tail);
				if((++tail)>pixels+64) tail = pixels;
				wait (60);
			}
			break;
		}
		
		for(j=0;j<apple_num;j++){
			if(x%2){
				setpixel(apples[j], 3);
			}else{
				clearpixel(apples[j]);
			}
		}
		
		wait (SNAKE_DELAY);
	}
}

void snake_game() {
	pixel pixels[64] = {{4, 14},{4, 13}};
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
			
		if (joy_cmd == right) {
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} else if (joy_cmd == left) {
				dir = direction_r(dir);
				dir = direction_r(dir);
				dir = direction_r(dir);
				joy_cmd = 0xff;
		} 
			
		// kopf einen weiter bewegen
		*head = next_pixel(old_head, dir);
						
		apple_found = 0;	
		
		// prfen ob man auf nen Apfel drauf ist 
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
			
			// setze neue pfel
			if ( (apple_num < 9) && (random8() < 10) ) {
				pixel new_apple = (pixel) {(random8() % (NUM_COLS-2))+1, 
										   (random8() % (NUM_ROWS-2))+1};
				if (!get_pixel(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
            // lsche Ende
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

