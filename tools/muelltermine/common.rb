require 'date'

CRLF = "\r\n"
NUM = "[0-9]"

def die(complaint, usage = false)
  $stderr << complaint + "\n"
  $stderr << "Nutzung: " + USAGE + "\n" if usage
  exit 1
end

class Date
  def self.from_ics_datetime(str)
    case str
    when %r{^(#{NUM}{4})(#{NUM}{2})(#{NUM}{2})T#{NUM}{6}Z$}
      self.civil($1.to_i, $2.to_i, $3.to_i)
    when %r{^(#{NUM}{4})(#{NUM}{2})(#{NUM}{2})T#{NUM}{6}$}
      self.civil($1.to_i, $2.to_i, $3.to_i)
    else
      die("Fehler: Datumsformat nicht unterstuetzt")
    end
  end
  
  def to_icsdate
    to_s.split("-").join("")
  end
end

module ICS
  #helper functions, classes and constants for understanding .ics format
  #very, very bare!

  #fold a single line
  #expects a string without line ending
  #returns array of CRLF terminated strings
  def self.fold_line(line) #no line ending!
    segments = []
    /^(.{0,75})(.*)$/.match(line)
    segments << $1 + CRLF
    remainder = $2
    while !remainder.empty?
      /^(.{0,74})(.*)$/.match(remainder)
      remainder = $2
      segments << " " + $1 + CRLF
    end
    return segments
  end
  
  FREQUENCIES = "SECONDLY,MINUTELY,HOURLY,DAILY,WEEKLY,MONTHLY,YEARLY".split(",")
  WEEKDAYS = Date::DAYNAMES.map { |s| s[0..1].upcase }

  #returns hash
  def self.parse_rrule(value)
    segments = Hash[*(value.split(/[;=]/))]
    result = {}
    segments.each_pair do |ky, val|
      ky = ky.downcase.to_sym
      case ky
      when :freq
        result[ky] = ICS::FREQUENCIES.index(val)
      when :interval
        result[ky] = val.to_i
      when :until
        result[ky] = Date.from_ics_datetime(val)
      when :byday, :wkst
        result[ky] = ICS::WEEKDAYS.index(val)
      else
        die("Unsupported RRULE segment")
      end
    end
    return result
  end

  def self.rrule_hash_to_value(hsh)
    hsh.each_key do |ky|
      case ky
      when :freq
        hsh[ky] = ICS::FREQUENCIES[hsh[ky]]
      when :interval
        hsh[ky] = hsh[ky].to_s
      when :until
        hsh[ky] = Date.from_ics_datetime(hsh[ky])
      when :byday, :wkst
        hsh[ky] = ICS::WEEKDAYS[hsh[ky]]
      end
    end
  end

  module Node
  end
  
  class ContentLine
    include Node
    
    attr_accessor :name
    attr_accessor :values
    attr_reader :params
    
    def initialize
      @params = {}
      #XXX not really sure a hash is appropriate here
      #(i.e. whether keys are unique)
    end
    
    def to_unfoldedline
      res = name
      params.each_pair do |ky, pv_ary|
        res << ";#{ky}=#{pv_ary.join(",")}"
      end
      res << ":" << values.join(",")
      return res
    end
  end

  class Component < Array
    include Node
    attr_accessor :name
  end

  class FileParser
    def initialize(name)
      file = File.open(name)
      @unfoldedlines = []
      file.each_line(CRLF) do |line|
        %r{^( ?)(.*)#{CRLF}$}.match(line)
        if $1.empty? then
          @unfoldedlines << $2
        else
          #unfold
          lastline = @unfoldedlines.pop
          @unfoldedlines.push(lastline + $2)
        end
      end
      file.close
    end

    #returns top level ICSComponent
    def parse
      current = nil
      stack = []
      cl = nil
      while (cl = get_contentline)
        case cl[0]
        when "BEGIN"
          sub = Component.new
          sub.name = cl.last[0]
          current << sub unless current.nil?
          stack << current unless current.nil?
          current = sub
        when "END"
          return current if stack.empty?
          current = stack.pop
        else
          clobj = ContentLine.new
          clobj.name = cl[0]
          clobj.values = cl[-1]
          cl[1..-2].each do |p|
            clobj.params[p[0]] = p[1]
          end
          current << clobj
        end
      end
    end
    
    private
    
    #returns [name, [param-name, [param-values]], ..., [param-name, [param-values]], [values]]
    #where each component is a string
    def get_contentline
      return nil if @unfoldedlines.empty?
      
      #XXX this does not interpret quoting!
      line = @unfoldedlines.shift
      %r{^([^;:]*)((;[^:;]*)*):(.*)$}.match(line)
      result = [$1]
      params = $2
      value = $4
      if !params.empty? then
        params = params[1..-1]
        params.split(";").each do |param|
          %r{^([^=]*)=(.*)$}.match(param)
          result << [$1,  $2.split(",")]
        end
      end
      result << value.split(",")
      return result
    end
  end
  
end