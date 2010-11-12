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
require './config.rb'

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
        system(@checkerscript+" > /dev/null")
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

# So far we're been developing code in Pleasantville, a wonderful place where 
# nothing ever, ever goes wrong. Every library call succeeds, users never enter 
# incorrect data, and resources are plentiful and cheap. Well, that's about to 
# change. Welcome to the real world!
begin
  # connect
  startup()
  # create initial message
  @m = Jabber::Message.new(@statusdaemon, 'status=off').set_type(:chat).set_id('1') 
  # try to send
  @client.send(@m)
  i=1
  #enter endless loop since connect and send was fine
  while i > 0 
    begin
      checkandsend()
    rescue Exception
      @client.close unless @client.nil? 
    end
    sleep(@interval)
    startup()
  end
  # we failed on senderror or on connect
rescue Exception
  puts "fail"
  @client.close unless @client.nil? 
end
# we should not reach this

