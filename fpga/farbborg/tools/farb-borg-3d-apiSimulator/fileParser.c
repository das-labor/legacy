#include "config.h"
#include "api.h"

#include <stdio.h>
#include <stdlib.h>

#include "fileparser.h"

/*
 * 
 */
typedef enum {
	ExpandPalette     = 'p',
	SetImagePalette   = 'I',
	SetImage          = 'i',
	ClearImagePalette = 'C',
	CLearImage        = 'c',
	SwapAndWait       = 'S',
	Fade              = 'F',
	SetVoxelsPalette  = 'V',
	SetVoxels         = 'v',
	NoCommand         =  0
} command;

static unsigned char hex2Byte(char *twoByteString) {
  unsigned char byteOut  = 0, value;

  switch (twoByteString[0] & 0x70) {
    case '0':
       value = twoByteString[0] & 0x0f;
       if (value < 10) {
          byteOut = value;
       }
       break;
    case 'A' & 0xf0:
    case 'a' & 0xf0:
       value = twoByteString[0] & 0x0f;
       if (value < 7 && value) {
          byteOut = 9 + value;
       }
       break;
  }
  byteOut <<= 4;
  switch (twoByteString[1] & 0x70) {
    case '0':
       value = twoByteString[1] & 0x0f;
       if (value < 10) {
          byteOut += value;
       }
       break;
    case 'A' & 0xf0:
    case 'a' & 0xf0:
       value = twoByteString[1] & 0x0f;
       if (value < 7 && value) {
          byteOut += 9 + value;
       }
       break;
  }
  return byteOut;
}

#define BUF_SIZE 80

void playPlaylist(char *filename) {
	FILE * apsFile;
	char buffer[BUF_SIZE];
	unsigned char ignoreNextLine = 0, x;
	
	apsFile = fopen(filename, "r");
	if (apsFile) {
		while (fgets(buffer, BUF_SIZE, apsFile)) {
			if (buffer[0] != '#' && !ignoreNextLine) {
				//printf("(%x) %s", buffer[0], buffer);
				if (buffer[0] != '\n' && buffer[0] != '\r' && buffer[0] != ' ') {
					for (x = 0; x < BUF_SIZE && buffer[x]; x++) {
						switch (buffer[x]) {
							case ' ':
							case '\t':
							case '\n':
							case '\r':
								buffer[x] = 0;
								break;
							default:
								break;
						}
					}
					playAnimFile(buffer);
				}
			} else {
				ignoreNextLine = 1;
				for (x = 0; x < BUF_SIZE && buffer[x]; x++) {
					if (buffer[x] == '\n' || buffer[x] == '\r') {
						ignoreNextLine = 0;
					}
				}
			}
		}
		fclose(apsFile);
	}
}

color palette[94];

/*
 */
void playAnimFile(char *filename) {
	FILE * animFile;
	char buffer[BUF_SIZE], *p;
	command com = NoCommand;
	unsigned char inStateCnt = 0;
	unsigned char idx, x, y, z, fadeTime;
	color setColor;
	voxel setVox;
	unsigned char ignoreNextLine = 0;
	
	palette['R'-32] = red;
	palette['G'-32] = green;
	palette['B'-32] = blue;
	palette['S'-32] = black;
	palette['W'-32] = white;
	
	animFile = fopen(filename, "r");
	if (animFile) {
		while (fgets(buffer, BUF_SIZE, animFile)) {
			// Falls kein Kommentar
			if (buffer[0] != '#' && !ignoreNextLine) {
				//printf("Command : %c(%X) cnt : %d - %s\n", com, com, inStateCnt, buffer);
				if (inStateCnt == 0) {
					com = (command) buffer[0];
					if (com != ' ' && com != '\n' && com != '\r') {
						inStateCnt++;
					}
				} else {
					switch (com) {
						case ExpandPalette:
							if (buffer[0] == '>') {
								inStateCnt = 0;
							} else {
								idx = buffer[0] - 32;
								if (idx < 96) {
									palette[idx].r = hex2Byte(&buffer[2]);
									palette[idx].g = hex2Byte(&buffer[4]);
									palette[idx].b = hex2Byte(&buffer[6]);
									/*printf(" - %c %2X%2X%2X \n", idx+32, 
										   palette[idx].r, palette[idx].g, 
										   palette[idx].b);*/
								}
							}
							break;
						case SetImagePalette:
							y = inStateCnt++ - 1;
							p = buffer;
							if (y < MAX_Y) {
								for (z = 0; z < MAX_Z; z++) {
									for (x = 0; x < MAX_X; x++) {
										setVoxel((voxel) {x, y, z}, 
												 palette[*p-32]);
										p += 2;
									}
									p += 2;
								}
							} else {
								inStateCnt = 0;
							}
							break;
						case SetImage:
							p = buffer;
							if (inStateCnt++ == 1) {
								y = 0;
								z = 0;
							}	
							if (y < MAX_Y) {
								for (x = 0; x < MAX_X; x++) {
									imag[z][y][x][R] = hex2Byte(p);
									p += 2;
									imag[z][y][x][G] = hex2Byte(p);
									p += 2;
									imag[z][y][x][B] = hex2Byte(p);
									p += 3;
								}
								y++;
							} else {
								z++;
								y = 0;
								if (z >= MAX_Z) {
									inStateCnt = 0;
								}
							}
							break;
						case ClearImagePalette:
							clearImage(palette[buffer[0]-32]);
							inStateCnt = 0;
							break;
						case CLearImage:
							setColor.r = hex2Byte(&buffer[0]);
							setColor.g = hex2Byte(&buffer[2]);
							setColor.b = hex2Byte(&buffer[4]);
							clearImage(setColor);
							inStateCnt = 0;
							break;
						case SwapAndWait:
							swapAndWait(atoi(buffer));
							inStateCnt = 0;
							break;
						case Fade:
							if (inStateCnt++ == 1) {
								fadeTime = atoi(buffer);
							} else {
								fade(fadeTime, atoi(buffer));
								inStateCnt = 0;
							}
							break;
						case SetVoxelsPalette:
							if (buffer[0] == '>') {
								inStateCnt = 0;
							} else {
								setVox.x = buffer[0] - '0';
								setVox.y = buffer[2] - '0';
								setVox.z = buffer[4] - '0';
								
								setVoxel(setVox, palette[buffer[6]-32]);
							}
							break;
						case SetVoxels:
							if (buffer[0] == '>') {
								inStateCnt = 0;
							} else {
								setVox.x = buffer[0] - '0';
								setVox.y = buffer[2] - '0';
								setVox.z = buffer[4] - '0';
								
								setColor.r = hex2Byte(&buffer[ 6]);
								setColor.g = hex2Byte(&buffer[ 8]);
								setColor.b = hex2Byte(&buffer[10]);
								
								setVoxel(setVox, setColor);
							}
							break;
						default:
							inStateCnt = 0;
							break;
					}
				}
			} else {
				ignoreNextLine = 1;
				for (x = 0; x < BUF_SIZE && buffer[x]; x++) {
					if (buffer[x] == '\n' || buffer[x] == '\r') {
						ignoreNextLine = 0;
					}
				}
			}
		}
		fclose(animFile);
	} //else {
		//printf("Datei %s wurde nicht gefunden!\n", filename);
	//}
}
