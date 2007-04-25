# Higher order messaging
# Implemented by Emil Lundström
#
# Inspirated by an implementation by Nat Pryce, 
# See: http://nat.truemesh.com/archives/000535.html

# A higher order message object takes a Proc when constructed. This proc
# is stored in an instance variable, and is called every time method_missing
# is invoked.
class HigherOrderMessage
  instance_methods.each { |method| undef_method(method) unless method =~ /__(.+)__|inspect|method/ }
  
  def initialize(&proc)
    @proc = proc
  end

  def method_missing(id, *args)
    @proc.call(id, *args)
  end
end

# Adds some methods to Enumerable.
module Enumerable
  # Passes a method to each element in this collection.
  #
  # $ array.do_each.do_something(arg)
  #   ... is equivalent to
  # $ array.each { |v| v.do_something(arg) }
  def do_each
    HigherOrderMessage.new do |id, *args|
      each { |x| x.__send__(id, *args) }
    end
  end

  # Passes a method to each element in this collection, then
  # collects the results into a new array.
  #
  # $ array.do_collect.get_something(arg)
  #   ... is equivalent to
  # $ array.collect { |v| v.get_something(arg) }
  def do_collect
    HigherOrderMessage.new do |id, *args|
      collect { |x| x.__send__(id, *args) }
    end
  end
  
  alias_method :do_map, :do_collect
  
  # Requires inject method, from available for example in extensions/all.
  #
  # $ array.do_inject(0).apply_something(arg)
  #   ... is equivalent to
  # $ array.inject(0) { |n,v| n.apply_something(v,arg) }
  def do_inject(start_value)
    HigherOrderMessage.new do |id, *args|
      inject(start_value) { |a,x| a.__send__(id, x, *args) }
    end
  end
  
  # Where, similar to select. Passes a method to each element in a
  # collection and creates a new collection consisting of the elements
  # for which the method returned true.
  #
  # $ array.where.condition(arg)
  #   ... is equivalent to
  # $ array.select { |v| v.condition(arg) }
  def where
    HigherOrderMessage.new do |id, *args|
      select { |x| x.__send__(id, *args) }
    end
  end

  # The opposite of where, since negating with ! sometimes
  # works badly.
  def where_not
    HigherOrderMessage.new do |id, *args|
      reject { |x| x.__send__(id, *args) }
    end
  end
  
  # A second order message. Having works like where, but instead of
  # checking a condition on the element itself, it checks the condition
  # on another method call.
  #
  # $ array.having.get_something(arg1).condition(arg2)
  #   ... is equivalent to
  # $ array.select { |v| v.get_something(arg1).condition(arg2) }
  def having
    HigherOrderMessage.new do |id, *args|
      HigherOrderMessage.new do |secid, *secargs|
        select { |x| x.__send__(id, *args).__send__(secid, *secargs) }
      end
    end
  end
  
  # The opposite of having.
  def not_having
    HigherOrderMessage.new do |id, *args|
      HigherOrderMessage.new do |secid, *secargs|
        reject { |x| x.__send__(id, *args).__send__(secid, *secargs) }
      end
    end
  end
end


