require 'rubygems'
require 'active_record'
require 'simple-rss'
require 'open-uri'
require 'twitter'

#twitter account to post to
twitter_email = "yourtwitteremail@bla.com"
twitter_password = "secret"

#rss feed to post
rss_url = "http://yoursite.com/index.xml"
rss_user_agent = "http://twitter.com/yourbot"

#sqlite db
path_to_sqlite_db = "/PATH/TO/db.sqlite"


ActiveRecord::Base.logger = Logger.new(STDERR)
ActiveRecord::Base.colorize_logging = false

ActiveRecord::Base.establish_connection(
    :adapter => "sqlite3",
    :dbfile  => path_to_sqlite_db
)

#uncomment this section the first time to create the table
#
#ActiveRecord::Schema.define do
#    create_table :item do |table|
#        table.column :title, :string
#        table.column :link, :string
#    end
#end

class Item < ActiveRecord::Base
  def to_s
    "#{self.title[0..(130-self.link.length)]} - #{self.link}"
  end
end

#run the beast
rss_items = SimpleRSS.parse open(rss_url ,"User-Agent" => rss_user_agent)

for item in rss_items.items
  Item.transaction do
    unless existing_item = Item.find(:all, :conditions => ["link=?", item.link]).first
      twitter ||= Twitter::Base.new(twitter_email, twitter_password)
      new_item = Item.create(:title => item.title, :link => item.link) 
      twitter.post(new_item.to_s)
    end
  end
end
