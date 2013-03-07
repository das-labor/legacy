///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// vgatoIQBaseband
// using GL_ARB_pixel_buffer_object extension
// It uses 2 PBOs to optimize uploading pipeline; application to PBO, and PBO to
// texture object.
//
//  AUTHOR:Patrick Rudolph (siro@das-labor.org)
//  CREATED: 2013-02-17
//  modified example from:
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-10-22
// UPDATED: 2012-06-07
///////////////////////////////////////////////////////////////////////////////

// in order to get function prototypes from glext.h, define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include "glext.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "shader.h"

#include "glInfo.h"                             // glInfo struct
#include "Timer.h"
#include "xrandr.h"



using std::stringstream;
using std::cout;
using std::endl;
using std::ends;
using std::ifstream;

// GLUT CALLBACK functions ////////////////////////////////////////////////////
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void updatePixels(GLubyte* dst, int size);
void showInfo();
void printTransferRate();


// constants
const float  CAMERA_DISTANCE = sqrt(3.0f);
const int    TEXT_WIDTH      = 8;
const int    TEXT_HEIGHT     = 13;
const int    IMAGE_WIDTH = 200;
const int    IMAGE_HEIGHT = 700;
const int    CHANNEL_COUNT = 2;
const int    BYTE_PER_CHANNEL = 4;
const int    DATA_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * CHANNEL_COUNT * BYTE_PER_CHANNEL;
const GLenum PIXEL_FORMAT = GL_RG;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
GLuint pboIds[2];                   // IDs of PBO
GLuint textureId;                   // ID of texture
GLubyte* imageData = 0;             // pointer to texture buffer
GLubyte* testpatternA = 0;
GLubyte* testpatternB = 0;
int screenWidth;
int screenHeight;
int enableconvolutional;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
bool pboisSupported;
bool fragmentisSupported;
bool beVerbose;
bool testpattern;
int pboMode = 0;
int drawMode = 0;
int cutofright = 1;
int cutofbottom = 2;
ifstream filestr;
char *VGAname = new char(255);
char *activemode = new char(255);

int vgax = 0,vgay = 0;
Timer timer, t1, t2;
float copyTime, updateTime;
GLint       program = 0;
GLint timeParam;

// function pointers for PBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif


///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int i;
    int msps = 64;

    
    if( argc < 2 ){
    	    cout << "No inputfile given." << endl;
    	    return -1;
    }
    
    initSharedMem();
    // register exit callback
    atexit(exitCB);
    
    for(i=1; i < argc; i++)
    {
        if( strcmp(argv[i],"-cutofright")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        cutofright=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"-cutofbottom")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        cutofbottom=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"-v")==0 || strcmp(argv[i],"-verbose")==0){
        	beVerbose = true;
        }
        
        if( strcmp(argv[i],"-pclk")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        msps=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }    
        
        
    }

    if( beVerbose )
    	cout << "hsync: " << cutofright << " vsync: " << cutofbottom << endl;

#ifdef _WIN32
	#warn modesetting not supported  
#else  
    if( init_xrandr() )
    {
    	cout << "failed to init xrandr" << endl;	    
    	exit(1);
    }
    
    if( find_VGA_output(&VGAname[0],&vgax, &vgay, &activemode[0]) ){
    	    cout << "couldn't find VGA port" << endl;
    	    exit(1);
    }
        
    if( beVerbose )
    {
    	    cout << "found port: " << VGAname << " @ " << vgax << "," << vgay << " mode: " << activemode << endl;
        if( !strlen(activemode) )
            cout << "cannot detect mode. no mode set ?" << endl;
    }

    //test if newmode is active, if so remove it
    if( strstr(activemode,"newmode") )
    {
    	if( beVerbose )
    	    cout << "newmode is active, removing it now..." << endl;
        enable_output(&VGAname[0],"1024x768", vgax, vgay);
        disable_output(&VGAname[0]);
        rm_mode(&VGAname[0],"newmode");
    }

    // add new modeline 
    add_custom_mode( &VGAname[0], msps, cutofright, cutofbottom );
    
    if( beVerbose )
    	    cout << "added custom mode: newmode" << endl;
    
    // set mode
    enable_output( &VGAname[0], "newmode", vgax, vgay );
    
    if( beVerbose )
    	    cout << "set mode \"newmode\" on VGA " << endl;
#endif

    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();
    
    // get OpenGL info
    glInfo glInfo;
    glInfo.getInfo();
    //glInfo.printSelf();
    
    // init 2 texture objects
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, IMAGE_WIDTH, IMAGE_HEIGHT, 0, PIXEL_FORMAT, GL_FLOAT, (GLvoid*)imageData); //GL_RGBA12
    glBindTexture(GL_TEXTURE_2D, 0);

