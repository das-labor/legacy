#ifndef _TEXTURES_H_
#define _TEXTURES_H_


#include <GL/gl.h>


typedef struct {
  int            width;
  int            height;
  GLint          format;
  unsigned char *bmp;
} Texture_t;


/* Funktion laedt eine rgb Datei. Rueckgabe ist NULL im Fehlerfall, 
   ein Zeiger auf eine Texture_t Struktur sonst. */
Texture_t *LoadSGI (char *filename);

/* Funktion gibt den Speicherplatz von "texture" wieder frei */
void DeleteTexture (Texture_t *texture);


#endif
