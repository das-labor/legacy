# http://blogs.pragprog.com/cgi-bin/pragdave.cgi/Tech/Ruby/Connections.rdoc
# http://wiki.rubyonrails.com/rails/pages/HowToUseActiveRecordOutsideRails
# http://wiki.rubyonrails.com/rails/pages/HowtoUseMultipleDatabases
# http://www.chwhat.com/articles/2005/10/01/multiple-databases-on-rails

# CREATE TABLE 'links' ('id' INTEGER PRIMARY KEY NOT NULL,'title' VARCHAR(255) DEFAULT NULL,'url' VARCHAR(255) DEFAULT NULL);

require 'rubygems'
require 'active_record'

def connect_sqlite3
  ActiveRecord::Base.establish_connection({
        :adapter => "sqlite3", 
        :dbfile => "database.sqlite3"
  })
end

def connect_mysql
  ActiveRecord::Base.establish_connection({
        :adapter  => "mysql", 
        :database => "mydb",
        :socket   => "/tmp/mysql.socket",
        :username => "mydb_user",
        :password => "foo" 
  })
end

def connect(environment)
  conf = YAML::load(File.open(File.dirname(__FILE__) + '/../config/database.yml'))
  ActiveRecord::Base.establish_connection(conf[environment])
end

class ActiveR < ActiveRecord::Base
  establish_connection({
    :adapter => "sqlite3", 
    :dbfile => "database.sqlite3"
   })
end
class ActiveB < ActiveRecord::Base
  establish_connection({
    :adapter => "sqlite3", 
    :dbfile => "database.sqlite3"
   })
end

class Link < ActiveR;end
class Lin < ActiveB;set_table_name("links");end

#$LOAD_PATH << File.expand_path(File.dirname(__FILE__) + '/../models/')

# require '/path/to/rails/app/config/environment' for simple rails script