require 'opengl'
require 'glut'
require 'mathn'

include Math

#texture = range(3)       # storage for 3 textures;
light = 0           # lighting on/off
blend = 0        # blending on/off
@xrot = 0            # x rotation
@yrot = 0            # y rotation
@xspeed = 0          # x rotation speed
@yspeed = 0          # y rotation speed
@walkbias = 0
@walkbiasangle = 0
@lookupdown = 0.0
$piover180 = 0.0174532925
@heading,@xpos,@zpos = 0.0, 0.0, 0.0
camx,camy,camz = 0, 0, 0 # camera location.
therotate = 0
@z=0.0                        # depth into the screen.
LightAmbient  = [0.5, 0.5, 0.5, 1.0]
LightDiffuse  = [1.0, 1.0, 1.0, 1.0]
LightPosition = [0.0, 0.0, 2.0, 1.0]
filter = 0       # texture filtering method to use (nearest, linear, linear + mipmaps)

class Vertex
  attr_accessor :x,:y,:z,:u,:v
  def initialize
    @x,@y,@z,@u,@v = 0,0,0,0,0
  end
end
class Triangle
  attr_accessor :vertex
  def initialize
    @vertex,@vertex[0],@vertex[1],@vertex[2] = [],Vertex.new,Vertex.new,Vertex.new
  end
end
class Sector
  attr_accessor :numtriangles
  def initialize
    @numtriangles = 0
    triangle = []
  end
end
class Imaged
  attr_accessor :sizeX,:sizeY,:data
  def initialize
    @sizeX = 0
    @sizeY = 0
    @data = []
  end
end


def rad(angle)
  return angle * $piover180
end


$sector1 = Sector.new

# A general OpenGL initialization function.  Sets all of the initial parameters.
def initGL(width, height) 	# We call this right after our OpenGL window is created.
    #LoadGLTextures()                           # load the textures.
    GL.Enable(GL::TEXTURE_2D)                    # Enable texture mapping.

    GL.BlendFunc(GL::SRC_ALPHA, GL::ONE)          # Set the blending function for translucency (note off at init time)
    GL.ClearColor(0.0, 0.0, 0.0, 0.0)	# This Will Clear The Background Color To Black
    GL.ClearDepth(1.0)				# Enables Clearing Of The Depth Buffer
    GL.DepthFunc(GL::LESS)                       # type of depth test to do.
    GL.Enable(GL::DEPTH_TEST)                    # enables depth testing.
    GL.ShadeModel(GL::SMOOTH)			# Enables Smooth Color Shading

    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity()				# Reset The Projection Matrix

    GLU.Perspective(45.0,width/height,0.1,100.0)	# Calculate The Aspect Ratio Of The Window

    GL.MatrixMode(GL::MODELVIEW)

    # set up lights.
    GL.Lightfv(GL::LIGHT1, GL::AMBIENT, LightAmbient)
    GL.Lightfv(GL::LIGHT1, GL::DIFFUSE, LightDiffuse)
    GL.Lightfv(GL::LIGHT1, GL::POSITION, LightPosition)
    GL.Enable(GL::LIGHT1)
end

def reSizeGLScene(width, height)
    if height == 0				# Prevent A Divide By Zero If The Window Is Too Small
	    height = 1
	  end

    GL.Viewport(0, 0, width, height)		# Reset The Current Viewport And Perspective Transformation
    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity()
    GLU.Perspective(45.0,width/height,0.1,100.0)
    GL.MatrixMode(GL::MODELVIEW)
end

# The main drawing function.
def drawGLScene
  # calculate translations and rotations.
  @xtrans = -@xpos
  @ztrans = -@zpos
  @ytrans = -@walkbias-0.25
  @sceneroty = 360.0 - @yrot

  GL.Clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT)		# Clear The Screen And The Depth Buffer
  GL.LoadIdentity()

  GL.Rotate(@lookupdown, 3.0, 0, 0)
  GL.Rotate(@sceneroty, 0, 1.0, 0)

  GL.Translate(@xtrans,@ytrans,@ztrans)

  #GL.BindTexture(GL_TEXTURE_2D, texture[filter])    # pick the texture.

  numtriangles = $sector1.numtriangles

  ## loop through all the triangles
  ##numtriangles.each { |i| # loop through all the triangles
  #numtriangles.times { |i| # loop through all the triangles
  #    GL.Begin(GL::TRIANGLES)
  #    GL.Normal3f(0.0, 0.0, 1.0)
  #
  #  	x_m = $sector1.triangle[i].vertex[0].x
  #  	y_m = $sector1.triangle[i].vertex[0].y
  #  	z_m = $sector1.triangle[i].vertex[0].z
  #  	u_m = $sector1.triangle[i].vertex[0].u
  #  	v_m = $sector1.triangle[i].vertex[0].v
  #  	#GL.TexCoord2f(u_m,v_m)
  #  	GL.Vertex3f(x_m,y_m,z_m)
  #
  #  	x_m = $sector1.triangle[i].vertex[1].x
  #  	y_m = $sector1.triangle[i].vertex[1].y
  #  	z_m = $sector1.triangle[i].vertex[1].z
  #  	u_m = $sector1.triangle[i].vertex[1].u
  #  	v_m = $sector1.triangle[i].vertex[1].v
  #    #GL.TexCoord2f(u_m,v_m)
  #  	GL.Vertex3f(x_m,y_m,z_m)
  #
  #  	x_m = $sector1.triangle[i].vertex[2].x
  #  	y_m = $sector1.triangle[i].vertex[2].y
  #  	z_m = $sector1.triangle[i].vertex[2].z
  #  	u_m = $sector1.triangle[i].vertex[2].u
  #  	v_m = $sector1.triangle[i].vertex[2].v
  #  	#GL.TexCoord2f(u_m,v_m)
  #  	GL.Vertex3f(x_m,y_m,z_m)
  #	GL.End()
  #}
  
