#include "programs.h"

#define STREAMER_NUM 30

typedef struct {
	pixel3d start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
} streamer;

void matrix() {
	unsigned int counter = 500; 				//run 500 cycles
	streamer streamers[STREAMER_NUM];
	unsigned char matrix_bright[NUM_PLANES*NUM_ROWS][NUM_COLS];
	unsigned char x, y;
	unsigned char index = 0;
	unsigned char draw;
	unsigned char streamer_num = 0;
	
	while (counter--) {
		unsigned char i, j;
		for (x = 0; x < (LEN_Y * LEN_X); x++)
			for (y = 0; y < (LEN_Y); y++)
				matrix_bright[x][y] = 0;
		
		for (i = 0; i < streamer_num; i++) {
			streamer str = streamers[i];
			
			unsigned char bright = 255; draw = 0;
			for(j = (str.len >> 3); j != 0xFF; j--) { //Draw streamer
				if (j+str.start.y < (NUM_COLS)){
					if (bright >> 6) 
						draw = 1;
					if (bright > matrix_bright[str.start.x][str.start.y+j]) {
						matrix_bright[str.start.x][str.start.y+j] = bright;
					}
				}
				bright -= ((bright >> 5) * str.decay);
			}
			str.len += str.speed>>1;
			streamers[i] = str;
			if (!draw) {
				for(j=i;j<streamer_num-1;j++){
					streamers[j] = streamers[j+1];
				}
				streamer_num--;
				i--;
			}			
		}
		
		for (y = 0; y < LEN_Y; y++)
			for (x = 0; x < (LEN_Y*LEN_Z); x++)
				setpixel3d((pixel3d){x % LEN_X, x / LEN_Z, LEN_X-y-1}, 
				                     matrix_bright[x][y] >> 6);
		
		unsigned char nsc;
		for (nsc = 0; nsc < 6; nsc++) {
			if (streamer_num < STREAMER_NUM){
				unsigned char sy = easyRandom() % (2*LEN_X);
				if (sy > NUM_COLS-1) 
					sy=0;
				streamers[streamer_num] = 
				 			  (streamer){{easyRandom()%(LEN_Y*LEN_Z), sy}, 
				                         0, (easyRandom()%8)+12, index++,
										 (easyRandom()%16)+3
										};
				streamer_num++;	
			}
		}
		wait(80);	
	}
} 


#define FEUER_Y (NUM_ROWS + 3)
#define FEUER_S 26
#define FEUER_N 3
#define FEUER_DIV 44;

void feuer()
{
	unsigned char z, y, x;
	unsigned int  t;
	unsigned char world[LEN_Z][LEN_Y][FEUER_Y];   // double buffer

	for(t=0; t<800; t++) {
		// update lowest line
		for(x=0; x<LEN_X; x++) {
            for(y=0; y<LEN_Y; y++) {
                world[x][y][FEUER_Y-1] = easyRandom();
            }
		}

		// diffuse
		for(z=1; z<FEUER_Y; z++) {
			for(x=0; x<LEN_X; x++) {
                for (y=0; y<LEN_Y; y++) {
					world[x][y][z-1] = (FEUER_N*world[(x-1)%LEN_Y][y][z] + 
					                    FEUER_N*world[(x+1)%LEN_X][y][z] +
						                FEUER_N*world[x][(y-1)%LEN_Y][z] + 
					                    FEUER_N*world[x][(y+1)%LEN_Y][z] +
						                FEUER_S*world[x][y][z]) 
										/ FEUER_DIV;
                }
			}
		}

		// copy to screen
		for(z=0; z<LEN_Z; z++) {
			for(x=0; x<LEN_X; x++) {
                for(y=0; y<LEN_Y; y++) {
					setpixel3d((pixel3d){x,y,7-z}, world[x][y][z] >> 5 );
                }
			}
		}
		wait(70);
	}
}


#define SNAKE_LEN 256

void snake3d(){
	pixel3d pixels[SNAKE_LEN]; 
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;

	pixel3d apples[10];
	unsigned char apple_num = 0;
	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;
	
	direction dir = forward;

	clear_screen(0);

	unsigned char x = 0, dead = 0;
	while (1) {
		x++;
		old_head = *head;
		if (++head == pixels + SNAKE_LEN) 
			head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for (j = 0; j < apple_num; j++) {
			unsigned char i;
			for (i = 0; i < 6; i++) {
				if ((next_pixel3d(old_head, (direction)i).x == apples[j].x) && 
					(next_pixel3d(old_head, (direction)i).y == apples[j].y) &&
				    (next_pixel3d(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i; 
					for(; j < apple_num-1; j++) {
						apples[j] = apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:
		if (!apple_found) {
			while (get_next_pixel3d(old_head, dir)) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}
		if (!dead) {
			*head = next_pixel3d(old_head, dir);
			setpixel3d(*head, 3);
			if (easyRandom() < 80) {
				dir = (direction) (easyRandom() % 6);
			}
			if((apple_num<10) && (easyRandom()<10)) {
				pixel3d new_apple = (pixel3d){easyRandom()%NUM_PLANES,
					                          easyRandom()%NUM_ROWS,
					                          easyRandom()%NUM_COLS};
				if (!get_pixel3d(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found) {
				clearpixel3d(*tail);
				if (++tail == pixels + SNAKE_LEN) tail = pixels;
			}
		} else {
			while (tail != head) {
				clearpixel3d(*tail);
				if ((++tail) > pixels+SNAKE_LEN) 
					tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x % 2) {
				setpixel3d(apples[j], 3);
			} else {
				clearpixel3d(apples[j]);
			}
		}
		wait(10);
	}
}
