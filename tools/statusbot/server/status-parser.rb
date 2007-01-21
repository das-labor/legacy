#!/usr/bin/ruby
require 'rubygems'
require 'gruff'

@ppath = '/home/das-labor.org/www.das-labor.org/'	# where to write the graphics (trailing slash!)

std = Array.new		# status data of max. one day
stw = Array.new		# status data of max. one week

# push log content into array
sdfile = File.new( "status_daily.log", "r" );
sdfile.each_line { |line| 
	std.push(line.to_i)
}
sdfile.close

# reset weekly log file if it contains more than 7 dates
if File.exists?("status_weekly.log") then
	swfcheck = File.new( "status_weekly.log", "r" );
	swfcheck.each_line { |line|
	        stw.push(line.to_i)
	}
	swfcheck.close
	if stw.length >= 7 then
		File.delete("status_weekly.log")
	end
end

# with the client submitting status data every 60s,
# we get 1440 entries per day. When reaching this count,
# reset the daily log, count the opening minutes, divide by 60
# and write this value to the weekly logfile.

if std.length >= 1440 then
	File.delete("status_daily.log")
	swfile  = File.new( "status_weekly.log", "a" );
	# here we count 
	sum = 0
	std.each{ |e|
		if e == 1
			sum += e
		end
	}
	day = sum.to_f / 60
	puts(sum)
	puts(day)
	swfile.puts(day)
	swfile.close
end

# Generate a bar diagram from the daily activity data
d = Gruff::Bar.new('400x200')
d.title = "Today's Nerd Activity" 
d.data("Status: 0 = closed, 1 = open, -1 = missing data", std)
d.labels = {0 => '0h', 180 => '3h', 360 => '6h', 540 => '9h', 720 => '12h', 900 => '15h', 1080 => '18h', 1260 => '21h', 1440 => '24h' }
d.write(@ppath+'activity_daily.png')

# Generate a bar diagram from weekly activity data (if exists)
if stw.length != 0 then
	w = Gruff::Bar.new('400x200')
	w.title = "This Week's Nerd Activity"
	w.data("Opening Hours", stw)
	w.labels = {0 => 'Mon', 1 => 'Tue', 2 => 'Wed', 3 => 'Thu', 4 => 'Fri', 5 => 'Sat', 6 => 'Sun' }
	w.write(@ppath+'activity_weekly.png')
end

