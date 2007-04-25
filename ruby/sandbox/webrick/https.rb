require 'webrick'
require 'webrick/https'

s = WEBrick::HTTPServer.new(
  :Port => 2000,
  :DocumentRoot => Dir.pwd + "/path/to/htdocs",
  :SSLEnable => true,
  :SSLVerifyClient => ::OpenSSL::SSL::VERIFY_NONE,
  :SSLCertName => [ ["C","DE"], ["O","WoNo"], ["CN", "WWW"] ])

trap("INT") { s.shutdown }
s.start