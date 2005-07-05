PATH="tmp/aps"

class APList
	attr_reader :path, :apHash

	def initialize()
		@path = PATH;
		@apHash = Hash.new;
		refresh
	end

	def refresh
		Dir.foreach(@path) { |entry| 
			# skip non MAC entries
			if not (/\S{6}/ =~ entry) then next; end

			ap = AccessPoint.new(entry);
			apHash[ap.mac] = ap;
		}
	end

        def method_missing(sym,*args)
                @apHash.send sym,*args
        end
end


class AccessPoint 
	attr_reader :ip, :hostname_line, :mac

	def initialize(mac)
		@mac = mac;
		@path = PATH + "/" + mac;
		read_hostname
	end

	def read_hostname
		File.open( "#{@path}/etc/hostname.sis0" ) do |f|
			@hostname_line = f.readline

			rx = /^inet\s+((?:\d{1,3}\.){3}\d{1,3})/
			array = rx.match( @hostname_line );
			if not array.nil? then
				@ip = array[1]
			else 
				puts "Could not parse IP";
			end
		end
	end

	def get_clients
		return execute( "w; ifconfig -a " );
	end

	def info
		return get_clients;
	end

private
	def execute( cmd )
		return `ssh #{ip} "#{cmd}"`;
	end
end

# 
# apList = APList.new( "tmp/aps" );
# 
# apHash = apList.apHash;
# 
# 
# apHash.each_value do  |ap|
# 	puts ap.info
# end
