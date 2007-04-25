# NOTE: When you load this, you might see this message:
#    warning: already initialized constant RowingSession
# This version of RowingSession is clashing with the earlier
# version. You need to exit IRB and restart it.

class Session
  def report
    print "#{@time} minutes on #{@name} = "
    print "#{calories} calories.\n"
  end

  def initialize(name, time)
    @name = name
    @time = time
  end
end

class RowingSession < Session
  def calories
    @time * 6
  end
end

class ClimbingSession < Session
  def initialize(name, time, program, weight)
    super(name, time)
    @program = program
    @weight = weight
  end

  def calories  # There are better ways to do this, but it will do for now.
    if @program == "steady"
      @time * 3 * (@weight / 70.0)
    elsif @program == "hill run"
      @time * 5 * (@weight / 50.0)
    end
  end
end

=begin

Try this:
    session = RowingSession.new("buffy", 30)
    session.report

    session = ClimbingSession.new("biff", 23, "hill run", 84)
    session.report

=end
