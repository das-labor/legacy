#!/usr/bin/env ruby

require 'rubygems'
require 'htmlentities'
require 'http-access2'
require 'twitter'

# http://tr.im is also a good solution, with well-defined returns (in xml). 
# Downside: must parse xml fist. http://is.gd ist simpler for now
#SERVICEURL = "http://ikr.me/api/url="	# prepends strange whitespaces 
#SERVICEURL = "http://r.im/api/index.cfm?long_url="		# gets you to a meta-page first
SERVICEURL = "http://is.gd/api.php?longurl="


class LaTweet
	def initialize()

	end

	def shorten(url)
		proxy = ENV['HTTP_PROXY']
		client  = HTTPAccess2::Client.new(proxy)
		txt   = client.get("#{SERVICEURL}#{url}").content
		url = HTMLEntities.decode_entities(txt)
                return url if !url.index("Error")
	end

	def post(txt)
		httpauth = Twitter::HTTPAuth.new(TUSERNAME, TPASSWD)
		base = Twitter::Base.new(httpauth)
		base.update("#{txt}")
	end

	def getmsgsto()
		msgs = Array.new
		httpauth = Twitter::HTTPAuth.new(TUSERNAME, TPASSWD)
		base = Twitter::Base.new(httpauth)
		base.replies.each{|r|
			msgs.push(r.text)
		}
		puts msgs[0]
	end

	def tweetwithurl(txt,url)
		surl = shorten(url)
	    tweet = "#{txt} #{surl}"
		if tweet.length > 140 then	# is our tweet longer then 140 chars?
			mlength = 140 - (surl.length+4)	# +4 as we need a whitespace between text 
											# and URL and we cut our text and replace 
											# the last three chars by "..."
			stxt = txt[0...mlength]
			tweet = stxt+"... "+surl
		end
#		puts tweet
#		puts tweet.length
#		puts "Posting ..."
		post(tweet)
#		puts "Loose tweets sink fleets!"
	end
end
