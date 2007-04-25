class IceCream

  def IceCream.small
    new(80)
  end
  
  def initialize(starting_licks)
    @left = starting_licks
  end

  def lick
    @left = @left - 1
    if @left > 0
      "yum!"
    elsif @left == 0
      "Good to the last lick!"
    else
      "all gone"
    end
  end
end

=begin
Try this:
   teensy = IceCream.new(1)
   teensy.lick
   teensy.lick

   small = IceCream.small
   81.times { puts small.lick } 

Ruby Note:

"times" is one way to write a loop. We'll see many more later.

"puts" is much like "print", but it prints to a new line every
time. "print" only does that if it's argument contains "\n".

=end
