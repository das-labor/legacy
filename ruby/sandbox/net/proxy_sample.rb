require 'net/http'
require 'uri'

url = URI.parse('http://w3c.org')

Net::HTTP::Proxy('127.0.0.1', 8118).start(url.host) {|http|
  print http.get(url.path).code
  print http.get(url.path).body
}