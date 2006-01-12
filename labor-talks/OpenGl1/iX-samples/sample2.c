/*
Linux/Unix: Uebersetzen mit:
gcc sample2.c -lMesaGLU -lMesaGL -lXext -lX11 -lm -lglut -lXi -lXmu \
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

GLenum         polygonMode=GL_LINE;
GLUquadricObj *cone;
GLfloat        headAngle=0.0;
GLfloat        elbowAngle=45.0;
GLfloat        baseAngle=10.0;
GLfloat        spinAngle=30.0;

static void DrawCube ()
{
  /* Wuerfel aus 2 Quadraten und einen "Quadratstreifen" zusammensetzen */
  glBegin (GL_QUADS);
    glColor3f (0.0, 0.0, 0.0); glVertex3f (0.0, 0.0, 0.0);   /* Unten */
    glColor3f (1.0, 0.0, 0.0); glVertex3f (1.0, 0.0, 0.0);
    glColor3f (1.0, 0.0, 1.0); glVertex3f (1.0, 0.0, 1.0);
    glColor3f (0.0, 0.0, 1.0); glVertex3f (0.0, 0.0, 1.0);

    glColor3f (0.0, 1.0, 0.0); glVertex3f (0.0, 1.0, 0.0);  /* Oben */
    glColor3f (0.0, 1.0, 1.0); glVertex3f (0.0, 1.0, 1.0);
    glColor3f (1.0, 1.0, 1.0); glVertex3f (1.0, 1.0, 1.0);
    glColor3f (1.0, 1.0, 0.0); glVertex3f (1.0, 1.0, 0.0);
  glEnd ();

  glBegin (GL_QUAD_STRIP);
    glColor3f (1.0, 0.0, 1.0); glVertex3f (1.0, 0.0, 1.0);   /* Vorne */
    glColor3f (1.0, 1.0, 1.0); glVertex3f (1.0, 1.0, 1.0);
    glColor3f (0.0, 0.0, 1.0); glVertex3f (0.0, 0.0, 1.0);
    glColor3f (0.0, 1.0, 1.0); glVertex3f (0.0, 1.0, 1.0);

    glColor3f (0.0, 0.0, 0.0); glVertex3f (0.0, 0.0, 0.0);   /* Links */
    glColor3f (0.0, 1.0, 0.0); glVertex3f (0.0, 1.0, 0.0);

    glColor3f (1.0, 0.0, 0.0); glVertex3f (1.0, 0.0, 0.0);   /* Hinten */
    glColor3f (1.0, 1.0, 0.0); glVertex3f (1.0, 1.0, 0.0);

    glColor3f (1.0, 0.0, 1.0); glVertex3f (1.0, 0.0, 1.0);   /* Rechts */
    glColor3f (1.0, 1.0, 1.0); glVertex3f (1.0, 1.0, 1.0);
  glEnd ();
}

static void Init(void)
{
  /* Z-Buffer f. Berechnung verdeckter Flaechen einschalten */
  glEnable(GL_DEPTH_TEST);
    
  /* Vorder- u. Rueckseite der Polygone nur als Randlinien darstellen */
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* Quadric Objekt fuer Ausgabe von Zylinder und Kegelstumpf erzeugen*/
  cone = gluNewQuadric ();
}

static void CleanUp ()
{
  gluDeleteQuadric (cone);
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
  switch (key) {
  case 27:
    exit(1);
  case ' ':
    if (polygonMode==GL_FILL)
      polygonMode=GL_LINE;
    else
      polygonMode=GL_FILL;
    glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
    break;
  case 'h':
    headAngle += 5;
    break;
  case 'H':
    headAngle -= 5;
    break;
  case 'e':
    elbowAngle += 5;
    break;
  case 'E':
    elbowAngle -= 5;
    break;
  default:
    break;
  }
  glutPostRedisplay ();
}

static void SpecialKey(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    baseAngle -= 5;
    break;
  case GLUT_KEY_DOWN:
    baseAngle += 5;
    break;
  case GLUT_KEY_LEFT:
    spinAngle -= 5;
    break;
  case GLUT_KEY_RIGHT:
    spinAngle += 5;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

static void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  /* Drehung um y-Achse, Neigung und Platzierung auf dem Tisch */
  glTranslatef (0.0, -1.0 ,-4.4);
  glRotatef (spinAngle, 0.0, 1.0, 0.0);
  glRotatef (baseAngle, 1.0, 0.0, 0.0);

  /* Ausgabe des Oberarms */
  glPushMatrix();
    /* Wuerfel aus Bsp. 1 durch asymetrische Skalierung zu einem */
    /* Quader deformieren */
    glScalef (0.2, 1.0, 0.2);
    glTranslatef (-0.5, 0.0 , -0.5);
    DrawCube ();
  glPopMatrix();

  /* Lampenkopf + Unterarm an Spitze Oberarm verschieben */
  glTranslatef (0.0, 1.0 , 0.0);

  /* Kippen des Kopfes + Unterarms um Ellenbogenwinkel */
  glRotatef (elbowAngle, 1.0, 0.0, 0.0);

  /* Ausgabe des Unterarms */
  glPushMatrix();
    glScalef (0.2, 1.0, 0.2);
    glTranslatef (-0.5, 0.0 , -0.5);
    DrawCube ();
  glPopMatrix();

  glColor3f(0.5, 0.0, 1.0);
  /* Lampenkopf an Spitze Unterarm verschieben */
  glTranslatef (0.0, 1.0 , 0.0);
  /* Lampenkopf kippen: */
  glRotatef (headAngle, 1.0, 0.0, 0.0);

  /* Ausgabe des Lampenkopfes */
  glPushMatrix();
    gluCylinder (cone, 0.2, 0.2, 0.2, 20, 4);
  glPopMatrix();

  glTranslatef (0.0, 0.0 , 0.2);
  gluCylinder (cone, 0.2, 0.4, 0.2, 20, 4);

  glutSwapBuffers();
}

void main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitWindowPosition(0, 0); glutInitWindowSize( 480, 340);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  if (glutCreateWindow("Sample2") == GL_FALSE)
    exit(1);

  Init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutSpecialFunc(SpecialKey);
  glutDisplayFunc(Display);
  glutMainLoop();

  CleanUp ();
}
