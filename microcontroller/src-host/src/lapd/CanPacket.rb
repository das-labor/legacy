
class CanPacket
	attr_reader :src_addr
	attr_reader :dest_addr
	attr_reader :src_port
	attr_reader :dest_port

	attr_reader :id
	attr_accessor :data

	def initialize
		@src_addr = 0
		@dest_addr = 0
		@src_port = 0
		@dest_port = 0

		@id = 0x1fffffff
		@data = Array.new
		@dlc = 0
	end

	def id_to_lap
		@src_addr = (@id>>8) & 0xff
		@dest_addr = @id & 0xff
		@src_port = (@id>>23) & 0x3f
		@dest_port = ((@id>>17) & 0x30) | ((@id>>16) & 0x0f)
	end

	def lap_to_id
		@id = (@src_addr<<8) | @dest_addr | (@src_port<<23) | ((@dest_port&0x30)<<17) | ((@dest_port&0x0f)<<16)
	end

	def read_from_socket
		s = $cand_socket
		c = s.getc
		
		bytes = Array.new
		(c+1).times do
			bytes << s.getc
		end

		if bytes[0] == 0x11 then
			@id = bytes[1] | (bytes[2] << 8) | (bytes[3] << 16) | (bytes[4] << 24)
			@dlc = bytes[5]
			@data = bytes[6..(5+@dlc)]
			self.id_to_lap()
		end
	end

	def transmit
		s = $cand_socket
		@dlc = @data.size
		s.putc(@dlc+5)
		s.putc(0x11)
		s.putc(@id&0xff)
		s.putc((@id>>8)&0xff)
		s.putc((@id>>16)&0xff)
		s.putc((@id>>24)&0xff)
		s.putc(@dlc)
		@dlc.times do |i|
			s.putc(@data[i])
		end
	end

	def set_lap(src_addr, src_port, dest_addr, dest_port)
		@src_addr = src_addr.to_i
		@dest_addr = dest_addr.to_i
		@src_port = src_port.to_i
		@dest_port = dest_port.to_i
		self.lap_to_id
	end
end