#ifdef _WIN32

    // check PBO is supported by your video card
    if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
    {
        // get pointers to GL functions
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
        glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
        glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
        glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

        // check once again PBO extension
        if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
           glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
        {
            pboisSupported = true;
            pboMode = 2;    // using 2 PBO
            //cout << "Video card supports GL_ARB_pixel_buffer_object." << endl;
        }
        else
        {
            pboisSupported = false;
            pboMode = 0;    // without PBO
            if( beVerbose )
            	    cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << endl;
        }
    }
    
    if(glInfo.isExtensionSupported("GL_ARB_fragment_shader"))
    {
        fragmentisSupported = true;
        //cout << "Video card supports GL_ARB_fragment_shader." << endl;
    }
    else
    {
        fragmentisSupported = false;
        if( beVerbose )
        	cout << "Video card does NOT support GL_ARB_fragment_shader." << endl;
    }
    
#else // for linux, do not need to get function pointers, it is up-to-date

    if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
    {

        pboisSupported = true;
        pboMode = 2;
        //cout << "Video card supports GL_ARB_pixel_buffer_object." << endl;
    }
    else
    {
        pboisSupported = false;
        pboMode = 0;
        if( beVerbose )
        	cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << endl;
    }

    if(glInfo.isExtensionSupported("GL_ARB_fragment_shader"))
    {
        fragmentisSupported = true;
        //cout << "Video card supports GL_ARB_fragment_shader." << endl;
    }
    else
    {
        fragmentisSupported = false;
        if( beVerbose )
        	cout << "Video card does NOT support GL_ARB_fragment_shader." << endl;
    }
    
#endif

    if( pboisSupported )
    {
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferDataARB with NULL pointer reserves only memory space.
        glGenBuffersARB(2, pboIds);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[0]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[1]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    }
    
    for(i=1; i < argc; i++)
    {
        if( strcmp(argv[i],"-nofilter")==0 )
        	fragmentisSupported = false;
        
        if( strcmp(argv[i],"-t")==0 )
        	testpattern = true; 
    }
    
    if( !testpattern )
    {
    	    filestr.open (argv[1], ifstream::binary);
    	    if ( filestr.fail() ){
    	    	    cout << "Error opening file." << endl;
    	    	    clearSharedMem();
    	    	    return -1;
    	    }
    }

    if( fragmentisSupported )
    	    setShaders( IMAGE_WIDTH,  IMAGE_HEIGHT , beVerbose);
    
    // start timer, the elapsed time will be used for updateVertices()
    timer.start();

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA); // display mode

    glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);               // window size

    
    glutInitWindowPosition(vgax, vgay);           // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    //int handle = 0;
    int handle = glutCreateWindow(argv[0]);     // param is the title of window
    glutFullScreen();

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutIdleFunc(idleCB);                       // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);

    return handle;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
    //@glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glShadeModel(GL_FLAT);                      // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glClearColor(0, 0, 0.5f, 0);                   // background color
}

///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    float* ptr;
    enableconvolutional = 0;

    screenWidth = IMAGE_WIDTH;
    screenHeight = IMAGE_HEIGHT;

    cameraAngleX = cameraAngleY = 0;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points
    
    beVerbose = false;
    testpattern = false;
    
    // allocate texture buffer
    imageData = new GLubyte[DATA_SIZE];
    memset(imageData, 0, DATA_SIZE);

    // allocate texture buffer
    testpatternA = new GLubyte[DATA_SIZE];
    
    ptr = (float*)testpatternA;
    for(int i = 0; i < IMAGE_HEIGHT; ++i)
    {
	    for(int j = 0; j < IMAGE_WIDTH; ++j)
	    {
		    if(j == IMAGE_WIDTH/2)
		    {
			    *ptr = float(1.0f);
			    ++ptr;
			    *ptr = float(1.0f);
		    }
		    else
		    {
			    *ptr = float(0);
			    ++ptr;
			    *ptr = float(0);
		    }
		    ++ptr;

	    } 
    }
    
    // allocate texture buffer
    testpatternB = new GLubyte[DATA_SIZE];
    
    ptr = (float*)testpatternB;
    for(int i = 0; i < IMAGE_HEIGHT * IMAGE_WIDTH; i++)
    {
    	    *ptr = float(sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f)*0.25f + 0.5f);
    	    ++ptr;
    	    *ptr = float(sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f + 3.1415928f/2.0f)*0.25f + 0.5f);
    	    ++ptr;
    }
   	    
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up shared memory
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
    // deallocate texture buffer
    delete [] imageData;
    imageData = 0;

    // clean up texture
    glDeleteTextures(1, &textureId);

    // clean up PBOs
    if( pboisSupported )
    {
        glDeleteBuffersARB(2, pboIds);
    }
    
    if( !testpattern )
    {
    	    filestr.close();
    }
}


///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}



///////////////////////////////////////////////////////////////////////////////
// copy an image data to texture buffer
///////////////////////////////////////////////////////////////////////////////
void updatePixels(GLubyte* dst, int size)
{
    static int cnt = 0;
    if(!dst)
        return;

    if( !testpattern )
    {
    	    char* ptr = (char*)dst;
    	    /*do{
    	    	    if(  filestr.eof() )
    	    	    	    ptr += filestr.gcount();
    	    	    filestr.clear();
    	    	    filestr.read (ptr,DATA_SIZE);
    	    	    if( filestr.bad() )
    	    	    {
    	    	    	    cout << "Error reading from file" << endl;
    	    	    	    return;
    	    	    }
    	    }while( filestr.eof() )*/
    	    filestr.read (ptr,DATA_SIZE);
    }
    else
    {
    	   cnt++;
    	   if(cnt < 300){
    	   	memcpy(dst,testpatternA,DATA_SIZE);
    	   }
    	   else if ( (cnt >= 300) && (cnt < 600))
    	   {
    	   	memcpy(dst,testpatternB,DATA_SIZE);   
    	   }
    	   else
    	   {
    	   	cnt = 0;   
    	   }
    }

}


