#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

SDL_Surface *mysurface;

uint16_t slidenum = 0;


void scene_init();
void dummyscene ();
void (*sceneptr)(void) = dummyscene;


void cleanup (int in_retcode)
{
    SDL_Quit();
    exit(in_retcode);
}

void dummyscene ()
{
	static uint8_t init_done = 0;
	static GLuint dummy_texture[1];
	static GLfloat xrot = 0;


	if (!init_done)
	{
		SDL_Surface *mytexture[1];
		mytexture[0] = SDL_LoadBMP ("test.bmp");
		glGenTextures (1, &dummy_texture[0]);
		glBindTexture (GL_TEXTURE_2D, dummy_texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D (GL_TEXTURE_2D, 0, 3, mytexture[0]->w,
			mytexture[0]->h, 0, GL_BGR,
			GL_UNSIGNED_BYTE, mytexture[0]->pixels);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps (GL_TEXTURE_2D, 3, mytexture[0]->w,
			mytexture[0]->h, GL_BGR,
			GL_UNSIGNED_BYTE, mytexture[0]->pixels);
		SDL_FreeSurface (mytexture[0]);
		init_done = 1;
	}

	scene_init();
	glLoadIdentity ();
	glTranslatef (0.0f, 0.0f, -3.5f);
	glRotatef (xrot, 1.0f, 0.5f, 0.3f);
	xrot += 0.8f;

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f (1.0f,1.0f,1.0f);
	glBegin (GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.6f, -1.0f, 1.0f);
		glTexCoord2f( 1.6f, 1.0f ); glVertex3f(  1.6f, -1.0f, 1.0f);
		glTexCoord2f( 1.6f, 0.0f ); glVertex3f(  1.6f,  1.0f, 1.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.6f,  1.0f, 1.0f );

		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.6f, -1.0f, -1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.6f,  1.0f, -1.0f );
		glTexCoord2f( 1.6f, 1.0f ); glVertex3f(  1.6f,  1.0f, -1.0f );
		glTexCoord2f( 1.6f, 0.0f ); glVertex3f(  1.6f, -1.0f, -1.0f );

		glTexCoord2f( 1.6f, 1.0f ); glVertex3f( -1.6f,  1.0f, -1.0f );
		glTexCoord2f( 1.6f, 0.0f ); glVertex3f( -1.6f,  1.0f,  1.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.6f,  1.0f,  1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.6f,  1.0f, -1.0f );

		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.6f, -1.0f, -1.0f );
		glTexCoord2f( 1.6f, 1.0f ); glVertex3f(  1.6f, -1.0f, -1.0f );
		glTexCoord2f( 1.6f, 0.0f ); glVertex3f(  1.6f, -1.0f,  1.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.6f, -1.0f,  1.0f );

		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 1.6f, -1.0f, -1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 1.6f,  1.0f, -1.0f );
		glTexCoord2f( 1.6f, 1.0f ); glVertex3f( 1.6f,  1.0f,  1.0f );
		glTexCoord2f( 1.6f, 0.0f ); glVertex3f( 1.6f, -1.0f,  1.0f );

		glTexCoord2f( 1.6f, 0.0f ); glVertex3f( -1.6f, -1.0f, -1.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.6f, -1.0f,  1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.6f,  1.0f,  1.0f );
		glTexCoord2f( 1.6f, 1.0f ); glVertex3f( -1.6f,  1.0f, -1.0f );
	glEnd ();


	SDL_GL_SwapBuffers ();
}

void keyhandler(SDL_keysym *in_key)
{
	switch (in_key->sym)
	{
		case SDLK_ESCAPE:
			cleanup (0);
		break;

		case SDLK_f:
			SDL_WM_ToggleFullScreen (mysurface);
		break;

		case SDLK_LEFT:
			if (slidenum)
				slidenum--;
		break;

		case SDLK_RIGHT:
			slidenum++;
		break;

		default:
		break;
	}
}

void updatewindow (uint16_t in_w, uint16_t in_h)
{
	GLfloat myratio;

	if (!in_w || !in_h) return;

	myratio = ((GLfloat) in_w / (GLfloat) in_h);
	glViewport (0, 0, (GLint) in_w, (GLint) in_h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0f, myratio, 0.1f, 100.0f);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}


void scene_init()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
}


void initgl()
{
	GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 0.0f, -2.0f, 1.0f };

	scene_init();


	glEnable (GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearDepth (1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
}

int main (int argc, char *argv[])
{
	int sdlvflags;
	SDL_Event sdlev;
	const SDL_VideoInfo *sdlvinfo;
	uint8_t dodraw = 1;

	
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		printf("Gaaah, SDL error... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

	sdlvinfo = SDL_GetVideoInfo ();

	if (!sdlvinfo)
	{
		printf("Gaaah, SDL error when trying to get video info... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

	sdlvflags = (SDL_OPENGL |  SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE);

	if ( sdlvinfo->blit_hw )
		sdlvflags |= SDL_HWACCEL;

	if (sdlvinfo->hw_available)
		sdlvflags |= SDL_HWSURFACE;
	else
		sdlvflags |= SDL_SWSURFACE;

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	mysurface = SDL_SetVideoMode (800, 600, 16, sdlvflags);

	if (!mysurface)
	{
		printf("Gaaah, SDL error when trying to create surface... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

    	SDL_EnableKeyRepeat(50, SDL_DEFAULT_REPEAT_INTERVAL);
	
	initgl();

	updatewindow (1280, 800);

	
	while (23)
	{
		while (SDL_PollEvent (&sdlev))
		{
			switch( sdlev.type )
			{
				case SDL_ACTIVEEVENT:
					dodraw = 1;

					if (sdlev.active.gain == 0)
						dodraw = 0;
				break;

				case SDL_VIDEORESIZE:
					mysurface = SDL_SetVideoMode
					(
						sdlev.resize.w,
						sdlev.resize.h,
						16, sdlvflags
					);
					updatewindow (sdlev.resize.w, sdlev.resize.h);
				break;

				case SDL_KEYDOWN:
					keyhandler (&sdlev.key.keysym);
				break;

				case SDL_QUIT:
					cleanup(0);
				break;

				case SDLK_f:
					SDL_WM_ToggleFullScreen (mysurface);
				break;

				default:
				break;
			}
		}

		if (dodraw)
		{
			sceneptr();
		}
	}
	cleanup (0);
}
