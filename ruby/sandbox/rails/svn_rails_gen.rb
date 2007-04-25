#!/usr/bin/ruby
# ... -d -- Use for debuging
 
require 'yaml'
 
class RubyRails
	def initialize
		@LOG_FILE = 'ruby_rails.log'
		@CONFIG_FILE = 'rr_conf.yml'
		@SQL_FILE = 'ruby_rails_sql.sql'
	end
 
	def start
		# The welcome line
		puts "This is the Ruby Rails! Ruby on Rails is a web devlopment framework, MySQL is a database system for storing information, and Ruby Rails is a quick-start script, that when given information, will generate the Ruby On Rails app and MySQL databases, then connect those together, all for you! Besides that, nothing will happen."
 
		print "First, please enter the name of your new application: "
		@app_name = gets.chop
 
		  get_config unless File.exists?(@CONFIG_FILE)
		puts "Loading configuration"
		  load_config
		puts "I'm now making your app, #{@app_name}"
 
		puts "Finished creating your application"
		puts "I need you password to the root account in MySQL"
		  generate_app
 
		puts "I'm done. Everything has been made. Have fun making dynamic web apps!"
	end
 
	def get_config
		puts "Configuration file not found. Initiliazing configuration"
		puts "Please enter the name of the account that will be running the WEBrick server: "
		@acc_name = gets.chop
		puts "Please enter the name of the MySQL account that Rails will be using in the production enviorment: "
		@rails_name = gets.chop
		puts "Please enter the password to #{@rails_name}: "
		@rails_pwd = gets.chop
		puts "Configuration done. Creating config file."
		f = File.open(@CONFIG_FILE, "w") do |f|
			f.write("# Ruby Rails config\n# This is the configuration for the Ruby Rails script\n# It generates a Rails app, for you! Just provide the name of the app\n\n")
			f.write("account: #{@acc_name}\n")
			f.write("rails1: #{@ralis_name}\n")
			f.write("rails2: #{@rails_pwd}\n")
			f.close
		end
	end
 
	def load_config
		@config = YAML.load_file("rr_conf.yml")
		@acc_name = @config["account"]
		@rails_name = @config["rails1"]
		@rails_pwd = @config["rails2"]
	end
 
	def log_cmd( command )
		cmd = IO.popen(command)
		f = File.open(@LOG_FILE, "a") do |f|
			f.write(cmd.readlines) # logging the output
			f.close # done logging
		end
		cmd.close # don't need the pipe, so end it
	end
 
	def write_sql
		f = File.open(@SQL_FILE, "w") do |f|
			f.write("create database #{@app_name}_development;")
			f.write("create database #{@app_name}_test;")
			f.write("create database #{@app_name}_production;")
			f.write("grant all on #{@app_name}_development.* to '#{@acc_name}'@'localhost';")
			f.write("grant all on #{@app_name}_test.* to '#{@acc_name}'@'localhost';")
			f.write("grant all on #{@app_name}_development.* to '#{@rails_name}'@'localhost' identified by '#{@rails_pwd}';")
			f.close
		end
	end
 
	def drop_sql()
		File.delete(@SQL_FILE)
	end
 
	def generate_app
		write_sql
		log_cmd("rails #{@app_name}")
		log_cmd("mysql -u root -p  < ruby_rails_sql.sql")
		drop_sql
		generate_db_yml
	end
 
	def generate_db_yml
		File.delete(@app_name + "/config/database.yml")
		file = <<END_OF_STRING
# MySQL (default setup).  Versions 4.1 and 5.0 are recommended.
#
# Install the MySQL driver:
#   gem install mysql
# On MacOS X:
#   gem install mysql -- --include=/usr/local/lib
# On Windows:
#   There is no gem for Windows.  Install mysql.so from RubyForApache.
#   http://rubyforge.org/projects/rubyforapache
#
# And be sure to use new-style password hashing:
#   http://dev.mysql.com/doc/refman/5.0/en/old-client.html
development:
  adapter: mysql
  database: #{@app_name}_development
  username: #{@acc_name}
  password:
  host: localhost
 
# Warning: The database defined as 'test' will be erased and
# re-generated from your development database when you run 'rake'.
# Do not set this db to the same as development or production.
test:
  adapter: mysql
  database: #{@app_name}_test
  username: #{@acc_name}
  password:
  host: localhost
 
production:
  adapter: mysql
  database: #{@app_name}_production
  username: #{@rails_name}
  password: #{@rails_pwd}
  host: localhost
END_OF_STRING
		f = File.open(@app_name + "/config/database.yml", "w") do |f|
			f.write(file)
			f.close
		end
	end
end
 
ruby = RubyRails.new
ruby.start