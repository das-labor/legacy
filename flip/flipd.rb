#!/usr/bin/env ruby

#
# Basic deamon for collecting important information from the known accesspoints
#

require 'ap.rb'

#
# get all new accesspoints 
#

def checkForNewAps( apList )
	usedIps = Array.new;

	apList.each_value { |ap|
		usedIps += ap.ip;
	}

end

#
# poll the status from the access points and return them periodicly
#

def pollStatus( apList )
	apList.apHash.each_value { |ap|
		stat = ap.collect_statistics;
		puts stat["wi0Clients"];
		puts stat["ath0Clients"];
	 	puts stat["ath1Clients"];

		puts stat["pf"]["PASS: IN ALL on sis0"]["bytes"];
	}
end

##############################################################################
# Main()

apList = APList.new( "tmp/aps" )


while true do
	apList.refresh;

#	checkForNewAps(apList);
	pollStatus(apList);
end

