#!/usr/bin/env ruby

require 'rexml/document'
require 'date'
require 'config.rb'
require 'latweet.rb'

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



# Terminerinnerung für den nächsten Tag an alle User im Roster
def tweminder_tomorrow(at)
	ltw = LaTweet.new
    at.each { |term|
	    if term.date == Date.today+1 then
    		tmorgen = String.new(TWINTRO)
	        tmorgen << " #{term.text}";
		    if tmorgen != TWINTRO
				puts "#{tmorgen} #{term.link}"
				ltw.tweetwithurl("#{tmorgen}","#{term.link}")
	    	end
			tmorgen = ""
		end
	}
end


if ENABLETWITTER then
	tweminder_tomorrow(at)
end
