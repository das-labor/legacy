#!/usr/bin/ruby

require 'optparse'
require 'date'
require 'socket'
require 'CanPacket'

class LapDevice
	attr_reader :type
	attr_reader :name
	def initialize(name)
		@name = name
	end
	def request_state()
	end
	def ==(n)
		n == @name
	end
	def to_s
		@name + " " + self.class.to_s + "\r\n"
	end
end

class SwitchDevice < LapDevice
	attr_reader :lap_addr
	attr_reader :num
	#format for args is name, lap_addr, num
	def initialize(str)
		@watching_servers = []
		name, args =  str.split(' ', 2)
		lap_addr, num = args.split(',')
		@lap_addr = lap_addr.to_i
		@num = num.to_i

		super(name)
		@state = 0
	end
	def request_state()
		can = CanPacket.new()
		can.set_lap(0, 0x18, self.lap_addr, 0x18)
		can.data = [2, self.num]
		can.transmit()
	end
	def set(arg)
		newstate = arg.to_i
		if (newstate == 0) || (newstate == 1) then
			if @state != newstate then
				can = CanPacket.new()
				can.set_lap(0, 0x18, @lap_addr, 0x18)
				can.data = [1, @num, newstate]
				can.transmit()
				state_changed(newstate)
				"o.k.\r\n"
			else
				"state not changed\r\n"
			end
		end
	end
	def toggle
		set(1-@state)
	end
	def get
		"info " + @name + " " + @state.to_s
	end
	def request(server, command, argstr)
		if argstr then
			args = argstr.split()
		end
		server.message case command
			when "set" 		then set(args[0])
			when "toggle"	then toggle
			when "get"		then get
			else			"error - no such command\r\n"
		end
	end
	def watch(server, args)
		@watching_servers << server
	end
	def state_changed(newstate)
		if @state != newstate then
			@state = newstate
			@watching_servers.each do |serv|
				if serv.alive then
					serv.message(get)
				else
					@watching_servers.delete(serv)
				end
			end
		end
	end
	def to_s
		super.chomp + " " + @lap_addr.to_s + "," + @num.to_s + " state=" + @state.to_s + "\r\n"
	end 
end


class PlantDevice < LapDevice
	attr_reader :lap_addr
	#format for args is name lap_addr
	def initialize(str)
		@watching_servers = []
		name, args =  str.split(' ', 2)
		@lap_addr = args.to_i

		super(name)
		@humidity = 0
	end
#	def set(arg)
#		newstate = arg.to_i
#		if (newstate == 0) || (newstate == 1) then
#			if @state != newstate then
#				can = CanPacket.new()
#				can.set_lap(0, 0x18, @lap_addr, 0x18)
#				can.data = [1, @num, newstate]
#				can.transmit()
#				state_changed(newstate)
#				"o.k.\r\n"
#			else
#				"state not changed\r\n"
#			end
#		end
#	end
	def get_humidity
		@name + " humidity " + @humidity.to_s
	end
	def get
		get_humidity()
	end
	def request(server, command, argstr)
		if argstr then
			args = argstr.split()
		end
		server.message case command
#			when "set" 		then set(args[0])
			when "get"		then get
			else			"error - no such command\r\n"
		end
	end
	def humidity_changed(newhumidity)
		@humidity = newhumidity		
	end
	def to_s
		super.chomp + " " + @lap_addr.to_s + "," + @num.to_s + " humidity=" + @humidity.to_s + "\r\n"
	end 
end







$devices = []

def read_devices(filename)
	file = File.new(filename, "r")
	file.each_line do |line|
		line.chomp!
		arr = line.split(' ', 2)
		if arr.size < 2 then break end
		$devices << case arr[0]
			when "Switch" then SwitchDevice.new(arr[1])
			when "Plant"  then PlantDevice.new(arr[1])

			else nil
		end
	end
end

$servers = []

