#####################################################
#
# Classes File for ap specific methods
#

require 'ftools'


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

			if @apHash.has_key?(entry) then
				@apHash[entry].refresh;
			else
				ap = AccessPoint.new(@path, entry);
				@apHash[ap.mac] = ap;
			end
		}
	end

	def execute_local_all(cmd)
		ret = Hash.new;
		@apHash.each_value{ |ap|
			next if !ap.enabled?
			ret[ap.mac] = ap.execute_local(cmd);
		}
		return ret;
	end

	def execute_remote_all(cmd)
		ret = Hash.new;
		@apHash.each_value{ |ap|
			next if !ap.enabled?
			ret[ap.ip] = ap.execute_remote(cmd);
		}
		return ret;
	end

	#
	# informations bezug und extrahieren der information 
	# aus von den accesspoints nach /tftpboot geladenen informations
	# files
	# An expected file looks like this: 
	# sis0: 000024c341e4
	# ath0: 00026f20f9b5
	# wi0: 00026f34e3df

	def gather_new(gatpath, templates)
		lastd = Dir.pwd
		Dir.foreach("#{gatpath}") { |entry|
			refresh
			array = Array.new
				@apHash.each_value{ |ap|
					a, b, c, d = ap.ip.split(".")
					intip = a.to_i * 16777216 + b.to_i * 65536 + c.to_i * 256 + d.to_i
					array.push(intip)
				}
			if array[0]==nil then
				intip = 167772161 
				array.push(intip)
			end
			array.sort!
			last = array.last
			new = last + 1
			d = new % 256
			c = (new >> 8) % 256
			b = (new >> 16) % 256
			a = new >> 24
			dottetip = a.to_s + "." + b.to_s + "." + c.to_s + "." + d.to_s
			regx = /\S{12}\.\w{7}\.\w{3}/
			if ( regx.match(entry) ) then 
				IO.foreach( "#{gatpath}/" + entry ) { |f|

					sis0 = /^sis0\:\s+((?:\S{12}))/
					wi0 = /^wi0\:\s+(?:\S{12})/
					ath0 = /^ath0\:\s+(?:\S{12})/
					ath1 = /^ath1\:\s+(?:\S{12})/

					if ( sis0.match(f) ) then 
						array = sis0.match(f)
						@info = array[1]
						if not array.nil? then 
							Dir.mkdir( "#{path}/#{info}", 0755 );
							Dir.mkdir( "#{path}/#{info}/etc", 0755 );
							File.open( "#{templates}/etc/hostname.sis0", "r" ) do |read|
								File.open( "#{path}/#{info}/etc/hostname.sis0" , "w" ) do |write|
									line = read.readline
									line.sub!(/IP-ADDRESS/ , dottetip )
									write.puts(line)
								end
							end

							File.copy( "#{templates}/etc/hostname.vlan0", "#{path}/#{info}/etc/" )
							File.copy( "#{templates}/etc/rc.local", "#{path}/#{info}/etc/" )
							File.copy( "#{templates}/etc/motd", "#{path}/#{info}/etc/" )
							Dir.mkdir( "#{path}/#{info}/etc/ssh", 0755 )
							`ssh-keygen -q -t rsa -f #{path}/#{info}/etc/ssh/ssh_host_dsa_key -N ''`
							`ssh-keygen -q -t rsa -f #{path}/#{info}/etc/ssh/ssh_host_rsa_key -N ''`
							`ssh-keygen -q -t rsa1 -f #{path}/#{info}/etc/ssh/ssh_host_key -N ''`
							Dir.mkdir( "#{path}/#{info}/root" , 0700 )
							Dir.mkdir( "#{path}/#{info}/root/.ssh/" , 0700 )
							File.copy( "#{templates}/soekris.dsa.pub", "#{path}/#{info}/root/.ssh/authorized_keys" ); 
						end
					elsif ( wi0.match(f) ) then 
						File.copy( "#{templates}/etc/hostname.wi0", "#{path}/#{info}/etc/" );
						File.copy( "#{templates}/etc/bridgename.bridge0", "#{path}/#{info}/etc/" );
					elsif ( ath0.match(f) ) then
						File.copy( "#{templates}/etc/hostname.ath0", "#{path}/#{info}/etc/" );
						File.copy( "#{templates}/etc/bridgename.bridge1", "#{path}/#{info}/etc/" );
					elsif ( ath1.match(f) ) then 
						File.copy( "#{templates}/etc/hostname.ath1", "#{path}/#{info}/etc/" );
						File.copy( "#{templates}/etc/bridgename.bridge2", "#{path}/#{info}/etc/" );
					end
				}
				File.delete( "#{gatpath}/" + entry )
			else
				next;
			end
		}
		Dir.chdir(lastd)
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

		up(false);
		refresh
	end

	def refresh
		read_hostname
	end

	def save_statistics
		puts "dsa #{@statistics}"
		File.open( @path + "/statistics.dump", "w" ) { |f|
			f.write( Marshal.dump(@statistics) );
		}
	end

	def load_statistics
		File.open( @path + "/statistics.dump", "r+" ) { |f|
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

	def enable(val)
		if val then
			File.open( @path + "/enabled", "w" ) { };
		else
			File.unlink( @path + "/enabled" ) if enabled?;
		end
	end

	def enabled?
		return File.exists?( @path + "/enabled" ) && File.exists?( "/tftpboot/" + @mac + "/config.tgz" );
	end

	def up(val)
		if val then
			File.open( @path + "/up", "w" ) { };
		else 
			File.unlink( @path + "/up" ) if up?;
		end
	end

	def up?
		return File.exists?( @path + "/up" );
	end

	def status_line
			
			str =  enabled? ? "  " : "!E";
			str += up? ? "  " : "!U";

			str += " MAC: #{@mac} IP: #{@ip}";
			return str;
	end

	def info
		return clients; 
	end

	def clients
		str = "wi0 Clients\n"
			@statistics["wi0Clients"].each { |c|
			str += c + "\n";
		}

		str += "ath0 Clients\n"
		@statistics["ath0Clients"].each { |c|
			str += c + "\n";
		}

		str += "ath1 Clients\n"
		@statistics["ath1Clients"].each { |c|
			str += c + "\n";
		}

		return  str;
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
		wi0Re  = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) (wi0) .*flags=/;
		ath0Re = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) (ath0) .*flags=/;
		ath1Re = /^\s*(\S\S:\S\S:\S\S:\S\S:\S\S:\S\S) (ath1) .*flags=/;
		lines.each { |line|
			m = wi0Re.match(line);
			wi0Clients.push(m[1]) if !m.nil?;

			m = ath0Re.match(line)
			ath0Clients.push(m[1]) if !m.nil? 

			m = ath1Re.match(line)
			ath1Clients.push(m[1]) if !m.nil?
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

		@statistics = stat;

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
		return `ssh -l root -i config/templates/soekris.dsa #{ip} "#{cmd}"`;
	end

private

end
