def seconds_to_human_format(seconds)
  seconds = seconds.to_f
  if seconds / (60 * 60 * 24 * 30) > 1 then
    weeks = (seconds / (60 * 60 * 24 * 30)).to_i
    seconds -= weeks * 60 * 60 * 24 * 30
  end
  if seconds / (60 * 60 * 24) > 1 then
    days = (seconds / (60 * 60 * 24)).to_i
    seconds -= days * 60 * 60 * 24
  end
  if seconds / (60 * 60) > 1 then
    hours = (seconds / (60 * 60)).to_i
    seconds -= hours * 60 * 60
  end
  if seconds / 60 > 1 then
    minutes = (seconds / 60).to_i
    seconds -= minutes * 60
  end
  seconds = seconds.to_i
  human = ""
  human += "#{weeks}W" if weeks
  human += "#{days}D" if days
  human += "#{hours}h" if hours
  human += "#{minutes}m" if minutes
  human += "#{seconds}s" if seconds and seconds > 0
  return human
end

p seconds_to_human_format(1987)