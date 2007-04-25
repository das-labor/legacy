class Array
  def /(key)
    self[key]
  end
end
class Hash
  def /(key)
    self[key]
  end
end
people = {:player => {:name => {:first => "Peter Hans",:second => "Klaus Wurst"}}}

p people/:player/:name/:first
p people/:player/:name/:second