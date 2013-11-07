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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "shader.h"

#include "timer.h"
#include "fifo.h"
#include "shader.h"
#include "xrandr.h"


#ifdef _DEBUG
	static void CheckOpenGLError(const char* stmt, const char* fname, int line)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			printf("OpenGL error %08x,", err);
			if( err == GL_INVALID_ENUM ) printf(" GL_INVALID_ENUM ");
			if( err == GL_INVALID_VALUE ) printf(" GL_INVALID_VALUE ");
			if( err == GL_INVALID_OPERATION ) printf(" GL_INVALID_OPERATION ");
			if( err == GL_STACK_OVERFLOW ) printf(" GL_STACK_OVERFLOW ");
			if( err == GL_STACK_UNDERFLOW ) printf(" GL_STACK_UNDERFLOW ");
			printf("at %s:%i - for %s\n", fname, line, stmt);
			abort();
		}
	}
    #define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
    #define GL_CHECK(stmt) stmt
#endif

// GLUT CALLBACK functions ////////////////////////////////////////////////////
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void clearSharedMem();
void showInfo();
void printTransferRate();
void usage();
void *input_thread_func( void* in_ptr );

// constants
#define			CHANNEL_COUNT 	2
#define		 	PIXEL_FORMAT 	GL_RG
#define 		PBO_COUNT 		2
#define			INTERNAL_PIXELFORMAT (bytesperchannel == 1) ? GL_RG8 : (bytesperchannel == 2) ? GL_RG16 : GL_RG32F //GL_RGBA8, GL_RGBA16F, GL_RGB8, GL_RGB16F
GLuint pboID[PBO_COUNT];           
GLuint textureId;                   // ID of texture
GLubyte* imageData = 0;             // pointer to texture buffer
GLubyte* testpatternA = 0;
GLubyte* testpatternB = 0;
    
int dataready = 0;
int enableconvolutional;
int pboisSupported = 0;
int forcefragmentoff = 0;
int beVerbose = 0;
int testpattern = 0;
int exit_now = 0;
int pboMode = 0;
int cutofright = 3;
int cutofbottom = 3;
int bytesperchannel=1;
int fps = 60;
float max_baseband_freq = 3810000.0f;
int conv_depth = 11;

pthread_mutex_t exit_mutex;
pthread_t 	input_thread;

vga_t vga_config;

float copyTime, updateTime;
mytimer_t timer, t1;

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

#define CUTOFRIGHT (vga_config.hTotal - vga_config.width)
#define CUTOFBOTTOM (vga_config.vTotal - vga_config.height)
#define DATASIZE vga_config.hTotal * vga_config.vTotal * CHANNEL_COUNT * bytesperchannel 

static GLenum GLgettype( int datasize )
{
	if(datasize == 4)
		return GL_FLOAT;
	if(datasize == 2)
		return GL_UNSIGNED_SHORT;
	if(datasize == 1)
		return GL_UNSIGNED_BYTE;
	fprintf( stderr, "ERROR: wrong datasize\nTerminating.\n" );
	exit( 1 );
}

