#ifndef MAIN_H
#define MAIN_H

// type make -f Makefile.osx
// to compile the simulator for MacOSX (tested on 10.2, 10.3, 10.4, 10.5, 10.6)
#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

extern int win;

#endif
