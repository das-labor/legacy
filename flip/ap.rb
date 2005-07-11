
class APList
	attr_reader :path, :apHash

	def initialize(path)
		@path = path 
		@apHash = Hash.new;
		refresh
	end

	def refresh
		Dir.foreach(@path) { |entry| 
			# skip non MAC entries
			if not (/\S{6}/ =~ entry) then next; end

			ap = AccessPoint.new(@path, entry);
			apHash[ap.mac] = ap;
		}
	end

        def method_missing(sym,*args)
                @apHash.send sym,*args
        end
end


class AccessPoint 
	attr_reader :ip, :hostname_line, :mac, :connected_clients, :client

	def initialize(path, mac)
		@mac = mac;
		@path = path + "/" + mac;
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
		return execute_remote( "brconfig bridge0 " ) 
	end
	
	def get_basic_information
		return execute_remote( "hostname; uptime;" );
	end

	def info
		return get_basic_information;
	end

	def clients
		return get_clients;
	end

	def execute_local( cmd )
		last = Dir.pwd
		Dir.chdir(@path)
		ret = `#{cmd}`
		Dir.chdir(last)
		return ret
	end
private

	def execute_remote( cmd )
		return `ssh -l root -i soekris.dsa #{ip} "#{cmd}"`;
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