static void GLconverttype( void *ptr, float val, int datasize )
{
	if(datasize == 4)
		*((float*)ptr) = (float)val;
	if(datasize == 2)
		*((uint16_t*)ptr) = (uint16_t)(val*65535.0);
	if(datasize == 1)
		*((uint8_t*)ptr) = (uint8_t)(val*255.0);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int i;
    
#ifdef _WIN32
	#warn modesetting not supported  
#else
    if( init_xrandr() )
    {
    	fprintf( stderr, "Error: failed to init xrandr\n" );
    	exit(1);
    }


    if( get_output_by_name( &vga_config , "VGA") && get_output_by_name( &vga_config , "CRT") ){
		fprintf( stderr, "WARN: couldn't find VGA output\n" );
		if( get_output_by_name( &vga_config , NULL ) )
		{
			fprintf( stderr, "ERR: couldn't find any output\n" );
		}
    }
#endif

	cutofright = (vga_config.hTotal - vga_config.width);
	cutofbottom = (vga_config.vTotal - vga_config.height);

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
        
        if( strcmp(argv[i],"--datasize")==0 || strcmp(argv[i],"-z")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        bytesperchannel=atoi(argv[i+1]);
        	        i++;
        	    }
        	}
        }
        
        if( strcmp(argv[i],"-v")==0 || strcmp(argv[i],"--verbose")==0){
        	beVerbose = 1;
        }

        if( strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
        	usage();
        	return 0;
        }
        
        if( strcmp(argv[i],"--pclk")==0 || strcmp(argv[i],"-p")==0 ){
        	if(argc > (i+1)){
        	    if(!(argv[i+1][0] == '-')){
        	        vga_config.dotClock=atoi(argv[i+1]);
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
        	forcefragmentoff = 1;
        }
        if( strcmp(argv[i],"-t")==0 || strcmp(argv[i],"--testpattern")==0 ){
        	testpattern = 1; 
        }       
    }

    if( vga_config.width == 0 || vga_config.height == 0 ){
		fprintf( stdout, "WARN: VGA port seems to be unconfigured.\n" );
		vga_config.width = 1366;
		vga_config.height = 768; 
		vga_config.hTotal = vga_config.width + 3;
		vga_config.vTotal = vga_config.height + 3;
		vga_config.dotClock = vga_config.hTotal * vga_config.vTotal * 30;
    }
    
    fps = vga_config.dotClock / ( vga_config.hTotal * vga_config.vTotal );    

    if( BufferInit( DATASIZE ) )
    {
    	fprintf( stderr, "Error: out of memory!\n" );
    	exit(1);
    }
    
    // allocate texture buffer
    imageData = (GLubyte*)malloc( DATASIZE );
    memset(imageData, 0x00, DATASIZE);

    if( testpattern )
    {
			void* ptr; 
			// allocate texture buffer
			testpatternA = (GLubyte*)malloc( DATASIZE );
			ptr = testpatternA;
			for(int i = 0; i < vga_config.vTotal; i++)
			{
				#if 1
				for(int j = 0; j < vga_config.hTotal; j++)
				{
					
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;						
				}
				#else
				for(int j = 0; j < vga_config.hTotal; j++)
				{
					if(j == vga_config.hTotal - 4)
					{
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
					}
					else if(j == vga_config.hTotal - 3)
					{
						GLconverttype( ptr, 0.0f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 0.0f, bytesperchannel );
						ptr+=bytesperchannel;
					}
					else if(j == vga_config.hTotal - 2)
					{
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
					}
					else if(j == 0)
					{
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 1.0f, bytesperchannel );
						ptr+=bytesperchannel;
					}
					else
					{
						GLconverttype( ptr, 0.5f, bytesperchannel );
						ptr+=bytesperchannel;
						GLconverttype( ptr, 0.5f, bytesperchannel );
						ptr+=bytesperchannel;

					}
					
				} 

				#endif
			}
			
			// allocate texture buffer
			testpatternB = (GLubyte*)malloc( DATASIZE );
			ptr = testpatternB;
			for(int i = 0; i < (vga_config.vTotal * vga_config.hTotal); i++)
			{
				GLconverttype( ptr, sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f)*0.25f+0.5f, bytesperchannel );
				ptr+=bytesperchannel;
				GLconverttype( ptr, sin(i*3.1415928f/180.0f*5.0f)*0.1f + sin(i*3.1415928f/180.0f + 3.1415928f/2.0f)*0.25f+0.5f, bytesperchannel );
				ptr+=bytesperchannel;
			}
    }
	
    // register exit callback
    atexit(exitCB);
    
    if( beVerbose )
    {
    	fprintf( stdout, "INFO: width %d , height %d\n" , vga_config.width , vga_config.height );
    	fprintf( stdout, "INFO: hTotal %d, vTotal %d, dotClock %d\n", vga_config.hTotal, vga_config.vTotal, vga_config.dotClock );
	    fprintf( stdout, "INFO: found port: %s@%d,%d %d\n" ,vga_config.outputname , vga_config.pos_x , vga_config.pos_y , vga_config.mode );
        if( !strlen(vga_config.activemode) )
            fprintf( stdout, "INFO: cannot detect mode. no mode set ?\n" );

		fprintf( stdout, "INFO: hsync: %d vsync: %d\n", cutofright , cutofbottom );
		fprintf( stdout, "INFO: pixelclock: %d fps: %d\n", vga_config.dotClock, fps );
		fprintf( stdout, "INFO: testpattern: %s\n", testpattern ? "TRUE" : "FALSE" );
		fprintf( stdout, "INFO: force fragment off: %s\n", forcefragmentoff ? "TRUE" : "FALSE" );
		fprintf( stdout, "INFO: convolutional depth: %d\n",conv_depth );
    }

