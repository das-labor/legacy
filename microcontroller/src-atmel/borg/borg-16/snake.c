
#include "pixel.h"
#include "util.h"
#include "prng.h"
#include "snake.h"
#include "joystick.h"

#define RANDOM8() (random8())

void snake_ki23(){
	pixel pixels[256];
	pixels[0] = (pixel){0, 0};
	pixels[1] = (pixel){0, 1};
	
	pixel * head = &pixels[1];
	pixel * tail = &pixels[0];
	pixel old_head;

	pixel apples[10];
	unsigned char apple_num = 0, schlangenlaenge=2, sauber_verlegt=2;
	
	direction dir = down;

	clear_screen(0);

	unsigned char x=0, dead=0;
	while(1){
		x++;
		old_head = *head;
		if(++head == pixels + 256) head = pixels;
		
		unsigned char dead_cnt=0;
		unsigned char apple_found = 0, j;

		//abkuerzung nehmen?
		uint8_t abkuerzen_ok = 1;
		if (schlangenlaenge > 23 && ((sauber_verlegt<schlangenlaenge && schlangenlaenge > 135) || sauber_verlegt<schlangenlaenge/2 || schlangenlaenge > 223) && old_head.y > 1 && old_head.y != 15) {
			abkuerzen_ok = 0;
			if (old_head.x % 2) {
				// sollrichtung ist hoch
				dir = up;
			} else {
				// sollrichtung ist runter
				dir = down;
			}
		}
		// sind wir in der letzten spalte? dann schnell nach oben!
		if (old_head.x == 15) {
			if(old_head.y != 15 && dir != up) {
				dir = up;
				if (sauber_verlegt>10) sauber_verlegt-=10; else sauber_verlegt=0;
			}
			if (schlangenlaenge > 123) sauber_verlegt = 0;
		} else if (abkuerzen_ok && old_head.y > 1 && old_head.y != 15) { //sind wir weder in einer der beiden oberster noch der unterstern zeile? (sonst: nur std-ki ausführen)
			if (dir == left) {
				if (sauber_verlegt>23) sauber_verlegt-=23; else sauber_verlegt=0;
				//wir sind schon auf ner abkuerzung.
				// liegt in aktueller spalte in soll-fahrtrichtung ein apfel?
				for(j=0;j<apple_num;){
					if (apples[j].x ==  old_head.x){
						break;
					}
					j++;
				}
				if (j<apple_num) {
					if (old_head.x % 2) {
						// sollrichtung ist hoch
						if (apples[j].y <  old_head.y) {
							dir = up;
						}
					} else {
						// sollrichtung ist runter
						if (apples[j].y >  old_head.y) {
							dir = down;
						}
					}
				} else if (old_head.x < 15) {
					// oder liegt in der nächsten spalte ein pixel in nicht der dann geltenden fahrtrichtung?
					for(j=0;j<apple_num;){
						if (apples[j].x ==  old_head.x + 1){
							break;
						}
						j++;
					}
					if (j<apple_num) {
						if (old_head.x % 2) {
							// sollrichtung ist im moment hoch
							if (apples[j].y <  old_head.y) {
								dir = up;
							}
						} else {
							// sollrichtung ist im moment runter
							if (apples[j].y >  old_head.y) {
								dir = down;
							}
						}
					}
				}
			} else {
				// wie sind im moment auf dem std-pfad
				//suche nach am weitesten rechts liegenden apfel der links von mir liegt
				pixel kleinstersapfle = (pixel){15,15};
				uint8_t abkuerzung_gefunden = 0;
				uint8_t normaler_weg_ist_bester = 0;
				for(j=0;j<apple_num;j++){
					if (apples[j].y != 0) { //ignoriere aepfle in der obersten zeile
						if (apples[j].x == old_head.x) {
							normaler_weg_ist_bester = 1;
							break;
						}
						if (apples[j].x >  old_head.x && apples[j].x <= kleinstersapfle.x){
							if (apples[j].x == kleinstersapfle.x) {
								if (apples[j].y < kleinstersapfle.y) {
									if (dir == up) {
										kleinstersapfle = (pixel){apples[j].x,apples[j].y};
										abkuerzung_gefunden = 1;
									}
								} else {
									if (dir == down) {
										kleinstersapfle = (pixel){apples[j].x,apples[j].y};
										abkuerzung_gefunden = 1;
									}
								}
							} else {
								kleinstersapfle = (pixel){apples[j].x,apples[j].y};
								abkuerzung_gefunden = 1;
							}
						}
					}
					if (abkuerzung_gefunden && kleinstersapfle.x == old_head.x + 1) {
						if (dir == up) {
							if (kleinstersapfle.y < old_head.y) {
								normaler_weg_ist_bester = 1;
							}
						} else {
							if (kleinstersapfle.y > old_head.y) {
								normaler_weg_ist_bester = 1;
							}
						}
					}
				}
				if (!normaler_weg_ist_bester) {
					if (abkuerzung_gefunden) {
						if (dir == up) {
							//liegt er unter oder neben mir? dann einen rüber und wenn in passender spalte angekommen wieder auf std-weg
							if (kleinstersapfle.y <= old_head.y) {
								dir = left;
								if (sauber_verlegt>23) sauber_verlegt-=23; else sauber_verlegt=0;
							}
						} else {
							//liegt er ueber oder neben mir? dann einen rüber und wenn in passender spalte angekommen wieder auf std-weg
							if (kleinstersapfle.y >= old_head.y) {
								dir = left;
								if (sauber_verlegt>23) sauber_verlegt-=23; else sauber_verlegt=0;
							}
							
						}
					} else {
						// kein apfel mehr links von uns? dann die ganz kurze abkürzung:
						if (schlangenlaenge<123) {
							dir = left;
							if (sauber_verlegt>23) sauber_verlegt-=23; else sauber_verlegt=0;
						}
					}
				}
			}
		}

		//std-ki
		if (old_head.y == 1 && old_head.x != 15 && old_head.x != 0) {
			dir = direction_r(dir);
			dir = direction_r(dir);
			dir = direction_r(dir);
		} else if(old_head.y == 15) {
			dir = direction_r(dir);
		} else if (old_head.y == 0 && (old_head.x == 0 || old_head.x == 15)) {
			dir = direction_r(dir);
		}

		for(j=0;j<apple_num;j++){
			if ( (next_pixel(old_head, dir).x == apples[j].x) && (next_pixel(old_head, dir).y == apples[j].y) ){
				apple_found = 1;
				apple_num--;
				for(;j<apple_num;j++){
					apples[j]=apples[j+1];
				}
				break;
			}
		}

		if(!apple_found){
			sauber_verlegt++;
			if(get_next_pixel(old_head, dir) || schlangenlaenge > 200 /* hier bis max 254 anpassen */){
				dead = 1;
			}
		} else {
			schlangenlaenge++;
		}

		if(!dead){
			*head = next_pixel(old_head, dir);
			setpixel(*head, 3);

			if( (apple_num<9) && ((RANDOM8()&0xff)<10) ){
				pixel new_apple = (pixel){RANDOM8()%NUM_COLS,RANDOM8()%NUM_ROWS};
				if(!get_pixel(new_apple)){
					apples[apple_num++]=new_apple;
				}
			}

			if(!apple_found){
				clearpixel(*tail);
				if(++tail == pixels + 256) tail = pixels;
			}
		}else{
			while(tail != head){
				clearpixel(*tail);
				if((++tail)>pixels+256) tail = pixels;
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
		
		wait (80);
	}
}


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

