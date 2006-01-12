/*
Linux/Unix: Uebersetzen mit:
gcc sample3.c -lMesaGLU -lMesaGL -lXext -lX11 -lm -lglut -lXi -lXmu \
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
GLenum         shadingMode=GL_SMOOTH;
GLUquadricObj *cone;
GLfloat        headAngle=60.0;
GLfloat        elbowAngle=90.0;
GLfloat        baseAngle=-5.0;
GLfloat        spinAngle=45.0;

static void DrawCube ()
{
  static float front_mat_shininess[] = {10.0};
  static float front_mat_specular[]  = {0.0, 0.0, 0.0, 1.0};
  static float front_mat_diffuse[]   = {0.5, 0.0, 0.0, 1.0};

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, front_mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, front_mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, front_mat_diffuse);

  glBegin (GL_QUADS);
    glNormal3f (0.0, 1.0, 0.0);
    glVertex3f (0.0, 0.0, 0.0);   /* Unten */
    glVertex3f (1.0, 0.0, 0.0);
    glVertex3f (1.0, 0.0, 1.0);
    glVertex3f (0.0, 0.0, 1.0);

    glNormal3f (0.0, -1.0, 0.0);
    glVertex3f (0.0, 1.0, 0.0);  /* Oben */
    glVertex3f (0.0, 1.0, 1.0);
    glVertex3f (1.0, 1.0, 1.0);
    glVertex3f (1.0, 1.0, 0.0);
  glEnd ();

  if (shadingMode==GL_FLAT) {
     /* Flat Shading - die Normalenvektoren stehen senkrecht auf
        den Flaechen des Lampenarms*/
     glBegin (GL_QUAD_STRIP);
       glNormal3f (0.0, 0.0, 1.0);   /* Vorne */
       glVertex3f (1.0, 0.0, 1.0);
       glVertex3f (1.0, 1.0, 1.0);
       glVertex3f (0.0, 0.0, 1.0);
       glVertex3f (0.0, 1.0, 1.0);

       glNormal3f (-1.0, 0.0, 0.0);   /* Links */
       glVertex3f ( 0.0, 0.0, 0.0);
       glVertex3f ( 0.0, 1.0, 0.0);

       glNormal3f (0.0, 0.0,-1.0);   /* Hinten */
       glVertex3f (1.0, 0.0, 0.0);
       glVertex3f (1.0, 1.0, 0.0);

       glNormal3f (1.0, 0.0, 0.0);   /* Rechts */
       glVertex3f (1.0, 0.0, 1.0);
       glVertex3f (1.0, 1.0, 1.0);
     glEnd ();
  } else {
     /* Gouraud Shading - die Normalenvektoren schliessen mit den
        angenzenden Flaechen einen Winkel von 135 Grad ein */
     glBegin (GL_QUAD_STRIP);
       glNormal3f (0.7, 0.0, 0.7);   /* Vorne */
       glVertex3f (1.0, 0.0, 1.0);
       glVertex3f (1.0, 1.0, 1.0);
       glNormal3f (-0.7, 0.0, 0.7);
       glVertex3f (0.0, 0.0, 1.0);
       glVertex3f (0.0, 1.0, 1.0);

       glNormal3f (-0.7, 0.0,-0.7);  /* Links */
       glVertex3f ( 0.0, 0.0, 0.0);
       glVertex3f ( 0.0, 1.0, 0.0);

       glNormal3f (0.7, 0.0,-0.7);   /* Hinten */
       glVertex3f (1.0, 0.0, 0.0);
       glVertex3f (1.0, 1.0, 0.0);

       glNormal3f (0.7, 0.0, 0.7);   /* Rechts */
       glVertex3f (1.0, 0.0, 1.0);
       glVertex3f (1.0, 1.0, 1.0);
     glEnd ();
  }

  /* Alternativ leisten diese beiden Aufrufe mit Hilfe von GLUT dasselbe
     wie obige glBegin()...glEnd() Anweisungen. Leider zeigen sie nicht
     die Anwendung von glNormal... */
  /*
  glTranslatef (0.5, 0.5, 0.5);
  glutSolidCube (1.0);
  */
}