#if 0
    memcpy( &new_config, &vga_config, sizeof(vga_t) );

    //test if newmode is active, if so remove it
    if( strstr(new_config.activemode,"newmode") )
    {
    	if( beVerbose )
    	    fprintf( stdout, "INFO: newmode is active, removing it now...\n" );
    
    	memcpy(new_config.activemode,"1024x768", sizeof("1024x768"));
        enable_output( &new_config );
        disable_output( &new_config );
        memcpy(new_config.activemode,"newmode", sizeof("newmode"));
        
        rm_mode( &new_config );
    }
	
    // add new modeline 
    add_custom_mode( &new_config, vga_config.dotClock , screenHeight, cutofright, cutofbottom );
    
    if( beVerbose )
		fprintf( stdout, "added custom mode: newmode\n" );
    
    // set mode
    memcpy(new_config.activemode,"newmode", sizeof("newmode"));
    enable_output( &new_config );
    
    if( beVerbose )
		fprintf( stdout, "set mode \"newmode\" on VGA \n" );
#endif

    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine

    glutInit( &argc, argv );
    glutInitDisplayString( "red=10 green=10 blue=10 depth double" ); 
  
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA ); // display mode
    glutInitWindowSize( vga_config.width, vga_config.height );               // window size
    glutInitWindowPosition( vga_config.pos_x, vga_config.pos_y );           // window location

    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    glutCreateWindow( argv[0] );     // param is the title of window  
    glutFullScreen();
	glutSetCursor( GLUT_CURSOR_NONE );
#if 0
    GLint red_bits, green_bits, blue_bits;
    GL_CHECK( glGetIntegerv(GL_RED_BITS, &red_bits) )
    GL_CHECK( glGetIntegerv(GL_GREEN_BITS, &green_bits) )
    GL_CHECK( glGetIntegerv(GL_BLUE_BITS, &blue_bits) )

    if( beVerbose )
		fprintf( stdout, "red bits %d green bits %d blue bits %d\n", red_bits , green_bits , blue_bits );
