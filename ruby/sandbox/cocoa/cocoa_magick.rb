# CocoaMagick.rb
#   Tim Burks / RubyCocoa Resources http://www.rubycocoa.com
#
# This is a quick-and-dirty replacement for RMagic written specifically 
# for Geoffrey Grosenbach's "gruff" graphing package.
# It uses RubyCocoa and the Cocoa API to draw the elements of gruff graphs.
# It is FAR from comprehensive, but works for nearly all of the test cases 
# distributed with gruff. Tested with gruff 0.1.2.  
#
# To use, change "require 'RMagick'"to "require 'CocoaMagick'" in gruff/base.rb.
# You also must have RubyCocoa installed, preferably the most recent version.
# This means you must also be running on a Mac, preferably with OS X 10.4 or later. 
# Due to a Cocoa limitation, this script must either be run by the user currently 
# logged into the console or as root.
#
# For help with RubyCocoa, visit "RubyCocoa Resources" at www.rubycocoa.com.
# For more on gruff, see http://nubyonrails.com/pages/gruff
# 
require 'osx/cocoa'

def log(str)
  puts str if false
end

class OSX::NSImage
  def writePNG(filename)
    bits = OSX::NSBitmapImageRep.alloc.initWithData(self.TIFFRepresentation)
    data = bits.representationUsingType_properties(OSX::NSPNGFileType, nil)
    data.writeToFile_atomically(filename, false)
  end
end

class OSX::NSColor
  def self.colorWithName(name) 
    log "colorWithName #{name}"
    if name[0..0] == "#"
      r = eval("0x"+name[1..2]) / 256.0
      g = eval("0x"+name[3..4]) / 256.0
      b = eval("0x"+name[5..6]) / 256.0
      colorWithDeviceRed_green_blue_alpha(r,g,b,1)
    elsif name == "transparent"
      OSX::NSColor.blackColor.colorWithAlphaComponent(0.0)
    elsif name == "grey"
      OSX::NSColor.colorWithDeviceRed_green_blue_alpha(0.5,0.5,0.5,1)
    else
      eval("OSX::NSColor.#{name}Color")
    end
  end
