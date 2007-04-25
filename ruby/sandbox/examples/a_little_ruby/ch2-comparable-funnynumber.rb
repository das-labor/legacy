
# This is the same FunnyNumber as in Chapter 1, but it uses the <=> operator
# and includes module Comparable.

class FunnyNumber
  include Comparable

  def <=>(other)
    self.as_integer <=> other.as_integer
  end

  def inspect
    "Funny #{@rep.length} (#{@rep})"
  end

  def initialize(from_integer)
    @rep = "n" * from_integer
  end

  def as_integer
    @rep.length
  end
end

=begin

Try this:
   FunnyNumber.new(2) > FunnyNumber.new(1)      # true
   FunnyNumber.new(300) <= FunnyNumber.new(100) # false
   FunnyNumber.new(2).between?(FunnyNumber.new(1), FunnyNumber.new(3)) # true

=end