#endif    
    
    // register GLUT callback functions
    glutDisplayFunc( displayCB );
    glutIdleFunc( idleCB );                       // redraw only every given millisec
    //glutReshapeFunc( reshapeCB );

    GL_CHECK( glViewport(0, 0, vga_config.width, vga_config.height) );

    GL_CHECK( glMatrixMode(GL_PROJECTION) );
    GL_CHECK( glLoadIdentity() );
    if( vga_config.width > vga_config.height ){
        GL_CHECK( glOrtho(0.0f, 1.0f, 0.0f, (float)vga_config.height / (float)vga_config.width, -1.0f, 1.0f) );
    }
    else
    {
        GL_CHECK( glOrtho(0.0f, (float)vga_config.width / (float)vga_config.height, 0.0f, 1.0f, -1.0f, 1.0f) );
    }
    GL_CHECK( glMatrixMode(GL_MODELVIEW) );
    GL_CHECK( glLoadIdentity() );

    if( vga_config.width > vga_config.height ){
       //Set world coordinates to coincide with screen pixels
       GL_CHECK( glScalef(1.0f / (float)vga_config.width , 1.0f / (float)vga_config.width, 1.0f) );
    }
    else
    {
       //Set world coordinates to coincide with screen pixels
       GL_CHECK( glScalef(1.0f / (float)vga_config.height, 1.0f / (float)vga_config.height, 1.0f) );
    }
    
    //fix for buggy drivers
    GL_CHECK( glBindTexture(GL_TEXTURE_2D,0) );

	//no smooth shading
    GL_CHECK( glShadeModel(GL_FLAT) );
    
    // 4-byte pixel alignment
    GL_CHECK( glPixelStorei(GL_UNPACK_ALIGNMENT, bytesperchannel) );
    GL_CHECK( glDisable(GL_LIGHTING) );
    GL_CHECK( glEnable(GL_TEXTURE_2D) );
    GL_CHECK( glEnable(GL_BLEND) );
    // just add the colors, do not use alpha
    GL_CHECK( glBlendFunc(GL_ONE, GL_ONE) );
    // background color, set blue channel to 50%
    GL_CHECK( glClearColor(0.0f, 0.0f, 0.5f, 1.0f) );                   
    
    // init a texture object
    GL_CHECK( glGenTextures(1, &textureId) );
    GL_CHECK( glBindTexture(GL_TEXTURE_2D, textureId) );



	GL_CHECK( glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_PIXELFORMAT, vga_config.hTotal, vga_config.vTotal, 0, PIXEL_FORMAT, GLgettype(bytesperchannel), (GLvoid*)imageData) ); //GL_RGBA12 	   
	
	/* call after glTexImage2D !!!! */
    GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
    GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
  	GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP) );
	GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP) );  
	GL_CHECK( glBindTexture(GL_TEXTURE_2D, 0) );
 
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
            pboisSupported = 1;
            pboMode = PBO_COUNT;    // using 2 PBO
            //cout << "Video card supports GL_ARB_pixel_buffer_object." << endl;
        }
        else
        {
            pboisSupported = 0;
            pboMode = 0;    // without PBO
            if( beVerbose )
				fprintf( stdout, "INFO: Video card does NOT support GL_ARB_pixel_buffer_object.\n" );
        }
    }
    
#else // for linux, do not need to get function pointers, it is up-to-date

    //glutExtensionSupported only works with an active window !
    if( glutExtensionSupported("GL_ARB_pixel_buffer_object") )
    {
        pboisSupported = 1;
        pboMode = PBO_COUNT;
        if( beVerbose )
        	fprintf( stdout, "INFO: Video card supports GL_ARB_pixel_buffer_object.\n" );
    }
    else
    {
        pboisSupported = 0;
        pboMode = 0;
        if( beVerbose )
        	fprintf( stdout, "INFO: Video card does NOT support GL_ARB_pixel_buffer_object.\n" );
    }
#endif
    if( glutExtensionSupported("GL_ARB_fragment_shader") )
    {
		if( !forcefragmentoff )
    	    setShaders( vga_config.width, vga_config.height, beVerbose, max_baseband_freq, (float)vga_config.dotClock, conv_depth );
        if( beVerbose )
        	fprintf( stdout, "INFO: Video card supports GL_ARB_fragment_shader.\n" );
    }
    else
    {
        if( beVerbose )
        	fprintf( stdout, "INFO: Video card does NOT support GL_ARB_fragment_shader.\n" );
    }
    
    if( glutExtensionSupported("GLX_EXT_swap_control") )
    {
    	if( beVerbose )
    		fprintf( stdout, "INFO: Video card supports GLX_EXT_swap_control.\n");
#ifdef _WIN32
    	        glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXT)wglGetProcAddress("glXSwapIntervalEXT");
    	        if( pglXSwapIntervalEXT ){
    	           	glXSwapIntervalEXT(1);
    	           	if( beVerbose )
    	           		fprintf( stdout, "Vsync enabled.\n" );
    	        }
#else
	#warning setting vsync not implemented yet
    	 //glXSwapIntervalEXT(1);
    	 if( beVerbose )
    	 	 fprintf( stdout, "Vsync enabled.\n" );      
