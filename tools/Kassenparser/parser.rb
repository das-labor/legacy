#  6000 = GetränkeKasse 
#  6001 = Mitgliedsbeiträge
#  6339 = Miete
#  6002 = Kontoführung
#  0920 = Kasse
require 'date'
require 'rubygems'
require 'gruff'
Kasse = 920
Getreanke = 6000
Mitglied = 6001
Miete = 6339
Kontofuehrung = 6002

start_date = Date.new(2006,2,1);
end_date = Date.new(2006,10,11);

masterarray = Array.new
foohash = Hash.new
mhash = Hash.new
dataarry = Array.new

IO.readlines("test").each do |line|
	# 2. Datum 8. Text 9. Konto rein 10. Konto raus 11. Betrag
	values = line.split(/;/)
	date_val = values[1].split(/\./)
	values[1] = Date.new(date_val[2].to_i,date_val[1].to_i,date_val[0].to_i)
	values[8] = values[8].to_i
	values[9] = values[9].to_i
	values[10] = values[10].to_f
					#  date      text      konto rein konto raus  betrag:w
	masterarray.push( [values[1],values[7],values[8],values[9],values[10]] )
end


masterarray.each do |datensatz|
	if datensatz[2] == Kasse 
		if foohash.has_key? datensatz[0]
			foohash[datensatz[0]] += datensatz[4] 
		else
			foohash.store(datensatz[0],datensatz[4])
		end
	else
		if foohash.has_key? datensatz[0]
			foohash[datensatz[0]] -= datensatz[4]
		else
			foohash.store(datensatz[0],0 - datensatz[4])
		end
	end
	if datensatz[3] == Mitglied
		if mhash.has_key? datensatz[0]
			mhash[datensatz[0]] += datensatz[4] 
		else
			mhash.store(datensatz[0],datensatz[4])
		end
	end
end
graph  = Gruff::Line.new
graph.title = "Labor Kasse #{start_date.to_s} bis #{end_date.to_s} "
n=0;
kassenstand = 0;
mitgliedsumme = 0
marry = Array.new
start_date.upto(end_date) do |date|
	if date.day.to_i == 1 
		graph.labels.store(n,date.month.to_s)
		mitgliedsumme = 0
	end
	foohash.each do |datadate,tagesumsatz|
		if datadate == date
			kassenstand += tagesumsatz
			
		end
	end
	mhash.each do |mdate,tagesumsatz|
		if mdate == date
			mitgliedsumme += tagesumsatz
		end
	end
 	marry.push mitgliedsumme
	dataarry.push kassenstand
	n += 1
end

graph.data("Kassenstand",dataarry)
graph.data("Mitglieder",marry)
graph.write("test.png")
