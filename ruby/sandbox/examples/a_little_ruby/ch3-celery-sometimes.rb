
# Everything but our new method.
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
  @created = 0
  def IceCream.new(starting_licks)
    @created += 1
    if @created % 5 == 0
      IceCream.new(starting_licks)
    else
      Celery.new
    end
  end
end

class Celery
end


=begin
Try this:

    IceCream.small.class    # Celery
    IceCream.small.class    # Celery
    IceCream.small.class    # Celery
    IceCream.small.class    # Celery
    IceCream.small.class    # Why is this Celery again?

=end
