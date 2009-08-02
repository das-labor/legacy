#include "config.h"
#include "api.h"
#include "util.h"
#include "ff.h"
#include <stddef.h>

#include "diskio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileParser.h"
#include "spike_hw.h"

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

FATFS fs;				/* File system object */
FIL fil;

void* memset (void *str, int c, size_t len)
{
  register char *st = str;

  while (len-- > 0)
    *st++ = c;
  return str;
}

int posFirstNewline(char *buffer, int len) {
	int i = 0;
	while (i < len && buffer[i] != '\n')
		i++;
	return i;
}


/*
void *memcpy (void *destaddr, void const *srcaddr, size_t len)
{
  char *dest = destaddr;
  char const *src = srcaddr;

  while (len-- > 0)
    *dest++ = *src++;
  return destaddr;
}
*/
int
memcmp(const void* ab1, const void* ab2, size_t n)
{
	register const unsigned char*	b1 = (const unsigned char*)ab1;
	register const unsigned char*	b2 = (const unsigned char*)ab2;
	register const unsigned char*	e = b1 + n;

	while (b1 < e)
		if (*b1++ != *b2++)
			return(*--b1 - *--b2);
	return(0);
}

void initFileReader() {
	uart_putstr("init filereader\n");
	memset(&fs, 0, sizeof(FATFS)); 	/* Clear file system object */
	FatFs = &fs;	                /* Assign it to the FatFs module */	
}


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
	WORD fsize;

	char buffer[BUF_SIZE];
	unsigned char ignoreNextLine = 0, x;
	unsigned int fileOffset = 0;

	while (1) { // fgets(buffer, BUF_SIZE, apsFile)
		if (f_open(&fil, filename,  FA_READ|FA_OPEN_EXISTING)) {
			uart_putstr("open error\n");
			break;		
		}
		if (f_lseek(&fil, fileOffset)) {
			uart_putstr("lseek\n");
			break;
		}
		if (f_read(&fil, buffer, BUF_SIZE, &fsize)) {
			uart_putstr("fread\n");
			break;
		}
		if (fsize <= 0) 
			break;
		fileOffset += posFirstNewline(buffer, fsize) + 1;
			
		f_close(&fil);
		
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
				uart_putstr("play ");
				uart_putstr(buffer);
				uart_putstr("\n");
	
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
		
}

color palette[94];

/*
 */
void playAnimFile(char *filename) {
	WORD fsize;
	char buffer[BUF_SIZE], *p;
	char help[10];
	command com = NoCommand;
	unsigned char inStateCnt = 0;
	unsigned char idx, x, y, z, fadeTime;
	color setColor;
	voxel setVox;
	unsigned char ignoreNextLine = 0;
	int fileOffset = 0;
	
	palette['R'-32] = red;
	palette['G'-32] = green;
	palette['B'-32] = blue;
	palette['S'-32] = black;
	palette['W'-32] = white;
	
	if (f_open(&fil, filename,  FA_READ|FA_OPEN_EXISTING)) {
		uart_putstr("open error\n");
		return;		
	}

	while (1) { // fgets(buffer, BUF_SIZE, animFile)
		if (f_lseek(&fil, fileOffset)) {
			uart_putstr("lseek\n");
			break;
		}
		if (f_read(&fil, buffer, BUF_SIZE, &fsize)) {
			uart_putstr("fread\n");
			break;
		}
		if (fsize <= 0) 
			break;
		fileOffset += posFirstNewline(buffer, fsize) + 1;	
		
		//uart_putstr(buffer);
		//uart_putchar('\n');
		//uart_putstr(itoa(fileOffset, help, 10));
		//uart_putchar('\n');

		// Falls kein Kommentar
		if (buffer[0] != '#' && !ignoreNextLine) {
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
	f_close(&fil);
}
