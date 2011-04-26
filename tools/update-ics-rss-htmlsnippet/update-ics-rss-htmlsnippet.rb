#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

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
##it seems these two are NOT needed for timezone stuff:
#require 'active_support'
#require 'active_support/time_with_zone'
require 'ri_cal'
gem 'builder', '~> 3.0'
require 'builder'

$KCODE = "UTF8" #make builder emit utf-8

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

class Date
  def wochentag
    return %w{So Mo Di Mi Do Fr Sa}[wday]
  end
end

class RiCal::Component::Event
  def link
    url || attach.first
  end
  
  def display_summary
    date = dtstart
    spacelist = [date.strftime("%d.%m.%Y"), "(#{date.wochentag})"]
    spacelist << date.strftime("%H:%M") if date.instance_of? DateTime
    spacelist += ["-", summary]
    return spacelist.join " "
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
      xml = Builder::XmlMarkup.new :target => f, :indent => 2 #needed for newlines
      
      xml.instruct!
      xml.rss("version" => "2.0") do
        xml.channel do
          xml.title title
          xml.link "http://www.das-labor.org"
          xml.description "Terminfeed des Labor e.V."
          xml.language "de-de"
          xml.pubDate Time.now.strftime("%a, %d %b %Y %H:%M:%S %Z")
          
          @events.each do |e|
            date = e.dtstart
            xml.item do
              xml.title e.summary
              xml.description e.display_summary
              link = e.link
              xml.link link if link
            end
          end
        end
      end
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
  
  MAX_EVENTS = 5
  
  def occurrence_options_for(rec)
    return { :before => Date.today + 19, :starting => Date.today, :count => MAX_EVENTS }
  end
  
  def initialize
    @events = []
  end
  
  def event(evt)
    @events << evt
  end
  
  def finish
    @events.sort! { |e1, e2| e1.dtstart <=> e2.dtstart }
    @events = @events[0...MAX_EVENTS]
    
    html = "" #temporary storage for html snippet
    
    xml = Builder::XmlMarkup.new :target => html, :indent => 2 #needed for newlines
    xml.instruct! #needed to produce utf8, but not desired in output! see below events loop
    
    @events.each do |e|
      date = e.dtstart
      link = e.link
      highlight = e.dtstart.to_date == Date.today
      
      if link
        if highlight
          xml.a("href" => link) do
            xml.b e.display_summary
          end
        else
          xml.a(e.display_summary, "href" => link)
        end
      else
        xml << "<b>" if highlight
        xml.text! e.display_summary
        xml << "</b>" if highlight
      end
      xml << "<br/>\n"
    end
    html.sub! /<\?[^?]*\?>/, "" #remove output by instruct! again
    html.sub! /^\s*/, "" #remove leading whitespace
    html.gsub! %r{</a>\n<br/>}, "</a><br/>" #nice for debugging!

    File.open(SNIPPETFILE, "w") do |f|
      f << html
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
