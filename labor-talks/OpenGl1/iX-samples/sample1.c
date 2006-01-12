/* Linux/Unix: uebersetzen mit
gcc sample1.c -lMesaGLU -lMesaGL -lXext -lX11 -lm -lglut -lXi -lXmu \
 -B/usr/X11/lib
*/

#ifdef OS_WIN32
#   include <windows.h>
#endif 

#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

GLenum      polygonMode=GL_LINE;
float       rotX=0.0, rotY=0.0;

static void DrawCube ()
{
  /* Wuerfel aus 2 Quadraten und einem Quadratstreifen zusammensetzen */
  glBegin (GL_QUADS);
    glColor3f (0.0, 0.0, 0.0); glVertex3f (0.0, 0.0, 0.0);   /* unten */
    glColor3f (1.0, 0.0, 0.0); glVertex3f (1.0, 0.0, 0.0);
    glColor3f (1.0, 0.0, 1.0); glVertex3f (1.0, 0.0, 1.0);
    glColor3f (0.0, 0.0, 1.0); glVertex3f (0.0, 0.0, 1.0);

    glColor3f (0.0, 1.0, 0.0); glVertex3f (0.0, 1.0, 0.0);   /* oben */
    glColor3f (0.0, 1.0, 1.0); glVertex3f (0.0, 1.0, 1.0);
    glColor3f (1.0, 1.0, 1.0); glVertex3f (1.0, 1.0, 1.0);
    glColor3f (1.0, 1.0, 0.0); glVertex3f (1.0, 1.0, 0.0);
  glEnd ();
  
  glBegin (GL_QUAD_STRIP);
    glColor3f (0.0, 0.0, 1.0); glVertex3f (0.0, 0.0, 1.0);   /* vorn */
    glColor3f (0.0, 1.0, 1.0); glVertex3f (0.0, 1.0, 1.0);
    glColor3f (1.0, 0.0, 1.0); glVertex3f (1.0, 0.0, 1.0);
    glColor3f (1.0, 1.0, 1.0); glVertex3f (1.0, 1.0, 1.0);

    glColor3f (1.0, 0.0, 0.0); glVertex3f (1.0, 0.0, 0.0);   /* rechts */
    glColor3f (1.0, 1.0, 0.0); glVertex3f (1.0, 1.0, 0.0);
    
    glColor3f (0.0, 0.0, 0.0); glVertex3f (0.0, 0.0, 0.0);   /* hinten */
    glColor3f (0.0, 1.0, 0.0); glVertex3f (0.0, 1.0, 0.0);

    glColor3f (0.0, 0.0, 1.0); glVertex3f (0.0, 0.0, 1.0);   /* links */
    glColor3f (0.0, 1.0, 1.0); glVertex3f (0.0, 1.0, 1.0);
  glEnd ();
}

static void Init(void)
{
  /* Z-Buffer f. Berechnung verdeckter Flaechen einschalten */
  glEnable(GL_DEPTH_TEST);
    
  /* Vorder- u. Rueckseite der Polygone nur als Randlinien darstellen */
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
}

static void Reshape(int width, int height)
{
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint)width, (GLint)height);

  /* Projektionsmatix initialisieren auf 60 Grad horizontales */
  /* Sichtfeld, Verhaeltnis Breite:Hoehe = 1:1, Clipping fuer z<1 */
  /* und z>200 */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* angle, aspect, near Clip, far Clip */
  gluPerspective(60.0, 1.0, 1.0, 200.0);

  /* Modelview Matrix wieder zur aktuellen Matrix machen */
  glMatrixMode(GL_MODELVIEW);
}

static void Key(unsigned char key, int x, int y)
{
  if (key==27) 
    exit(1);
  if (key==' ') {
    if (polygonMode==GL_FILL)
      polygonMode=GL_LINE;
    else
      polygonMode=GL_FILL;

    /* Polygondarstellung zw. Outline und Gefuellt umschalten... */
    glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

    /* ... und Bild erneut rendern */
    glutPostRedisplay ();
  }
}

static void SpecialKey(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    rotX -= 5;
    break;
  case GLUT_KEY_DOWN:
    rotX += 5;
    break;
  case GLUT_KEY_LEFT:
    rotY -= 5;
    break;
  case GLUT_KEY_RIGHT:
    rotY += 5;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

static void Display(void)
{
  /* FrameBuffer unf Z-Buffer loeschen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  /* Modelview Matrix initialisieren */
  glLoadIdentity ();
  
  /* Modeltransformation ausfuehren... */
  glTranslatef (0.0, 0.0 ,-3.0);    /* Verschiebung um -3 in z-Richtung */
  glRotatef (rotY, 0.0, 1.0, 0.0);  /* Rotation um die Y-Achse */
  glRotatef (rotX, 1.0, 0.0, 0.0);  /* Rotation um die X-Achse */
  glTranslatef (-0.5, -0.5 , -0.5); /* Verschieb. um -0.5, alle Richt. */
  
  /* ...und den Wuerfel unter der Modeltransformation zeichnen. */
  DrawCube ();
  
  /* Back-Buffer in den Front-Buffer kopieren. */
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  /* Ausgabefenster definieren */
  glutInitWindowPosition(0, 0);
  glutInitWindowSize( 300, 300);
  /* Renderkontext mit Z-Buffer, Doublebuffer fuer RGB-Modus anfordern.*/
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  
  if (glutCreateWindow("Sample1") == GL_FALSE)
    exit(1);
  
  Init();
  
  /* Callback Funktionen vereinbaren */
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutSpecialFunc(SpecialKey);
  glutDisplayFunc(Display);
  
  /* Kontrolle an GLUT Eventloop uebergeben */
  glutMainLoop();
}

