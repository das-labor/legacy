require 'net/telnet'
# http://ntp.isc.org/bin/view/Servers/StratumOneTimeServers
TIME_SERVER = 'ntp2.fau.de'

options = {
  "Host" => TIME_SERVER, 
  "Telnetmode" => false, 
  "Timeout" => 30, 
  "Port" => "time"
}

# NTP timestamps are represented as a 64-bit fixed-point number, in
# seconds relative to 0000 UT on 1 January 1900.  The integer part is
# in the first 32 bits and the fraction part in the last 32 bits.
# We juts get the first 4 bytes and convert them
ntp_seconds = Net::Telnet.new(options).recv(4).unpack('N').first

# The Ruby Time class handles dates with an epoch 
# starting at midnight january 1 1970
# We have to use the Date class to work with pre-epoch dates.
require 'date'
def get_seconds_diff_1970_1900
  # you might want to cache the Cache result... it won't change ;-)
  (DateTime.new(1970, 1, 1) - DateTime.new(1900, 1, 1)).to_i * 24 * 60 * 60
end

# Convert seconds to a Time object
remote_time = Time.at(ntp_seconds - get_seconds_diff_1970_1900).strftime("%Y-%m-%d %H:%M:%S")

print "Time from #{TIME_SERVER} -> #{remote_time}"
