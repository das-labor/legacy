/**
 * http://www.das-labor.org/wiki/Farbborgcontest
 * einreichung von Guido Pannenbecker (took@jabber.ccc.de)
 * 31.10.2009, V2.1
 **/
#include "api.h" // Schau dir die BASIC API an

void took()
{
	int x,y,z;
	int dir_x = 1, dir_y = 1, dir_z = 1;
	int rainbow_mode = 0;
	int i = 23;
	color c = (color) {0, 255, 255};

	clearImage(black);
	fade(5, 128);

	clearImage(c);
	fade(10, 255);

	while (i--) {
		for (z = (dir_z>0?0:4); z >= 0 && z < 5; z += dir_z) {
			for (y = (dir_y>0?0:4); y >= 0 &&  y < 5; y += dir_y) {
				for (x = (dir_x>0?0:4); x >= 0 &&  x < 5; x += dir_x) {
					switch (rainbow_mode) {
						case 0:
							c.r++;
							c.g--;
							if (c.r == 255) rainbow_mode++;
						break;
						case 1:
							c.g++;
							c.b--;
							if (c.g == 255) rainbow_mode++;
						break;
						case 2:
							c.b++;
							c.r--;
							if (c.b == 255) rainbow_mode=0;
						break;
					}
					setVoxel((voxel) {x, y, z}, c);
					swapAndWait(23);
				}
				dir_x *= -1;
			}
			dir_y *= -1;
		}
		dir_z *= -1;
	}

	clearImage(black);
	fade(15, 255);

	swapAndWait(420);
}
