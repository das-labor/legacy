#!/usr/bin/env ruby

require "#{File.dirname __FILE__}/common"

unless ARGV.length == 1
  $stderr << "Usage: #{$0} <path to yaml file>\n"
  $stderr << "Convert records in yaml file into a gnucash-friendly csv format on stdout\n"
  exit 1
end

setup_db

yamls = `cat #{ARGV[0]}`
abort("Could not open file, cat exited with status #{$?.exitstatus}") unless $?.exitstatus.zero?

transactions = []
YAML::load_documents(yamls) { |doc| transactions << Transaction.new(doc) }

puts '"Datum";"Beschreibung";"Einzahlung"'
transactions.each do |tran|
  amount = sprintf("%.2f", tran.exportamount / 100.0).to_s
  amount.gsub! '.', ','
  values = [tran.exportdate, tran.exportdescr, amount]
  
  #quote all values as strings in '"' characters
  values.collect! do |v|
    value = v.to_s.gsub '"', '""'
    "\"#{value}\""
  end
  
  puts values.join(";")
end
