#!/usr/bin/env ruby

require 'xmpp4r'
require 'xmpp4r/roster/helper/roster'
require 'xmpp4r/vcard/helper/vcard'
require 'rexml/document'
require 'date'


XMLSOURCE=ARGV[0]+"/termine.xml";

##xml parsen

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

doc = REXML::Document.new( File.new( XMLSOURCE ));
at = Array.new

# XML-Elemente ins Array
doc.elements.each("/termine/termin") { |tx|
    at.push(Termin.new(tx));
}

# Array nach Datum sortieren
at.sort!{ |a,b| a.date <=> b.date }


## Dieser Teil ist nicht schön, tut aber folgendes:

# Gucken, ob morgen was ist, falls ja: zurückgeben; falls nein: Motivationstext
def tomorrow(at)
	morgen = String.new("\n")
	at.each { |term|
    if term.date == Date.today+1 then
        morgen << "Morgen im LABOR: \n#{term}  #{term.link}";
    end
    }
    if morgen == "\n"
        morgen << "Morgen ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
    end
	return morgen
end

# Gucken, ob heute was ist, falls ja: zurückgeben
def today(at)
	heute = String.new("\n")
    at.each { |term|
    if term.date == Date.today then
        heute << "Heute im LABOR: \n#{term}  #{term.link}";
    end
    }
	if heute == "\n"
		heute << "Heute ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
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

# Mit Jabber-Server verbinden
@client = Jabber::Client.new(Jabber::JID.new('terminbot@das-labor.org/LABOR-Veranstaltungen'))
@client.connect
@client.auth('foobar23')


# Roster bauen
@roster = Jabber::Roster::Helper.new(@client)

# Alle subscription requests akzeptieren und ebenfalls subscription requesten
@roster.add_subscription_request_callback { |item, presence|
	if presence.type == :subscribe
    	@roster.accept_subscription(presence.from)
#		puts("subscribed")
		item.subscribe()
	end
}

# Wenn ein User unsubscribet, also die Authorisation zurückzieht,
# entferne diesen User aus dem Roster des Bots
@roster.add_subscription_callback { |item, presence|
    if presence.type == :unsubscribed
#        puts("unsubscribed")
        item.remove()
    end
}

@client.send(Jabber::Presence.new.set_show(:chat).set_status('waiting for your requests'))
#@client.send(Jabber::Presence.new(:chat, 'LABOR-Termine'))


@client.add_message_callback { |m|
#	puts(m.body())
#	puts Date.today
	if m.body == "tomorrow" then
		if(tomorrow(at))
			answer = Jabber::Message.new(m.from)
			answer.type = :chat
			answer.set_body(tomorrow(at))
			@client.send(answer)
		end
    elsif m.body == "today" then
		if(today(at))
	        answer = Jabber::Message.new(m.from)
    	    answer.type = :chat
        	answer.set_body(today(at))
	        @client.send(answer)
		end
    elsif m.body == "all" then
        if(listall(at))
            answer = Jabber::Message.new(m.from)
            answer.type = :chat
            answer.set_body(listall(at))
            @client.send(answer)
        end
    elsif m.body == "unsubscribe" then
		remove_jid(m.from)
	elsif
		answer = Jabber::Message.new(m.from)
		answer.type = :chat
        answer.set_body("
Command not known!
I know the following commands:
all			Shows all events
today		Shows todays events
tomorrow	Shows tomorrows events
")

        @client.send(answer)
	end
}

Thread.stop

@client.close