class LapServer
	attr_accessor :sock
	attr_reader   :alive
	def initialize(sock)
		@sock = sock
		@alive = true
		@messages = []
		begin
			@message_thread = Thread.new{
				while 1 do
					msg = @messages.shift 
					if msg then
						sock.puts(msg)
					else
						Thread.stop
					end
				end
			}
			@server_thread = Thread.new{
				begin
					while 1
						line = sock.gets
						if line == nil then break end
						puts line
						line.chomp!
						command, name, args = line.split(' ',3);
			
						if command == "show" then
							show(name, args)
						elsif command == "exit" then
							sock.puts("bye!\r\n")
							break
						else
							i = $devices.index(name)
								if i then
								if command == "watch" then
									$devices.at(i).watch(self, args)
									$devices.at(i).request(self, "get", args)
								else 
									$devices.at(i).request(self, command, args)
								end
								else
							sock.puts("no such device\r\n")
							end
						end
					end
					self.destruct
				rescue Errno::ECONNRESET
					print "Errno:ECONRESET rescued\r\n"
					self.destruct
				end
			}
		end
	end
	def destruct
		@alive = false
		$servers.delete(self)
		@sock.close
		@message_thread.kill
	end
	def show(name, argstr)
		 message case name
			when "devices" then $devices.to_s
			when "servers" then $servers.to_s
			when "threads" then Thread.list.to_s
		end
	end
	def message(str)
		@messages << str
		@message_thread.run
	end
	def to_s
		@sock.peeraddr[2] + ":" + @sock.peeraddr[1].to_s + "\r\n" +\
		"alive: " + @alive.to_s + "\r\n" +\
		"server_thread:  " + @server_thread.to_s + "\r\n" +\
		"message_thread: " + @message_thread.to_s + "\r\n" +\
		"messages: " + @messages.to_s + "\r\n\r\n"
	end
end

##############################################################################
# Utility Functions

def debug(level, msg)
	if $VerboseLevel >= level then
		$LogHandle.puts "#{Time.now.strftime( "%Y-%m-%d %H-%M-%S" )} - #{msg}"
		$LogHandle.flush
	end
end

def error(msg)
	debug(0, msg )
end

##############################################################################
# Main

$Port         = 2343
$LogHandle    = STDERR
$VerboseLevel = 3

opts = OptionParser.new do |o|
	o.banner = "lapd -- Labor Control Daemon"
	o.separator ""

	o.on( "-v", "--verbose LEVEL", Integer,
			"Set verbosity to LEVEL (default: #{$VerboseLevel})" ) do |level|
		$VerboseLevel = level
	end

	o.on( "-l", "--logfile FILENAME",
			"Path to logfile (default: STDOUT)" ) do |logfile|
		$LogHandle = File.new( logfile, "a" )
	end

	o.on( "-p", "--port PORT", Integer, 
			"TCP/IP Port to listen on (default: #{$Port})" ) do |port|
		$Port = port
	end

	o.on( "-h", "--help",
			"Display help message" ) do 
		puts
		puts opts
		puts
		exit 0
	end
end

begin
	opts.parse!( ARGV )
rescue => e
	STDERR.puts 
	STDERR.puts "Fatal error: #{e}"
	STDERR.puts 
	STDERR.puts opts
	exit 1
end


Thread.abort_on_exception = true

#begin
#	debug 5, "Reading devices.lst..."
#	read_devices("devices.lst")
#
#	$devices.each do |d|
#		debug 7, d
#	end
#rescue => e 
#	error "Error reading devices.lst: #{e}"
#	exit 2
#end

#server_thread = Thread.new do 
#	begin
#		debug 5, "Accepting connections on port #{$Port}..."
#
#		socket = TCPServer.new( '' , $Port )
#		while true do
#			$servers << LapServer.new(socket.accept)
#		end
#	rescue => e
#		error "Error in server_thread: #{e}"
#		exit 2
#	end
#end
		
$cand_socket = TCPSocket.new('192.168.0.1', 2342)
cand_thread = Thread.new do
	begin
		debug 5, "Connecting to cand..."

		while true do
			can = CanPacket.new
			can.read_from_socket
			debug 7, "Recieved CanPacket"
			
			if (can.dest_port == 0x18) && (can.data[0] == 0) then
				$devices.each do |device|
					if (device.lap_addr == can.src_addr) && (device.num == can.data[1]) then
						device.state_changed(can.data[2])
					end
				end
			end
			if (can.dest_port == 0x26) then
				$devices.each do |device|
					if (device.lap_addr == can.src_addr) then
						device.humidity_changed(can.data[0])
					end
				end
			end
		end
	rescue => e
		error "Error in cand_thread: #{e}"
		exit 2
	end
end

$devices.each do |device|
	device.request_state()
end

server_thread.join

