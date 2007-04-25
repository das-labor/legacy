#!/usr/local/bin/ruby
require 'pcap'
require 'logger'
logger = Logger.new('test.log')

dev = Pcap.lookupdev
cap = Pcap::Capture.open_live(dev)
#cap.setfilter("ip")
cap.loop do |pkt|
    print pkt, "\n"
    logger.info { "#{pkt}" }
end
cap.close
