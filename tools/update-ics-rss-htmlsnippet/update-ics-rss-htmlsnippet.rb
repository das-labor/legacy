#!/usr/bin/env ruby

###CONFIG starts here

BASEPATH = "/srv/www.das-labor.org/htdoc/"
#BASEPATH = "./" #for debugging

class TalksWorkshopsRSS
  RSSFILE = "#{BASEPATH}/talksworkshops.rss"
  RSSDESCRIPTION = "LABOR Bochum: Vorträge und Workshops" #must be UTF8
end

class TermineRSS
  RSSFILE = "#{BASEPATH}/termine.rss"
  RSSDESCRIPTION = "LABOR Bochum: Termine" #must be UTF8
end

class OutputHTML
  SNIPPETFILE = "#{BASEPATH}/termine_html"
end

module Main
  #where to download the .ics to (we advertise this location in our wiki!)
  ICS_FILE = "#{BASEPATH}/termine.ics" #must not contain white space!
  
  ICS_FETCH_OPTIONS = "--silent"
  cmdstr_part = %Q{curl #{ICS_FETCH_OPTIONS} -o}
  #Fetch command: array of arguments to Kernel#system
  ICS_FETCH_COMMAND = cmdstr_part.split(/\s+/) << ICS_FILE <<
    'https://anonymous:labor@www.das-labor.org/davical/caldav.php/labor-termine/home/'
end

###CONFIG end

require 'rubygems'
require 'tzinfo' #needed for timezone hack, see below
#require 'active_support'
#require 'active_support/time_with_zone'
require 'ri_cal'

###MONKEYPATCH starts here
#Someone is off the spec, dunno if webdav client (Lightning), server or RiCal.
#There are EXDATE properties with UTC dates for VEVENTs with Europe/Berlin dtstart property.
#Thus, we need to teach RiCal to compare dates in different time zones (only the two zones involved)

#declare default timezone. Dunno if this has any effect...
RiCal::PropertyValue::DateTime.default_tzid = 'Europe/Berlin'

class RiCal::PropertyValue::DateTime
  DEFAULT_TZ_STR = 'Europe/Berlin'
  DEFAULT_TZ = TZInfo::Timezone.get(DEFAULT_TZ_STR)
  DEFAULT_TZ_RE = /Europe\/Berlin/

  alias_method :old_spaceship, :<=>
  
  #provide FastDateTime object in default time zone. caching.
  def defaulttz_fast_date_time
    @defaulttz_fast_date_time ||= case tzid
    when nil
      $stderr << "WARNING: nil time zone \"#{tzid}\" (#{__FILE__}:#{__LINE__})\n"
      @date_time_value #better than failing
    when "UTC"
      RiCal::FastDateTime.from_date_time(DEFAULT_TZ.utc_to_local(to_datetime))
    when DEFAULT_TZ_RE
      #we should actually never get here
      @date_time_value
    else
      $stderr << "WARNING: not supported time zone \"#{tzid}\" (#{__FILE__}:#{__LINE__})\n"
      @date_time_value #better than failing
    end
  end
  
  def <=>(other)
    #check if other is not of the same class
    #we can't do anything in this case, tz info lost
    return old_spaceship(other) unless RiCal::PropertyValue::DateTime === other

    otzid = other.tzid || DEFAULT_TZ_STR
    mtzid = tzid || DEFAULT_TZ_STR
    
    if mtzid == otzid
      return old_spaceship(other) #same tzid, fast implementation
    elsif mtzid == "UTC"
      $stderr << "WARNING: not supported time zone for #{other.inspect} (#{__FILE__}:#{__LINE__})\n" unless
        DEFAULT_TZ_RE.match(otzid)
      return other.cmp_fast_date_time_value defaulttz_fast_date_time
    elsif otzid == "UTC"
      $stderr << "WARNING: not supported time zone for #{inspect} (#{__FILE__}:#{__LINE__})\n" unless
        DEFAULT_TZ_RE.match(mtzid)
      return -(cmp_fast_date_time_value other.defaulttz_fast_date_time)
    else
      $stderr << "WARNING: not supported time zone for one of #{inspect}, #{other.inspect} (#{__FILE__}:#{__LINE__})\n"
      return old_spaceship(other)
    end
  end
  
  def ==(other)
    return self.<=>(other).zero?
  end
end
###MONKEYPATCH end

###ESCAPING

#XML Character converter, from Sam Ruby:
#(see http://intertwingly.net/stories/2005/09/28/xchar.rb).
module XChar
  # http://intertwingly.net/stories/2004/04/14/i18n.html#CleaningWindows
  CP1252 = {
    128 => 8364, # euro sign
    130 => 8218, # single low-9 quotation mark
    131 =>  402, # latin small letter f with hook
    132 => 8222, # double low-9 quotation mark
    133 => 8230, # horizontal ellipsis
    134 => 8224, # dagger
    135 => 8225, # double dagger
    136 =>  710, # modifier letter circumflex accent
    137 => 8240, # per mille sign
    138 =>  352, # latin capital letter s with caron
    139 => 8249, # single left-pointing angle quotation mark
    140 =>  338, # latin capital ligature oe
    142 =>  381, # latin capital letter z with caron
    145 => 8216, # left single quotation mark
    146 => 8217, # right single quotation mark
    147 => 8220, # left double quotation mark
    148 => 8221, # right double quotation mark
    149 => 8226, # bullet
    150 => 8211, # en dash
    151 => 8212, # em dash
    152 =>  732, # small tilde
    153 => 8482, # trade mark sign
    154 =>  353, # latin small letter s with caron
    155 => 8250, # single right-pointing angle quotation mark
    156 =>  339, # latin small ligature oe
    158 =>  382, # latin small letter z with caron
    159 =>  376} # latin capital letter y with diaeresis
  
  # http://www.w3.org/TR/REC-xml/#dt-chardata
  PREDEFINED = {
    38 => '&amp;', # ampersand
    60 => '&lt;',  # left angle bracket
    62 => '&gt;'}  # right angle bracket
  
  # http://www.w3.org/TR/REC-xml/#charsets
  VALID = [[0x9, 0xA, 0xD], (0x20..0xD7FF), 
           (0xE000..0xFFFD), (0x10000..0x10FFFF)]
end

class Fixnum
  # xml escaped version of chr
  def xchr
    n = XChar::CP1252[self] || self
    n = 42 unless XChar::VALID.find {|range| range.include? n}
    XChar::PREDEFINED[n] or (n<128 ? n.chr : "&##{n};")
  end
end

class String
  # xml escaped version of to_s
  def to_xs
    unpack('U*').map {|n| n.xchr}.join # ASCII, UTF-8
  rescue
    unpack('C*').map {|n| n.xchr}.join # ISO-8859-1, WIN-1252
  end
end

class String
  alias_method :escapeXML, :to_xs
  
  #For escaping inside attribute values
  def escapeQ
    escapeXML.gsub(/(\\|")/) { |match| "\\#{match}" }
  end
end

###ESCAPING end

class Date
  def wochentag
    return %w{So Mo Di Mi Do Fr Sa}[wday]
  end
end

class RiCal::Component::Event
  def link
    url || attach.first
  end
end

module OutputHandler
  def cached_occurrence_options_for(rec)
    @oocache ||= { false => occurrence_options_for(false),
                   true => occurrence_options_for(true) }
    return @oocache[rec]
  end
  
  #Below is the general api for output handlers, should we need to create a new one.

  #Return occurrence options hash for events that are recurring or not.
  #Return nil if you do not want these events at all.
  def occurrence_options_for(rec)
    return nil
  end

  #Be notified of an event occurrence.
  def event(evt)
  end
  
  #Postprocess your events and produce the output.
  def finish
  end
end

#Include this module for RSS output support.
#You only need to provide the RSSFILE and RSSDESCRIPTION constants and
#the occurrence_options_for method.
module RSSOutput
  def initialize
    @events = []
  end
  
  def event(evt)
    @events << evt
  end
  
  def finish
    @events.sort! { |e1, e2| e1.dtstart <=> e2.dtstart }
    
    output_rss(self.class::RSSFILE, self.class::RSSDESCRIPTION, @events)
  end
  
  private
  
  def output_rss(filename, title, events)
    File.open(filename, "w") do |f|
      f << %q{<?xml version="1.0" encoding="UTF8"?>
<rss version="2.0">
<channel>
}
      f << %Q{<title>#{title.escapeXML}</title>
<link>http://www.das-labor.org</link>
<description>Terminfeed des Labor e.V.</description>
<language>de-de</language>
<pubDate>} + Time.now.strftime("%a, %d %b %Y %H:%M:%S %Z") + "</pubDate>\n"
      @events.each do |e|
        date = e.dtstart
        f << %Q{<item>
<title>#{e.summary.escapeXML}</title>
<description>#{date.strftime("%d.%m.%Y")} (#{date.wochentag}) #{date.strftime("%H:%M")} - #{e.summary.escapeXML}</description>\n}
        if link = e.link
          f << "<link>#{link.escapeXML}</link>\n"
        end
        f << "</item>\n"
      end
      f << "</channel>\n</rss>"
    end
  end
end

class TalksWorkshopsRSS
  include OutputHandler
  include RSSOutput
  
  def occurrence_options_for(rec)
    return nil if rec #only non-recurring events
    return { :starting => Date.today }
  end
end

class TermineRSS
  include OutputHandler
  include RSSOutput
  
  def occurrence_options_for(rec)
    return { :before => Date.today + 19, :starting => Date.today }
  end
end

class OutputHTML
  include OutputHandler
  
  def occurrence_options_for(rec)
    return { :before => Date.today + 19, :starting => Date.today }
  end
  
  def initialize
    @events = []
  end
  
  def event(evt)
    @events << evt
  end
  
  def finish
    @events.sort! { |e1, e2| e1.dtstart <=> e2.dtstart }
    
    File.open(SNIPPETFILE, "w") do |f|
      @events.each do |e|
        date = e.dtstart
        hline = "#{date.strftime("%d.%m.%Y")} (#{date.wochentag}) #{date.strftime("%H:%M")} - #{e.summary.escapeXML}"
        if link = e.link
          hline = "<a href=\"#{link.escapeQ}\">#{hline}</a>"
        end
        if date.to_date == Date.today
          f << "<b>#{hline}</b><br/>\n"
        else
          f << "#{hline}<br/>\n"
        end
      end
    end
  end
end

module Main
  class << self
    attr_reader :handlers
  end
  
  @handlers = []
  
  def self.run
    if !system(*ICS_FETCH_COMMAND)
      $stderr << "ERROR: Unable to fetch .ics file, process exited with status #{$?.exitstatus}:\n" <<
        "Subprocess was: #{ICS_FETCH_COMMAND.join ' '}\n"
      exit 1
    end
    
    File.open(ICS_FILE) do |file|
      RiCal.parse(file).first.events.each do |e|
        @handlers.each do |handler|
          opts = handler.cached_occurrence_options_for e.recurs? ? true : false #XXX recurs? is not documented!
          e.occurrences(opts).each do |evt|
            handler.event evt
          end unless opts.nil?
        end
      end
    end
    
    @handlers.each { |h| h.finish }
  end
end

Main.handlers.push TalksWorkshopsRSS.new, TermineRSS.new, OutputHTML.new
Main.run