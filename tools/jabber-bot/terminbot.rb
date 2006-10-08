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

# config options. adjust to your needs
######################################

@interval=30	#keep-alive interval
@account = 'terminbot@das-labor.org/LABOR-Termine'	# the bots account / resource
@password = 'foobar23'	# password (ah, no! just kidding! password must have a different meaning)
@status = 'waiting for your requests'	# the bots status message

# The answer, if there is no event today
@notoday = "Heute ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
# The answer, if there is no event tomorrow
@notomorrow = "Morgen ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
# The leading text for today's events
@introtoday = "Heute im LABOR:"
@introtomorrow = "Morgen im LABOR:"

# the text a user gets on subscription
@instructiontext = "Welcome to the event reminder service!\n
For the bot to work properly (including being able to unsubscribe from the bot!), you have to grant your authorization.
The bot will remind you one day, and again 4h before any event. Furthermore it knows the following commands: today, tomorrow, all"

# help message
@helpmessage = "
Command not known!
I know the following commands:
all         Shows all events
today       Shows todays events
tomorrow    shows tomorrows events

To unsubscribe from my service, just revoke my authorization or delete me from your roster."

# Tim requested an annoyance feature
@timsjid = "opti ät amessage dot de" # for the bot to work correctly, enter Tim's correct jid here
@annoytext = "hoho!"

######################################

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
        morgen << " \n#{term}  #{term.link}";
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
        heute << " \n#{term}  #{term.link}";
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
		alles << "#{term} \n#{term.link} \n\n"
	end
	}
	return alles
end


#puts(listall(at))

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
			# puts("subscribed")
			item.subscribe()
			# Dem User erklären, warum er den Bot authorisieren muss und was der Bot kann
			instruction = Jabber::Message.new(item.jid)
			instruction.type = :chat
			instruction.set_body(@instructiontext)
			@client.send(instruction)
		end
	}

	# Wenn ein User unsubscribet, also die Authorisation zurückzieht,
	# entferne diesen User aus dem Roster des Bots
	@roster.add_subscription_callback { |item, presence|
	    if presence.type == :unsubscribed
			# puts("unsubscribed")
	        item.remove()
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
    elsif m.body == "today" then
        answer = Jabber::Message.new(m.from)
   	    answer.type = :chat
       	answer.set_body(today(@at))
        @client.send(answer)
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
        end
# as tim requested an annoyance feature for the bot, there it is
	elsif m.body == "annoytim" then
		answer = Jabber::Message.new(m.from)
        answer.type = :chat
        answer.set_body("You just annoyed Tim")
		annoy = Jabber::Message.new(@timsjid)
		annoy.type = :chat
		annoy.set_body(@annoytext)
		@client.send(annoy)
        @client.send(answer)
	elsif
		answer = Jabber::Message.new(m.from)
		answer.type = :chat
        answer.set_body(@helpmessage)
        @client.send(answer)
	end
}
end

# Die Verbindung bricht ständig weg.
# Wir brauchen etwas, das wenig, aber regelmäßig Traffic macht
def keepalive
	loop {
		iq = Jabber::Version::IqQueryVersion.new()
#		puts("#{iq}")
		@client.send(iq)
#		puts("sent")
		sleep(@interval)
	}
end

# und alles ausführen (in der richtigen Reihenfolge)
setup_connection()
setup_roster()
connect()
setup_messagehandler()
keepalive()
Thread.stop

@client.close