#endif
    }
       // pboisSupported = 0;
       // pboMode = 0;
    if( pboisSupported )
    {
        // create PBO_COUNT pixel buffer objects, you need to delete them when program exits.
        // glBufferDataARB with NULL pointer reserves only memory space.
        GL_CHECK( glGenBuffersARB(PBO_COUNT, pboID) );
		for(i=0;i<PBO_COUNT;i++){
        	GL_CHECK( glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[i]) );
        	GL_CHECK( glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATASIZE, 0, GL_STREAM_DRAW_ARB) );
		}
        GL_CHECK( glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0) );
    }
   
    pthread_mutex_init( &exit_mutex, NULL );
    pthread_create( &input_thread, NULL, input_thread_func, NULL ); 

	Timer_init( &timer );
	Timer_start( &timer );
	Timer_init( &t1 );
    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    while( !BufferFull() )
    {
    	usleep( 10000 );
    }
    glutMainLoop(); // Start GLUT event-processing loop
    
    pthread_mutex_destroy( &exit_mutex );
    
    return 0;
}

void usage(void)
/* print help message */
{
	fprintf( stdout, 
	"Usage:\nvgatoiqbaseband [args]\n"
	"\t-h , --help\t\t\tthis message\n"
	"\t-v , --verbose\t\t\tbe verbose\n"
	"\t-n , --nofilter\t\t\tturn off fragment-shader\n"
	"\t-f <f>, --freq <f>\t\tset the maximum baseband frequency\n"
	"\t-d <x>, --depth <x>\t\tset the convolutional filter depth\n"
	"\t-z <x>, --datasize <x>\t\t4 for float, 2 for ushort, 1 for uchar, default short\n"
#if 0
	"\t-p <f>, --pclk <x>\t\tset the pixel clock rate to x Sps\n"
	"\t-r <x>, --cutofright <x>\t\tdo not display x elements at the right screen border\tdefault: 1 (7 pixel)\n";	
	"\t-b <x>, --cutofbottom <x>\t\tdo not display x elements at the bottom screen border\tdefault: 2\n";	
#endif
	"\t-t , --testpattern\t\tdo not read from stdin, generate test patterns\n"		
	"\t <x> is Integer, <f> is float\n" );

}

void clearSharedMem()
/* clean up memory */
{
    // deallocate texture buffer
    free( imageData );
    
    // remove fifo buffer
    BufferClean();
    
    // clean up texture
    glDeleteTextures(1, &textureId);

    // clean up PBOs
    if( pboisSupported )
    {
        glDeleteBuffersARB(PBO_COUNT, &pboID[0]);
    }
    
    if( testpattern )
    {
		free( testpatternA );
		free( testpatternB );
    }
}

void *input_thread_func( void* in_ptr )
/* input queue */
{
    static int cnt = 0;
    	if( beVerbose )
			fprintf( stdout, "INFO: input thread started\n" );
	while( 1 )
	{
	   if( !BufferFull() )
	   {
			if( testpattern )	/* generate pattern */
			{
			   
				   cnt++;
				   if(cnt < 300)
				   {
						memcpy( BufferIn_Start(), testpatternA, DATASIZE );
						BufferIn_Finish();
				   }
				   else if ( (cnt >= 300) && (cnt < 600))
				   {
						memcpy( BufferIn_Start(), testpatternB, DATASIZE );
						BufferIn_Finish();
				   }
				   else
				   {
					cnt = 0;   
				   }
			}
			else	/* read from stdin */
			{
				unsigned char *ptr;
				int read_bytes;
				ptr = (unsigned char*)BufferIn_Start();
				read_bytes = fread(ptr, 1, DATASIZE, stdin);
				BufferIn_Finish();
				if( read_bytes < DATASIZE )
				{
					if( beVerbose )
						fprintf( stderr, "Error: read from stdin failed\n" );
					memset( ptr + read_bytes, 0 , DATASIZE - read_bytes );
					pthread_mutex_lock (&exit_mutex);
					exit_now = 1;
					pthread_mutex_unlock (&exit_mutex);
					break;
				}
			}
	    }
	    else
	    {
			usleep( 10000 );
	    }
	}

    return NULL;
}

