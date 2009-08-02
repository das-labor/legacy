#include "colorSnake.h"

void colorSnake() {
	pixel3d apples[10];
	pixel3d snake[CSNAKE_LEN]; 
	pixel3d *head = &snake[1];
	pixel3d *tail = &snake[0];
	pixel3d *digest;
	voxel old_head, new_apple;
	color snakeColor = {255, 255, 255};
	color darkColor = {1,1,1}; // beats the race condition two apples on one spot
	color dColor, tColor;
	unsigned char apple_cnt = 0;
	unsigned char dead = 0;
	unsigned char dead_cnt, apple_found, i, d;
	direction dir = randomDirection();

	tail->pp.x = tail->pp.y = tail->pp.z = 2;
	tail->col = snakeColor;
	head->pp = getNextVoxel(tail->pp, dir); 
	head->col = snakeColor;

	clearScreen(black);

	while (!dead) {
		apple_found = 0;
		old_head = head->pp;
		if (++head >= &snake[CSNAKE_LEN]) head = &snake[0];
		// look if there is an apple next to the head:
		for (i = 0; i < apple_cnt; i++) {
			for (d = 1; d < 7; d++) {
				new_apple = getNextVoxel(old_head, (direction)d);
				if (sameCoord(new_apple, apples[i].pp)) {
					apple_found = 1;
					*head = apples[i];
					for(; i < apple_cnt-1; i++) {
						apples[i] = apples[i+1];
					}
					apple_cnt--;
					i = apple_cnt; d = 7; // break(2);
				}
			}
		}
		if (!apple_found) {
			dead_cnt = 0;
			// look for a direction the snake can move to:
			while (isVoxelSet(getNextVoxel(old_head, dir))) {
				if ((dead_cnt++) >= 8) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
			head->pp = getNextVoxel(old_head, dir);
		}
		if (dead) {
			// rollup the snake from the tail:
			while (tail != head) {
				setVoxel(tail->pp, black);
				if (++tail >= &snake[CSNAKE_LEN]) tail = &snake[0];
				swapAndWait(255);
			}
			setVoxel(tail->pp, black);
		} else {
			// look for a prefered direction for next cycle:
			if (easyRandom() < 80) {
				dir = randomDirection();
			}
			// maybe place a new apple:
			if ((apple_cnt < 10) && (easyRandom() < (apple_cnt+1) * 3)) {
				new_apple = randomPos();
				if (!isVoxelSet(new_apple)) {
					apples[apple_cnt].pp = new_apple;
					apples[apple_cnt++].col = randomColor();
				}
			}
			// let the apples blink:
			darkColor.b = darkColor.b?0:1;
			for (i = 0; i < apple_cnt; i++) {
				setVoxel(apples[i].pp, darkColor.b?darkColor:apples[i].col);
			}
			// shorten the snake once in a while:
			tail->col = fadeBGR(tail->col);
			if (sameColor(tail->col, black)) {
				if (head == tail) dead = 1;
				setVoxel(tail->pp, tail->col); // black
				if (++tail >= &snake[CSNAKE_LEN]) tail = &snake[0];
			}
			// move the snake:
			if (!apple_found) {
				digest = tail;
				tColor = digest->col;
				while (digest != head) {
					if (++digest >= &snake[CSNAKE_LEN]) digest = &snake[0];
					dColor = digest->col;
					digest->col = tColor;
					tColor = dColor;
					setVoxel(digest->pp, digest->col);
				}
				setVoxel(tail->pp, black);
				if (++tail >= &snake[CSNAKE_LEN]) tail = &snake[0];
			}
		} // if (!dead)
		swapAndWait(155);
	} // while (!dead)
	// flash the cube two times when the snake is dead:
	clearScreen(snakeColor);
	swapAndWait(255);
	clearScreen(black);
	swapAndWait(255);
	clearScreen(snakeColor);
	swapAndWait(255);
	clearScreen(black);
} // void colorSnake()

