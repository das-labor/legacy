#!/usr/bin/env ruby

# ABOUT
# Log your gps position to a webserver (e.g. with web_trakkcor/record_GET.php 
# as server-side element). Uses gpsd to retrieve gps data
#
# USAGE
# Adjust the settings below:
# INTERVAL: how often do we post our position? (in seconds)
# DEVICEID: give a name to identify where the data you are sending comes from
# SITE: base URL to your website
# PATH: path to the server-side code that will receive your position data

# MIT LICENSE
# 
# Copyright (c) 2009 Tilman Frosch (frosch.org.uk)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


require 'net/http'
require 'rubygems'
require 'gps'

#### settings

INTERVAL=60
DEVICEID="foo"
SITE="http://your_server/"
PATH="web_trakkcor/record_GET.php"

#### /settings. There should be no need to edit the lines below.


gps = Gps::Receiver.create("gpsd")
gps.start

while true
	gps.on_position_change(0.00003) do
		ts=gps.timestamp
		lat=gps.latitude
		lon=gps.longitude
		alt=gps.altitude
		course=gps.course
		spd=gps.speed
		puts "TS: #{Time.at(ts).strftime("%H%M%S%Z")}, lat: #{lat}, lon: #{lon}, alt: #{alt}, course: #{course}, speed: #{spd}"
		query = "id=#{DEVICEID}&ts=#{Time.at(ts).strftime("%H%M%S%Z")}&lat=#{lat}&lon=#{lon}&alt=#{alt}&hea=#{course}&spd=#{spd}"
#		puts query
		result = Net::HTTP.get(URI.parse("#{SITE}#{PATH}?#{query}"))
		puts result
	end

	sleep INTERVAL

end

