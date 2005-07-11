#!/usr/bin/env ruby

require 'ap.rb'


def checkForNewAps( apList )
	usedIps = Array.new;

	apList.each_value { |ap|
		usedIps += ap.ip;
	}

	//
end

def pollStatus( apList )
	apList.each_value { |ap|
		ap.execute_remote
	}
	
end

##############################################################################
# Main()

apList = APList.new( "tmp/aps" )

while true do
	apList.refresh;
	
	checkForNewAps(apList);
	pollStatus(apList);
end
