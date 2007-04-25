#Use this software as you please, just please give me credit when doing so.
#Jason Palmer
#http://www.jason-palmer.com

#!/usr/bin/env ruby
require 'rubygems'
require 'rmagick'

#Arguments
src_dir = ARGV[0] unless ARGV[0].nil?
dst_dir = ARGV[1] unless ARGV[1].nil?
maxwidth = ARGV[2].to_f() unless ARGV[2].nil?
maxheight = ARGV[3].to_f() unless ARGV[3].nil?

#Variables
src_dir_pwd = ''

if src_dir.nil? || dst_dir.nil? || maxwidth.nil? || maxheight.nil?
  puts "Usage: img_resize.rb src_dir dst_dir max_width max_height"
  exit(1)
end

if !File.directory?(src_dir) || !File.directory?(dst_dir)
  puts "Source or Destination directories cannot be found"
  puts "Make sure to use absolute directory values"
  exit(1)
end

#Make sure src_dir exists and is a directory
begin
  Dir.chdir(src_dir)
  src_dir_pwd = Dir.getwd
  files = Dir.entries(src_dir_pwd)
rescue Exception=>e
  puts "Failed to list files in #{src_dir}"
  exit(1)
end

files.each do |file|
  begin
    #Make sure the file is an image
    if file =~ /(\.jpg)$|(\.gif)$|(\.jpeg)$/i
      #Do some RMagick

      #Read in the source image
      begin
       puts "RMagick: Examining file => #{file}"
       src = Magick::Image::read(src_dir_pwd + "/" + file).first
      rescue Exception=>e
        puts e.to_s()
        exit(1)
      end

      #Resize source image and pipe returned value into new image object
      begin
        puts "RMagick: Resizing file => #{file}"
        if(src.columns > maxwidth || src.rows > maxheight)
          #Image needs to be resized
          
          if src.columns == src.rows
            #Image is square
            destination = src.thumbnail(maxwidth, maxheight)
          else
            if src.columns > src.rows
              #Image is wider than tall
              ratio = maxwidth / src.columns
              destination = src.thumbnail(ratio)
            else
              #Image is taller than wide
              ratio = maxheight / src.rows
              destination = src.thumbnail(ratio)
            end
          end
        else
          #Leave image as-is
          destination = src
        end
      rescue Exception=>e
        puts e.to_s()
        exit(1)
      end
      
      #Write the new image file
      begin
        puts "RMagick: Writing new file => #{dst_dir + file}"
        if dst_dir =~ /(\/)$/
          destination.write(dst_dir + file)
        else
          destination.write(dst_dir + "/" + file)
        end
      rescue Exception=>e
        puts e.to_s()
        exit(1)
      end
    end
  rescue Exception=>e
    puts e.to_s()
    exit(1)
  end
end

