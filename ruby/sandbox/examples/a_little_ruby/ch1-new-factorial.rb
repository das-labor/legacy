class Integer
  def new_factorial
    if self == 1
      self
    else
      self * (self-1).new_factorial
    end
  end
end

=begin
Try this:
    5.new_factorial)


NOTES ON RUBY:

Integer's "new_factorial" could actually be named "factorial". That name
would not conflict with the factorial defined in ch1-factorial.rb. I
used different names only so that you, the reader, would be sure
which version I was referring to.

The "class Integer ... end" construct doesn't define a new Integer
class, because Integers already exist. It adds something new to the
existing class. If the Integer class didn't exist, this construct
would have created it.

=end