end

 module Magick
  class Draw
    attr_accessor :pointsize, :fill, :stroke, :font_weight, :gravity, :font, 
      :stroke_color, :fill_color, :stroke_width, :dasharray
    def stroke_and_fill(path)
      @stroke_color.colorWithAlphaComponent(@stroke_opacity).set
      path.stroke
      @fill_color.colorWithAlphaComponent(@fill_opacity).set
      path.fill
    end 
    def initialize
      @fill = "white"
      @pointsize = 18
      @stroke_opacity = 1
      @fill_opacity = 1
      @stroke_width = 1
      @stroke_color = OSX::NSColor.colorWithName("grey")
      @fill_color = OSX::NSColor.colorWithName("grey")
      @stack = []
      OSX::NSBezierPath.setDefaultLineJoinStyle(OSX::NSRoundLineJoinStyle)
      OSX::NSBezierPath.setDefaultLineWidth(1)
    end
    def push
      log "Draw.push"
      @stack.push({ # add more if needed
        :fill_color => @fill_color, 
        :stroke_color => @stroke_color, 
        :dasharray => @dasharray, 
        :dashcount => @dashcount,
        :stroke_width => @stroke_width
      })
      self
    end
    def pop
      log "Draw.pop"  
      hash = @stack.pop
      hash.keys.each {|key| self.instance_variable_set("@"+key.to_s, hash[key])}
      self
    end
    def scale(x,y)
      log "scale #{x} #{y}"
      @scalex = x
      @scaley = y
      self
    end
    def string_attributes
      attributes = OSX::NSMutableDictionary.alloc.initWithCapacity_(10)
      attributes.setObject_forKey(OSX::NSColor.colorWithName(@fill), 
        OSX.NSForegroundColorAttributeName)
      if @font_weight == BoldWeight
        attributes.setObject_forKey(OSX::NSFont.boldSystemFontOfSize(@pointsize), 
          OSX.NSFontAttributeName) 
      else
        attributes.setObject_forKey(OSX::NSFont.systemFontOfSize(@pointsize), 
          OSX.NSFontAttributeName) 
      end     
      attributes
    end  
    def get_type_metrics(a,text)
      log "get_type_metrics #{a} #{text.inspect}"
      attributes = string_attributes()
      metrics = TypeMetric.new
      size = OSX::NSString.stringWithString(text).sizeWithAttributes(attributes)
      metrics.width = size.width
      metrics
    end
    def annotate(image,width,height,x,y,text)
      log "annotate #{image} #{width} #{height} #{x} #{y} #{text}"
      attributes = string_attributes()      
      string = OSX::NSString.stringWithString(text)
      size = string.sizeWithAttributes(attributes) 
      if @gravity == EastGravity or @gravity == SouthEastGravity or @gravity == NorthEastGravity
        x = x + width - size.width/2
      elsif @gravity == CenterGravity or @gravity == NorthGravity or @gravity == SouthGravity
        x = x + width/2 - size.width/2 
      end
      if @gravity == NorthGravity or @gravity == NorthEastGravity or @gravity == NorthWestGravity
        y = y + height - size.height
      elsif @gravity == CenterGravity or @gravity == EastGravity or @gravity == WestGravity
        y = y + height/2 - size.height/2
      else
        y = y - size.height/2
      end
      string.drawAtPoint_withAttributes([x,image.height-y-size.height], attributes)
      self
    end
    def stroke(color)
      log "stroke #{color}"
      @stroke_color = OSX::NSColor.colorWithName(color)
      self
    end
    def fill(color)
      log "fill #{color}"
      @fill_color = OSX::NSColor.colorWithName(color)
      self
    end
    def rectangle(a,b,c,d)
      log "rectangle #{a} #{b} #{c} #{d}"
      @fill_color.set
      OSX::NSRectFill([a*@scalex,$height-d*@scalex,(c-a)*@scalex,(d-b)*@scaley])
      self
    end
    def stroke_width(a)
      log "stroke_width #{a}"
      @stroke_width = a
      OSX::NSBezierPath.setDefaultLineWidth(a*@scalex)
      self
    end
    def line(a,b,c,d)
      log "line #{a} #{b} #{c} #{d}"
      @stroke_color.set
      path = OSX::NSBezierPath.bezierPath
      path.setLineDash_count_phase(@dasharray, @dashcount, 0) if @dasharray
      path.moveToPoint([a*@scalex,$height-b*@scaley])
      path.lineToPoint([c*@scalex,$height-d*@scaley])
      path.stroke
      self
    end
    def stroke_opacity(opacity)
      log "stroke_opacity #{opacity}"
      @stroke_opacity = opacity
      self
    end
    def stroke_color(color)
      stroke(color)
      self
    end
    def fill_opacity(opacity)
      @fill_opacity = opacity
      self
    end
    def stroke_dasharray(a, b)
      log "stroke_dasharray #{a} #{b}"
      @dashcount = 2
      @dasharray = [a,b].pack('f2')
      self
    end
    def circle(cx,cy,px,py)
      log "circle #{cx} #{cy} #{px} #{py}"
      r = Math::sqrt((cx-px)*(cx-px) + (cy-py)*(cy-py))
      path = OSX::NSBezierPath.bezierPathWithOvalInRect(
        [(cx-r)*@scalex,$height-(cy+r)*@scaley,2*r*@scalex,2*r*@scaley])
      stroke_and_fill(path)
      self
    end
    def ellipse(x, y, w, h, as, ae)
      log "ellipse #{x} #{y} #{w} #{h} #{as} #{ae}"
      push
      stroke_width(1)
      @fill_color = @stroke_color   
      path = OSX::NSBezierPath.bezierPath
      center = [x*@scalex,$height-y*@scaley]
      path.moveToPoint(center)
      path.appendBezierPathWithArcWithCenter_radius_startAngle_endAngle(
        center,2*w*@scalex,-ae,-as)
      path.closePath
      stroke_and_fill(path)
      pop
      self
    end
    def polyline(*args)
      path = OSX::NSBezierPath.bezierPath
      path.moveToPoint([args[0]*@scalex,$height-args[1]*@scaley])
      i = 2
      while(i < args.length) 
        path.lineToPoint([args[i]*@scalex,$height-args[i+1]*@scaley])
        i = i + 2
      end
      stroke_and_fill(path)
      self
    end
    def polygon(*args)
      path = OSX::NSBezierPath.bezierPath
      path.moveToPoint([args[0]*@scalex,$height-args[1]*@scaley])
      i = 2
      while(i < args.length) 
        path.lineToPoint([args[i]*@scalex,$height-args[i+1]*@scaley])
        i = i + 2
      end
      path.closePath
      stroke_and_fill(path)
      self
    end
    def draw(a)
      log "draw #{a}"
      self
    end
  end
  class Image
    attr_accessor :image, :width, :height   
    def initialize(width, height, fill=nil)
      log "Image.initialize #{width} #{height} #{fill}"
      @width = width
      @height = height
      @image = OSX::NSImage.alloc.initWithSize([width,height])
      @image.lockFocus
      if fill == nil 
        OSX::NSColor.blackColor.set
        OSX::NSRectFill([0,0,width,height])
      elsif fill.class == Magick::GradientFill
        gradient = fill.image
        gradient.drawInRect_fromRect_operation_fraction([0,0,width,height], 
          [0,0,gradient.size.width,gradient.size.height], OSX::NSCompositeCopy, 1.0)
      else
        fill.drawInRect_fromRect_operation_fraction([0,0,width,height], 
          [0,0,fill.size.width,fill.size.height], OSX::NSCompositeCopy, 1.0)
      end
      $width = width
      $height = height
      self
    end
    def write(filename)
      log "Image.write #{filename}"
      @image.unlockFocus
      @image.writePNG(filename)
      self
    end
    def self.read(filename)
      log "Image.read #{filename}"
      image = OSX::NSImage.alloc.initWithContentsOfFile(filename)
      [Image.new(800,600,image)] 
    end
  end
  class GradientFill
    attr_accessor :image
    def initialize(a,b,c,d,e,f)
      log "GradientFill.initialize #{a} #{b} #{c} #{d} #{e} #{f}"     
      # There's no direct gradient fill support in Cocoa, so fake it
      resolution = 100  # increase this to reduce striping and increase run time...
      @image = OSX::NSImage.alloc.initWithSize([1,resolution])
      @image.lockFocus
      bottomColor = OSX::NSColor.colorWithName(f)
      topColor = OSX::NSColor.colorWithName(e)
      resolution.times {|i|
        bottomColor.blendedColorWithFraction_ofColor(i/(resolution - 1.0), topColor).set
        OSX::NSRectFill([0,i,1,i+1])
      }
      self
    end
  end
  class TypeMetric
    attr_accessor :width
    def initialize
      @width = 50 
    end
  end
  NormalWeight = 400
  BoldWeight = 700
  WestGravity = :west
  EastGravity = :east
  CenterGravity = :center
  SouthGravity = :south
  NorthGravity = :north
  SouthWestGravity = :southwest
  NorthWestGravity = :northwest
  NorthEastGravity = :northeast
  SouthEastGravity = :southeast
end

# this prevents a warning from AppKit
app = OSX::NSApplication.sharedApplication