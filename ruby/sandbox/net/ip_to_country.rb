# location: http://www.soulretrieval.de/trash/ip-to-country.csv.zip
require 'csv'

def get_city(str)
  result = []
  ip = str.split(".").join
  p "such nach #{myip}"
  CSV::Reader.parse(File.open('ip-to-country.csv','rb')) do |row|
    p "processing row #{row}"
    row[0].upto(row[1]) do |n|
      if n.eql? ip
        p "found in #{row}"
        result << row[3]
      end
    end
  end  
end