##########################################################################
## Terminbot
##
## Author: til ät das-labor döt org
##
## WORKS: 
## * subscripe unsubscribe users
## * respond to certain messages
## * reminde subscribers of events (reminder.rb)
## * keep-alive by sending version-requests to the server
##
## TODO:
## * some proper keep-alive routine
###########################################################################




#!/usr/bin/env ruby

require 'xmpp4r'
require 'xmpp4r/roster/helper/roster'
require 'xmpp4r/version/iq/version'
require 'rexml/document'
require 'date'
require 'config.rb'

XMLSOURCE=ARGV[0]+"/termine.xml";

# xml parsen
class Termin
  attr_reader :day, :wday, :month, :year, :text, :link, :date, :datestr, :hour, :min;

  def initialize(xml)
    @day   = xml.elements["day"].text.to_i;
    @wday  = xml.elements["wday"].text;
    @month = xml.elements["month"].text.to_i;
    @year  = xml.elements["year"].text.to_i;
    @text  = xml.elements["text"].text;
    @link  = xml.elements["link"].text;
    @hour  = xml.elements["hour"].text;
    @min   = xml.elements["min"].text;

    @year += 2000 if @year < 2000;

    @date  = Date.new( @year, @month, @day );
    @month = @date.strftime("%m")
    @day = @date.strftime("%d")
    @datestr = "#{@day}.#{@month}.#{@year} (#{@wday})"
  end

  def to_s
    return @datestr + " - " + @text;
  end
end

# doc bauen, relevante Elemente in Array schubs, Array sortieren
doc = REXML::Document.new( File.new( XMLSOURCE ));
@at = Array.new

# XML-Elemente ins Array
doc.elements.each("/termine/termin") { |tx|
   	@at.push(Termin.new(tx));
}

# Array nach Datum sortieren
@at.sort!{ |a,b| a.date <=> b.date }

## Dieser Teil ist nicht schön, tut aber folgendes:
# Gucken, ob morgen was ist, falls ja: zurückgeben; falls nein: Motivationstext
def tomorrow(at)
	morgen = String.new("\n")
	at.each { |term|
    if term.date == Date.today+1 then
		morgen << @introtomorrow
        morgen << " \n#{term.hour}:#{term.min} #{term.text}\n#{term.link}";
    end
    }
    if morgen == "\n"
        morgen << @notomorrow
    end
	return morgen
end
# Gucken, ob heute was ist, falls ja: zurückgeben
def today(at)
	heute = String.new("\n")
    at.each { |term|
    if term.date == Date.today then
		heute << @introtoday
        heute << " \n#{term.hour}:#{term.min} #{term.text}\n#{term.link}";
    end
    }
	if heute == "\n"
		heute << @notoday
	end
    return heute
end
# Alle zukünftigen Termine zurückgeben
def listall(at)
	alles = String.new("\n")
	at.each { |term|
	if term.date > Date.today-1 then
		alles << "#{term.day}.#{term.month}.#{term.year} (#{term.wday}) #{term.hour}:#{term.min} #{term.text}\n#{term.link} \n\n"
	end
	}
	return alles
end

if @debug == 1
	puts
	puts("listall:")
	puts(listall(@at))
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
			# Dem User erklären, warum er den Bot authorisieren muss und was der Bot kann
			instruction = Jabber::Message.new(item.jid)
			instruction.type = :chat
			instruction.set_body(@instructiontext)
			@client.send(instruction)
            if @debug == 1
                puts("Sent instruction text to user  #{presence.from}.")
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
	@client.send(Jabber::Presence.new.set_show(:chat).set_status(@status))
	#@client.send(Jabber::Presence.new(:chat, 'LABOR-Termine'))
end

def setup_messagehandler
@client.add_message_callback { |m|
#	puts(m.body())
#	puts Date.today
	if m.body == "tomorrow" then
		answer = Jabber::Message.new(m.from)
		answer.type = :chat
		answer.set_body(tomorrow(@at))
		@client.send(answer)
        if @debug == 1
	        puts("Sent content \'tomorrow\' to #{m.from}.")
        end
    elsif m.body == "today" then
        answer = Jabber::Message.new(m.from)
   	    answer.type = :chat
       	answer.set_body(today(@at))
        @client.send(answer)
        if @debug == 1
            puts("Sent content \'today\' to #{m.from}.")
        end
#    elsif m.body == "next7" then
#		answer = Jabber::Message.new(m.from)
#        answer.type = :chat
#        answer.set_body(next7(@at))
#        @client.send(answer)
    elsif m.body == "all" then
        if(listall(@at))
            answer = Jabber::Message.new(m.from)
            answer.type = :chat
            answer.set_body(listall(@at))
            @client.send(answer)
		    if @debug == 1
    	        puts("Sent content \'all\' to #{m.from}.")
	        end
        end
# as tim requested an annoyance feature for the bot, there it is
	elsif m.body == "annoytim" then
		answer = Jabber::Message.new(m.from)
        answer.type = :chat
        answer.set_body("You just annoyed Tim")
		annoy = Jabber::Message.new(@timsjid)
		annoy.type = :chat
		annoy.set_body(@annoytext + "\n This message has been requested by #{m.from} for reasons of annoyment. Do not blame me!")
		@client.send(annoy)
        @client.send(answer)
        if @debug == 1
            puts("User #{m.from} annoyed Tim.")
        end
	elsif
		answer = Jabber::Message.new(m.from)
		answer.type = :chat
        answer.set_body(@helpmessage)
        @client.send(answer)
        if @debug == 1
            puts("Sent help message to #{m.from}.")
        end
	end
}
end

# Die Verbindung bricht ständig weg.
# Wir brauchen etwas, das wenig, aber regelmäßig Traffic macht
def keepalive
	loop {
		iq = Jabber::Version::IqQueryVersion.new()
		@client.send(iq)
		if @debug == 1
			puts("Keep-alive request sent:")
			puts("#{iq}")
		end
		sleep(@interval)
	}
end

#def reminder_daily(rtime)
#	curtime = "#{DateTime.now.hour}#{DateTime.now.min}"
#	if curtime == rtime
#		# call the reminder
#		# reconnect bot
#	end
#end

# debug routinen (allgemein)
if @debug == 1
	puts("Checking variables and content functions")
    puts("today:")
	puts(today(@at))
	puts("tomorrow:")
	puts(tomorrow(@at))
    puts("listall:")
    puts(listall(@at))
	puts("\n")
	puts("notoday: " + @notoday)
	puts("notomorrow: " +  @notomorrow)
	puts("introtoday: " + @introtoday)
	puts("introtomorrow: " + @introtomorrow)
	puts("instructiontext: " + @instructiontext)
	puts("helpmessage: " + @helpmessage)
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
		puts("Setting up keepalive with interval #{@interval}")
	end
	keepalive()
end

startup()
Thread.stop

@client.close