#  GL.Color3ub(255, 0, 0);
      GL.Begin(GL::LINE_STRIP)
      GL.Vertex3f(0.0, 0.0, 0.0)
      GL.Vertex3f(1.0, 0.0, 0.0)
      GL.Vertex3f(0.75, 0.25, 0.0)
      GL.Vertex3f(0.75, -0.25, 0.0)
      GL.Vertex3f(1.0, 0.0, 0.0)
      GL.Vertex3f(0.75, 0.0, 0.25)
      GL.Vertex3f(0.75, 0.0, -0.25)
      GL.Vertex3f(1.0, 0.0, 0.0)
      GL.End
      GL.Begin(GL::LINE_STRIP)
      GL.Vertex3f(0.0, 0.0, 0.0)
      GL.Vertex3f(0.0, 1.0, 0.0)
      GL.Vertex3f(0.0, 0.75, 0.25)
      GL.Vertex3f(0.0, 0.75, -0.25)
      GL.Vertex3f(0.0, 1.0, 0.0)
      GL.Vertex3f(0.25, 0.75, 0.0)
      GL.Vertex3f(-0.25, 0.75, 0.0)
      GL.Vertex3f(0.0, 1.0, 0.0)
      GL.End
      GL.Begin(GL::LINE_STRIP)
      GL.Vertex3f(0.0, 0.0, 0.0)
      GL.Vertex3f(0.0, 0.0, 1.0)
      GL.Vertex3f(0.25, 0.0, 0.75)
      GL.Vertex3f(-0.25, 0.0, 0.75)
      GL.Vertex3f(0.0, 0.0, 1.0)
      GL.Vertex3f(0.0, 0.25, 0.75)
      GL.Vertex3f(0.0, -0.25, 0.75)
      GL.Vertex3f(0.0, 0.0, 1.0)
      GL.End
      
    GL.Translate(-3,0,-3)  
  #GL.PushMatrix()
    GLUT.WireSphere(1.0, 12, 12);   # draw sun */
    GL.Translate(1,0,-3)
    GLUT.WireSphere(1.0, 12, 12);   # draw sun */
  #GL.PopMatrix()
  # since this is double buffered, swap the buffers to display what just got drawn.
  GLUT.SwapBuffers()
end

# Change view angle
def specialKeyPressed(k, x, y)
  case k
    when GLUT::KEY_PAGE_UP
      @z -= 0.2
      @lookupdown -= 1.0
    when GLUT::KEY_PAGE_DOWN
      @z += 0.2
      @lookupdown += 1.0
    when GLUT::KEY_UP
      @xpos -= sin(@yrot*$piover180) * 0.20
    	@zpos -= cos(@yrot*$piover180) * 0.20
    	if @walkbiasangle >= 359.0
    	  @walkbiasangle = 0.0
    	else
    	  @walkbiasangle+= 10
  	  end
	  	@walkbias = sin(@walkbiasangle * $piover180)/20.0
    when GLUT::KEY_DOWN
    	@xpos += sin(@yrot*$piover180) * 0.20
    	@zpos += cos(@yrot*$piover180) * 0.20
    	if @walkbiasangle <= 1.0
    	  @walkbiasangle = 359.0
    	else
    	  @walkbiasangle-= 10
  	  end
  	  @walkbias = sin(@walkbiasangle * $piover180)/20.0
    when GLUT::KEY_LEFT
      @yrot += 3.0
    when GLUT::KEY_RIGHT
    	@yrot -= 3.0
  end
  #GLUT.PostRedisplay()
end

def main
  #setupWorld()
  
  #   Initialize GLUT state - glut will take any command line arguments that pertain to it or
  #   X Windows - look at its documentation at http:reality.sgi.com/mjk/spec3/spec3.html 
  GLUT.Init()
  # Select type of Display mode: # Double buffer # RGBA color # Depth buffer # Alpha blending 
  GLUT.InitDisplayMode(GLUT::RGB | GLUT::DEPTH | GLUT::DOUBLE)
  # get a 800 x 600 window 
  GLUT.InitWindowSize(600, 600)
  # the window starts at the upper left corner of the screen 
  GLUT.InitWindowPosition(0,0)
  # Open a window 
  window = GLUT.CreateWindow('mil.3d')
  # Register the function to do all our OpenGL drawing. 
  GLUT.DisplayFunc(method(:drawGLScene).to_proc)
  # Go fullscreen.  This is as soon as possible. 
  #GLUT.FullScreen()
  # Even if there are no events, redraw our gl scene
  GLUT.IdleFunc(method(:drawGLScene).to_proc)
  # Register the function called when our window is resized. 
  GLUT.ReshapeFunc(method(:reSizeGLScene).to_proc)
  # Register the function called when the keyboard is pressed. 
  #GLUT.KeyboardFunc(method(:keyPressed).to_proc)
  # Register the function called when special keys (arrows, page down, etc) are pressed.
  GLUT.SpecialFunc(method(:specialKeyPressed).to_proc)
  # Initialize our window.
  initGL(600,600)
  # Start Event Processing Engine 
  GLUT.MainLoop()
end

main()