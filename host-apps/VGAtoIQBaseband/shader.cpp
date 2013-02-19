#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;

#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include "glext.h"

#ifdef _WIN32
#warn untested code
PFNGLCREATESHADERARBPROC pglCreateShaderARB = 0;
PFNGLSHADERSOURCEARBPROC pglShaderSourceARB = 0;
PFNGLCOMPILESHADERARBPROC pglCompileShaderARB = 0;
PFNGLCREATEPROGRAMARBPROC pglCreateProgramARB = 0;
PFNGLATTACHSHADERARBPROC pglAttachShaderARB = 0;
PFNGLLINKPROGRAMARBPROC pglLinkProgramARB = 0;
PFNGLUSEPROGRAMMARBPROC pglUseProgramARB = 0;

#define glCreateShader            pglCreateShaderARB
#define glShaderSource            pglShaderSourceARB
#define glCompileShader           pglCompileShaderARB
#define glCreateProgram           pglCreateProgramARB
#define glAttachShader            pglAttachShaderARB
#define glLinkProgram             pglLinkProgramARB
#define glUseProgram              pglUseProgramARB
#endif

//Function from: http://www.evl.uic.edu/aej/594/code/ogl.cpp
//Read in a textfile (GLSL program)
// we need to pass it as a string to the GLSL driver
char *textFileRead(char *fn) 
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
        		fseek(fp, 0, SEEK_END);
        		count = ftell(fp);
        		rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		
		}
	}

	return content;
}

//Function from: http://www.evl.uic.edu/aej/594/code/ogl.cpp
//Read in a textfile (GLSL program)
// we can use this to write to a text file
int textFileWrite(char *fn, char *s) 
{
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

//Got this from http://www.lighthouse3d.com/opengl/glsl/index.php?oglinfo
// it prints out shader info (debugging!)
void printShaderInfoLog(GLuint obj, bool beVerbose)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        if( beVerbose )
		cout << "printShaderInfoLog:" << infoLog << endl;
        free(infoLog);
	}else{
	if( beVerbose )
		cout << "Shader Info Log: OK" << endl;
	}
}

//Got this from http://www.lighthouse3d.com/opengl/glsl/index.php?oglinfo
// it prints out shader info (debugging!)
void printProgramInfoLog(GLuint obj, bool beVerbose)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        if( beVerbose )
		cout << "printProgramInfoLog: " << infoLog << endl;
        free(infoLog);
    }else{
    	    if( beVerbose )
		cout << "Program Info Log: OK" << endl;
	}
}

//YEAH, YEAH, YEAH ... global vars, this is a freggn demo!
GLuint f,p;			//Handlers for our vertex, geometry, and fragment shaders
//int gw,gh;				//Keep track of window width and height

//Setup shaders
void setShaders(float width, float height, bool beVerbose) 
{
	//a few strings
	// will hold onto the file read in!
	char *fs = NULL; //, *fs2 = NULL, *gs = NULL,*vs = NULL, ;
	
#ifdef _WIN32
	glCreateShader = (PFNGLCREATESHADERARBPROC)wglGetProcAddress("glCreateShaderARB");
        glShaderSource = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
        glCompileShader = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
        glCreateProgram = (PFNGLCREATEPROGRAMARBPROC)wglGetProcAddress("glCreateProgramARB");
        glAttachShader = (PFNGLATTACHSHADERARBPROC)wglGetProcAddress("glAttachShaderARB");
        glLinkProgram = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
        glUseProgram = (PFNGLUSEPROGRAMMARBPROC)wglGetProcAddress("glUseProgramARB");
#endif

	//First, create our shaders 
	f = glCreateShader(GL_FRAGMENT_SHADER);
	
        char filename[] = "fragment.glsl";
        
	//Read in the programs
	fs = textFileRead(&filename[0]);

	//Setup a few constant pointers for below
	const char * ff = fs;

	glShaderSource(f, 1, &ff, NULL);

	free(fs);//free(gs);free(vs);

	glCompileShader(f);

	p = glCreateProgram();

	glAttachShader(p,f);
        
	glLinkProgram(p);
	glUseProgram(p);
        GLint myLoc = glGetUniformLocation(p, "width");
        glUniform1f( myLoc, width );

        myLoc = glGetUniformLocation(p, "height");
        glUniform1f( myLoc, height );

	printShaderInfoLog(f, beVerbose);
	printProgramInfoLog(p, beVerbose);
}