void printTransferRate()
/* print transfer rates */
{
    const double INV_MEGA = 1.0 / (1024 * 1024);
    static int count = 0;
    double elapsedTime;

    // loop until 1 sec passed
    elapsedTime = Timer_getElapsedTimeInMilliSec( &timer );
    
    count ++;
    if(elapsedTime >= 1000)
    {
    	if( beVerbose ){
			fprintf( stdout, "Transfer Rate: %f MB/s %f FPS\n" , ((double)count / elapsedTime) * DATASIZE * INV_MEGA * 1000, (double)count / elapsedTime * 1000 );
			fprintf( stdout, "elapsedTime: %f ms\n", elapsedTime );
    	}
    	
        count = 0;                      // reset counter
        Timer_start( &timer );          // restart timer
    }
}


void displayCB()
/* GLUT display callback */
{
    static int index = 0;
    int nextIndex = 0;                  // pbo index used for next frame
	
    // notify GLUT to exit after this function ends
    pthread_mutex_lock (&exit_mutex);
    if( exit_now && BufferEmpty() )
    {
		pthread_mutex_unlock (&exit_mutex);
		exit(0);
    }
    pthread_mutex_unlock (&exit_mutex);

    GL_CHECK( glBindTexture(GL_TEXTURE_2D, textureId) );
    
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
        Timer_start( &t1 );

        // bind the texture and PBO
        GL_CHECK( glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[index]) );

        // copy pixels from PBO to texture object
        // Use offset instead of pointer. 
		GL_CHECK( glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vga_config.hTotal, vga_config.vTotal, PIXEL_FORMAT, GLgettype(bytesperchannel), 0) );

        // measure the time copying data from PBO to texture object
        Timer_stop( &t1 );
        copyTime = Timer_getElapsedTimeInMilliSec( &t1 );

        // start to modify pixel values ///////////////////
        Timer_start( &t1 );
        
        // bind PBO to update pixel values
        GL_CHECK( glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID[nextIndex]) );

        // map the buffer object into client's memory
        // Note that glMapBufferARB() causes sync issue.
        // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
        // for GPU to finish its job. To avoid waiting (stall), you can call
        // first glBufferDataARB() with NULL pointer before glMapBufferARB().
        // If you do that, the previous data in PBO will be discarded and
        // glMapBufferARB() returns a new allocated pointer immediately
        // even if GPU is still working with the previous data.
        GL_CHECK( glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATASIZE, 0, GL_STREAM_DRAW_ARB) );
        GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        if(ptr)
        {
        	if( BufferEmpty() )
        	{
        		if( beVerbose )
        			fprintf( stdout, "Buffer empty ! waiting...\n" );
        		usleep( 500000 / fps );
        		/* still empty ? drop frame */
        		if( BufferEmpty() )
        		{
        			if( beVerbose )
        				fprintf( stdout, "Buffer underrun ! Skipping frame.\n" );
        			memset( ptr, 0, DATASIZE );
        			BufferSkip();
        		}
        		else
        		{
        			memcpy( ptr, BufferOut_Start(), DATASIZE );
        			BufferOut_Finish();
        		}
        	}
        	else
        	{
        		memcpy( ptr, BufferOut_Start(), DATASIZE );
        		BufferOut_Finish();
        	}
            GL_CHECK( glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB) ); // release pointer to mapping buffer
        }
		else
		{
			fprintf( stderr, "ERROR: glMapBufferARB failed.\n" );
		}
        // measure the time modifying the mapped buffer
        Timer_stop( &t1 );
        updateTime = Timer_getElapsedTimeInMilliSec( &t1 );
        ///////////////////////////////////////////////////

        // it is good idea to release PBOs with ID 0 after use.
        // Once bound with 0, all pixel operations behave normal ways.
        GL_CHECK( glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0) );
    }
    else
    {
        ///////////////////////////////////////////////////
        // start to copy pixels from system memory to textrure object
        Timer_start( &t1 );
                
        if( BufferEmpty() )
        {
        	usleep( 500000 / fps );
        	/* still empty ? drop frame */
        	if( BufferEmpty() )
        	{
        		fprintf( stdout, "Buffer underrun ! Skipping frame.\n" );
        		memset( imageData, 0,  DATASIZE );
        		
        		BufferSkip();
        	}
        	else
        	{
        		memcpy( imageData, BufferOut_Start(), DATASIZE );
        		BufferOut_Finish();
        	}
        }
        else
        {
        	memcpy( imageData, BufferOut_Start(), DATASIZE );
        	BufferOut_Finish();
        }

        Timer_stop( &t1 );
        copyTime = Timer_getElapsedTimeInMilliSec( &t1 );
        
        ///////////////////////////////////////////////////
        Timer_start( &t1 );
		GL_CHECK( glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vga_config.hTotal, vga_config.vTotal, PIXEL_FORMAT, GLgettype(bytesperchannel), (GLvoid*)imageData) );
        Timer_stop( &t1 );
        updateTime = Timer_getElapsedTimeInMilliSec( &t1 );
    }
