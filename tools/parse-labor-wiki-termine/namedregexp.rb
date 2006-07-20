class NamedRegexp
  def initialize(regex)
    regex_string = regex.inspect

    @match_names = ["MATCH"]
    regex_string.gsub!(/\?:<[^>]+>/) { |match|
      name = match[3..-2]
      @match_names << name
      ""
    }

    @regex = eval regex_string
  end

  def match(string)
    match_data = @regex.match(string)
    return nil if match_data.nil?
    match_data = match_data.to_a
    named_match_data = {}
    @match_names.each { |name|
      named_match_data[name] = match_data.shift
    }
    named_match_data
  end
end
