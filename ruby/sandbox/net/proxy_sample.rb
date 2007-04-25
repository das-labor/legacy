require 'net/http'
require 'uri'

url = URI.parse('http://www.communicode.de/website/de/index.jsp')

Net::HTTP::Proxy('127.0.0.1', 8118).start(url.host) {|http|
  print http.get(url.path).code
  print http.get(url.path).body
}