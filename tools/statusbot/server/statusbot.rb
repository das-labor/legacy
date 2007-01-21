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
require 'config.rb'

@timeout=1


def setlabstatus(status)
	if status == "on" then
		File.copy(@iconon, @staticon)
		@client.send(Jabber::Presence.new.set_show(:chat).set_status('Offen!'))
	elsif status == "off" then
		File.copy(@iconoff, @staticon)
		 @client.send(Jabber::Presence.new.set_show(:xa).set_status('Geschlossen'))
	elsif status == "none" then
		File.copy(@iconnone, @staticon)
		 @client.send(Jabber::Presence.new.set_show(:dnd).set_status(@status))
	end
end

def writelog(status)
	log = File.open( "status_daily.log", "a" );
        if status == "on" then
		log.puts("1")
        elsif status == "off" then
		log.puts("0")
        elsif status == "none" then
		log.puts("-1")
        end
	log.close
end

def offset()
	if !File.exists?("status.lock") then
        	lc = File.open( "status.lock", "w" );
	        lc.puts(Time.now.strftime("%H"))
	        lc.close
	end
end

def setup_connection
	# Mit Jabber-Server verbinden
	@client = Jabber::Client.new(Jabber::JID.new(@account))
	@client.connect
	@client.auth(@password)
end

def setup_roster
	# Roster bauen
	@roster = Jabber::Roster::Helper.new(@client)


	# Alle subscription requests akzeptieren und ebenfalls subscription requesten
	@roster.add_subscription_request_callback { |item, presence|
		if presence.type == :subscribe
	    	@roster.accept_subscription(presence.from)
			item.subscribe()
			if @debug == 1
				puts("Accepted subscription from jid #{presence.from}. Sent subscribe to this user.")
			end
		end
	}

	# Wenn ein User unsubscribet, also die Authorisation zurückzieht,
	# entferne diesen User aus dem Roster des Bots
	@roster.add_subscription_callback { |item, presence|
	    if presence.type == :unsubscribed
			# puts("unsubscribed")
	        item.remove()
            if @debug == 1
                puts("Received unsubscribe request from #{presence.from}. Removed user from roster.")
            end
	    end
	}
end

def connect
	@client.send(Jabber::Presence.new.set_show(:dnd).set_status(@status))
end

def setup_messagehandler
@client.add_message_callback { |m|
        # Set the LABOR's status on the website
        if m.body == "status=on" then
		if @debug == 1
			puts("#{m.from}")
		end
                if m.from == @setaccount
                        if @debug == 1
                                puts("Set status ON")
                        end
                        setlabstatus("on")
			@timeout = @statustimeout
			offset()
			writelog("on")
                end   

        elsif m.body == "status=off" then
		if @debug == 1
			puts("#{m.from}")
		end
		if m.from == @setaccount
			if @debug == 1
		                puts("Set status OFF")
        		end
	        	setlabstatus("off")
			@timeout = @statustimeout
			offset()
			writelog("off")
        	end   
	end     

	
}
end

# Die Verbindung bricht ständig weg.
# Wir brauchen etwas, das wenig, aber regelmäßig Traffic macht
# Nebenbei bauen wir einen timeout, der den status auf NONE setzt,
# wenn er länger nichts vom client gehört hat
def keepalive
	loop {
		#iq = Jabber::Version::IqQueryVersion.new()
		@client.send(" \t ")
		if @debug == 1
			puts("Keep-alive request sent")
		end
		if @timeout > 0
			@timeout -= 1
			if @timeout < 1 then
				setlabstatus("none")
				if @debug == 1
					puts("Timeout! Setting Status: NONE")
				end
			end
		end
                if @timeout < 1 then
			offset()
	                writelog("none")
        	        if @debug == 1
                 	       puts("Timeout! Logging: NONE")
                        end
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
		puts("Setting up the roster...")
	end
	setup_roster()
	
	if @debug == 1
		puts("Connecting to server...")
	end
	connect()

	if @debug == 1
		puts("Setting up the message handler...")
	end
	setup_messagehandler()
	
        if @debug == 1
                puts("setting status icon to statusNONE")
        end
        setlabstatus("none")

	if @debug == 1
		puts("Setting up keepalive with interval #{@interval}")
	end
	keepalive()
end

startup()
Thread.stop

@client.close
setlabstatus("none")
