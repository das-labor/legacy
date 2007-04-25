require 'rubygems'
require "RMagick"

if !ARGV[0]
    puts "Usage: polaroid.rb path-to-image"
    exit
end

image = Magick::Image.read(ARGV[0]).first

#image.border!(18, 18, "#f0f0ff")
image.border!(12, 12, "#f0f0ff")

# Bend the image
image.background_color = "none"

amplitude = image.columns * 0.01 # vary according to taste
wavelength = image.rows  * 2

image.rotate!(90)
#image = image.wave(amplitude, wavelength)
image.rotate!(-90)

# Make the shadow
shadow = image.flop
shadow = shadow.colorize(1, 1, 1, "gray75")     # shadow color can vary to taste
shadow.background_color = "white"       # was "none"
shadow.border!(10, 10, "white")
shadow = shadow.blur_image(0, 3)        # shadow blurriness can vary according to taste

# Composite image over shadow. The y-axis adjustment can vary according to taste.
image = shadow.composite(image, -amplitude/2, 5, Magick::OverCompositeOp)

image.rotate!(5)                       # vary according to taste
image.trim!

# Add -print to image basename, write to file.
out = ARGV[0].sub(/\./, "-print.")
puts "Writing #{out}"
image.write(out)