static void DrawLamp()
{
  static float front_mat_shininess[] = {30.0};
  static float front_mat_specular[]  = {1.0, 1.0, 1.0, 1.0};
  static float front_mat_diffuse[]   = {0.0, 1.0, 0.0, 1.0};
  static float back_mat_shininess[]  = {50.0};
  static float back_mat_specular[]   = {0.0, 0.0, 1.0, 1.0};
  static float back_mat_diffuse[]    = {1.0, 0.0, 0.0, 1.0};
  static float light1_pos[]          = {0.0, 0.4, 0.0, 1.0};
  static float light1_dir[]          = {0.0, 1.0, 0.0, 1.0};
  
  /* Drehung um y-Achse, Neigung und Platzierung auf dem Tisch */
  glTranslatef (0.0, -1.0 ,-4.4);

  glRotatef (spinAngle, 0.0, 1.0, 0.0);
  glRotatef (baseAngle, 1.0, 0.0, 0.0);
      
  /* Ausgabe des Oberarms */
  glPushMatrix();
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

  /* Materialeigenschaften fuer Innen- und Aussenseite des */
  /* Lampenkopfs einstellen */
  glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
  glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
  glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
  glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);

  /* Lampenkopf an Spitze Unterarm verschieben */
  glTranslatef (0.0, 1.0 , 0.0);

  /* Lampenkopf kippen: */
  glRotatef (headAngle, 1.0, 0.0, 0.0);

  /* Position und Richtung von Lichtquelle 1 HIER angeben, damit */
  /* sie denselben Transformationen wie der Lampenkopf selbst */
  /* unterliegen*/
  glLightfv(GL_LIGHT1, GL_POSITION,light1_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,light1_dir);

  /* Ausgabe des Lampenkopfes */
  glPushMatrix();
    glRotatef (-90, 1.0, 0.0, 0.0);
    gluCylinder (cone, 0.2, 0.2, 0.2, 20, 4);
    glTranslatef (0.0, 0.0 , 0.2);
    gluCylinder (cone, 0.2, 0.4, 0.2, 20, 4);
  glPopMatrix();
}

static void DrawTable ()
{
  static float front_mat_shininess[] = {80.0};
  static float front_mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  static float front_mat_diffuse[] = {0.4, 0.4, 0.8, 1.0};

  /* Materialeigenschaften (Glanz, diffuse und glaenzende Reflexion) */
  /* fuer den Tisch einstellen */
  glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);

  glPushMatrix();
    glTranslatef (0.0, -1.0, -4.4);
    glRotatef (-90, 1,0,0);
    gluDisk (cone, 0, 3, 60, 20);
  glPopMatrix();
}

static void Init(void)
{
  static float light0_ambient[] = {0.4, 0.4, 0.4, 1.0};
  static float light0_diffuse[] = {0.8, 0.8, 0.8, 1.0};
  static float light0_pos[]    = {1.0, 1.0, 1.0, 0.0};
  static float light1_ambient[] = {0.0, 0.0, 0.0, 1.0};
  static float light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  static float lmodel_ambient[] = {0.0, 0.0, 0.0, 1.0};

  glEnable(GL_DEPTH_TEST);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* Lichtquelle 0 Position sowie Farben fuer ambienten und diffusen */
  /* Lichtanteil zuweisen */
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION,light0_pos);

  /* Lichtquelle 1 Farben fuer ambienten und diffusen Lichtanteil zuweisen */
  /* Lichtkegel auf 45 Grad einstellen */
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);

  /* Ambientes Licht fuer die gesamte Szene setzen und verschiedene */
  /* Materialeigenschaften fuer Vorder- und Rueckseite zulassen */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  /* Beleuchtungsberechnung und beide Lichtquellen einschalten */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
    
  cone = gluNewQuadric ();
}

static void CleanUp ()
{
  gluDeleteQuadric (cone);
}

static void Reshape(int width, int height)
{
  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float)width/(float)height, 1.0, 200.0);
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
  case 's':
    if (shadingMode==GL_FLAT)
      shadingMode=GL_SMOOTH;
    else
      shadingMode=GL_FLAT;
    glShadeModel (shadingMode);
    break;
  case 'h':
    headAngle -= 2;
    break;
  case 'H':
    headAngle += 2;
    break;
  case 'e':
    elbowAngle -= 2;
    break;
  case 'E':
    elbowAngle += 2;
  default:
    break;
  }
  glutPostRedisplay ();
}

static void SpecialKey(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    baseAngle -= 2;
    break;
  case GLUT_KEY_DOWN:
    baseAngle += 2;
    break;
  case GLUT_KEY_LEFT:
    spinAngle -= 2;
    break;
  case GLUT_KEY_RIGHT:
    spinAngle += 2;
  default:
    break;
  }
  glutPostRedisplay();
}

static void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
    DrawTable ();
    DrawLamp ();
  glPopMatrix();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitWindowPosition(0, 0); glutInitWindowSize( 480, 340);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  if (glutCreateWindow("Sample3") == GL_FALSE)
    exit(1);

  Init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutSpecialFunc(SpecialKey);
  glutDisplayFunc(Display);
  glutMainLoop();

  CleanUp ();
  return 0;
}
