
# This is the same class definition as in the previous icecream file.
class IceCream

  def IceCream.small
    new(80)
  end
  
  def initialize(starting_licks)
    @left = licks
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

# Now we change the new method

class IceCream
  def IceCream.new(starting_licks)
    Celery.new
  end
end

# To check that we really have a Celery:
class Celery
  def lick
    "licking celery? yuck!"
  end
end


=begin
Try this:
   IceCream.new(100).lick
   IceCream.small.lick

=end
