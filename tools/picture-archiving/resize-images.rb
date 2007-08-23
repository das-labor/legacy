#!/usr/bin/env ruby

require 'RMagick'

WIDTH = 1440
HEIGHT = 1080

WD = File.expand_path(ARGV[0])
@forced = false
@ts = 0

def resize_image(f)
	img = Magick::Image.read(f).first
	img.resize_to_fit!(WIDTH,HEIGHT)
	img.to_blob { self.quality = 85 }
	img.write(f)
end

def is_too_big(f)
	img = Magick::Image.read(f).first
	width = img.columns
	height = img.rows
	if width > WIDTH || height > HEIGHT
		action = true
	else
		action = false
	end
	
	return action
end

def process_images(dir)
    images = Dir["*.{jpg,JPG,jpeg,JPEG,png,PNG,gif,GIF,tiff,tif,TIF,TIFF}"]
    images.each{|img|
		fimg = "#{dir}/#{img}"
		print "Checking image #{fimg} ...\n"
		if changed(fimg)
			if is_too_big(fimg)
				print "Resizing image #{fimg} ...\n\n"
				resize_image(fimg)
			end
		end
	}
end

def changed(file)   # has a file changed? compare mtime to timestamp
    mt = File.mtime(file).strftime("%Y%m%d%H%M%S").to_i
#   puts ts
    if mt > @ts
        modified = true
    elsif @forced
        modified = true
    else
        modified = false
    end

    return modified
end

def set_ts                                  # set timestamp, write it to file 
    tsfile = "#{WD}/timestamp.txt"     # used for checking if files have changed
    ts = Time.now.strftime("%Y%m%d%H%M%S")  # since the last run of the script
    f = File.new(tsfile,  "w+")
    f.print ts
    f.close
end

def get_ts                                  # read timestap from file, 
    tsfile = "#{WD}/timestamp.txt"     # write to @ts
	if File.exist?(tsfile)
	    f = File.new(tsfile)
	    @ts = f.readlines[0].to_i
	    f.close
	else
		@ts = 0
	end
end


def walk_dirs()
	Dir.chdir(WD)
	dcontent = Dir["*"]
    dcontent.each {|sdir|
        if FileTest.directory?(sdir) then
			fd = "#{WD}/#{sdir}"
			print "\nProcessing images in #{fd} \n\n"
			Dir.chdir(fd)
			process_images(fd)
			Dir.chdir(WD)
        end
	}
end

def main()
	get_ts()
	walk_dirs()
	set_ts()
end

main()
