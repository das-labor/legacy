#Nutzung: <Skriptname> <RESTMUELL_ICS> <BLAUMUELL_ICS> <GELBMUELL_ICS>
#Das Skript gibt ein generiertes .ics aus.
#Die drei *_ICS sind die .ics von der USB-Seite
##GELBMUELL_ICS_SNIPPET ist das partielle .ics, welches das Skript gelbkalendersnippet.rb generiert.

require 'common'

#configuration begins here

RESTMUELL_SUMMARY = "USB Restmuell am Folgetag: Nachschauen, wieviele Tonnen geleert wurden!"
BLAUMUELL_SUMMARY = "USB Altpapier, Abholung am Folgetag: Tonnen (grosse und kleine) zur Strasse!"
GELBMUELL_SUMMARY = "USB KWTonne, Abholung am Folgetag: Tonnen zur Strasse!"

#work begins here

#USAGE = "#{$0} <RESTMUELL_ICS> <BLAUMUELL_ICS> <GELBMUELL_ICS_SNIPPET>"
USAGE = "#{$0} <RESTMUELL_ICS> <BLAUMUELL_ICS> <GELBMUELL_ICS>"
die("Falsche Anzahl an Parametern", true) unless $*.length == 3

#typical values:
#RESTMUELL_ICS = "USB_1_586_Stand_20100316.ics"
#BLAUMUELL_ICS = "USB_3_586_Stand_20100316.ics"
RESTMUELL_ICS, BLAUMUELL_ICS, GELBMUELL_ICS = $* #store arguments

def dumpline(line)
  $stdout << ICS::fold_line(line)
end

#wrap a Date object to allow repetition (currently, only weekly recurrence rule supported)
class RepeatedDate
  attr_reader :date
  
  def initialize(date, numweeks)
    @date = date
    @daily_increment = 7 * numweeks
  end

  def succ
    RepeatedDate.new(@date + @daily_increment, @daily_increment / 7)
  end

  def <=>(other)
    return @date <=> other.date
  end
end

def dumppartialcalender(fname, summary)
  parser = ICS::FileParser.new(fname)
  pr = parser.parse

  (pr.select { |o| o.name == "VEVENT" }).each do |vev|
    event = {}
    
    vev.each do |node| #this code might as well go into common.rb
      case node
      when ICS::ContentLine
        case node.name
        when "DTSTART", "DTEND"
          #we ignore the TZID param
          event[node.name.downcase.to_sym] = Date.from_ics_datetime(node.values[0])
        when "RRULE"
          hsh = ICS::parse_rrule(node.values[0])
          die("Unsupported repetition") unless
              hsh[:freq] == ICS::FREQUENCIES.index("WEEKLY")
          event[:rrule] = hsh
        when "RDATE", "EXDATE"
          #we ignore all parameters (should only be timezone):
          event[node.name.downcase.to_sym] = node.values.collect do |val|
            Date.from_ics_datetime(val)
          end
        else
          #ignored
        end
      when ICS::Component
        #ignore components inside a VEVENT
      end
    end

    die("Repetition error: first occurence not aligned to pattern") unless
        event[:dtstart].wday == event[:rrule][:byday]
    
    #compute recurrence set
    original_dates = []
    if event[:rdate].nil? then
      original_dates = [event[:dtstart]]
    else
      original_dates = event[:rdate].clone << event[:dtstart]
    end

    Range.new(RepeatedDate.new(event[:dtstart], event[:rrule][:interval]),
        RepeatedDate.new(event[:rrule][:until], event[:rrule][:interval])).each do |repdate|
      original_dates << repdate.date
    end
    
    original_dates.uniq!
    original_dates -= event[:exdate] unless event[:exdate].nil?
    
    #dump our events now
    original_dates.sort.each do |date|
      datestr = (date - 1).to_icsdate #shift one day back, need to move trashcans the day BEFORE!
      dumpline("BEGIN:VEVENT")
      dumpline("DTSTART;VALUE=DATE:#{datestr}")
      dumpline("DTEND;VALUE=DATE:#{datestr}")
      dumpline("SUMMARY:#{summary}")
      dumpline("END:VEVENT")
    end
  end
end

dumpline("BEGIN:VCALENDAR")
dumpline("VERSION:2.0")

##Old code for REMONDIS currently not needed, but keep it
# snippet_file = File.open(GELBMUELL_ICS_SNIPPET)
# snippet = snippet_file.read
# snippet_file.close
# $stdout << snippet

dumppartialcalender(RESTMUELL_ICS, RESTMUELL_SUMMARY)
dumppartialcalender(BLAUMUELL_ICS, BLAUMUELL_SUMMARY)
dumppartialcalender(GELBMUELL_ICS, GELBMUELL_SUMMARY)

dumpline("END:VCALENDAR")