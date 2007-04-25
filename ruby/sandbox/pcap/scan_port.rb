require 'socket'
@host = ARGV[0]
@range = ARGV[1].split("-")
@range[0].to_i.upto(@range[1].to_i) do |port|
  begin
    t = TCPSocket.new(@host, port)
  rescue
    #p port
  else
    t.close
    s = Socket.getnameinfo(["AF_INET", port, 'localhost'])
    print "Port #{port} (#{s[1]}) ist offen.\n"
  end
  port += 1
end