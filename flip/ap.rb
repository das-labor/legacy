


class APList
	attr_reader :path, :ap_array
	def initialize( path )
		@path = path;
		@ap_array = Array.new;
		refresh
	end

	def refresh
		Dir.foreach(@path) { |entry| 
			# skip non MAC entries
			if not (/\S{6}/ =~ entry) then next; end

			aAp = AccessPoint.new( @path+"/"+entry );
		}
	end
end


class AccessPoint 
	def initialize(path)
		puts "Building from #{path}";
	end
end


apl = APList.new( "tmp/aps" );