///////////////////////////////////////////////////////////////////////////////
// print transfer rates
///////////////////////////////////////////////////////////////////////////////
void printTransferRate()
{
    const double INV_MEGA = 1.0 / (1024 * 1024);
    static Timer timer;
    static int count = 0;
    static stringstream ss;
    double elapsedTime;

    // loop until 1 sec passed
    elapsedTime = timer.getElapsedTime();
    if(elapsedTime < 1.0)
    {
        ++count;
    }
    else
    {
        cout << std::fixed << std::setprecision(1);
        cout << "Transfer Rate: " << (count / elapsedTime) * DATA_SIZE * INV_MEGA << " MB/s. (" << count / elapsedTime << " FPS)\n";
        cout << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
        count = 0;                      // reset counter
        timer.start();                  // restart timer
    }
}



///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip
    gluPerspective(60.0f, 1.0f, 1.0f, 10.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
    static int index = 0;
    int nextIndex = 0;                  // pbo index used for next frame

    if(pboMode > 0)
    {
        // "index" is used to copy pixels from a PBO to a texture object
        // "nextIndex" is used to update pixels in a PBO
        if(pboMode == 1)
        {
            // In single PBO mode, the index and nextIndex are set to 0
            index = nextIndex = 0;
        }
        else if(pboMode == 2)
        {
            // In dual PBO mode, increment current index first then get the next index
            index = (index + 1) % 2;
            nextIndex = (index + 1) % 2;
        }

        // start to copy from PBO to texture object ///////
        t1.start();

        // bind the texture and PBO
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[index]);

        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, PIXEL_FORMAT, GL_FLOAT, 0);

        // measure the time copying data from PBO to texture object
        t1.stop();
        copyTime = t1.getElapsedTimeInMilliSec();
        ///////////////////////////////////////////////////


        // start to modify pixel values ///////////////////
        t1.start();

        // bind PBO to update pixel values
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[nextIndex]);

        // map the buffer object into client's memory
        // Note that glMapBufferARB() causes sync issue.
        // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
        // for GPU to finish its job. To avoid waiting (stall), you can call
        // first glBufferDataARB() with NULL pointer before glMapBufferARB().
        // If you do that, the previous data in PBO will be discarded and
        // glMapBufferARB() returns a new allocated pointer immediately
        // even if GPU is still working with the previous data.
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
        GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        if(ptr)
        {
            // update data directly on the mapped buffer
            updatePixels(ptr, DATA_SIZE);
            glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
        }

        // measure the time modifying the mapped buffer
        t1.stop();
        updateTime = t1.getElapsedTimeInMilliSec();
        ///////////////////////////////////////////////////

        // it is good idea to release PBOs with ID 0 after use.
        // Once bound with 0, all pixel operations behave normal ways.
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    }
    else
    {
        ///////////////////////////////////////////////////
        // start to copy pixels from system memory to textrure object
        t1.start();

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, PIXEL_FORMAT, GL_FLOAT, (GLvoid*)imageData);

        t1.stop();
        copyTime = t1.getElapsedTimeInMilliSec();
        ///////////////////////////////////////////////////


        // start to modify pixels /////////////////////////
        t1.start();
        updatePixels(imageData, DATA_SIZE);
        t1.stop();
        updateTime = t1.getElapsedTimeInMilliSec();
        ///////////////////////////////////////////////////
    }

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform camera
    glTranslatef(0, 0, -cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading

    // draw a point with texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, -1.0f - 2.0f/IMAGE_HEIGHT * cutofbottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f);   glVertex3f( 1.0f + 2.0f/IMAGE_WIDTH * cutofright, -1.0f - 2.0f/IMAGE_HEIGHT * cutofbottom, 0.0f);
    glTexCoord2f(1.0f, 1.0f);   glVertex3f( 1.0f + 2.0f/IMAGE_WIDTH * cutofright,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f,  1.0f, 0.0f);

    glEnd();

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // draw info messages
    printTransferRate();

    glPopMatrix();

    glutSwapBuffers();
}

void reshapeCB(int width, int height)
{
    screenWidth = width;
    screenHeight = height;

    toPerspective();
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void idleCB()
{
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        exit(0);
        break;

    default:
        ;
    }
}

void exitCB()
{
    clearSharedMem();
#ifdef _WIN32
	#warn modesetting not supported  
#else
    if( strlen(activemode) )
    {
        enable_output( &VGAname[0],&activemode[0], vgax, vgay );
        disable_output( &VGAname[0] );
        rm_mode( &VGAname[0],"newmode" );
        enable_output( &VGAname[0],&activemode[0], vgax, vgay );
    }
#endif
}
