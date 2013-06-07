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
#include <GL/glx.h>
#endif

#include "glext.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "shader.h"

#include "glInfo.h"                             // glInfo struct
#include "Timer.h"
#include "fifo.h"

#ifdef _WITH_XRANDR
#include "xrandr.h"
#endif

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

bool initSharedMem();
void clearSharedMem();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void showInfo();
void printTransferRate();
void usage();
void *input_thread_func( void* in_ptr );

// constants
const float  CAMERA_DISTANCE = sqrt(3.0f);
const int    TEXT_WIDTH      = 8;
const int    TEXT_HEIGHT     = 13;
const int    IMAGE_WIDTH = 200;
const int    IMAGE_HEIGHT = 800;
const int    CHANNEL_COUNT = 2;
const int    BYTE_PER_CHANNEL = 4;
const int    DATA_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * CHANNEL_COUNT * BYTE_PER_CHANNEL;
const GLenum PIXEL_FORMAT = GL_RG;
#define PBO_COUNT 2

// global variables
void *font = GLUT_BITMAP_8_BY_13;

GLuint pboID[PBO_COUNT];           
GLuint textureId;                   // ID of texture
GLubyte* imageData = 0;             // pointer to texture buffer
GLubyte* testpatternA = 0;
GLubyte* testpatternB = 0;
//char *inputfile = 0;

int dataready = 0;
int screenWidth;
int screenHeight;
int enableconvolutional;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
bool pboisSupported;
bool fragmentisSupported;
bool forcefragmentoff;
bool beVerbose;
bool testpattern;
bool exit_now;
int pboMode = 0;
int cutofright = 1;
int cutofbottom = 3;
int fps = 0;
float max_baseband_freq = 3810000.0f;
int conv_depth = 17;

pthread_mutex_t exit_mutex;
pthread_t 	input_thread;

#ifdef _WITH_XRANDR
vga_t initial_config;
#endif

Timer timer, t1, t2;
float copyTime, updateTime;
GLint       program = 0;
GLint 	timeParam;

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
PFNGLXSWAPINTERVALEXTPROC pglXSwapIntervalEXT = 0;

#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#define glXSwapIntervalEXT 	  pglXSwapIntervalEXT
#endif


///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int i;
    float msps = 64.0;
    
#ifdef _WITH_XRANDR    
    vga_t new_config;
#endif

    if( argc < 2 ){
    	    cout << "No args given." << endl;
    	    usage();
    	    return -1;
    }

    initSharedMem();
    // register exit callback
    atexit(exitCB);
  
    for(i=1; i < argc; i++)
    {
        if( strcmp(argv[i],"--cutofright")==0 || strcmp(argv[i],"-r")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        cutofright=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"--cutofbottom")==0 || strcmp(argv[i],"-b")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        cutofbottom=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"-v")==0 || strcmp(argv[i],"--verbose")==0){
        	beVerbose = true;
        }

        if( strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
        	usage();
        	return 0;
        }
        
        if( strcmp(argv[i],"--pclk")==0 || strcmp(argv[i],"-p")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        msps=atof(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"--freq")==0 || strcmp(argv[i],"-f")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        max_baseband_freq=atof(argv[i+1]);
        	        i++;
        	    }
        	}
        }  
        
        if( strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--depth")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        conv_depth=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }    
        
        if( strcmp(argv[i],"--nofilter")==0 || strcmp(argv[i],"-n")==0 ){
        	forcefragmentoff = false;
        }
        if( strcmp(argv[i],"-t")==0 || strcmp(argv[i],"--testpattern")==0 ){
        	testpattern = true; 
        }
#if 0
        if( strcmp(argv[i],"-i")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        inputfile = strdup(argv[i+1]); 
        	        i++;
        	    }
        	}
        }
#endif        
    }

    fps = (float)(IMAGE_WIDTH * 7 * IMAGE_HEIGHT) / msps;
    
    if( beVerbose ){
    	cout << "hsync: " << cutofright << " vsync: " << cutofbottom << endl;
	cout << "msps: " << msps << " fps: " << fps << endl;
	cout << "testpattern: " << testpattern << endl;
    }
    screenWidth = (IMAGE_WIDTH - cutofright) * 7;
    screenHeight = IMAGE_HEIGHT - cutofbottom;
    
