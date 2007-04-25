class Foo
  def method_missing(name, *args)
    if name.to_s =~ /(.*)=$/ && args[0].is_a?(Proc)
      self.class.instance_eval do
        define_method($1) do |*a|
          args[0].call(*a)
        end
      end
    else
      super
    end
  end
  def test
    p "ich"
  end
  def change
    self.test = proc { p "du" }
  end
end

f = Foo.new
f.test
f.change
f.test

#f = Foo.new
#f.greet = lambda {|t| p "Hello #{t}!"}
#f.greet "Lucas Carlson" # => Hello Lucas Carlson!
#f.greet = lambda {|t| p "hhhhhhhhhHello #{t}!"}
#f.greet "Lucas Carlson" # => Hello Lucas Carlson!

#j = Foo.new
#j.greet "World" # => Hello World!