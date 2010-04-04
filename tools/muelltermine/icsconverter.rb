#Nutzung: <Skriptname> <RESTMUELL_ICS> <BLAUMUELL_ICS> <GELBMUELL_ICS_SNIPPET>
#Das Skript gibt ein generiertes .ics aus.
#RESTMUELL_ICS und BLAUMUELL_ICS sind die beiden .ics von der USB-Seite
#GELBMUELL_ICS_SNIPPET ist das partielle .ics, welches das Skript gelbkalendersnippet.rb generiert.

require 'common'

#configuration begins here

RESTMUELL_SUMMARY = "USB Restmuell am Folgetag: Nachschauen, wieviele Tonnen geleert wurden!"
BLAUMUELL_SUMMARY = "USB Altpapier, Abholung am Folgetag: Tonnen (grosse und kleine) zur Strasse!"

#work begins here

USAGE = "#{$0} <RESTMUELL_ICS> <BLAUMUELL_ICS> <GELBMUELL_ICS_SNIPPET>"
die("Falsche Anzahl an Parametern", true) unless $*.length == 3

#typical values:
#RESTMUELL_ICS = "USB_1_586_Stand_20100316.ics"
#BLAUMUELL_ICS = "USB_3_586_Stand_20100316.ics"
RESTMUELL_ICS, BLAUMUELL_ICS, GELBMUELL_ICS_SNIPPET = $* #store arguments

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
				when "DTSTART"
					#we ignore the TZID param
					event[:dtstart] = Date.from_ics_datetime(node.values[0])
				when "DTEND"
					#we ignore the TZID param
					event[:dtend] = Date.from_ics_datetime(node.values[0])
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
		
		event[:rdate].each do |date|
			datestr = (date - 1).to_icsdate
			dumpline("BEGIN:VEVENT")
			dumpline("DTSTART;VALUE=DATE:#{datestr}")
			dumpline("DTEND;VALUE=DATE:#{datestr}")
			dumpline("SUMMARY:#{summary}")
			dumpline("END:VEVENT")
		end

		event[:exdate].map! { |d| d - 1 } #shift all exdates one day back
		rng = Range.new(RepeatedDate.new(event[:dtstart] - 1, event[:rrule][:interval]),
		                RepeatedDate.new(event[:rrule][:until] - 1, event[:rrule][:interval]))
		rng.each do |wdate|
			if event[:exdate].index(wdate.date).nil? then
				datestr = wdate.date.to_icsdate
				dumpline("BEGIN:VEVENT")
				dumpline("DTSTART;VALUE=DATE:#{datestr}")
				dumpline("DTEND;VALUE=DATE:#{datestr}")
				dumpline("SUMMARY:#{summary}")
				dumpline("END:VEVENT")
			end
		end
	end
end

dumpline("BEGIN:VCALENDAR")
dumpline("VERSION:2.0")

snippet_file = File.open(GELBMUELL_ICS_SNIPPET)
snippet = snippet_file.read
snippet_file.close
$stdout << snippet

dumppartialcalender(RESTMUELL_ICS, RESTMUELL_SUMMARY)
dumppartialcalender(BLAUMUELL_ICS, BLAUMUELL_SUMMARY)

dumpline("END:VCALENDAR")