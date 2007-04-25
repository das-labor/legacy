class Post < ActiveRecord::Base
  def self.save(person)
      File.open("pictures/#{person['name']}/picture.jpg", "w") { |f| f.write(person['picture'].read) }
  end
end