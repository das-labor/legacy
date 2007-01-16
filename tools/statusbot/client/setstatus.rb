#!/usr/bin/env ruby
##########################################################################
## Statusbot
##
## Author: til ät das-labor döt org
##
## WORKS: 
## * subscripe unsubscribe users
## * set the correct status, when queried by the correct user 
## * keep-alive by sending \t to the server
##
## TODO:
## * some proper keep-alive routine
###########################################################################




require 'xmpp4r'
require 'xmpp4r/roster/helper/roster'
require 'xmpp4r/version/iq/version'
require 'date'
require 'ftools'
require 'ping'
require 'config.rb'

status = ARGV[0];

def setup_connection
	# Mit Jabber-Server verbinden
	@client = Jabber::Client.new(Jabber::JID.new(@account))
	@client.connect
	@client.auth(@password)
end

def connect
	@client.send(Jabber::Presence.new.set_show(:dnd).set_status(@status))
end

def checkandsend
        loop {
		status = Ping.pingecho(@checkhost)
		if status then
        		@m = Jabber::Message.new(@statusdaemon, 'status=on').set_type(:chat).set_id('1') 
		else
		        @m = Jabber::Message.new(@statusdaemon, 'status=off').set_type(:chat).set_id('1')
		end
		@client.send(@m)
		if @debug == 1
		        puts @m.to_s
		end
		
		sleep(@interval)
	}
end



# und alles ausführen (in der richtigen Reihenfolge)
def startup

	if @debug == 1
		puts("Setting up the connection with the following parameters: ")
		puts("account: " + @account)
		puts("password: " + @password)
		puts("statusmessage: " + @status)
	end
	setup_connection()

	
	if @debug == 1
		puts("Connecting to server...")
	end
	connect()

end

startup()
checkandsend()
@client.close
