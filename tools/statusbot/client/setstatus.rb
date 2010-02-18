#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
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
require '/usr/local/status-client/config.rb'

def setup_connection
        # Mit Jabber-Server verbinden
        @client = Jabber::Client.new(Jabber::JID.new(@account))
        @client.connect
        @client.auth(@password)
end

def connect
        @client.send(Jabber::Presence.new.set_show(:dnd).set_status(@status))
end

def pingtest
	if @debug == 1
	        puts("ping -c 2 "+@checkhost+" > /dev/null")
	end
        system("ping -c 2 "+@checkhost+" > /dev/null")
        if $? == 0 then
                return true
        else
                return false
        end
end

def checkandsend
        loop {
                status = false
                status = pingtest()
		if @debug == 1 then
	                puts("pingausgabe:"+ @status)
		end
                if status then
                        @m = Jabber::Message.new(@statusdaemon, 'status=on').set_type(:chat).set_id('1') 
                else
                        @m = Jabber::Message.new(@statusdaemon, 'status=off').set_type(:chat).set_id('1')
                end
                @client.send(@m)
                if @debug == 1
                        puts @m.to_s
                end
#                GC.start()
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
Thread.stop
@client.close
