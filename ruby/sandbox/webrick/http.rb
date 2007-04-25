require 'webrick'
include WEBrick

s = HTTPServer.new(
  :Port => 2000,
  :DocumentRoot => Dir::pwd + "/htdocs" )

trap("INT"){ s.shutdown }
s.start