#ifdef _WITH_XRANDR 
#ifdef _WIN32
	#warn modesetting not supported  
	memset( &new_config, 0, sizeof(vga_t) );
#else


    if( init_xrandr() )
    {
    	cout << "failed to init xrandr" << endl;	    
    	exit(1);
    }
    
    if( find_VGA_output( &initial_config ) ){
    	    cout << "couldn't find VGA port" << endl;
    	    exit(1);
    }
    
    if( beVerbose )
    {
    	    cout << "found port: " << initial_config.outputname << " @ " << initial_config.pos_x << "," << initial_config.pos_y << " mode: " << initial_config.mode << endl;
        if( !strlen(initial_config.activemode) )
            cout << "cannot detect mode. no mode set ?" << endl;
    }
    
    memcpy( &new_config, &initial_config, sizeof(vga_t) );

    //test if newmode is active, if so remove it
    if( strstr(new_config.activemode,"newmode") )
    {
    	if( beVerbose )
    	    cout << "newmode is active, removing it now..." << endl;
    
    	memcpy(new_config.activemode,"1024x768", sizeof("1024x768"));
        enable_output( &new_config );
        disable_output( &new_config );
        memcpy(new_config.activemode,"newmode", sizeof("newmode"));
        
        rm_mode( &new_config );
    }
	
    // add new modeline 
    add_custom_mode( &new_config, msps, screenHeight, cutofright, cutofbottom );
    
    if( beVerbose )
    	    cout << "added custom mode: newmode" << endl;
    
    // set mode
    memcpy(new_config.activemode,"newmode", sizeof("newmode"));
    enable_output( &new_config );
    
    if( beVerbose )
    	    cout << "set mode \"newmode\" on VGA " << endl;
#endif
#endif
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA); // display mode

    glutInitWindowSize(screenWidth, screenHeight);               // window size
#ifdef _WITH_XRANDR
    glutInitWindowPosition(new_config.pos_x, new_config.pos_y);           // window location
#endif

    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    glutCreateWindow(argv[0]);     // param is the title of window  
    //glutFullScreen();

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutIdleFunc(idleCB);                       // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);

    // init GL
    glShadeModel(GL_FLAT);                      // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable / disable features
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glClearColor(0.0f, 0.0f, 0.5f, 0);                   // background color
    
    // init 2 texture objects
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, IMAGE_WIDTH, IMAGE_HEIGHT, 0, PIXEL_FORMAT, GL_FLOAT, (GLvoid*)imageData); //GL_RGBA12
    glBindTexture(GL_TEXTURE_2D, 0);

#ifdef _WIN32

    // check PBO is supported by your video card

    if(glutExtensionSupported("GL_ARB_pixel_buffer_object"))
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
            pboMode = PBO_COUNT;    // using 2 PBO
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
    
#else // for linux, do not need to get function pointers, it is up-to-date

    if(glutExtensionSupported("GL_ARB_pixel_buffer_object"))
    {

        pboisSupported = true;
        pboMode = PBO_COUNT;
        //cout << "Video card supports GL_ARB_pixel_buffer_object." << endl;
    }
    else
    {
        pboisSupported = false;
        pboMode = 0;
        if( beVerbose )
        	cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << endl;
    }
    
#endif

    if(glutExtensionSupported("GL_ARB_fragment_shader"))
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
    
    if(glutExtensionSupported("GLX_EXT_swap_control"))
    {
    	if( beVerbose )
    		cout << "Video card supports GLX_EXT_swap_control." << endl;
#ifdef _WIN32
    	        glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXT)wglGetProcAddress("glXSwapIntervalEXT");
    	        if( pglXSwapIntervalEXT ){
    	           	glXSwapIntervalEXT(1);
    	           	if( beVerbose )
    	           		cout << "Vsync enabled." << endl;
    	        }
