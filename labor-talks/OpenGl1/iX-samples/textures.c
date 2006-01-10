
#include "textures.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  short          magic;
  char           storage;
  char           bpc;
  unsigned short dimension;
  unsigned short width;
  unsigned short height;
  unsigned short channels;
  long           pixMin;
  long           pixMax;
  char           dummy[4];
  char           imageName [80];
  long           colormap;
  char           unused [404];
} SGIHeader_t;



char GetByte (FILE *f)
{
  unsigned char b;
  fread (&b, 1, 1, f);
  return b;
}

short GetShort (FILE *f)
{
  unsigned char b[2];
  fread (b, 2,1, f);
  return (b[0]<<8)+(b[1]);
}

long GetLong (FILE *f)
{
  unsigned char b[4];
  fread (b, 4,1,f);
  return (b[0]<<24)+(b[1]<<16)+(b[2]<<8)+(b[3]);
}

Texture_t *LoadSGI (char *filename)
{
  FILE       *f;
  SGIHeader_t sgiHeader;
  Texture_t  *tex;
  int         c,x,y, i, offset;
  int         tablen, rleLen, out;
  long       *lengthtab, *offsettab;
  char       *b;
  unsigned char pixel, count, *rlePnt;

  if ((f=fopen (filename, "rb"))==NULL) {
    return NULL;
  }

  sgiHeader.magic = GetShort (f);
  sgiHeader.storage = GetByte (f);
  sgiHeader.bpc= GetByte (f);
  sgiHeader.dimension = GetShort (f);
  sgiHeader.width = GetShort (f);
  sgiHeader.height = GetShort (f);
  sgiHeader.channels = GetShort (f);
  sgiHeader.pixMin  = GetLong (f);
  sgiHeader.pixMax= GetLong (f);
  fread (sgiHeader.dummy, 4,1,f);
  fread (sgiHeader.imageName, 80,1,f);
  sgiHeader.colormap= GetLong (f);
  fread (sgiHeader.unused, 404,1,f);

  if (sgiHeader.magic != 474){
    fclose (f);
    return NULL;
  }

  if (sgiHeader.bpc != 1){
    /* 1 oder 2 - Praezision in Byte pro Channel */
    fclose (f);
    return NULL;
  }

  if (sgiHeader.dimension == 1){
    /* Eindimensionale (einzeilige) Images werden nicht unterstuetzt */
    fclose (f);
    return NULL;
  }

  if (sgiHeader.dimension == 2) {
    /* 2D-Image mit nur einem Channel. -> entweder Color indexed oder BW */
    /* -> werden nicht unterstuetzt */
    fclose (f);
    return NULL;
  }

  /* Hier ist Dimension in jedem Fall 3! -> Pro Farbe/Alpha ein Channel */
  if ((tex=calloc (1, sizeof (Texture_t)))==NULL){
    fclose (f);
    return NULL;
  }

  tex->width  = sgiHeader.width;
  tex->height = sgiHeader.height;
  tex->format = (sgiHeader.channels==3) ? GL_RGB : GL_RGBA;
  
  tex->bmp = (char*) calloc (tex->width*tex->height,sgiHeader.channels);
  if (tex->bmp==NULL) {
    fclose (f);
    return NULL;
  }

  if (sgiHeader.storage == 1) {
    /* RLE Codierung - ScanlineTables einlesen: */
    tablen = tex->height*sgiHeader.channels*sizeof(long);
    lengthtab = (long*) malloc (tablen);
    offsettab = (long*) malloc (tablen);
    fseek (f, 512, SEEK_SET);
    for (i=0; i<tex->height*sgiHeader.channels; i++)
      offsettab[i] = GetLong (f);
    for (i=0; i<tex->height*sgiHeader.channels; i++)
      lengthtab[i] = GetLong (f);
    
    /* Jetzt Imagedaten zeilenweise lesen und dekomprimieren */
    for (c=0; c<sgiHeader.channels; c++) {
      out=c;
      for (y=0; y<sgiHeader.height; y++){
	/* Scanlines der Reihe nach einlesen und in den Zielpuffer dekomprimieren */
	rleLen = lengthtab [c*sgiHeader.height+y];
	offset = offsettab [c*sgiHeader.height+y];
	b    = (char*) malloc (rleLen);
	fseek (f, offset, SEEK_SET);
	fread (b, rleLen, 1, f);
	
	rlePnt=(unsigned char*) b;
	
	while (1) {
	  pixel = *rlePnt++;
	  if (!(count=(pixel & 0x7f)))
	    /* Ende der Scanline */
	    break;
	  if (pixel & 0x80) {
	    /* count Bytes kopieren */
	    while (count--){
	      tex->bmp[out] = *rlePnt++;
	      out+=sgiHeader.channels;
	    }
	  } else {
	    /* Byte count-mal kopieren */
	    pixel = *rlePnt++;
	    while (count--){
	      tex->bmp[out] = pixel;
	      out+=sgiHeader.channels;
	    }
	  }
	}	
	free (b);
      }
    }
    free (lengthtab);
    free (offsettab);
  } else {
    /* keine Kompression */
    for (c=0; c<sgiHeader.channels; c++){
      offset = c;
      for (y=0; y<sgiHeader.height; y++)
	for (x=0; x<sgiHeader.width; x++) {
	  fread (&tex->bmp[offset], 1, 1, f);
	  offset+=sgiHeader.channels;
        }
    }
  }

  fclose (f);
  return tex;
}



void DeleteTexture (Texture_t *texture)
{
  if (texture) {
    if (texture->bmp)
      free (texture->bmp);
    free (texture);
  }
}
