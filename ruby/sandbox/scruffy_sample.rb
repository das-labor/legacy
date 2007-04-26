require 'rubygems'
require 'scruffy'

def sample
  graph = Scruffy::Graph.new(:title => "Monthly Profits", :theme => Scruffy::Themes::RubyBlog.new)

  graph.add(:line, 'John', [100, -20, 30, 60])
  graph.add(:line, 'Sara', [120, 50, -80, 20])

  graph << Scruffy::Layers::Line.new(:title => 'John', :points => [100, -20, 30, 60])
  graph << Scruffy::Layers::Line.new(:title => 'Sara', :points => [120, 50, -80, 20])

  graph.render(:width => 600, :as => 'PNG', :to => 'scruffy_test.png')
end