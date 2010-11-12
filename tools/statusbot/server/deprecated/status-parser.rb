#!/usr/bin/ruby
require 'rubygems'
#require 'gruff'
require '/usr/lib/ruby/gems/1.8/gems/gruff-0.2.8/lib/gruff.rb'

@picdir = '/home/das-labor.org/www.das-labor.org/'  # where to write to images (trailing slash!)
std = Array.new		# status data of max. one day
stw = Array.new		# status data of max. one week

# read logging start time
lc = File.new("status.lock" , "r");
logstart = lc.readline
lc.close
#puts logstart
ofs = logstart.to_i

# calculating label offset
h1 = ofs.modulo(24).to_s
h3 = (ofs+3).modulo(24).to_s
h6 = (ofs+6).modulo(24).to_s
h9 = (ofs+9).modulo(24).to_s
h12 = (ofs+12).modulo(24).to_s
h15 = (ofs+15).modulo(24).to_s
h18 = (ofs+18).modulo(24).to_s
h21 = (ofs+21).modulo(24).to_s
h24 = ofs.modulo(24).to_s

def new_offset()
  lc = File.open( "status.lock", "w" );
  lc.puts(Time.now.strftime("%H"))
  lc.close
end
                                    

# push log content into array
sdfile = File.new( "status_daily.log", "r" );
sdfile.each_line { |line| 
	std.push(line.to_i)
}
sdfile.close
#std.each { |ho|
#  puts ho
#}
# reset weekly log file if it contains more than 7 dates
if File.exists?("status_weekly.log") then
	swc = Array.new
	swfcheck = File.new( "status_weekly.log", "r" );
	swfcheck.each_line { |line|
	        swc.push(line.to_i)
	}
	swfcheck.close
	if swc.length >= 7 then
		File.delete("status_weekly.log")
	end
end

# with the client submitting status data every 60s,
# we get 1440 entries per day. When reaching this count,
# reset the daily log, count the opening minutes, divide by 60
# and write this value to the weekly logfile.

if std.length >= 1440 then
	swfile  = File.new( "status_weekly.log", "a" );
	# here we count 
	sum = 0
	today = std.slice!(1,1440)
	nd = File.new("status_daily.log", "w")
	std.each{ |f|
		nd.puts f
	}
	nd.close
  new_offset()
	today.each{ |e|
		if e == 1
			sum += e
		end
	}
	day = sum.to_f / 60
	puts(sum)
	puts(day)
	swfile.puts(day)
	swfile.close
	swwfile = File.new( "status_weekly.log", "r" );
	swwfile.each_line { |line|
                stw.push(line.to_i)
        }

end

# Generate a bar diagram from the daily activity data
d = Gruff::Bar.new('400x200')
d.title = "Today's Nerd Activity" 
d.data("Status: 0 = closed, 1 = open, -1 = missing data", std)
d.labels = {0 => h1, 180 => h3, 360 => h6, 540 => h9, 720 => h12, 900 => h15, 1080 => h18, 1260 => h21, 1440 => h24 }
d.write(@picdir+'activity_daily.png')

# Generate a bar diagram from weekly activity data (if exists)
if stw.length != 0 then
	w = Gruff::Bar.new('400x200')
	w.title = "This Week's Nerd Activity"
	w.data("Opening Hours", stw)
	w.labels = {0 => 'Mon', 1 => 'Tue', 2 => 'Wed', 3 => 'Thu', 4 => 'Fri', 5 => 'Sat', 6 => 'Sun' }
	w.write(@picdir+'activity_weekly.png')
end

