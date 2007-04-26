require 'URI'
require 'net/http'

host = 'http://blog.innerewut.de/articles/2006/04/21/scaling-rails-with-apache-2-2-mod_proxy_balancer-and-mongrel'
url = URI.parse(host)

# p url.methods.sort

Net::HTTP.start(url.host, url.port) { |http| p http.get(url.path) }