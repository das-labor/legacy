#!/usr/bin/env ruby

require "serialport.so"
require "optparse"

port_baud     = 115200
port_path     = "/dev/ttyUSB0"
verbose       = false
command       = nil
outfile       = nil

$port          = nil

class MemmoryRange
	attr_accessor :start
	attr_accessor :ending
	attr_accessor :value
	def initialize
		@start = 0
		@ending = 0
	end
	def set(arg)
		arr = arg.split(',', 2)
		arg = arr[0]
		komma = arr[1]
		arr = arg.split('-', 2)
		@start = Integer(arr[0])
		if arr[1] then
			@ending = Integer(arr[1])
		else
			@ending = @start+1
		end
		if komma then
			@value = Integer(komma)
		end
	end
	def to_s
		"0x" + @start.to_s(16) + "-" + "0x" + @ending.to_s(16)
	end
	def size
		ending-start
	end
end

class MySerialPort < SerialPort
	def puti (integer)
		self.putc(integer/256)
		self.putc(integer%256)
	end
end


$dumpRange = MemmoryRange.new
$writeRange = MemmoryRange.new

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

	o.on( "-d", "--dump BEGIN-END", 
			"Dump a memmory range") do |range|
		command = "dump"
		$dumpRange.set(range)
	end

	o.on( "-w", "--write ADDRESS,VALUE",
			"Write value to given Address") do |parm|
		command = "write"
		$writeRange.set(parm)
	end

	o.on( "-o", "--outfile OUTFILE",
			"set File to dump to") do |file|
		outfile = file
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

	raise "No command given" if command == nil;

	$port = MySerialPort.new(port_path, port_baud, 8, 1, MySerialPort::NONE)
	raise "Could not open serial port." if not $port;

rescue => e
	STDERR.puts "\nERROR: #{e.message}"
	STDERR.puts
	STDERR.puts opts
	STDERR.puts
	exit 1
end


def dump
	$port.putc('d');
	$port.puti($dumpRange.start)
	$port.puti($dumpRange.ending)
	
	mem = Array.new

	$dumpRange.size.times do |i|
		mem[i] = $port.getc
	end

	hexdump(mem)

end

def write
	$port.putc('w');
	$port.puti($writeRange.start)
	$port.puti($writeRange.ending)
	
	$writeRange.size.times do |i|
		$port.putc($writeRange.value)
	end


end

#############################################################################
# 
begin
	case command
		when "dump" then dump
		when "write" then write
	end


end
