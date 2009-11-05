#include "api.h"

void Benny()
{
int8_t full =0;
int8_t x;
int8_t y;
int8_t z;
int8_t r;
int8_t g;
int8_t b;
int8_t temp;
while(full<125)
{

x = easyRandom()%5;
y = easyRandom()%5;
r = easyRandom();
g = easyRandom();
b = easyRandom();
temp = 0;

for (z=4; z>=0; z--)
{
	if(isVoxelSet((voxel){x,y,z}))
{
		z=-1;


}
	else
{

		setVoxel((voxel) {x, y,z}, (color) {r,g,b});
		setVoxel((voxel) {x, y,z+1}, black);
		fade(6,10);
		temp=1;
}
}
	if(temp==1)
		full++;
}
while(full>0)
{
x = easyRandom()%5;
y = easyRandom()%5;
z = easyRandom()%5;

	if(isVoxelSet((voxel){x,y,z}))
{
		full--;
		setVoxel((voxel){x,y,z}, black);
		fade(6,10);
}

}
}
