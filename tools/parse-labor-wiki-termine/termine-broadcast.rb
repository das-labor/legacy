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

htmlout = File.new( ARGV[0]+"/termine_html", "w" );

at.each { |term|
  if term.date == Date.today then
    htmlout.puts "<b><a href=\"#{term.link}\">#{term.day}.#{term.month}.#{term.year} (#{term.wday}) #{term.hour}:#{term.min} - #{term.text}</a></b><br/>"
  elsif term.date > Date.today-1 then
    htmlout.puts "<a href=\"#{term.link}\">#{term.day}.#{term.month}.#{term.year} (#{term.wday}) #{term.hour}:#{term.min} - #{term.text}</a><br/>"
  end

}

htmlout.close


rss20out = File.new( ARGV[0]+"/rss20", "w" );

rss20out.puts "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?> \n 
<rss version=\"2.0\"> \n 
<channel> \n 
<title>LABOR Bochum: Termine</title> \n
<link>http://www.das-labor.org</link> \n 
<description>Terminfeed des LABORs</description> \n 
<language>de-de</language> \n
<pubDate>#{Time.now.strftime("%a, %d %b %Y %H:%M:%S %Z")}</pubDate>"


at.each { |term|
  if term.date > Date.today-1 then
    rss20out.puts "<item>\n<title>#{term.text}</title>\n<description>#{term.day}.#{term.month}.#{term.year} (#{term.wday}) #{term.hour}:#{term.min} - #{term.text}</description>\n<link>#{term.link}</link>\n</item>\n\n"
  end
}

rss20out.puts "</channel> \n</rss>"

rss20out.close