#else
	#warning setting vsync not implemented yet
    	 //glXSwapIntervalEXT(1);
    	 if( beVerbose )
    	 	 cout << "Vsync enabled." << endl;      
#endif
    }
    
    if( pboisSupported )
    {
        // create PBO_COUNT pixel buffer objects, you need to delete them when program exits.
        // glBufferDataARB with NULL pointer reserves only memory space.
	for(i=0;i<PBO_COUNT;i++){
        	glGenBuffersARB(1, &pboID[i]);
        	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[i]);
        	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
	}
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    }
    
    if( fragmentisSupported && !forcefragmentoff)
    	    setShaders( screenWidth, screenHeight, beVerbose, max_baseband_freq, (float)msps*1000000.0f, conv_depth);
   
    pthread_mutex_init( &exit_mutex, NULL );
    pthread_create( &input_thread, NULL, input_thread_func, NULL ); 
    
    // start timer, the elapsed time will be used for updateVertices()
    timer.start();

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    while( !BufferFull() )
    {
    	usleep( 5000 );
    }
    glutMainLoop(); /* Start GLUT event-processing loop */
    
    pthread_mutex_destroy( &exit_mutex );
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// print help message
///////////////////////////////////////////////////////////////////////////////
void usage(void)
{
	cout << "Usage:\nvgatoiqbaseband [inputfile] [args]\nargs can be:" << endl;
	cout << "\t-h , --help\t\tthis message" << endl;	
	cout << "\t-v , --verbose\t\tbe verbose" << endl;	
	cout << "\t-nofilter\t\tturn off fragment-shader" << endl;
	cout << "\t-pclk <x>\t\tset the pixel clock rate to x MSps" << endl;	
#ifdef _WITH_XRANDR
	cout << "\t-cutofright <x>\t\tdo not display x elements at the right screen border\tdefault: 1 (7 pixel)" << endl;	
	cout << "\t-cutofbottom <x>\t\tdo not display x elements at the bottom screen border\tdefault: 2" << endl;	
#endif
	cout << "\t-t\t\t\tdo not read inputfile, generate test patterns" << endl;		
}

///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    float* ptr;
    enableconvolutional = 0;

    cameraAngleX = cameraAngleY = 0;
    cameraDistance = CAMERA_DISTANCE;
    
    beVerbose = false;
    testpattern = false;
    forcefragmentoff = false;
    exit_now = false;
    
    BufferInit( DATA_SIZE );
    //inputfile = new char(255);
    //memset(inputfile, 0, 255);

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
#if 0
		    if(j == IMAGE_WIDTH/2)
		    {
			    *ptr = float(1.0f);
			    ++ptr;
			    *ptr = float(1.0f);
		    }
		    else if(j == IMAGE_WIDTH/4 || j == 3*IMAGE_WIDTH/4 )
		    {
			    *ptr = float(0.0f);
			    ++ptr;
			    *ptr = float(0.0f);
		    }
		    else
		    {
			    *ptr = float(0.5f);
			    ++ptr;
			    *ptr = float(0.5f);
		    }
#else

		if(j == IMAGE_WIDTH - 4)
		    {
			    *ptr = float(1.0f);
			    ++ptr;
			    *ptr = float(1.0f);
		    }
		else if(j == IMAGE_WIDTH - 3)
		    {
			    *ptr = float(0.0f);
			    ++ptr;
			    *ptr = float(0.0f);
		    }
		else if(j == IMAGE_WIDTH - 2)
		    {
			    *ptr = float(1.0f);
			    ++ptr;
			    *ptr = float(1.0f);
		    }
		else if(j == 0)
		    {
			    *ptr = float(1.0f);
			    ++ptr;
			    *ptr = float(1.0f);
		    }
		else
		    {
			    *ptr = float(0.5f);
			    ++ptr;
			    *ptr = float(0.5f);
		    }
