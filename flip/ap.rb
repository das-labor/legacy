#####################################################
#
# Classes File for ap specific methods
#

class APList
	attr_reader :path, :apHash, :info;

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

	def execute_local_all(cmd)
		ret = Hash.new;
		@apHash.each_value{ |ap|
			ret[ap.mac] = ap.execute_local(cmd);
		}
		return ret;
	end

	def execute_remote_all(cmd)
		ret = Hash.new;
		@apHash.each_value{ |ap|
			ret[ap.ip] = ap.execute_remote(cmd);
		}
		return ret;
	end

	#
	# informations bezug und extrahieren der information 
	# aus von den accesspoints nach /tftpboot geladenen informations
	# files
	#

	def gather_new(gatpath)
		last = Dir.pwd
		Dir.foreach("#{gatpath}") { |entry|
			if not (/\S{12}\.\w{7}\.\w{3}/ =~ entry) then next; end
			IO.foreach( "#{gatpath}/" + entry ) { |f|

				rex1 = /^sis0\:\s+((?:\S{12}))/
				rex2 = /^wi0\:\s+(?:\S{12})/
				rex3 = /^ath0\:\s+(?:\S{12})/
				rex4 = /^ath1\:\s+(?:\S{12})/

				if ( rex1.match(f) ) then 
					array = rex1.match(f)
					if not array.nil? then 
						debug("array0 " + array[0]);
						debug("array1 " + array[1]);
					end
					debug(">>1"+ f); 
				elsif ( rex2.match(f) ) then 
					debug(">>2"+ f); 
				elsif ( rex3.match(f) ) then
					 debug(">>3"+ f); 
				elsif ( rex4.match(f) ) then 
					debug(">>4"+ f); 
				end
			}
		}
	end

        def method_missing(sym,*args)
                @apHash.send sym,*args
        end
end


class AccessPoint 
	attr :statistics;
	attr_reader :ip, :hostname_line, :mac, :connected_clients, :client, :path;

	#
	# accesspoints initialisieren 
	#

	def initialize(path, mac)
		@mac = mac;
		@path = path + "/" + mac;
		read_hostname
	end

	def save_statistics
		File.open( @path + "/statistics.dump", "w" ) { |f|
			Marshal.dump(@statistics, f);
		}
	end

	def load_statistics
		File.open( @path + "/statistics.dump", "r" ) {
			@statistics = Marshal.load(f);
		}
	end

	#
	# aus lokaler information ip addresse des accesspoints extrahieren
	# und verfuegbar machen 
	#

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

	#
	# methode die von flipd.rd genutzt wird um von allen accesspoints 
	# die angegebene information zu beziehen und diese geparsed auszugeben.
	#

	def collect_statistics
		stat = Hash.new;
		ret = execute_remote( "ifconfig -a ; brconfig bridge0 ; pfctl -s label" );

		# extract connected clients
		wi0Clients = Array.new;
		ath0Clients = Array.new;
		ath1Clients = Array.new;

		lines = ret.split("\n");
		wi0Re  = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) wi0 .*flags=/;
		ath0Re = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) ath0 .*flags=/;
		ath1Re = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) vlan0 .*flags=/;
		lines.each { |line|
			wi0Clients.push  m[1] if (m = wi0Re.match(line))
			ath0Clients.push m[1] if (m = ath0Re.match(line))
			ath1Clients.push m[1] if (m = ath1Re.match(line))
		}

		stat["wi0Clients"]  = wi0Clients;
		stat["ath0Clients"] = ath0Clients;
		stat["ath1Clients"] = ath1Clients;


		stat["pf"] = Hash.new if stat["pf"].nil?
		pf = stat["pf"];

		# pf log 
		pfRe = /^<PF> (.+on.+) (\d+) (\d+) (\d+) (\d+) (\d+) (\d+) (\d+)$/;
		lines.each { |line|
			if (m = pfRe.match(line)) then
				if pf[m[1]].nil?  then
					pf[m[1]] = Hash.new 
					pf[m[1]]["eval"]      = 0;
					pf[m[1]]["pkt"]       = 0;
					pf[m[1]]["bytes"]     = 0;
					pf[m[1]]["pkt_in"]    = 0;
					pf[m[1]]["bytes_in"]  = 0;
					pf[m[1]]["pkt_out"]   = 0;
					pf[m[1]]["bytes_out"] = 0;
				end
				
				pf[m[1]]["eval"]      += m[2].to_i; 
				pf[m[1]]["pkt"]       += m[3].to_i;
				pf[m[1]]["bytes"]     += m[4].to_i;
				pf[m[1]]["pkt_in"]    += m[5].to_i;
				pf[m[1]]["bytes_in"]  += m[6].to_i;
				pf[m[1]]["pkt_out"]   += m[7].to_i;
				pf[m[1]]["bytes_out"] += m[8].to_i;
			end
		}
		return stat
	end

	#
	# erstellen des config.tgz das von den Clients in der Konfigurationsphase
	# geladen und ausgfuehrt wird.
	# 

	def build_config_tgz( dstpath )
		if !File.exists?( "#{dstpath}/#{@mac}" ) then
			 Dir.mkdir( "#{dstpath}/#{@mac}", 0755 )   # will only work with superuser privs
								   # Normal Umask for Directory creation 
		end
		if File.exists?( "#{dstpath}/#{@mac}/config.tgz" ) then
			File.rename( "#{dstpath}/#{@mac}/config.tgz", "#{dstpath}/#{@mac}/config.tgz.old" )
		end
		execute_local( "tar -cvzf #{dstpath}/#{@mac}/config.tgz ." );
			
	end


	#
	# ausfuehren lokaler kommandos auf einem uebergeben Pfad
	#

	def execute_local( cmd )
		last = Dir.pwd
		Dir.chdir(@path)
		ret = `#{cmd}`
		Dir.chdir(last)
		return ret
	end

	#
	# ausfuehren von commandos auf einer soekris Boc
	# 

	def execute_remote( cmd )
		return `ssh -l root -i soekris.dsa #{ip} "#{cmd}"`;
	end

private

end
