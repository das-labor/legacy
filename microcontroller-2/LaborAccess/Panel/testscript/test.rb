#!/usr/bin/env ruby

require "serialport.so"
require "optparse"

port_baud     = 4800
port_path     = "/dev/ttyUSB0"
verbose       = false
command       = nil

$port          = nil

class MySerialPort < SerialPort
	def puti (integer)
		self.putc(integer/256)
		self.putc(integer%256)
	end
	def putc c
		if( @mode != "t") then
			@mode = "t"
			p "t"
		end
		p c
		super c
	end
	def getc
		if( @mode != "r") then
			@mode = "r"
			p "r"
		end
		c = super
		p c
	end

end


class Message
	attr_accessor :id
	attr_accessor :data
	def initialize
		@id = nil
		@data = []
	end
	def from_array a
		id = a.shift
		data = a
	end
	def size
		@data.size
	end
end

class FooPort
	def initialize(port)
		@port = port
	end
	def read_byte
		c = @port.getc
		if c == 0x42 then
			c = @port.getc
			c ^= 0x55
		elsif c == 0x23 then
			nil
		else
			c
		end
	end
	def send_byte b
		if b == 0x23 || b == 0x42 then
			@port.putc 0x42
			b ^= 0x55
		end
		@port.putc b
	end
	def get_message
		m = Message.new
		while ((c = self.read_byte) == nil)
		end
		m.id = read_byte
		c.times do |x|
			m.data[x] = read_byte	
		end		
		m
	end
	def send_message(m)
		if m.class == Array then
			mes = Message.new
			mes.from_array m
			m = mes
		end
		p m
		@port.putc 0x23
		send_byte m.size
		send_byte m.id
		m.size.times do |x|
			send_byte m[x]
		end
		
	end
end


opts = OptionParser.new do |o|
	o.banner = "Usage: upload.rb [options] <file.srec>"

	o.on( "-b", "--baud BAUDRATE", Integer, 
			"Serial port baudrate (default: #{port_baud})" ) do |baud|
		port_baud = baud
	end

	o.on( "-s", "--serial SERIALPORT", 
	         "Path to serial port (default: #{port_path})" ) do |port|
		port_path = port
	end

	o.on( "-v", "--verbose", 
	        "Be verbose and show serial I/O" ) do
		verbose = true
	end

	o.on_tail( "-h", "--help", "Display this help message" ) do
		puts o
		exit 0
	end
end

def hexdump(bytearray)
	count = 0
	bytearray.each do |x|
		printf("%02X ", x)
		count += 1
		if count == 16 then
			count = 0
			printf ("\n")
		end
	end
	printf ("\n")
end




#############################################################################
# Check arguments, open serial port and file
begin
	opts.parse!(ARGV)

	$port = MySerialPort.new(port_path, port_baud, 8, 1, MySerialPort::NONE)
	raise "Could not open serial port." if not $port;

rescue => e
	STDERR.puts "\nERROR: #{e.message}"
	STDERR.puts
	STDERR.puts opts
	STDERR.puts
	exit 1
end


#############################################################################
# 
begin

	foo = FooPort.new($port)

	while(1)
		m = foo.get_message

		p m

		case m.data[0]
			#reset
			when 0xff then foo.send_message([0x00, 0x00]) #send o.k.

		end

	end

end
