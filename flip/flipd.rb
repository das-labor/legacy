#!/usr/bin/env ruby

#
# Basic deamon for collecting important information from the known accesspoints
#

require 'ap.rb'

#
# get all new accesspoints 
#

def check_new_aps( apList )
		apList.gather_new( "/tftpboot", "config/templates"  )
end

###############################################################################
# poll the status from the access points and return them periodicly
def poll_statistics( apList )
	apList.apHash.each_value { |ap|
		stat = ap.collect_statistics;
#		ap.statistics = stat;
#		puts stat["wi0Clients"];
#		puts stat["ath0Clients"];
#	 	puts stat["ath1Clients"];

#		puts stat["pf"]["PASS: IN ALL on sis0"]["bytes"];
	}
end


###############################################################################
# (de-)serialize statistic into file
def load_statistics( apList ) 
	apList.apHash.each_value { |ap|
		ap.load_statistics;
	}
end

def save_statistics( apList ) 
	apList.apHash.each_value { |ap|
		ap.save_statistics;
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
	check_new_aps(apList);

	puts "Polling Statistics"
	poll_statistics(apList);

	puts "Saving Statistics"
	save_statistics(apList);

	sleep 1
end

