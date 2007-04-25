require 'rubygems'
require 'RMagick'

maxwidth = 128
maxheight = 128
aspectratio = maxwidth.to_f / maxheight.to_f
imgfile = 'beached_limo-754970'

pic = Magick::Image.read(imgfile + '.jpg').first
imgwidth = pic.columns
imgheight = pic.rows
imgratio = imgwidth.to_f / imgheight.to_f
imgratio > aspectratio ? scaleratio = maxwidth.to_f / imgwidth : scaleratio = maxheight.to_f / imgheight
thumb = pic.resize(scaleratio)

white_bg = Magick::Image.new(maxwidth, thumb.rows)
pic = white_bg.composite(thumb, Magick::CenterGravity, Magick::OverCompositeOp)
pic.write(imgfile + '.thumb.jpg')

#def make_tex(file)
#  clown = Magick::ImageList.new(file)
#  clown.flip!
#  clown.resize!(128,128)
#  p   clown.format
#  clown.write("#{file}_tmp")
#  `convert #{file}_tmp #{file}.rgb`
#end
#
#make_tex "apple.tablet.jpg"

#clown.change_geometry!('64x64') { |cols, rows, img| img.resize!(cols, rows) }