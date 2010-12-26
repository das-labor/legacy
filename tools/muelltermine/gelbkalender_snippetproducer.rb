#Nutzung: <Skriptname> <Jahreszahl>
#Erwartet in der Standardeingabe den relevanten Teil des Inhalts der Seite
#Nimm nur einen Ausschnitt der Seite! Abgesehen von <br> erwartet das Skript
#naemlich korrektes XML!
#Das Skript gibt einen Ausschnitt eines .ics aus, welches von dem anderen Skript
#eingelesen werden kann.

require 'common'

require 'rexml/document'

#configuration begins here

GELBMUELL_SUMMARY = "REMONDIS DSD, Abholung am Folgetag: gelbe Tonnen (grosse und kleine) zur Strasse!"

#work begins here

USAGE = "#{$0} <Jahreszahl>"
die("Falsche Anzahl an Parametern", true) unless $*.length == 1

YEAR = $*[0].to_i

def dumpline(line)
  $stdout << ICS::fold_line(line)
end

data = $stdin.read
data.gsub!("<br>", "<br/>") #hope this is sufficient to produce valid XML

doc = REXML::Document.new data
doc.elements.each("div[@class='tx-3sdsdyellowbags-pi2']/table/tbody/tr[position() = 2]/td[position() = 1]/table/tbody/tr/") do |el|
  el.elements.each("td[position() = 3]") do |el|
    el.texts.each do |text|
      if (%r{(#{NUM}{2}).(#{NUM}{2}).}.match(text.to_s)) then
        datestr = (Date.new(YEAR, $2.to_i, $1.to_i) - 1).to_icsdate
        
        dumpline("BEGIN:VEVENT")
        dumpline("DTSTART;VALUE=DATE:#{datestr}")
        dumpline("DTEND;VALUE=DATE:#{datestr}")
        dumpline("SUMMARY:#{GELBMUELL_SUMMARY}")
        dumpline("END:VEVENT")
      end
    end
  end
end