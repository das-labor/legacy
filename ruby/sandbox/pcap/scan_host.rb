require 'socket'
@host = ARGV[0]
@range = ARGV[1].split("-")
@range[0].to_i.upto(@range[1].to_i) do |num|
  begin
    t = TCPSocket.new("#{@host}.#{num}", 80)
  rescue
    #p port
  else
    t.close
    s = Socket.getnameinfo(["AF_INET", 80, 'localhost'])
    print "Port #{num} (#{s[1]}) ist offen.\n"
  end
  num += 1
end