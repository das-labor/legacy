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

def wwwd_stats
  graph = Scruffy::Graph.new(:title => 'WWWD stats')
  graph.add :line, 'Total AP Found',    [9374,24958,88122, 228537]
  graph.add :line, 'No WEP Enabled', [6549, 17988, 59695,140890] 
  graph.add :line, 'WEP Enabled', [2825, 6970, 28427,87647]
  graph.point_markers = ['WWWD1', 'WWWD2', 'WWWD3', 'WWWD4']

  graph.render(:width => 600, :as => 'PNG', :to => 'scruffy_test1.png')
end    

sample
wwwd_stats