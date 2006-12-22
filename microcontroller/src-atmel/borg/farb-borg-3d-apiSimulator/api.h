#define R 0
#define G 1
#define B 2

typedef struct {
	unsigned char x, y, z;
} voxel; 

typedef enum {
	up, down, 
	right, left,
	forward, back
} direction;

extern unsigned char imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

void clearScreen(unsigned char red, unsigned char  green, unsigned char blue);

void setVoxel(voxel pos, unsigned char red, unsigned char green, unsigned char blue);

void shift(direction dir);

void fade(unsigned char msProStep, unsigned char steps);
void swapAndWait(unsigned char ms);

unsigned char easyRandom();

/* not jet implementet

void blurX(unsigned char filter[3]);
void blurY(unsigned char filter[3]);
void blurZ(unsigned char filter[3]);

*/