//    fprintf( stdout, "copytime: %fms, updateTime %fms\n" , copyTime, updateTime );

    GL_CHECK( glClear(GL_COLOR_BUFFER_BIT) );
    GL_CHECK( glEnable(GL_BLEND) );
    GL_CHECK( glEnable(GL_TEXTURE_2D) );
	GL_CHECK( glEnableClientState(GL_VERTEX_ARRAY) );
	GL_CHECK( glEnableClientState(GL_NORMAL_ARRAY) );
	GL_CHECK( glEnableClientState(GL_TEXTURE_COORD_ARRAY) );
	//GL_CHECK( glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) );

	static const GLfloat vertices[4][3] = {
		{-1.0, -1.0, -0.0},
        { 1.0, -1.0, -0.0},
        {-1.0,  1.0, -0.0},
        { 1.0,  1.0, -0.0}

    };
    static const GLfloat normals[4][3] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}
    };
    static const GLfloat texCoords[8] = {
		0.0, 1.0,
		1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };

	//GL_CHECK( glColor4f(1.0, 1.0, 0.0, 0.5f) );
	GL_CHECK( glPushMatrix() );
	GL_CHECK( glBindTexture( GL_TEXTURE_2D, textureId ) );
	GL_CHECK( glTranslatef(vga_config.hTotal /2 , vga_config.vTotal /2, 0.0f) );
	GL_CHECK( glScalef(vga_config.hTotal/2, vga_config.vTotal/2, 1.0f) );
	GL_CHECK( glVertexPointer(3, GL_FLOAT, 0, vertices) );
	GL_CHECK( glNormalPointer(GL_FLOAT, 0, normals) );
	GL_CHECK( glTexCoordPointer(2, GL_FLOAT, 0, texCoords) );
	GL_CHECK( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
	GL_CHECK( glPopMatrix() );
	GL_CHECK( glBindTexture(GL_TEXTURE_2D,0) );

    GL_CHECK( glDisableClientState(GL_VERTEX_ARRAY) );
    GL_CHECK( glDisableClientState(GL_NORMAL_ARRAY) );
    GL_CHECK( glDisableClientState(GL_TEXTURE_COORD_ARRAY) );
    GL_CHECK( glDisable(GL_TEXTURE_2D) );
    GL_CHECK( glDisable(GL_BLEND) );

    glutSwapBuffers();
    printTransferRate();


    //glutPostRedisplay();
}

void reshapeCB(int width, int height)
/* GLUT reshape callback */
{
	fprintf( stdout, "reshapeCB width: %d height: %d", width, height );
}

void timerCB(int millisec)
/* GLUT timer callback */
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}

void idleCB()
/* GLUT idle callback */
{
    glutPostRedisplay();
}

void exitCB()
{
    clearSharedMem();
}

