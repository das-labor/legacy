require 'rubygems'
require 'RMagick'
include Magick

def backandforth(degree)
  polarity = rand(2) * -1
  return rand(degree) * polarity if polarity < 0
  return rand(degree)
end

def create_slide(image)
  slide = Image.read('slide.png').first
  slide_background = Image.new(slide.columns, slide.rows) { self.background_color = 'transparent' }
  photo = Image.read(image).first
  
  # create a grey scale gradient fill for our mask
  mask_fill = GradientFill.new(0, 0, 0, 88, '#FFFFFF', '#F0F0F0')
  mask = Image.new(photo.columns, photo.rows, mask_fill)
  # create thumbnail sized square image of photo
  photo.crop_resized!(88,88)

  # apply alpha mask to slide
  photo.matte = true
  mask.matte = false
  photo.composite!(mask, 0, 0, CopyOpacityCompositeOp)
  
  # composite photo and slide on transparent background
  slide_background.composite!(photo, 16, 16, OverCompositeOp)
  slide_background.composite!(slide, 0, 0, OverCompositeOp)
  
  # rotate slide +/- 45 degrees
  rotation = backandforth(45)
  slide_background.rotate!(rotation)
  
  # create workspace to apply shadow
  workspace = Image.new(slide_background.columns+5, slide_background.rows+5) { self.background_color = 'transparent' }
  shadow = slide_background.shadow(0, 0, 0.0, '20%')
  workspace.composite!(shadow, 3, 3, OverCompositeOp)
  workspace.composite!(slide_background, NorthWestGravity, OverCompositeOp)
  
  return workspace
end

# In this example I have 7 images labled 1.png..7.png 
# The following loop picks 4 random images from the 7
# 1 for the main image and 3 for the slides
images = Array.new
(1..4).each do 
  image_number = rand(7) + 1
  redo if images.include? image_number # make sure the picked images are unique before adding to images array
  images << image_number
end

template = Image.read('template.png').first
photo = Image.read("#{images.shift}.png").first # shift the 1st image off the array (array now contains 3 images for slides)
photo.crop_resized!(710,200)

template.composite!(photo, 10, 10, OverCompositeOp)

(images.size-1).downto(0) do |i|
  slide = create_slide("#{images[i]}.png")
  template.composite!(slide, i * 100 + rand(15), 150 + rand(15), OverCompositeOp)
end

# save finished collage
template.write('collage.jpg')
