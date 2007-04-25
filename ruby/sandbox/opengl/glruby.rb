require "opengl"
require "glut"
require "mathn"
require "rubyruby"

$shownormals = false

$xspin = 0.0
$yspin = 1.0
$zspin = 0.0
$xspinspeed = 0.0
$yspinspeed = 90.0
$zspinspeed = 0.0

$rubyList = 1

# init() initializes material property, light source, lighting model.

def init
  GL.ClearColor(0.0, 0.1, 0.2, 0.0)
  GL.ShadeModel(GL::SMOOTH)

  GL.Enable(GL::LIGHTING)
  GL.Enable(GL::LIGHT0)
  GL.Enable(GL::BLEND)
  GL.Enable(GL::NORMALIZE)

  GL::DepthFunc(GL::LESS);
  GL::ShadeModel(GL::SMOOTH);
  GL::BlendFunc(GL::ONE, GL::ONE)

  lm_ambient = [ 0.2, 0.2, 0.2, 0.0 ];

  GL.Light(GL::LIGHT0, GL::POSITION, [4, 4, 4])
  GL.Light(GL::LIGHT0, GL::POSITION, [-2, 6, 3])
  #GL::LightModel(GL::LIGHT_MODEL_AMBIENT, lm_ambient);
  GL::LightModel(GL::LIGHT_MODEL_TWO_SIDE, 2);

  $ruby = RubyRuby.new
  GL::NewList($rubyList, GL::COMPILE)
  $ruby.render
  GL::EndList()
end

# rerender() will rerender the ruby. Called when any of the parameters
# change.

def rerender
  GL::NewList($rubyList, GL::COMPILE)
  $ruby.render
  GL::EndList()
  GLUT.PostRedisplay
end

# Here is where the light position is reset after the modeling
# transformation (glRotated) is called.  This places the light at a
# new position in world coordinates.  The cube represents the position
# of the light.

display = Proc.new {
  GL.Clear(GL::COLOR_BUFFER_BIT)
  GL.LoadIdentity()
  GLU.LookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0)

  GL.Rotate($xspin, 1, 0, 0)
  GL.Rotate($yspin, 0, 1, 0)
  GL.Rotate($zspin, 0, 0, 1)

  GL.Translate(0,-1,0)
  GL::CallList($rubyList);
  GL.Flush()
  GLUT.SwapBuffers
}

idle = Proc.new {
  d = sincelast
  $xspin = ($xspin + $xspinspeed * d) % 360.0
  $yspin = ($yspin + $yspinspeed * d) % 360.0
  $zspin = ($zspin + $zspinspeed * d) % 360.0
  GLUT.PostRedisplay
}

reshape = Proc.new {|w, h|
  GL.Viewport(0, 0, w,  h)
  GL.MatrixMode(GL::PROJECTION)
  GL.LoadIdentity()
  GLU.Perspective(45.0, w/h, 1.0, 20.0)
  GL.MatrixMode(GL::MODELVIEW)
}

# ARGSUSED1 */
keyboard = Proc.new {|key, x, y|
  case key
  when ?t			# increase thickness
    $ruby.thickness += 0.01
    rerender
    GLUT.PostRedisplay()
  when ?T			# decrease thickness
    $ruby.thickness -= 0.01
    rerender
    GLUT.PostRedisplay()
  when ?f			# increase facet size
    $ruby.facets += 2
    rerender
  when ?F			# decrease facet size
    $ruby.facets -= 2
    rerender
  when ?r			# start spinning
    GLUT.IdleFunc(idle)
  when ?R			# stop spinning
    GLUT.IdleFunc(nil)
  when ?x
    $xspinspeed += 1.0
  when ?X
    $xspinspeed -= 1.0
  when ?y
    $yspinspeed += 1.0
  when ?Y
    $yspinspeed -= 1.0
  when ?z
    $zspinspeed += 1.0
  when ?Z
    $zspinspeed -= 1.0
  when ?s
    $ruby.shapenum += 1
    rerender
  when ?.
    $xspin = $yspin = $zspin = 0.0
    $xspinspeed = $zspinspeed = 0.0
    $yspinspeed = 90.0
  when 27,?q			# escape
    exit(0)
  end
}

# normal(a,b) calcutes the normal vector to a plane defined by the
# vectors a and b (in a right hand coordinate system). 

def normal(a, b)
  [a[1]*b[2]-a[2]*b[1],
    a[2]*b[0]-a[0]*b[2],
    a[0]*b[1]-a[1]*b[0]]
end

def sincelast
  now = Time.now
  $lasttime ||= now-1
  diff = now - $lasttime
  $lasttime = now
  return diff
end

GLUT.Init
GLUT.InitDisplayMode(GLUT::DOUBLE | GLUT::RGBA)
GLUT.InitWindowSize(300, 300)
GLUT.CreateWindow("RubyRuby")
init()
GLUT.DisplayFunc(display) 
GLUT.ReshapeFunc(reshape)
GLUT.IdleFunc(idle)
GLUT.KeyboardFunc(keyboard)
GLUT.MainLoop()