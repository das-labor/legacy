/*
Linux/Unix: Uebersetzen mit:
gcc sample4.cpp -lMesaGLU -lMesaGL -lXext -lX11 -lm -lglut -lXi -lXmu -B/usr/X11/lib
*/

#ifndef OS_WIN32
#  include <sys/time.h>
#  include <unistd.h>
#else
#  include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "textures.h"

#define RPM          1    /* Rounds per minute */
#define EARTH        0    /* Texturnummern...  */
#define WOOD         1
#define SOCLE        2
#define NUM_TEXTURES 3

typedef unsigned char *pchar;

GLenum         polygonMode = GL_FILL;
GLint          textureMode = GL_REPLACE;
GLint          texFilter   = GL_LINEAR;
GLUquadricObj *quadric;
GLint          textures[3];
pchar          texFileNames[3] = {"earth.rgb", "wood.rgb", "socle.rgb"};
float          angle;
short          doBlend=0, useTextures=1, cullBackface=0;


#ifndef OS_WIN32
/* Funktion GetTickCount des Win32-API unter Nicht-Windowssystemen emulieren*/
long GetTickCount ()
{
  struct timeval tv;

  gettimeofday (&tv, NULL);
  return 1000*tv.tv_sec + tv.tv_usec/1000;
}
#endif


/* Funktion ist eine Abwandlung der Mesa-Implementierung von glutSolidTorus().
   Die Funktion ist um die Moeglichkeit erweitert, Teilringe des Winkels
   arc zu zeichnen. */
