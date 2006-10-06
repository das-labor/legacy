#!/usr/bin/env ruby

require 'xmpp4r'
require 'xmpp4r/roster/helper/roster'
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




# Mit Jabber-Server verbinden
@client = Jabber::Client.new(Jabber::JID.new('terminbot@das-labor.org/LABOR-Reminder'))
@client.connect
@client.auth('foobar23')

@client.send(Jabber::Presence.new.set_show(:chat).set_status('reminding...'))
#client.send(Jabber::Presence.new(:chat, 'LABOR-Termine'))


# So, eigentlich macht das hier einen Roster, und acceptet alle Subscriptions
# tut's aber nicht.
@roster = Jabber::Roster::Helper.new(@client)

mainthread = Thread.current

@roster.add_query_callback { |iq|
  mainthread.wakeup
}

Thread.stop


# Items aus dem Roster lesen und Empfängerliste erstellen
@jids = Array.new
@roster.groups.each { |group|
	@roster.find_by_group(group).each { |item|
            @jids.push(item.jid)
	}
}


# Terminerinnerung für den nächsten Tag an alle User im Roster
def reminder_tomorrow(at)
    morgen = String.new("\nMorgen im LABOR:")
    at.each { |term|
    if term.date == Date.today+1 then
        morgen << "\n#{term} \n #{term.link}\n\n";
    end
    }
    if morgen != "Morgen im LABOR:"
		@jids.each { |jid|
	        msg = Jabber::Message.new("#{jid}")
    	    msg.type = :chat
			msg.set_body(morgen)
	        @client.send(msg)
		}
    end
	puts(morgen)
end
#puts("----\n")
#@jids.each { |jid|
#    puts("#{jid}")
#}
#puts("----\n")

reminder_tomorrow(at)


@client.close
