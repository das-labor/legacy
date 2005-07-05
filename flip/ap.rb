


class APList
	attr_reader :path, :aps

	def initialize( path )
		@path = path;
		@aps = Hash.new;
		refresh
	end

	def refresh
		Dir.foreach(@path) { |entry| 
			# skip non MAC entries
			if not (/\S{6}/ =~ entry) then next; end

			ap = AccessPoint.new( @path+"/"+entry );
			aps[ap.mac] = ap;
		}
	end

        def method_missing(sym,*args)
                @aps.send sym,*args
        end
end


class AccessPoint 
	attr_reader :ip, :hostname_line

	def initialize(path)
		@path = path;
		puts "Building from #{path}";
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

	def mac
		return "112233445566";
	end

	def get_clients
		puts execute "w"
	end

	def info
		return get_clients
	end

private
	def execute( cmd )
		return `ssh #{ip} #{cmd}`;
	end
end


apList = APList.new( "tmp/aps" );

apHash = apList.aps;


apHash.each_value do  |ap|
	puts ap.info
end
