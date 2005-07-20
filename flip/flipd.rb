#!/usr/bin/env ruby

#
# Basic deamon for collecting important information from the known accesspoints
#

require 'ap.rb'

#
# get all new accesspoints 
#

def check_aps( apList )
	apList.gather_new( "/tftpboot", "config/templates"  )

	ips = Array.new;
	apList.apHash.each_value { |ap|
		ips.push(ap.ip);
	}
	
	str = ips.join(" ");
	lines = `fping -a -A #{str}`
	
	lines.each { |line|
		line.chomp!
		puts "#{line} is alive"
		
		apList.apHash.each_value { |ap|
			ap.up(true) if ap.ip == line;
		}
	}
end

###############################################################################
# poll the status from the access points and return them periodicly
def poll_statistics( apList )
	apList.apHash.each_value { |ap|
		next if !ap.up? || !ap.enabled?

		puts "Polling #{ap.ip}"
		stat = ap.collect_statistics;
		ap.save_statistics;
	}
end


###############################################################################
# (de-)serialize statistic into file
def load_statistics( apList ) 
	apList.apHash.each_value { |ap|
		ap.load_statistics;
	}
end

##############################################################################
# Main()

apList = APList.new( "tmp/aps" )

# load_statistics(apList);

while true do
	puts "Refreshing AP list..."
	apList.refresh;

	puts "Cheking for APs ..."
	check_aps(apList);

	puts "Polling Statistics"
	poll_statistics(apList);

	sleep 1
end