static void DrawTorus (GLfloat r, GLfloat R, GLint nsides, GLint rings, float arc)
{
  int     i, j;
  GLfloat theta, phi, theta1;
  GLfloat cosTheta, sinTheta;
  GLfloat cosTheta1, sinTheta1;
  GLfloat ringDelta, sideDelta;
  GLfloat cosPhi, sinPhi, dist;

  ringDelta = 2.0 * M_PI / rings;
  sideDelta = 2.0 * M_PI / nsides;

  theta = 0.0; cosTheta = 1.0; sinTheta = 0.0;
  for (i = rings - 1; i >= rings*(1.0-arc/360.0); i--) {
    theta1 = theta + ringDelta;
    cosTheta1 = cos(theta1);
    sinTheta1 = sin(theta1);

    glBegin (GL_QUAD_STRIP);
    phi = 0.0;
    for (j = nsides; j >= 0; j--) {
      phi += sideDelta;
      cosPhi = cos(phi); sinPhi = sin(phi);
      dist = R + r * cosPhi;

      glNormal3f (cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
      glVertex3f (cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
      glNormal3f (cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
      glVertex3f (cosTheta * dist, -sinTheta * dist,  r * sinPhi);
    }
    glEnd ();
    theta = theta1; cosTheta = cosTheta1; sinTheta = sinTheta1;
  }
}

/* Funktion zeichnet Kugel mit Polen auf der Z-Achse und Mittelpunkt im */
/* Ursprung. Die Kugel wird Breitengrad fuer Breitengrad aufgebaut      */
static void DrawGlobe (float radius, int slices)
{
  float        zf = -M_PI/2.0;
  float        fslices = slices;
  float        zkHi, zkLo, lenHi, lenLo;
  float        texX, texYlo, texYhi, xk, yk;
  short        xy, z;
  
  glPushMatrix ();
  glScalef (radius, radius, radius);

  zkHi = -1.0;
  for (z=0; z<slices; z++) {
    zf  += M_PI/fslices;
    zkLo = zkHi; zkHi = sin (zf);
    lenLo= sqrt (1.0-zkLo*zkLo);lenHi= sqrt (1.0-zkHi*zkHi);
    texYlo = ((float) z) / fslices; texYhi = ((float) z+1) / fslices;

    /* Breitengrade als Quadstrips zeichen */
    glBegin (GL_QUAD_STRIP);
    for (xy=0; xy<=slices; xy++) {
      texX = 1.0- ((float)xy) / fslices;
      xk   = sin (2*M_PI * ((float)(xy))/fslices);
      yk   = cos (2*M_PI * ((float)(xy))/fslices);
      
      glTexCoord2f (texX, texYlo);
      glNormal3f (lenLo * xk, lenLo * yk, zkLo);
      glVertex3f (lenLo * xk, lenLo * yk, zkLo);
      glTexCoord2f (texX, texYhi);
      glNormal3f (lenHi * xk, lenHi * yk, zkHi);
      glVertex3f (lenHi * xk, lenHi * yk, zkHi);
    }
    glEnd ();
  }
  glPopMatrix();
}

static void Init(void)
{
  static float light0_ambient[] = {0.4, 0.4, 0.4, 1.0};
  static float light0_diffuse[] = {0.8, 0.8, 0.8, 1.0};
  static float light0_pos[]    = {1.0, 1.0, 1.0, 1.0};
  static float lmodel_ambient[] = {0.0, 0.0, 0.0, 1.0};
  static float mat_shininess[] = {8.0};
  static float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  short int    i;
  Texture_t   *t;

  glEnable (GL_DEPTH_TEST);                   /* Z-Buffer Test einschalten */
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); /* Polygone gefuellt rendern */
  /* Farbe zum Loeschen des Framebuffers setzen*/
  glClearColor (0.2, 0.1, 0.1, 1.0);          
  /* Gewichtsfaktoren f. Blending einstellen */
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /* Bei aktiviertem Culling die Rueckseite nicht darstellen*/
  glCullFace (GL_BACK);                       

  /* Quadric f. Darstellung des Globussockels erzeugen */
  quadric = gluNewQuadric ();
  gluQuadricTexture (quadric, GL_TRUE);

  /* --- Light Setup --- */

  /* Lichtquelle 0 einstellen*/
  glLightfv (GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION,light0_pos);

  /* Beleuchtungsmodell waehlen*/
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  /* Materialeigenschaften definieren */
  glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

  /* ...und Beleuchtung einschalten. */
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
    
  /* --- Texture Setup --- */
  glGenTextures (3, textures); // Platz f. 3 Texturobjekte anfordern...

  for (i=0; i<NUM_TEXTURES; i++) {
    printf ("Loading %s\n", texFileNames[i]);
    /* Aktuelles Texturobjekt selektieren */
    glBindTexture (GL_TEXTURE_2D, textures[i]);
    /* Image laden.. */
    t = LoadSGI (texFileNames[i]); /* "wood.rgb"); */
    /* ...und Pixeldaten an das Texturobjekt uebergeben. */
    glTexImage2D (GL_TEXTURE_2D, 0, t->format, t->width, t->height, 
		  0, t->format, GL_UNSIGNED_BYTE, t->bmp);
    /* OpenGL "merkt" sich die Imagedaten im Texturobjekt, so dass   */
    /* hier bereits der Hauptspeicher wieder freigegeben werden kann */
    DeleteTexture (t);

    /* Textur f. Koord. ausserhalb v. [0;1] zyklisch wiederholen (in S-
       und T-Richtung */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* Filtermodus f. Vergroesserung und Verkleinerung einstellen */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  /* Per Default berechnete Pixelfarbe durch Texelfarbe ersetzen */
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  /* ... und Texturemapping einschalten nicht vergessen...*/
  glEnable (GL_TEXTURE_2D);
}

/* Aktionen bei Aenderung der Groesse des Ausgabefensters */
static void Reshape (int width, int height)
{
  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (60.0, 1.0, 1.0, 200.0);
  glMatrixMode (GL_MODELVIEW);
}

/* Reaktionen auf Tastaturereignisse */
static void Key (unsigned char key, int x, int y)
{
  short int    i;

  switch (key) {
  case 27:  /* Esc */
    exit(1);
  case ' ': /* Blank */
    if (polygonMode==GL_FILL)
      polygonMode=GL_LINE;
    else
      polygonMode=GL_FILL;
    glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
    break;
  case 'b':
    /* Alpha-Blending ein-/ausschalten */
    doBlend = 1-doBlend;
    if (doBlend) glEnable (GL_BLEND); else glDisable (GL_BLEND);
    break;
  case 'c':
    /* Backface-Culling ein-/ausschalten */
    cullBackface = 1-cullBackface;
    if (cullBackface) glEnable (GL_CULL_FACE); else glDisable (GL_CULL_FACE);
    break;
    /* Texture-Mapping ein-/ausschalten */
  case 't':
    useTextures = 1-useTextures;
    if (useTextures) glEnable (GL_TEXTURE_2D); else glDisable (GL_TEXTURE_2D);
    break;
  case 'f':
    /* Filtermodus fuer Texturen wechseln */
    if (texFilter==GL_LINEAR) texFilter=GL_NEAREST; else texFilter=GL_LINEAR;
    for (i=0;i<NUM_TEXTURES;i++) {
      glBindTexture (GL_TEXTURE_2D, textures[i]);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilter);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilter);
    }
    break;
  case 'm':
    /* Texturfunktion aendern */
    if (textureMode==GL_REPLACE) textureMode=GL_MODULATE;
    else if (textureMode==GL_MODULATE) textureMode=GL_BLEND;
    else textureMode=GL_REPLACE;
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);
  default:
    break;
  }
  glutPostRedisplay ();
}

