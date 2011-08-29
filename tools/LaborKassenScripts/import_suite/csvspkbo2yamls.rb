#!/usr/bin/env ruby

require "#{File.dirname __FILE__}/common"

$check_imported = true #we don't configure this currently

class CSVLine
  include Enumerable
  
  def initialize(text)
    @text = text
  end
  
  def each(&block)
    remainder = nil
    case @text[0]
    when nil #empty last field
      block.call nil
      return
    when ";"[0] #empty mid field
      block.call nil
      remainder = @text[1..-1]
    else
      md = /^"(([^"]|"")*)"(.*)$/.match @text
      abort "CSV parse error (could not parse field)" if md.nil? || md[1].nil? || md[3].nil?
      block.call md[1]
      if md[3][0] == ";"[0]
        remainder = md[3][1..-1]
      else
        abort "CSV parse error (no semicolon or EOL after quotation mark)" unless md[3].empty?
        return
      end
    end
    CSVLine.new(remainder).each(&block) #recurse
  end
end

class SpkBoCSV
  attr_reader :transactions
  
  HEADERS = %W(Auftragskonto Buchungstag Valutadatum Buchungstext Verwendungszweck) +
      %W(Beg\374nstigter/Zahlungspflichtiger Kontonummer BLZ Betrag W\344hrung Info)
  
  COLUMNS = [:auftragskonto, :butag, :valtag, :text, :zweck, :person, :konto, :blz, :betrag, :currency, :info]
  INDICES = Hash[*COLUMNS.zip((0...COLUMNS.length).to_a).reduce(:+)]
  
  def initialize(raw)
    @transactions = []
    
    num = 1
    raw.each_line do |line|
      fields = []
      line.chomp! "\n"
      fields = CSVLine.new(line).to_a
      abort "CSV Format not as expected (not 11 columns)" unless fields.length == 11
      
      if num == 1
        abort "CSV Format not as expected (column headers)" unless fields == HEADERS
        info "CSV Column headers look ok."
      else
        check_line num, line, fields
      end
      num += 1
    end
  end
  
  private
  
  def parse_date(linenum, valtag)
    begin
      return Date.strptime valtag, "%d.%m.%y"
    rescue ArgumentError
      #drop into special handling
    end
    
    #bad date. take the following heuristic instead:
    numbers = valtag.split(".").collect(&:to_i)
    
    if numbers[1] == 12
      numbers[1] = 1
      numbers[2] += 1
    else
      numbers[1] += 1
    end

    date = Date.civil numbers[2] + 2000, numbers[1], 1 #first of next month
    warning "Invalid date '#{valtag}' in row #{linenum}, taking #{date} instead"
    return date
  end
  
  def make_description(num, fhash)
    return "#{fhash[:text]}|#{fhash[:zweck]}|#{fhash[:person]}"
  end
  
  #line is string without trailing newline
  def check_line(num, line, fields)
    fieldhash = Hash.new { |hsh, key| hsh[key] = fields[INDICES[key]] }
    abort "Column 'Auftragskonto': this is not our account." unless fieldhash[:auftragskonto] == "33419177"
    abort "Column 'Waehrung': not 'EUR'." unless fieldhash[:currency] == "EUR"
    
    case fieldhash[:info]
    when "Umsatz gebucht"
      #fine
    when "Umsatz vorgemerkt"
      info "Ignoring row #{num} ('Umsatz vorgemerkt')"
      return
    else
      warning "Unknown 'Info' value in row #{num}, ignoring row"
      return
    end
    
    date = parse_date num, fieldhash[:valtag]
    
    amount = fieldhash[:betrag].gsub(",", "") #take cents instead of EUR (as 0,00)
    
    desc = make_description num, fieldhash
    
    if $check_imported
      unknown = Transaction.count(:csvline => line).zero?
      info "ignoring row #{num} (already in database)" unless unknown
      return unless unknown
    end
    
    trn = Transaction.new :csvline => line, :exportdate => date,
        :exportamount => amount, :exportdescr => desc
    
    @transactions << trn
  end
end

unless ARGV.length == 1
  $stderr << "Usage: #{$0} <path to sparkasse bochum exported csv file\n"
  $stderr << "Outputs on stdout records for the other scripts in yaml format.\n"
  $stderr << "Does not touch database...\n"
  exit 1
end

importcsv = `cat #{ARGV[0]}`
abort("Could not open file, cat exited with status #{$?.exitstatus}") unless $?.exitstatus.zero?

setup_db

csv = SpkBoCSV.new importcsv
csv.transactions.each { |t| puts t.to_yaml }
