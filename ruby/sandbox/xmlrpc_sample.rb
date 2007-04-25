#### ::: Server
require 'webrick'
require 'xmlrpc/server.rb'

# create a servlet to handle XML-RPC requests:
servlet = XMLRPC::WEBrickServlet.new
servlet.add_handler("upper_case") { |a_string| a_string.upcase }
servlet.add_handler("query") { |a_string| test('jau') }

def test(val)
  val + ' works fine..'
end

# create a WEBrick instance to host this servlet:
server=WEBrick::HTTPServer.new(:Port => 9090)
trap("INT"){ server.shutdown }
server.mount("/MIL", servlet)
server.start


###
#### ::: Client 
require 'xmlrpc/client'
server = XMLRPC::Client.new("127.0.0.1", "/MIL", 9090)
puts server.call('upper_case', 'the fat dog chased the cat on Elm Street.')
puts server.call('test', 'ich bin ein string')