/* Auszufuehrende Aktion, wenn gerade kein bes. Ereignis anliegt */
void OnIdle ()
{
  float tc = GetTickCount ();

  angle = -RPM *((tc/1000.0)*2 *M_PI) / 60.0;
  glutPostRedisplay ();                       /* Bild neu aufbauen */
}

static void Display(void)
{
  static float mat_diffuse[] = {0.8, 0.8, 0.8, 0.2};

  /* Altes Bild loeschen */
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  /* Modelvie Matrix initialisieren */
  glLoadIdentity ();
  /* View Transformation von gluLookAt berechnen lassen */
  gluLookAt (0.0,0.9,3.6, 0.0,0.1,0.0, 0.0,1.0,0);

  /* --- Tisch darstellen --- */
  /* aktuelle Textur setzen... */
  glBindTexture (GL_TEXTURE_2D, textures[WOOD]);
  /* ... und Primitiv rendern. Die Texturkoordinaten nutzen GL_REPEAT.*/
  glBegin (GL_QUADS);
    glTexCoord2i (0, 0);  glVertex3i (-2, -1, -2);
    glTexCoord2i (0, 5);  glVertex3i (-2, -1,  2);
    glTexCoord2i (5, 5);  glVertex3i ( 2, -1,  2);
    glTexCoord2i (5, 0);  glVertex3i ( 2, -1, -2);
  glEnd ();

  /* --- Globusstativ darstellen --- */
  glRotatef (20,0,1,0);
  /* Status der momentan aktivierte Features, Materialeigenschaften */
  /* und Linieneinstellungen speichern */
  glPushAttrib (GL_ENABLE_BIT | GL_LINE_BIT | GL_LIGHTING_BIT);
    /* Texture-Mapping f. Rahmen und Achse deaktivieren */
    glDisable (GL_TEXTURE_2D);
    /* Material mit Alpha=0.2 setzen... */
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glPushMatrix ();
      glRotatef (-60,0,0,1);
      DrawTorus (0.04, 0.85, 10, 60, 180);    /* Rahmen */
      glLineWidth (2.0);                      /* Linienbreite = 2 Pixel */
      glBegin (GL_LINES);                     /* Achse */
        glVertex3f ( 0.85, 0.0, 0.0);
        glVertex3f (-0.85, 0.0, 0.0);
      glEnd();
    glPopMatrix ();
  /* Und alten Status wieder herstellen */
  glPopAttrib ();

  glPushMatrix ();
    glTranslatef (0.0, -0.8, 0.0);
    glRotatef (90,1,0,0);
    glBindTexture (GL_TEXTURE_2D, textures[SOCLE]);
    gluCylinder (quadric, 0.0, 0.6, 0.2, 20, 1); /* Sockel rendern */
  glPopMatrix ();

  /* --- Globus darstellen --- */
  glPushMatrix ();
    glRotatef ( 30,0,0,1);
    glRotatef (-90,1,0,0);
    glRotatef (-angle*180.0 / M_PI,0,0,1);
    glBindTexture (GL_TEXTURE_2D, textures[EARTH]);
    DrawGlobe (0.8,40);
  glPopMatrix ();

  /* ...und BackBuffer in Frontbuffer kopieren */
  glutSwapBuffers ();
}

int main(int argc, char **argv)
{
    glutInit (&argc, argv);

    glutInitWindowPosition (0, 0); glutInitWindowSize (640, 640);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);

    if (glutCreateWindow ("Sample4") == GL_FALSE)
	exit(1);

    Init();

    glutReshapeFunc (Reshape);
    glutKeyboardFunc (Key);
    glutDisplayFunc (Display);
    glutIdleFunc (OnIdle);

    glutMainLoop ();

    /* Texturspeicher freigeben */
    glDeleteTextures (3, textures);

    return 0;
}
