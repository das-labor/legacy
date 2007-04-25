class FunnyNumber
  def inspect
    "Funny #{@rep.length} (#{@rep})"
  end

  def initialize(from_integer)
    @rep = "n" * from_integer
  end
end

=begin
Try this:
   FunnyNumber.new(3).inspect

=end

