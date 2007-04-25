# NOTE: When you load this, you might see this message:
#    warning: already initialized constant RowingSession
# This version of RowingSession is clashing with the earlier
# version. You need to exit IRB and restart it.

class Session
  def report
    print "#{@time} minutes on #{@name} = "
    print "#{calories} calories.\n"
  end
end

class RowingSession < Session
  def initialize(name, time)
    @name = name
    @time = time
  end

  def calories
    @time * 6
  end
end

=begin

Try this:
    session = RowingSession.new("buffy", 30)
    session.report

=end
