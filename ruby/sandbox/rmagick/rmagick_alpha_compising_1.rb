require 'rubygems'
require 'rmagick'
include Magick

module Magick
    class Draw
    def star(sides, inner_radius, outer_radius)
    theta = 0.0
    incr = Math::PI * 2.0 / sides
    half_incr = incr / 2.0
    points = []
    sides.times do
        points << outer_radius * Math::cos(theta)
        points << outer_radius * Math::sin(theta)
        points << inner_radius * Math::cos(theta + half_incr)
        points << inner_radius * Math::sin(theta + half_incr)
        theta += incr
        end
        polygon *points
        end
    end

    class Image
        def star(sides, inner_radius, outer_radius, stroke, fill)
            gc = Draw.new
            gc.translate columns/2.0, rows/2.0
            gc.fill fill
            gc.stroke stroke
            gc.star sides, inner_radius, outer_radius
            gc.draw self
        end
    end
end



WIDTH = 650
HEIGHT = 40

# Create gradient background

stripes = ImageList.new

top_grad = GradientFill.new(0, 0, WIDTH, 0, "#dddddd", "#888888")
stripes << Image.new(WIDTH, HEIGHT, top_grad)

bottom_grad = GradientFill.new(0, 0, WIDTH, 0, "#757575", "#555555")
stripes << Image.new(WIDTH, HEIGHT, bottom_grad)

combined_grad = stripes.append(true)

color = Image.new(combined_grad.columns, combined_grad.rows) {
    self.background_color = "#87a5ff"
    }
background = combined_grad.composite(color, CenterGravity, ColorizeCompositeOp)


# Draw "RMAGICK" with its reflection
gc = Draw.new
gc.fill = 'white'
gc.stroke = 'none'
gc.pointsize = 42

gc.annotate(background, 0, 0, 70, HEIGHT, "RMAGICK")

reflection = stripes[1].flip
reflection.composite!(color, CenterGravity, ColorizeCompositeOp)
gc.annotate(reflection, 0, 0, 70, HEIGHT, "RMAGICK")

grad = GradientFill.new(0, 0, WIDTH, 0, "black", "gray35")
opacity_mask = Image.new(WIDTH, HEIGHT, grad)

reflection.matte = true
opacity_mask.matte = false
reflection.composite!(opacity_mask, CenterGravity, CopyOpacityCompositeOp)

reflection.flip!
background.composite!(reflection, SouthWestGravity, OverCompositeOp)


# Add soft shadow and put composite on a larger canvas

shadow = Image.new(background.columns+10, background.rows+10)

gc = Draw.new
gc.fill "gray30"
gc.rectangle 5, 5, background.columns+5, background.rows+5
gc.draw(shadow)
shadow = shadow.blur_image(0, 2)

shadowed_background = shadow.composite(background, CenterGravity, OverCompositeOp)

big_canvas = Image.new(shadowed_background.columns, HEIGHT*5)
big_background = big_canvas.composite(shadowed_background, NorthGravity, OverCompositeOp)


# Add green starburst with shadow

black_star = Image.new(170,170)
black_star.star(25, 60, 80, 'none', 'black')
black_star.rotate!(-20)

star_shadow = black_star.copy.blur_image(3, 2)

shadow_mask = star_shadow.negate

shadow_mask.matte = false
star_shadow.matte = true
star_shadow.composite!(shadow_mask, CenterGravity, CopyOpacityCompositeOp)

grad = GradientFill.new(0, 0, black_star.columns, 0, "#99eb24", "#3c7f05}")
green_grad = Image.new(black_star.columns, black_star.rows, grad)

gc = Draw.new
gc.annotate(green_grad, 0, 0, 0, 0, "Ruby\n+\nMagick!") do
gc.gravity = CenterGravity
gc.stroke = 'none'
gc.fill = 'yellow'
gc.pointsize = 24
gc.font_weight = BoldWeight
end
green_grad.rotate!(-20)

inverse_black_star = black_star.negate
inverse_black_star.matte = false
green_grad.matte = true
green_star = green_grad.composite(inverse_black_star, CenterGravity, CopyOpacityCompositeOp)

# Uncomment this line if you're using RMagick < 1.10.1
#green_star.crop!(CenterGravity, star_shadow.columns, star_shadow.rows)

shadowed_green_star = star_shadow.composite(green_star, CenterGravity, OverCompositeOp)

result = big_background.composite(shadowed_green_star, 400, 0, OverCompositeOp)
result.write('result.jpg')