
#include "util.h"
#include "pixel.h"
#include "anim3d.h"

void test1() {
	unsigned char x, y, z;
	for (z=0;z<3;z++) {	
		for (y=0;y<3;y++){
			for (x=0;x<3;x++){
				setpixel3d((pixel3d){x,y,z}, 3);
				wait(200);
			}
		}
	}
}

void test3() {
	clear_screen(0);
	setpixel3d((pixel3d){0,0,0}, 3);
	wait(300);
	setpixel3d((pixel3d){0,0,0}, 0);
	setpixel3d((pixel3d){1,0,0}, 3);
	setpixel3d((pixel3d){0,1,0}, 3);
	setpixel3d((pixel3d){0,0,1}, 3);
	wait(300);
	setpixel3d((pixel3d){1,0,0}, 0);
	setpixel3d((pixel3d){0,1,0}, 0);
	setpixel3d((pixel3d){0,0,1}, 0);
	setpixel3d((pixel3d){2,0,0}, 3);
	setpixel3d((pixel3d){0,2,0}, 3);
	setpixel3d((pixel3d){0,0,2}, 3);
	setpixel3d((pixel3d){1,1,0}, 3);
	setpixel3d((pixel3d){0,1,1}, 3);
	setpixel3d((pixel3d){1,0,1}, 3);
	wait(300);	
	setpixel3d((pixel3d){2,0,0}, 0);
	setpixel3d((pixel3d){0,2,0}, 0);
	setpixel3d((pixel3d){0,0,2}, 0);
	setpixel3d((pixel3d){1,1,0}, 0);
	setpixel3d((pixel3d){0,1,1}, 0);
	setpixel3d((pixel3d){1,0,1}, 0);
	setpixel3d((pixel3d){2,1,0}, 3);
	setpixel3d((pixel3d){1,2,0}, 3);
	setpixel3d((pixel3d){2,0,1}, 3);
	setpixel3d((pixel3d){1,1,1}, 3);
	setpixel3d((pixel3d){0,2,1}, 3);
	setpixel3d((pixel3d){1,0,2}, 3);
	setpixel3d((pixel3d){0,1,2}, 3);
	wait(300);	
	setpixel3d((pixel3d){2,1,0}, 0);
	setpixel3d((pixel3d){1,2,0}, 0);
	setpixel3d((pixel3d){2,0,1}, 0);
	setpixel3d((pixel3d){1,1,1}, 0);
	setpixel3d((pixel3d){0,2,1}, 0);
	setpixel3d((pixel3d){1,0,2}, 0);
	setpixel3d((pixel3d){0,1,2}, 0);	
	setpixel3d((pixel3d){0,2,2}, 3);
	setpixel3d((pixel3d){2,0,2}, 3);
	setpixel3d((pixel3d){2,2,0}, 3);
	setpixel3d((pixel3d){1,1,2}, 3);
	setpixel3d((pixel3d){2,1,1}, 3);
	setpixel3d((pixel3d){1,2,1}, 3);
	wait(300);
	setpixel3d((pixel3d){0,2,2}, 0);
	setpixel3d((pixel3d){2,0,2}, 0);
	setpixel3d((pixel3d){2,2,0}, 0);
	setpixel3d((pixel3d){1,1,2}, 0);
	setpixel3d((pixel3d){2,1,1}, 0);
	setpixel3d((pixel3d){1,2,1}, 0);
	setpixel3d((pixel3d){1,2,2}, 3);
	setpixel3d((pixel3d){2,1,2}, 3);
	setpixel3d((pixel3d){2,2,1}, 3);
	wait(300);
	setpixel3d((pixel3d){1,2,2}, 0);
	setpixel3d((pixel3d){2,1,2}, 0);
	setpixel3d((pixel3d){2,2,1}, 0);
	setpixel3d((pixel3d){2,2,2}, 3);
	wait(600);
	setpixel3d((pixel3d){2,2,2}, 0);
}

void schachBlinking() {
	pixel3d curr;
	unsigned char odd = 0, waitT = 150;
	clear_screen(0);
	for (; waitT > 30;) {
		for (curr.z = 0; curr.z < 3; curr.z++) {
			for (curr.y = 0; curr.y < 3; curr.y++) {
				for (curr.x = 0; curr.x < 3; curr.x++) {
					setpixel3d(curr, ((curr.x + curr.y + curr.z)% 2) == odd ? 3:0);
				}
			}
		}
		odd = (odd+1)%2;
		wait(waitT -= 2);
	}
}

