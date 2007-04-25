# 3-D gear wheels. This program is in the public domain.
#
# Command line options:
#    -info      print GL implementation information
#    -exit      automatically exit after 30 seconds
#
# 2005-05-01 Ruby version by Arto Bendiken based on gears.c rev 1.8.
# 2005-01-09 Original C version (gears.c) by Brian Paul et al.
# http://cvs.freedesktop.org/mesa/Mesa/progs/demos/gears.c?rev=1.8

require 'opengl'
require 'glut'

class Gears

  POS = [5.0, 5.0, 10.0, 0.0]
  RED = [0.8, 0.1, 0.0, 1.0]
  GREEN = [0.0, 0.8, 0.2, 1.0]
  BLUE = [0.3, 0.2, 1.0, 1.0]

  include Math

  # Draw a gear wheel. You'll probably want to call this function when
  # building a display list since we do a lot of trig here.
  #
  # Input:  inner_radius - radius of hole at center
  #         outer_radius - radius at center of teeth
  #         width - width of gear
  #         teeth - number of teeth
  #         tooth_depth - depth of tooth
  def gear(inner_radius, outer_radius, width, teeth, tooth_depth)
    r0 = inner_radius
    r1 = outer_radius - tooth_depth / 2.0
    r2 = outer_radius + tooth_depth / 2.0

    da = 2.0 * PI / teeth / 4.0

    GL.ShadeModel(GL::FLAT)

    GL.Normal(0.0, 0.0, 1.0)

    # Draw front face
    GL.Begin(GL::QUAD_STRIP)
    for i in 0..teeth
      angle = i * 2.0 * PI / teeth
      GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5)
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5)
      if i < teeth
        GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5)
        GL.Vertex3f(r1 * cos(angle + 3 * da),
          r1 * sin(angle + 3 * da), width * 0.5)
      end
    end
    GL.End()
  
    # Draw front sides of teeth
    GL.Begin(GL::QUADS)
    for i in 0...teeth
      angle = i * 2.0 * PI / teeth
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5)
      GL.Vertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5)
      GL.Vertex3f(r2 * cos(angle + 2 * da),
        r2 * sin(angle + 2 * da), width * 0.5)
      GL.Vertex3f(r1 * cos(angle + 3 * da),
        r1 * sin(angle + 3 * da), width * 0.5)
    end
    GL.End()
    
    GL.Normal(0.0, 0.0, -1.0)
    
    # Draw back face
    GL.Begin(GL::QUAD_STRIP)
    for i in 0..teeth
      angle = i * 2.0 * PI / teeth
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5)
      GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5)
      if i < teeth
        GL.Vertex3f(r1 * cos(angle + 3 * da),
          r1 * sin(angle + 3 * da), -width * 0.5)
        GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5)
      end
    end
    GL.End()
    
    # Draw back sides of teeth
    GL.Begin(GL::QUADS)
    for i in 0...teeth
      angle = i * 2.0 * PI / teeth
      GL.Vertex3f(r1 * cos(angle + 3 * da),
        r1 * sin(angle + 3 * da), -width * 0.5)
      GL.Vertex3f(r2 * cos(angle + 2 * da),
        r2 * sin(angle + 2 * da), -width * 0.5)
      GL.Vertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5)
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5)
    end
    GL.End()
    
    # Draw outward faces of teeth
    GL.Begin(GL::QUAD_STRIP)
    for i in 0...teeth
      angle = i * 2.0 * PI / teeth
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5)
      GL.Vertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5)
      u = r2 * cos(angle + da) - r1 * cos(angle)
      v = r2 * sin(angle + da) - r1 * sin(angle)
      len = sqrt(u * u + v * v)
      u /= len
      v /= len
      GL.Normal(v, -u, 0.0)
      GL.Vertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5)
      GL.Vertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5)
      GL.Normal(cos(angle), sin(angle), 0.0)
      GL.Vertex3f(r2 * cos(angle + 2 * da),
        r2 * sin(angle + 2 * da), width * 0.5)
      GL.Vertex3f(r2 * cos(angle + 2 * da),
        r2 * sin(angle + 2 * da), -width * 0.5)
      u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da)
      v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da)
      GL.Normal(v, -u, 0.0)
      GL.Vertex3f(r1 * cos(angle + 3 * da),
        r1 * sin(angle + 3 * da), width * 0.5)
      GL.Vertex3f(r1 * cos(angle + 3 * da),
        r1 * sin(angle + 3 * da), -width * 0.5)
      GL.Normal(cos(angle), sin(angle), 0.0)
    end
    GL.Vertex3f(r1 * cos(0), r1 * sin(0), width * 0.5)
    GL.Vertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5)
    GL.End()
  
    GL.ShadeModel(GL::SMOOTH)
  
    # Draw inside radius cylinder
    GL.Begin(GL::QUAD_STRIP)
    for i in 0..teeth
      angle = i * 2.0 * PI / teeth
      GL.Normal(-cos(angle), -sin(angle), 0.0)
      GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5)
      GL.Vertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5)
    end
    GL.End()
  end

  def draw
    GL.Clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT);

    GL.PushMatrix()
    GL.Rotate(@view_rotx, 1.0, 0.0, 0.0)
    GL.Rotate(@view_roty, 0.0, 1.0, 0.0)
    GL.Rotate(@view_rotz, 0.0, 0.0, 1.0)

    GL.PushMatrix()
    GL.Translate(-3.0, -2.0, 0.0)
    GL.Rotate(@angle, 0.0, 0.0, 1.0)
    GL.CallList(@gear1)
    GL.PopMatrix()
    
    GL.PushMatrix()
    GL.Translate(3.1, -2.0, 0.0)
    GL.Rotate(-2.0 * @angle - 9.0, 0.0, 0.0, 1.0)
    GL.CallList(@gear2)
    GL.PopMatrix()
    
    GL.PushMatrix()
    GL.Translate(-3.1, 4.2, 0.0)
    GL.Rotate(-2.0 * @angle - 25.0, 0.0, 0.0, 1.0)
    GL.CallList(@gear3)
    GL.PopMatrix()
    
    GL.PopMatrix()
    
    GLUT.SwapBuffers()

    @frames = 0 if not defined? @frames
    @t0 = 0 if not defined? @t0

    @frames += 1
    t = GLUT.Get(GLUT::ELAPSED_TIME)
    if t - @t0 >= 5000
      seconds = (t - @t0) / 1000.0
      fps = @frames / seconds
      printf("%d frames in %6.3f seconds = %6.3f FPS\n",
        @frames, seconds, fps)
      @t0, @frames = t, 0
      exit if defined? @autoexit and t >= 999.0 * @autoexit
    end
  end

  def idle
    t = GLUT.Get(GLUT::ELAPSED_TIME) / 1000.0
    @t0_idle = t if !defined? @t0_idle
    # 90 degrees per second
    @angle += 70.0 * (t - @t0_idle)
    @t0_idle = t
    GLUT.PostRedisplay()
  end

  # Change view angle, exit upon ESC
  def key(k, x, y)
    case k
      when ?z
        @view_rotz += 5.0
      when ?Z
        @view_rotz -= 5.0
      when ?i
        printf("GL_RENDERER   = %s\n", GL.GetString(GL::RENDERER))
        printf("GL_VERSION    = %s\n", GL.GetString(GL::VERSION))
        printf("GL_VENDOR     = %s\n", GL.GetString(GL::VENDOR))
        printf("GL_EXTENSIONS = %s\n", GL.GetString(GL::EXTENSIONS))
      when 27 # Escape
        exit
    end
    GLUT.PostRedisplay()
  end

  # Change view angle
  def special(k, x, y)
    case k
      when GLUT::KEY_UP
        @view_rotx += 5.0
      when GLUT::KEY_DOWN
        @view_rotx -= 5.0
      when GLUT::KEY_LEFT
        @view_roty += 5.0
      when GLUT::KEY_RIGHT
        @view_roty -= 5.0
    end
    GLUT.PostRedisplay()
  end

  # New window size or exposure
  def reshape(width, height)
    h = height.to_f / width.to_f
    GL.Viewport(0, 0, width, height)
    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity()
    GL.Frustum(-1.0, 1.0, -h, h, 5.0, 60.0)
    GL.MatrixMode(GL::MODELVIEW)
    GL.LoadIdentity()
    GL.Translate(0.0, 0.0, -40.0)
  end

  def init
    @angle = 0.0
    @view_rotx, @view_roty, @view_rotz = 20.0, 30.0, 0.0

    GL.Lightfv(GL::LIGHT0, GL::POSITION, POS)
    GL.Enable(GL::CULL_FACE)
    GL.Enable(GL::LIGHTING)
    GL.Enable(GL::LIGHT0)
    GL.Enable(GL::DEPTH_TEST)

    # Make the gears
    @gear1 = GL.GenLists(1)
    GL.NewList(@gear1, GL::COMPILE)
    GL.Material(GL::FRONT, GL::AMBIENT_AND_DIFFUSE, RED)
    gear(1.0, 4.0, 1.0, 20, 0.7)
    GL.EndList()

    @gear2 = GL.GenLists(1)
    GL.NewList(@gear2, GL::COMPILE)
    GL.Material(GL::FRONT, GL::AMBIENT_AND_DIFFUSE, GREEN)
    gear(0.5, 2.0, 2.0, 10, 0.7)
    GL.EndList()

    @gear3 = GL.GenLists(1)
    GL.NewList(@gear3, GL::COMPILE)
    GL.Material(GL::FRONT, GL::AMBIENT_AND_DIFFUSE, BLUE)
    gear(1.3, 2.0, 0.5, 10, 0.7)
    GL.EndList()

    GL.Enable(GL::NORMALIZE)

    ARGV.each do |arg|
      case arg
        when '-info'
          printf("GL_RENDERER   = %s\n", GL.GetString(GL::RENDERER))
          printf("GL_VERSION    = %s\n", GL.GetString(GL::VERSION))
          printf("GL_VENDOR     = %s\n", GL.GetString(GL::VENDOR))
          printf("GL_EXTENSIONS = %s\n", GL.GetString(GL::EXTENSIONS))
        when '-exit'
          @autoexit = 30
          printf("Auto Exit after %i seconds.\n", @autoexit);
      end
    end
  end

  def visible(vis)
    GLUT.IdleFunc((vis == GLUT::VISIBLE ? method(:idle).to_proc : nil))
  end

  def mouse(button, state, x, y)
    @mouse = state
    @x0, @y0 = x, y
  end
  
  def motion(x, y)
    if @mouse == GLUT::DOWN then
      @view_roty += @x0 - x
      @view_rotx += @y0 - y
    end
    @x0, @y0 = x, y
  end

  def initialize
    GLUT.Init()
    GLUT.InitDisplayMode(GLUT::RGB | GLUT::DEPTH | GLUT::DOUBLE)

    GLUT.InitWindowPosition(0, 0)
    GLUT.InitWindowSize(300, 300)
    GLUT.CreateWindow('Gears')
    init()

    GLUT.DisplayFunc(method(:draw).to_proc)
    GLUT.ReshapeFunc(method(:reshape).to_proc)
    GLUT.KeyboardFunc(method(:key).to_proc)
    GLUT.SpecialFunc(method(:special).to_proc)
    GLUT.VisibilityFunc(method(:visible).to_proc)
    GLUT.MouseFunc(method(:mouse).to_proc)
    GLUT.MotionFunc(method(:motion).to_proc)
  end

  def start
    GLUT.MainLoop()
  end

end

Gears.new.start