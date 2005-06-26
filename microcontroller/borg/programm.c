void tixiv1(){
	clear_screen();
	CURSOR.mode = set;
	set_cursor(0,0);
	unsigned char clearbit=0;
	CURSOR.direction = right;
	while(1){
	
		while (get_next_pixel()^CURSOR.mode){
			walk(1, 50);
			clearbit=0;
		}
		if (clearbit){
			CURSOR.mode=clear;
		}
		if (!clearbit){
			CURSOR.mode=set;	
		}
		switch (CURSOR.direction){
			case right:
				CURSOR.direction=down;
				break;
			case down:
				CURSOR.direction=left;
				break;
			case left:
				CURSOR.direction=up;
				break;
			case up:
				CURSOR.direction=right;
				break;		
		}
		clearbit=1;		
	}
}


void joern2(){
	clear_screen();
	unsigned int clearbit;
	CURSOR.mode = set;
	CURSOR.direction = right;
	set_cursor(0,0);
	clearbit=0;	
	while(1){
		if (clearbit<5){
			while (!get_next_pixel()){
				walk(1, 25);
				clearbit=0;
			}
			switch (CURSOR.direction){
				case right:
					CURSOR.direction=down;
					clearbit++;
					break;
				case down:
					CURSOR.direction=left;
					clearbit++;
					break;
				case left:
					CURSOR.direction=up;
					clearbit++;
					break;
				case up:
					CURSOR.direction=right;
					clearbit++;
					break;		
			}
		}
		else{
			wait(100);
			unsigned char i;
			unsigned char j;
			CURSOR.mode=clear;
			CURSOR.direction = left;
			for (i=0;i<9;i++){
				for(j=0;j<2;j++){
					walk(i,50);
					switch (CURSOR.direction){
						case right:
							CURSOR.direction=up;
							break;
						case left:
							CURSOR.direction=down;
							break;
						case down:
							CURSOR.direction=right;
							break;
						case up:
							CURSOR.direction=left;
							break;
					}
				}
			}
			clearbit=0;
			CURSOR.direction = right;
			CURSOR.mode = set;
			set_cursor(0,0);
		}
	}
}
void draw_line( unsigned char yabs, signed char delta){
	CURSOR.mode = set;
	set_cursor(yabs, 0);
	unsigned char hr_x = CURSOR.x << 5;
	
	

	while ((CURSOR.x<8)&&(CURSOR.y<7)){
		hr_x +=delta;
		set_cursor(hr_x>>5,CURSOR.y+1);
	};

}
void testline(){
	unsigned char i;
	for(i = 0; i<255; i++){
		draw_line(0,i);
		wait(50);
		clear_screen();
	}
		
}

void schachbrett(unsigned char times){
	unsigned char j;
	for(j=0;j<times;j++){
		unsigned char i;
		for(i = 0; i<8; i++){
			pixmap[i] = 0x55<<(i&0x01);
		}
		wait(100);
		for(i = 0; i<8; i++){
			pixmap[i] = 0xAA>>(i&0x01);
		}
		wait(100);
	}
}

void draw_l(){
	clear_screen();
        setpixel(2,1);
        setpixel(2,2);
        setpixel(2,3);
        setpixel(2,4);
        setpixel(2,5);
        setpixel(2,6);
        setpixel(2,7);
        setpixel(3,7);
        setpixel(4,7);
        setpixel(5,7);
        setpixel(6,7);
}

void draw_a(){
	clear_screen();
        setpixel(3,1);
        setpixel(4,1);
	setpixel(2,2);
	setpixel(5,2);
        setpixel(2,3);
        setpixel(5,3);
        setpixel(2,6);
        setpixel(2,4);
        setpixel(3,4);
        setpixel(4,4);
        setpixel(5,4);
        setpixel(2,5);
        setpixel(5,5);
        setpixel(2,6);
        setpixel(5,6);
        setpixel(5,7);
        setpixel(2,7);
}
void draw_b(){
	clear_screen();
        setpixel(3,1);
	setpixel(2,1);
        setpixel(4,1);
	setpixel(2,2);
	setpixel(5,2);
        setpixel(2,3);
        setpixel(5,3);
        setpixel(2,6);
        setpixel(2,4);
        setpixel(3,4);
        setpixel(4,4);
        setpixel(5,4);
        setpixel(2,5);
        setpixel(5,5);
        setpixel(2,6);
        setpixel(5,6);
        setpixel(2,7);
	setpixel(3,7);
	setpixel(4,7);
}
void draw_o(){
	clear_screen();
        setpixel(3,1);
        setpixel(4,1);
	setpixel(2,2);
	setpixel(5,2);
        setpixel(2,3);
        setpixel(5,3);
        setpixel(2,6);
        setpixel(2,4);
        setpixel(5,4);
        setpixel(2,5);
        setpixel(5,5);
        setpixel(2,6);
        setpixel(5,6);
        setpixel(3,7);
	setpixel(4,7);
}
void draw_r(){
	clear_screen();
        setpixel(3,1);
	setpixel(2,1);
        setpixel(4,1);
	setpixel(2,2);
	setpixel(5,2);
        setpixel(2,3);
        setpixel(5,3);
        setpixel(2,4);
	setpixel(2,5);
	setpixel(2,6);
	setpixel(2,7);
	setpixel(3,4);
	setpixel(3,5);
	setpixel(4,4);
	setpixel(4,6);
	setpixel(5,3);
	setpixel(5,7);
}

void test1(){
unsigned char x,y;
for (y=0;y<8;y++){
	for (x=0;x<8;x++){
		setpixel(x,y);
		wait(100);
	}
}
