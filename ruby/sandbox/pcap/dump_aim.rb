require 'pcaplet'
require 'logger'
log = Logger.new('aim.log')

$network = Pcaplet.new('-s 1500')

def has_nonprint? n
  # figure out if the string has non-printable characters
  n.each_byte {|x| return false if x < 32 or x > 126}
end

def aim_msg_parse p
  # figure out how many text characters are in the screen name
  name_length = p.tcp_data[26..26].unpack("c")
  # extract the screen name from the packet
  name = p.tcp_data[27..(27 + name_length[0])]
  # filter out all other text
  p.tcp_data[85..-1][/<[^>]+>(.*)<\//]
  msg = $1.gsub(/<[^>]+>/,"").strip
  # make sure that it is an actual message and then return it
  return [name, msg] if msg and not has_nonprint?(name) and
    name =~ /^[a-zA-Z]/ and not name.include?("/")
  # if it isn't really a text message, return nothing
  nil
rescue
end

$www_filter = Pcap::Filter.new('tcp and dst port 80', $network.capture)
$aim_recv_filter = Pcap::Filter.new('tcp and src port 5190', $network.capture)
$aim_send_filter = Pcap::Filter.new('tcp and dst port 5190', $network.capture)

$network.add_filter($aim_recv_filter | $aim_send_filter | $www_filter)

for p in $network
  if $www_filter =~ p and p.tcp_data =~ /GET(.*)HTTP.*Host:([^\r\n]*)/xm
    puts "#{p.src} - http://#{$2.strip}#{$1.strip}"
    log.info { "#{p.src} - http://#{$2.strip}#{$1.strip}" }
  elsif $aim_recv_filter =~ p
    name, msg = aim_msg_parse p
    puts "(<-) <#{p.dst}> from #{name}: #{msg}" if name and msg
    log.info { "(<-) <#{p.dst}> from #{name}: #{msg}" if name and msg }
  elsif $aim_send_filter =~ p
    name, msg = aim_msg_parse p
    puts "(->) <#{p.src}> to #{name}: #{msg}" if name and msg
    log.info { "(->) <#{p.src}> to #{name}: #{msg}" if name and msg }
  end
end