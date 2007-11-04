#!/usr/bin/env ruby

#######################################################################################
#                                                                                     #
# PODCAST HARVESTER                                                                   #
#                                                                                     #
# Author: til ät das-labor döt org                                                    #
# published under GNU GPL version 2 as found under                                    #
# http://www.gnu.org/licenses/gpl-2.0.txt on 02.11.2007                               #
# This software is provided as-is. The author is by no means responsible for          #
# any damage that may or may not occure by using this software to the extend          #
# permitted by German law.                                                            #
#                                                                                     #
# The podcast harvester takes a list of podcast feeds and automaticly                 #
# downloads the latest media file per feed, if the file is not already present.       #
# You can make it re-fetch all the latest files (existing or not) via the -r switch.  #
# You can also let the harvester automaticly sync the prevailing media files          #
# to another directory. In my case, this directory is on my portable mp3 player.      #
# Use -h oder --help to display any available options                                 #
#                                                                                     #
# Needed libraries:                                                                   #
# - from ruby standard lib:                                                           #
#   - optparse                                                                        #
#   - ftools                                                                          #
# - REXML                                                                             #
# - gems:                                                                             #
#   - simplehttp                                                                      #
#                                                                                     #
# #####################################################################################

## CONFIG: edit the following 3 setting according to your needs

# full path to the file containing the urls of your subscriptions
@SUBFILE = "/home/user/podcast-harvester/subscriptions"

# full path to the directory where the downloaded media files go to. Mind the trailing slash!
@MDIR	= "/home/user/podcast-harvester/media/"

# full path the directory we sync to. A directory on your portable mp3-player for example. Trailing slash!
@SYNCDIR = "/path/to/your/portable/mediaplayers/podcast/dir/"



###################################################
# There should be no need to edit below this line # 
###################################################


require "optparse"
require "ftools"
require "rubygems"
require "simple_http"
require "rexml/document"
include REXML

@options = {}

@downloads = Array.new

def get_latest_file_url(xmlfeed)	# returns the first occurance of the enclosure tag, takes raw xml as argument
	u = Array.new					# (enclosure is where the url of the media file goes to)
	doc = Document.new xmlfeed
	doc.elements.each("*/channel/item/enclosure") { |element| u.push(element.attributes["url"]) }
#	puts u[0]
	return u[0].to_s
end

def get_podcast_feed(feedurl)		# takes the url to the xml podcast feed as argument, returns its content
	begin 
		req = SimpleHttp.new feedurl
		req.set_proxy ENV['HTTP_PROXY']
		txt = req.get
	rescue Timeout::Error => e
		puts e.message
		puts "Timeout, skipping file"
	end
	return txt
end

def get_latest_file(url)
	name = url.sub(/http\:.*.\//, "")
	dest = "#{@MDIR}#{name}"
#	puts dest
	@downloads.push(name)
	puts "Downloading #{url}, please wait ..."
	if !(FileTest.exists?(dest)) ||  @options[:refresh]==1 then
		begin
			req	= SimpleHttp.new url
			req.follow_num_redirects = 10
			req.set_proxy ENV['HTTP_PROXY']
			podcast = req.get
			f = File.new(dest,  "w+")
			f << podcast
			f.close
			puts "Finished downloading #{name}"
		rescue Timeout::Error => e
			puts e.message
			puts "Timeout, skipping file"
		end
	else
		puts "Nothing new here, keeping the current file."
		puts ""
	end
end

def read_subscriptions(sfile)				# read the subscriptions file and push the individual lines in an array
	urls = Array.new
	if FileTest.exist?(sfile)
		f = File.new(sfile)
		f.readlines.each{|l| urls.push(l) }
		f.close
	end
	return urls
end

def delete_old()
	Dir.chdir(@MDIR)
	current = Dir["*"]
	dfiles = current - @downloads
	if !dfiles.empty? then
		dfiles.each{|f|
			print "deleting #{f}\n"
			File.delete(f)
		}
	end
end

def sync_to_dir()	# sync the @MDIR to any other dir, e.g. a folder on a portable mp3-player
	Dir.chdir(@MDIR)
	ofiles = Dir["*"]
	if FileTest.exists?(@SYNCDIR)
		Dir.chdir(@SYNCDIR)
		target = Dir["*"]
		puts "Starting synchronisation ..."
		puts "Deleting old files in synchronisation target directory..."
		target.each{ |f|
			File.delete(f)
		}
		if !(ofiles.empty?)
			puts "Copying files ..."
			ofiles.each{ |f|
				File.copy("#{@MDIR}#{f}","#{@SYNCDIR}#{f}")
			}
		else
			puts "No files to copy."
		end
		puts "Synchronisation finished."
	else
		puts "Synchronisation failed! Target directory does not exist."
	end
end

def setup_dirs()		# check if the configured directories exist, else create them
	if !(FileTest.exists?(@MDIR))then
		Dir.mkdir(@MDIR)
	end
	if (@options[:sync]==1) & !(FileTest.exists?(@SYNCDIR)) then
		Dir.mkdir(@SYNCDIR)
	end
end

def parse_args()		# shiny new command line switches parser
	OptionParser.new do |opts|
		opts.banner = "Usage: #{$0} [options]"
		opts.on("-s", "--sync",	"Sync to folder (e.g. portable mp3 player)") do |s|
			@options[:sync] = 1
		end
		opts.on("-r", "--refresh",	"Refresh all podcasts, you subscribed to, independent of existing files") do |s|
			@options[:refresh] = 1
		end
		opts.on("-k", "--keep",	"Do not delete outdated podcasts or any other file in your podcast directory") do |s|
			@options[:keep] = 1
		end
		opts.on("-h", "--help", "This help" ) do
		    puts opts
		    exit
		end
	end.parse!
end

def main
	parse_args()
	setup_dirs()
	subs = read_subscriptions(@SUBFILE)
	if !(subs.empty?)
		subs.each{ |s| 
#			puts s
			feed = get_podcast_feed(s)
#			puts feed
			file_url = get_latest_file_url(feed)
#			puts file_url
			get_latest_file(file_url)
		}
	end

	if !(@options[:keep]==1)
		delete_old()
	else
		printf "\n\nClean-Up disabled, keeping even all the outdated podcasts.\n"
	end

	if @options[:sync]==1 then
		sync_to_dir()
	end
end

main()
