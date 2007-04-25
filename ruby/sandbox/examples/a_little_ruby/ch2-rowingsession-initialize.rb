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

=begin

Try this:
    session = RowingSession.new("buffy", 30)
    session.report

=end