void schachBlinking2() {
	pixel3d curr;
	unsigned char odd = 0, waitT = 150;
	clear_screen(0);
	for (; waitT > 30;) {
		for (curr.z = 0; curr.z < 3; curr.z++) {
			for (curr.y = 0; curr.y < 3; curr.y++) {
				for (curr.x = 0; curr.x < 3; curr.x++) {
					setpixel3d(curr, ((curr.x + curr.y + curr.z)% 2) == odd ? 3:1);
				}
			}
		}
		odd = (odd+1)%2;
		wait(waitT -= 2);
	}
}


// muss noch erweitert werden, sodass 
void cool(unsigned char waitTime, unsigned char count) {
	unsigned char i = 0;
	signed char add = -1;
	clear_screen(0);
	pixel3d pprev, prev = {0, 1, 0}, curr = {0, 0, 0};
	while (i < count) {
		while (curr.x < 2) {
			pprev = prev;
			prev = curr;
			curr.x++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.y < 2) {
			pprev = prev;
			prev = curr;
			curr.y++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.x) {
			pprev = prev;
			prev = curr;
			curr.x--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.y) {
			pprev = prev;
			prev = curr;
			curr.y--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		pprev = prev;
		prev = curr;
		if (curr.z == 0) 
			add = 1;
		if (curr.z == 2) 
			add = -1;
		curr.z = curr.z + add;
		setpixel3d(curr,3);
		setpixel3d(prev,0);
		setpixel3d(pprev,0);
		wait(waitTime);
		i++;
	}
	i = 0;
	while (i < count) {
		while (curr.y < 2) {
			pprev = prev;
			prev = curr;
			curr.y++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.z < 2) {
			pprev = prev;
			prev = curr;
			curr.z++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.y) {
			pprev = prev;
			prev = curr;
			curr.y--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.z) {
			pprev = prev;
			prev = curr;
			curr.z--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		pprev = prev;
		prev = curr;
		if (curr.x == 0) 
			add = 1;
		if (curr.x == 2) 
			add = -1;
		curr.x = curr.x + add;
		setpixel3d(curr,3);
		setpixel3d(prev,0);
		setpixel3d(pprev,0);
		wait(waitTime);
		i++;
	}	
	i = 0;
	while (i < count) {
		while (curr.z < 2) {
			pprev = prev;
			prev = curr;
			curr.z++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.x < 2) {
			pprev = prev;
			prev = curr;
			curr.x++;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.z) {
			pprev = prev;
			prev = curr;
			curr.z--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		while (curr.x) {
			pprev = prev;
			prev = curr;
			curr.x--;
			setpixel3d(curr,3);
			setpixel3d(prev,1);
			setpixel3d(pprev,0);
			wait(waitTime);
		}
		pprev = prev;
		prev = curr;
		if (curr.y == 0) 
			add = 1;
		if (curr.y == 2) 
			add = -1;
		curr.y = curr.y + add;
		setpixel3d(curr,3);
		setpixel3d(prev,0);
		setpixel3d(pprev,0);
		wait(waitTime);
		i++;
	}
}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char random(){
	static unsigned int muh = 0xAA;
	unsigned char x;
	for(x=0;x<8;x++){
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;

}

void snake(){
	pixel3d pixels[23] = {{1, 1, 0},{1, 2, 0}};
	pixel3d apple = {random()%3, random()%3, random()%3};
	unsigned char i, x, h, head = 1, tail = 0;
	direction dir = forward;
	setpixel3d(pixels[head], 3);
	setpixel3d(pixels[tail], 3);
	clear_screen(0);

	while (1) {
		i = 0;
		x++;
		if (random() < 80) {
			dir = random() % 6;
		}
		while (h = get_next_pixel3d(pixels[head], dir) && i < 7 ) {
			dir = direction_r(dir);
			i++;
		}
		if (i > 6) 
			break;
		clearpixel3d(pixels[tail]);
		if (apple.x == pixels[head].x &&
			apple.y == pixels[head].y &&
			apple.z == pixels[head].z) {
			while (get_pixel3d(apple = (pixel3d){random()%3, random()%3, random()%3}));
		} else {
			tail = (tail + 1) % 23;
		}
		pixels[(head + 1) % 23] = next_pixel3d(pixels[head], dir);
		head = (head + 1) % 23;		
		setpixel3d(pixels[head], 3);
		if (x % 2) {
			setpixel3d(apple, 3);
		} else {
			clearpixel3d(apple);
		}
		wait(100);
		clearpixel3d(apple);
	}

}
