#!/usr/bin/env ruby
#  uncool und geht im prinzip viel besser

require 'rexml/document'
require 'date'

XMLSOURCE=ARGV[0]+"/termine.xml";

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


timestamp = Time.now.to_i

doc = REXML::Document.new( File.new( XMLSOURCE ));
at = Array.new

doc.elements.each("/termine/termin") { |tx|
    at.push(Termin.new(tx));
}

at.sort!{ |a,b| a.date <=> b.date }

icalout = File.new( ARGV[0]+"/LABOR-Termine.ics", "w" );

icalout.puts "BEGIN:VCALENDAR
VERSION:2.0
X-WR-CALNAME:LABOR-Termine
PRODID:-//Apple Computer\, Inc//iCal 2.0//EN
X-WR-RELCALID:5644223456g
X-WR-TIMEZONE:Europe/Berlin
CALSCALE:GREGORIAN
METHOD:PUBLISH"

at.each { |term|
    icalout.puts "BEGIN:VEVENT
LOCATION:Labor
DTSTAMP:#{timestamp}Z
UID:#{timestamp}#{term.year}#{term.month}#{term.day}@labor
SEQUENCE:10
URL;VALUE=URI:#{term.link}
DTSTART;TZID=Europe/Berlin:#{term.year}#{term.month}#{term.day}T#{term.hour}#{term.min}00
SUMMARY:#{term.text}
DTEND;TZID=Europe/Berlin:#{term.year}#{term.month}#{term.day}T235900
LAST-MODIFIED:#{timestamp}
DESCRIPTION:#{term}
END:VEVENT\n"

}


icalout.puts "END:VCALENDAR"

icalout.close

