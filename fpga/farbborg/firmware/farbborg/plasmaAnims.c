#include "plasmaAnims.h"
#include "config.h"
#include "api.h"
#include <math.h>
#include <stdlib.h>

float offset;
unsigned int ioffset;

#define fMAX_X (float)MAX_X
#define fMAX_Y (float)MAX_Y
#define fMAX_Z (float)MAX_Z

//anim specific
#define PLASBALL_ITERATIONS 8
#define PLASBALL_M_FILTER 15
#define SNAKE_LEN 100

//math :-)
#define PI 3.14159265
#define SQUARE(x) ((x)*(x))
#define max(x, y) (((x) > (y))?(x):(y))
#define min(x, y) (((x) < (y))?(x):(y))
#define abs(x) (((x) < 0)?-(x):(x))

void plasmaSnake()
{
	float scale = 4.0;
	
	voxel pixels[SNAKE_LEN]; 
	voxel *head = &pixels[1];
	voxel *tail = &pixels[0];
	voxel old_head;
	float snakeColor;
	voxel apples[10];
	unsigned char apple_num = 0;
	//char addR = 2, addG = -1, addB = 1;
	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;
	
	direction dir = forward;

	clearScreen(black);

	unsigned char x = 0, dead = 0;
	while (1) {
		offset += 0.09;
		x++;
		old_head = *head;
		if (++head == pixels + SNAKE_LEN) 
			head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for (j = 0; j < apple_num; j++) {
			unsigned char i;
			for (i = 1; i < 7; i++) {
				if ((getNextVoxel(old_head, (direction)i).x == apples[j].x) && 
					(getNextVoxel(old_head, (direction)i).y == apples[j].y) &&
				    (getNextVoxel(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i+1; 
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
			while (isVoxelSet(getNextVoxel(old_head, dir))) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}
		if (!dead) {
			*head = getNextVoxel(old_head, dir);
			snakeColor = 0.5;
			snakeColor += 0.5 * sinf(((float)head->x * (PI / (fMAX_X * scale))) + ((float)head->y * (PI / (fMAX_Y * scale))) + ((float)head->z * (PI / (fMAX_Z * scale))) + offset);
			setVoxelH_f(head->x, head->y, head->z, snakeColor);
			if (easyRandom() < 80) {
				dir = 1 + (direction) (easyRandom() % 6);
			}
			if ((apple_num<10) && (easyRandom()<10)) {
				voxel new_apple = (voxel) {easyRandom() % MAX_X,
										   easyRandom() % MAX_Y,
										   easyRandom() % MAX_Z};
				if (!isVoxelSet(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found) {
				setVoxel(*tail, black);
				if (++tail == pixels + SNAKE_LEN) 
					tail = pixels;
			}
		} else {
			while (tail != head) {
				setVoxel(*tail, black);
				if ((++tail) > pixels + SNAKE_LEN) 
					tail = pixels;
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x & 1) { // let the apples blink
				snakeColor = 0.5;
				snakeColor += 0.5 * sinf(((float)apples[j].x * (PI / (fMAX_X * scale))) + ((float)apples[j].y * (PI / (fMAX_Y * scale))) + ((float)apples[j].z * (PI / (fMAX_Z * scale))) + offset);
				setVoxelH_f(apples[j].x, apples[j].y, apples[j].z, snakeColor);
			} else {
				setVoxel(apples[j], black);
			}
		}
		/*if (snakeColor.r < 5 || snakeColor.r > 250) 
			addR = -addR;
		if (snakeColor.g < 5 || snakeColor.g > 250) 
			addG = -addG;
		if (snakeColor.b < 5 || snakeColor.b > 250) 
			addB = -addB;
			
		snakeColor.r += addR;
		snakeColor.g += addG;
		snakeColor.b += addB;
		*/
		swapAndWait(90);
	}
}


#define SQRT0x8000 181
#define SQRT25x3   0x45483
void plasmaTest()
{
	int32_t col, scale;
	int32_t sqx, sqy, sqz;
	int32_t x, y, z;
	scale = 3*0x5100;
	ioffset = 0;
	
	while (1)
	{
		ioffset += 290; //700;
		
		for(x = 0; x < MAX_X; x++)
		{
			for(y = 0; y < MAX_Y; y++)
			{
				for(z = 0; z < MAX_Z; z++)
				{
					//reset color;
					col = 0;
					
					//diagonal scrolling sine 
					col += 0x2000 * Sine((-x * (0x8000*0x8000 / (MAX_X * scale))) + 
							               (y * (0x8000*0x8000 / (MAX_Y * scale))) + 
										   (-z * (0x8000*0x8000 / (MAX_Z * scale))) + 
										   ioffset
										   ) / 0x8000;
					
					//polar sine
					sqx  = x - 2;
					sqx *= sqx*0x8000;
					sqy  = y - 2;
					sqy *= sqy*0x8000;
					sqz  = z - 2;
					sqz *= sqz*0x8000;
					col += 0x8000/5 * Sine(
						isqrt(sqx + sqy + sqz)*SQRT0x8000/8 + ioffset + 
					    (x*y*z*0x8000*20)/(scale*(1+x+y+z))
				    ) / 0x8000;  //end of sine
					col +=  (
							  (0x3200 * Sine(( x * (0x8000*0x8000 / (MAX_X * scale))) + ioffset) / 0x8000) 
							+ (0x5300 * Sine((-y * (0x8000*0x8000 / (MAX_Y * scale))) + ioffset) / 0x8000)
							+ (0x4400 * Sine((-z * (0x8000*0x8000 / (MAX_Z * scale))) + ioffset) / 0x8000)
							) / (0x13000);
					
					//colorspace offset
					col += 0x2500 + (ioffset/32);

					setVoxelH(x,y,z,col);
				} 
				//printf("\n");
			}
			//printf("\n");
		}
		//printf("\n");
		
		fade(10,2);
		
		if ((ioffset) >= 4*0x8000)
			break;
	}
	clearImage(black);
	fade(15, 100);
}


//polar coordinate sine
//-> sine of distance from centerpoint (note the pythagoras to calc the distance)
//here are some defines to calculate and precache the precacheable values
#define BORGDIAMETER (isqrt(SQUARE(MAX_X) + SQUARE(MAX_Y)))
#define ISQPX(x) (SQUARE(x - (MAX_X / 2)))
#define ISQPY(y) (SQUARE(y - (MAX_Y / 2)))

int plasmaPolarFlat(int sqpx, int sqpy, int scale, int borgDiameter, int ioff)
{
	return Sine(isqrt(sqpx + sqpy) * (0xFFFF / ((scale * borgDiameter) / 2)) + ioff);
//end of sine
}

int plasmaDiag(int x, int y, int z, int scale, int ioff)
{
	return Sine((-x * (0x8000*0x8000 / (MAX_X * scale))) + (y * (0x8000*0x8000 / (MAX_Y * scale))) + (-z * (0x8000*0x8000 / (MAX_Z * scale))) + ioff);
}


//plasma sea support stuff
typedef struct
{
	voxel vPos;
	unsigned int zDist;
	unsigned int speed;
	int spawnFlag;
} plasmaSeaDrop;

typedef struct
{
	voxel center;
	unsigned int pDist;
	unsigned int speed;
	unsigned int myIOff;
} plasmaSeaWave;

typedef struct
{
    plasmaSeaWave element;
    void *next; //FIXME: howto forward declare this type, so we can use a proper pointer ?
} plasmaSeaWaveList;

#define PLASSEA_MAXDROPS 3
#define PLASSEA_ITERATIONS 8
#define PLASSEA_PLASMASPEED 64
#define PLASSEA_M_FILTER 28
#define PLASSEA_Z_HEADROOM 4
#define PLASSEA_ZDIST_RESSCALE 24
#define PLASSEA_ZDISTMAX ((PLASSEA_Z_HEADROOM + MAX_Z) * PLASSEA_ZDIST_RESSCALE)
#define PLASSEA_PDISTMAX (MAX_Z * PLASSEA_ZDIST_RESSCALE)

#define false 0
void plasmaSea()
{
	int32_t col, scale, tmpR, tmpG, tmpB;
	int32_t x, y, z, i;
	int sqx, sqy, dingsVal, borgDiameter, distCalc;
	color colRGB;
    plasmaSeaWaveList dummy;
	plasmaSeaWaveList *head = &dummy, *runner;
	
	//drops
	plasmaSeaDrop drops[PLASSEA_MAXDROPS];

	scale = 16;
	borgDiameter = BORGDIAMETER;
	ioffset = 0;
	dummy.next = NULL;

	//iqnit all drops as "fallen", so they'll get initialized by the usual pseudorandomness
	for(i = 0; i < PLASSEA_MAXDROPS; i++)
	{	
        drops[i].vPos.x = (MAX_X - 1) / 2;
        drops[i].vPos.y = (MAX_Y - 1) / 2;
		drops[i].zDist = PLASSEA_ZDISTMAX + 16; //overshoot zdistmax, to be sure
	}
	
	while(!false)
	{
		//clear backbuffer
		clearImage(black);
		
		//move plasma "forward"
		ioffset += PLASSEA_PLASMASPEED;

		//advance the drops
		for(i = 0; i < PLASSEA_MAXDROPS; i++)
		{
            //is the drop in z=0 and has spawned no effect yet?
            if(!drops[i].spawnFlag && (drops[i].zDist > (PLASSEA_ZDISTMAX - PLASSEA_ZDIST_RESSCALE)))
            {
                drops[i].spawnFlag = 1;
                
            	//add a new wave to the list
				for(runner = head; runner->next != NULL; runner = runner->next);
				runner->next = (plasmaSeaWaveList *)malloc(sizeof(plasmaSeaWaveList));
				runner = runner->next;
				runner->next = NULL;
				
				runner->element.center.x = drops[i].vPos.x;
			    runner->element.center.y = drops[i].vPos.y;
				runner->element.center.z = 0;
				runner->element.pDist = 0;			
				//set a custom offset for the plasma colors
				runner->element.myIOff = easyRandom();
		        runner->element.speed = 1;
            }
			//is the drop already fallen?
			else if(drops[i].zDist > PLASSEA_ZDISTMAX)
			{
				//yes, spawn a new drop			
				drops[i].vPos.x = easyRandom() % MAX_X;
				drops[i].vPos.y = easyRandom() % MAX_Y;
				drops[i].speed = (easyRandom() % 2) + 1;
				//put drop somewehere in the non-visible range
				drops[i].zDist = easyRandom() % (PLASSEA_Z_HEADROOM * PLASSEA_ZDIST_RESSCALE);
				drops[i].spawnFlag = 0;
			}
			else
			{
				//no, add speed 
				drops[i].zDist += drops[i].speed;
			}
		}

		//advance and recycle waves
		for(runner = head; runner->next != NULL; runner = runner->next)
		{
            plasmaSeaWaveList *ltmp = (plasmaSeaWaveList *)runner->next;
            if(ltmp->element.pDist > PLASSEA_PDISTMAX)
            {
                //recycle
                runner->next = ltmp->next;
                free(ltmp);
                
                if(runner->next == NULL)
                    break;
            }
            else
    		{
                //add speed, but half as fast
                if(ioffset % (3*PLASSEA_PLASMASPEED) == 0)
                   ltmp->element.pDist += ltmp->element.speed;
            }
        }

		for(x = 0; x < MAX_X; x++)
		{
			//z = 0
			for(y = 0; y < MAX_Y; y++)
			{
                tmpR = 0; tmpG = 0; tmpB = 0;
                
                //for all waves
                for(runner = head; runner != NULL; runner = runner->next)
		        {                 
                    if((runner != head) && (runner->element.pDist <= PLASSEA_PDISTMAX))
                    {
                        //modified calculation with custom centerpoint
                        sqx = SQUARE(x - runner->element.center.x);
         				sqy = SQUARE(y - runner->element.center.y);
         				
         				//calculate distance to wave center
	                    distCalc = isqrt(sqx*SQUARE(PLASSEA_ZDIST_RESSCALE) + sqy*SQUARE(PLASSEA_ZDIST_RESSCALE));
			
        				//reset color;
        				col = 0;
        				
        				//diagonal scrolling sine 
        				col += plasmaPolarFlat(sqx, sqy, scale, borgDiameter, ioffset + runner->element.myIOff);
        			
        				//colorspace offset
        				col += 0x8000;
        				col  = (col * 49152) / 0xFFFF;
        				
        				//some mystic scaling stolen from setVoxelH, this removes yet another color clipping effect
        				col = (col*49152)/32768;
        				
        				//calculate a voxel brightness (?) weakening value based on the distance to the balls outer hull
    					dingsVal = abs((int)runner->element.pDist - distCalc);
    					dingsVal *= 255 / PLASBALL_M_FILTER;
    					if (dingsVal > 255)
    						continue;
						
        				//get new voxels color
        				colRGB = HtoRGB(col);
        				//alter the voxels brightness (?), without uint overflows ;-)
                        colRGB.r = max((int)colRGB.r - dingsVal, 0);
         				colRGB.g = max((int)colRGB.g - dingsVal, 0);
		                colRGB.b = max((int)colRGB.b - dingsVal, 0);
        				
        				//finally add the new color
        				tmpR += colRGB.r; tmpG += colRGB.g; tmpB += colRGB.b;
                    }
                }
                
                //draw the voxel
                tmpR /= 2; tmpG /= 2; tmpB /= 2;
                colRGB.r = min(tmpR, 255);
                colRGB.g = min(tmpG, 255);
                colRGB.b = min(tmpB, 255);
                setVoxel((voxel) {x, y, 0}, colRGB);
                
                //normalize();
                    
				//for all drops
				for(i = 0; i < PLASSEA_MAXDROPS; i++)
				{                                     	
					//check if this drop is in our z-column
					if((drops[i].vPos.x == x) && (drops[i].vPos.y == y))
					{
						//if so, walk the z-range and see if we have to light up the pixel
						//see plasmaball for this technique
						for(z = 0; z < MAX_Z; z++)
						{
							dingsVal = abs((int)drops[i].zDist - ((4 - z + PLASSEA_Z_HEADROOM) * PLASSEA_ZDIST_RESSCALE));
							dingsVal *= 255 / PLASSEA_M_FILTER;
							if (dingsVal > 255)
								continue;
							
							//if(z == 0)
							//{
                                // colRGB.r = 0;
							   //  colRGB.g = 128;
							 //    colRGB.b = 255;
                            //}
                            //else
                            //{
    							//alter the voxels brightness (?), without uint overflows ;-)
    							colRGB.r = 0;//max(0 - dingsVal, 0);
    							colRGB.g = max(128 - dingsVal, 0);
    							colRGB.b = max(255 - dingsVal, 0);
                            //}
							
							//finally draw the voxel
							drops[i].vPos.z = z;
							setVoxel(drops[i].vPos, colRGB);					
						}
					}
				}
			}
		}

		//show frame
		swapAndWait(10);
		
		if((ioffset) >= PLASSEA_ITERATIONS * 0x8000)
			break;
	}
}

//this asnimation features a nice plasmaball emerging from the borg's center
//a hollowed sphere is drawn by altering a voxels color-brightness depending on it's distance to the sphere's radius
//the color brightness weakening, and thus the sphere's hull thickness, which is in fact a gradient, is tuned by PLASBALL_M_FILTER, see top of file
//in other words, voxels which are too distant from the sphere, are weakened till they reach zero, producing a nice sphere in the video ram
void plasmaBall()
{
	int32_t col, scale;
	int32_t x, y, z;
	int sqx, sqy, distCalc;
	unsigned int dist;
	color colRGB;
	int dingsVal;

	scale = 10*0x5000;
	dist = 0;
	ioffset = 0;
	
	while (!false)
	{
		//clear backbuffer
		clearImage(black);
		
		//move plasma "forward"
		ioffset += 48;

		//dist is the ball's radius
		dist = (ioffset / 128) % 80;

		for(x = 0; x < MAX_X; x++)
		{
			//cache squared X
			sqx = SQUARE(x-2) * 256;
	
			for(y = 0; y < MAX_Y; y++)
			{
				sqy = SQUARE(y-2) * 256;
				
				for(z = 0; z < MAX_Z; z++)
				{
					//calculate distance to center and check against desired distance
					distCalc = isqrt(sqx + sqy + (z-2)*(z-2)*256);
					
					//reset color;
					col = 0;
					
					//diagonal scrolling sine 
					col += plasmaDiag(x, y, z, scale, ioffset);
				
					//colorspace offset
					col += 0x8000 + (ioffset/32);

					//calculate a voxel brightness (?) weakening value based on the distance to the balls outer hull
					dingsVal = abs((int)dist - distCalc);
					dingsVal *= 255 / PLASBALL_M_FILTER;
					if (dingsVal > 255)
						continue;
					
					//get the voxels rgb color value
					colRGB = HtoRGB(col);
					//alter the voxels brightness (?), without uint overflows ;-)
					colRGB.r = max((int)colRGB.r - dingsVal, 0);
					colRGB.g = max((int)colRGB.g - dingsVal, 0);
					colRGB.b = max((int)colRGB.b - dingsVal, 0);
					
					//finally draw the voxel
					setVoxel((voxel) {x, y, z}, colRGB);			
				} 
			}
		}

		//show frame
		swapAndWait(10);
		
		if((ioffset) >= PLASBALL_ITERATIONS * 0x8000)
			break;
	}
}

/* ^
 * | 4   6 7 0 1 2
 * | 3   5   |>  3
 * y 2   4   +   4
 * | 1   3  <|   5
 * | 0   2 1 0 7 6
 * | 
 * |     0 1 2 3 4
 * +-------- x ------->
 */
static void drawLineZAngle(unsigned char angle, unsigned char z, color value) {
	// could be optimised in programcode
	unsigned char x1[8] = {2, 1, 0, 0, 0, 0, 0, 1};
	unsigned char y1[8] = {0, 0, 0, 1, 2, 3, 4, 4};
	unsigned char x2[8] = {2, 3, 4, 4, 4, 4, 4, 3};
	unsigned char y2[8] = {4, 4, 4, 3, 2, 1, 0, 0};
	angle &= 0x07;
	drawLine3D(x1[angle], y1[angle], z, x2[angle], y2[angle], z, value);	
}


void spirale() {
	unsigned char z, angle, count = 0, i = 0, angleAdd, angleAdd2;
	int32_t color, scale;
	scale = 3*0x9000;
	
	for (angleAdd2 = 0; angleAdd2 < 17; count++) {
		angleAdd = angleAdd2 < 9 ? angleAdd2 : 17 - angleAdd2; 
		//printf("%d %d %d  %d %d \n", curColor.r, curColor.g, curColor.b, index, value);
		for (angle = 0; angle < 8; angle++) {
			ioffset += 200; //700;
			for (z = 0; z < 5; z++) {
				ioffset += 8;
				color  = ioffset;
				color += 0x3000 * Sine((4-z * (0x8000*0x8000 / (MAX_Y * scale))) + 
										ioffset*4
									   ) / 0x8000;
	
				drawLineZAngle((angle+(angleAdd*z/4)) & 0x07, z, HtoRGB(color*49152/32768));		
			}
			swapAndWait(50);
			clearImage(black);
			
			if (count > 4) { 
				angleAdd2++;
				count = 0;
			}
		}
		i++;
	}
	fade(15, 100);
}
