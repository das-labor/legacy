require 'rubygems'
require 'dm-core'
require 'dm-migrations'
require 'dm-aggregates'

#XXX this is too dangerous for production
DATABASE = "#{Dir.pwd}/imported.sqlite"

class Transaction
  include DataMapper::Resource
  
  property :id, Serial
  
  property :csvline, Text
  
  property :exportdate, Date
  property :exportamount, Integer #Eurocent
  property :exportdescr, Text
  
  def to_yaml
    attributes.to_yaml
  end
end

DataMapper.finalize

def setup_db
  DataMapper::Logger.new($stderr, :warn)
  
  #This will give a deprecation warning
  #See http://groups.google.com/group/datamapper/browse_thread/thread/51d6c6414b177273/e59821aba4bafffc?show_docid=e59821aba4bafffc
  DataMapper.setup(:default, "sqlite://#{DATABASE}")
  
  DataMapper.auto_upgrade!
end

def info(msg)
  $stderr << "Info: #{msg}\n"
end

def warning(msg)
  $stderr << "Warning: #{msg}\n"
end

def abort(msg)
  $stderr << "Error: #{msg}\nAborting.\n"
  exit 1
end