#endif
		    ++ptr;

	    } 
    }
    
    // allocate texture buffer
    testpatternB = new GLubyte[DATA_SIZE];
    ptr = (float*)testpatternB;
    for(int i = 0; i < (IMAGE_HEIGHT * IMAGE_WIDTH); i++)
    {
    	    *ptr = float(sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f)*0.25f+0.5f);
    	    ++ptr;
    	    *ptr = float(sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f + 3.1415928f/2.0f)*0.25f+0.5f);
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
    
    // remove fifo buffer
    BufferClean();
    
    // clean up texture
    glDeleteTextures(1, &textureId);

    // clean up PBOs
    if( pboisSupported )
    {
        glDeleteBuffersARB(PBO_COUNT, &pboID[0]);
    }
    
    if( !testpattern )
    {
    	    
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


void *input_thread_func( void* in_ptr )
/* input queue */
{
    static int cnt = 0;
    	if( beVerbose )
    		cout << "input thread started" << endl;
	while( 1 )
	{
	   if( !BufferFull() )
	   {
		if( testpattern )	/* generate pattern */
		{
		   
			   cnt++;
			   if(cnt < 300)
			   {
				memcpy( BufferIn_Start(), testpatternA, DATA_SIZE );
				BufferIn_Finish();
			   }
			   else if ( (cnt >= 300) && (cnt < 600))
			   {
				memcpy( BufferIn_Start(), testpatternB, DATA_SIZE );
				BufferIn_Finish();
			   }
			   else
			   {
				cnt = 0;   
			   }
		}
#if 0    
	    	else if( strlen(inputfile) )	/* read from inputfile */
	    	{
		    unsigned char *ptr;
		    int read_bytes;
		    filestr.open (inputfile, ifstream::binary);
		    if ( filestr.fail() ){
			    pthread_mutex_lock (&exit_mutex);
			    exit_now = true;
			    pthread_mutex_unlock (&exit_mutex);
			    cout << "Error opening file." << endl;
			    break;
		    }
	
		    ptr = (unsigned char*) BufferIn_Start();
		    filestr.read ( (char*)ptr, DATA_SIZE );
		    BufferIn_Finish();
		    read_bytes = filestr.gcount();
		    if( read_bytes < DATA_SIZE )
		    {
			    cout << "WARN: couldn't read enough" << endl;
			    cout << "WARN: exit now ?" << endl;
			    memset( ptr + read_bytes, 0 , DATA_SIZE - read_bytes );
			    pthread_mutex_lock (&exit_mutex);
			    exit_now = true;
			    pthread_mutex_unlock (&exit_mutex);
			    break;
		    }
	    }
#endif
	       else	/* read from stdin */
	       {
	    	    unsigned char *ptr;
		    int read_bytes;
		    ptr = (unsigned char*)BufferIn_Start();
		    read_bytes = fread(ptr, 1, DATA_SIZE, stdin);
		    BufferIn_Finish();
		    if( read_bytes < DATA_SIZE )
		    {
		    	    if( beVerbose )
		    	    	    cout << "read from stdin failed" << endl;
			    memset( ptr + read_bytes, 0 , DATA_SIZE - read_bytes );
			    pthread_mutex_lock (&exit_mutex);
			    exit_now = true;
			    pthread_mutex_unlock (&exit_mutex);
			    break;
		    }
	       }
	    }
	    else
	    {
		usleep( 2000 );
	    }
	}

    return NULL;
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
    	if( beVerbose ){
    		cout << std::fixed << std::setprecision(1);
    		cout << "Transfer Rate: " << (count / elapsedTime) * DATA_SIZE * INV_MEGA << " MB/s. (" << count / elapsedTime << " FPS)\n";
    		cout << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
    	}
    	
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
	
    // notify GLUT to exit after this function ends
    pthread_mutex_lock (&exit_mutex);
    if( exit_now && BufferEmpty() )
    	    exit(0);
    pthread_mutex_unlock (&exit_mutex);
    
    if(pboMode > 0)
    {
        // "index" is used to copy pixels from a PBO to a texture object
        // "nextIndex" is used to update pixels in a PBO
        if(pboMode == 1)
        {
            // In single PBO mode, the index and nextIndex are set to 0
            index = nextIndex = 0;
        }
        else
        {
            // In dual PBO mode, increment current index first then get the next index
            index = (index + 1) % pboMode;
            nextIndex = (index + 1) % pboMode;
        }

        // start to copy from PBO to texture object ///////
        t1.start();

        // bind the texture and PBO
       	glBindTexture(GL_TEXTURE_2D, textureId);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[index]);

        // copy pixels from PBO to texture object
        // Use offset instead of pointer.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, PIXEL_FORMAT, GL_FLOAT, 0);

        // measure the time copying data from PBO to texture object
        t1.stop();
        copyTime = t1.getElapsedTimeInMilliSec();
        ///////////////////////////////////////////////////


        // start to modify pixel values ///////////////////
        t1.start();
        
        // bind PBO to update pixel values
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[nextIndex]);

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
        	if( BufferEmpty() )
        	{
        		if( beVerbose )
        			cout << "Buffer empty ! waiting...\n" << endl;
        		usleep( 500000.0f / fps );
        		/* still empty ? drop frame */
        		if( BufferEmpty() )
        		{
        			if( beVerbose )
        				cout << "Buffer underrun ! Skipping frame.\n" << endl;
        			memset( ptr, 0, DATA_SIZE );
        			BufferSkip();
        		}
        		else
        		{
        			memcpy( ptr, BufferOut_Start(), DATA_SIZE );
        			BufferOut_Finish();
        		}
        	}
        	else
        	{
        		memcpy( ptr, BufferOut_Start(), DATA_SIZE );
        		BufferOut_Finish();
        	}
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
        
        if( BufferEmpty() )
        {
        	usleep( 500000.0f / fps );
        	/* still empty ? drop frame */
        	if( BufferEmpty() )
        	{
        		cout << "Buffer underrun ! Skipping frame.\n" << endl;
        		memset( imageData, 0, DATA_SIZE);
        		
        		BufferSkip();
        	}
        	else
        	{
        		memcpy( imageData, BufferOut_Start(), DATA_SIZE );
        		BufferOut_Finish();
        	}
        }
        else
        {
        	memcpy( imageData, BufferOut_Start(), DATA_SIZE );
        	BufferOut_Finish();	
        }

        t1.stop();
        copyTime = t1.getElapsedTimeInMilliSec();
        
        ///////////////////////////////////////////////////
        t1.start();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, PIXEL_FORMAT, GL_FLOAT, (GLvoid*)imageData);
        t1.stop();
        updateTime = t1.getElapsedTimeInMilliSec();
    }
    //cout << "copy time " << copyTime << "ms , updatetime " << updateTime << "ms" << endl;
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

    glTexCoord2f(0.0f, 1.0f);   glVertex3f( -1.0f, -1.0f - 2.0f/IMAGE_HEIGHT * cutofbottom, 0.0f);
    glTexCoord2f(1.0f, 1.0f);   glVertex3f( 1.0f + 2.0f/IMAGE_WIDTH * cutofright, -1.0f - 2.0f/IMAGE_HEIGHT * cutofbottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f);   glVertex3f( 1.0f + 2.0f/IMAGE_WIDTH * cutofright,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( -1.0f,  1.0f, 0.0f);
    glEnd();

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // draw info messages
    printTransferRate();
    
    glPopMatrix();

    glutSwapBuffers();
    //glutPostRedisplay();
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
  
}

void exitCB()
{
    clearSharedMem();
#ifdef _WIN32
	#warn modesetting not supported  
#else
#ifdef _WITH_XRANDR
    if( strlen(initial_config.activemode) > 1)
    {
    	char initialmode[255];
    	    
        enable_output( &initial_config );
        disable_output( &initial_config );
        memcpy(initialmode, initial_config.activemode, sizeof(initial_config.activemode) );
        memcpy(initial_config.activemode,"newmode", sizeof("newmode"));
        rm_mode( &initial_config );
        memcpy(initial_config.activemode, initialmode, sizeof(initial_config.activemode) );        
        enable_output( &initial_config );
    }
#endif
#endif
}

