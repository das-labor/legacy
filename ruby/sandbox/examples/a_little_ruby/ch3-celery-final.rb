
# Everything but our new method.
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

# Here's a better version of new that works with subclasses.
class IceCream
  @created = 0

  def IceCream.allowed?
    @created += 1
    @created % 5 == 0
  end

  def IceCream.new(starting_licks)
    if IceCream.allowed?
      super(starting_licks)
    else
      Celery.new
    end
  end
end

class TripleFudge < IceCream
end

class Celery
end



=begin
Try this:

    IceCream.new(10).class       # Celery
    TripleFudge.new(1000).class    # Celery
    IceCream.new(10).class       # Celery
    TripleFudge.new(1000).class    # Celery
    TripleFudge.new(1000).class    # IceCream

and this:

    TripleFudge.small.class     # Celery
    TripleFudge.small.class     # Celery
    TripleFudge.small.class     # Celery
    TripleFudge.small.class     # Celery
    TripleFudge.small.class     # IceCream
=end
