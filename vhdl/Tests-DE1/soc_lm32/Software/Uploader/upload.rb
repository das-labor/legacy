#!/usr/bin/env ruby
$0 = __FILE__

require "serialport.so"

sp = SerialPort.new(ARGV[0], 57600, 8, 1, SerialPort::NONE)
checksum = 0
upFileSize =  File.size(ARGV[1])
#upFileSize = 10
uploadFile = File.open(ARGV[1], "r")
puts "File loaded." if uploadFile
puts upFileSize

open("/dev/tty", "r+") { |tty|
	tty.sync = true
	Thread.new {
		while true do
		  c = sp.getc
		  tty.printf("%c(%x) ", c, c)
		end
	}

	sp.putc 'u'	
	sp.putc 0xB0
	sp.putc 0x00
	sp.putc 0x00
	sp.putc 0x00
	tty.getc
	sp.putc((upFileSize >> 24) & 0xff)
	sp.putc((upFileSize >> 16) & 0xff)
	sp.putc((upFileSize >> 8)  & 0xff)
	sp.putc((upFileSize >> 0)  & 0xff)
	tty.getc
	i = 0
	uploadFile.each_byte do |b|
		checksum += b
		sp.putc b
		i += 1
		#tty.printf("%2x", b)
	end
	tty.printf("%i Bytes written\n", i)
	tty.puts "Checksum"
	tty.puts((checksum & 0xff).to_s)
	tty.getc
	tty.puts "Run"
	sp.putc 'g'	
	sp.putc 0xB0
	sp.putc 0x00
	sp.putc 0x00
	sp.putc 0x00
	tty.puts "Complete"
	while true do
	end
}
=begin
open("/dev/tty", "r+") { |tty|
  tty.sync = true
  Thread.new {
    while true do
      tty.printf("%c", sp.getc)
    end
  }
  while (l = tty.gets) do
    sp.write(l.sub("\n", "\r"))
  end
}
=end

uploadFile.close()
sp.close
