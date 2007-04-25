
# This is the same FunnyNumber as in ch1-funnynumber.rb
# I could tell Ruby that file is required by this one, but then
# I'd have to tell you how Ruby uses environment variables to know
# where to look for files. It's not complicated - it's pretty much the
# same way all languages do it - but it's outside the scope of this
# book. 

class FunnyNumber
  def inspect
    "Funny #{@rep.length} (#{@rep})"
  end

  def initialize(from_integer)
    @rep = "n" * from_integer
  end
end

# Add two new methods.
class FunnyNumber
  def as_integer
    @rep.length
  end

  def <(other)
    self.as_integer < other.as_integer
  end
end

# Same ascending as in ch1-ascending.rb.
def ascending?(first, second, third)
  first < second && second < third
end

=begin
Try this:
   puts ascending?(FunnyNumber.new(1),
                   FunnyNumber.new(2),
                   FunnyNumber.new(3))

=end


